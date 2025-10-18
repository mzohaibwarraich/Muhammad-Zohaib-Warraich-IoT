#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define blueLed 18    // GPIO pin for blue LED
#define redLed 17     // GPIO pin for red LED
#define orangeLed 16  // GPIO pin for orange LED
#define blueButton 25 // GPIO pin for blue button
#define redButton 26  // GPIO pin for red button

int mode = 1;               // mode counter starting from 1
int brightness = 0;         // for PWM fade
bool toggle = false;        // for alternate blink
unsigned long lastTime = 0; // for non-blocking timing

int frequency = 5000;       // PWM frequency
int resolution = 8;         // PWM resolution
int channelBlueLed = 0;     // PWM channel for blue LED
int channelRedLed = 1;      // PWM channel for red LED
int channelOrangeLed = 2;   // PWM channel for orange LED
int currentLED = 0; // to track which LED to turn on

// For debounce
volatile unsigned long lastBluePress = 0; // timestamp of last blue button press
volatile unsigned long lastRedPress = 0;  // timestamp of last red button press

int blueBrightness = 0, blueFade = 20;
int orangeBrightness = 0, orangeFade = 20;
int redBrightness = 0, redFade = 20;

unsigned long lastBlueTime = 0;
unsigned long lastRedTime = 0;
unsigned long lastOrangeTime = 0;

// for displaying mode on OLED display
void updateDisplay()
{
  display.clearDisplay();              // clear previous display
  display.setTextSize(1);              // text size set to 1
  display.setTextColor(SSD1306_WHITE); // draw white text
  display.setCursor(0, 10);            // cursor for writing text will start at top-left corner

  if (mode == 1)
    display.print("Mode 1: Both OFF");
  else if (mode == 2)
    display.print("Mode 2: Alt Blink");
  else if (mode == 3)
    display.print("Mode 3: Both ON");
  else if (mode == 4)
    display.print("Mode 4: PWM Fade");

  display.display(); // actually display all of the above
}

// ISR for Blue button (next mode)
void IRAM_ATTR bluePressed()
{
  if (millis() - lastBluePress > 300)
  {                           // debounce
    mode++;                   // go to next mode
    if (mode > 4)             // start from mode 1 after mode 4
      mode = 1;               //
    lastBluePress = millis(); // milis() equals to current time in milliseconds
  }
}

// ISR for Red button (reset to mode 1)
void IRAM_ATTR redPressed()
{
  if (millis() - lastRedPress > 300)
  { // debounce
    mode = 1;
    lastRedPress = millis(); // update last press time
  }
}

void setup()
{
  Serial.begin(115200);
  // setting up pin modes for buttons
  pinMode(blueButton, INPUT_PULLUP);
  pinMode(redButton, INPUT_PULLUP);

  // Setting up interrupts for buttons to call the ISRs functions whenever the button is clicked
  attachInterrupt(digitalPinToInterrupt(blueButton), bluePressed, FALLING); // calling the ISR function when the blue button is pressed and FALLING means when the button is pressed the pin goes from HIGH to LOW

  attachInterrupt(digitalPinToInterrupt(redButton), redPressed, FALLING); // calling the ISR function when the red button is pressed and FALLING means when the button is pressed the pin goes from HIGH to LOW

  // Setting up PWM for LEDs and attaching them to the pins of the ESP32
  ledcSetup(channelBlueLed, frequency, resolution);   // setting up PWM channel properties for blue LED
  ledcSetup(channelRedLed, frequency, resolution);    // setting up PWM channel properties for red LED
  ledcSetup(channelOrangeLed, frequency, resolution); // setting up PWM channel properties for orange LED
  ledcAttachPin(blueLed, channelBlueLed);             // attaching the blue LED to the PWM channel
  ledcAttachPin(redLed, channelRedLed);               // attaching the red LED to the PWM channel
  ledcAttachPin(orangeLed, channelOrangeLed);         // attaching the orange LED to the PWM channel

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {                                    // initialize OLED display with I2C address 0x3C
    Serial.println("OLED not found!"); // if OLED display is not found, print error message
    while (true); // ESP32 will go in infinite loop here if OLED display is not found
  }

  updateDisplay(); // initial display update
}
void mode1()
{
  ledcWrite(channelBlueLed, LOW);   // turn off blue LED
  ledcWrite(channelRedLed, LOW);    // turn off red LED
  ledcWrite(channelOrangeLed, LOW); // turn off orange LED
}
void mode2()
{
  if (millis() - lastTime > 500) // toggle every 500 ms
    { 
      // set all LED to off at first
      ledcWrite(channelBlueLed, 0);
      ledcWrite(channelRedLed, 0);
      ledcWrite(channelOrangeLed, 0);

      // turn on the current LED
      if (currentLED == 0) 
        ledcWrite(channelOrangeLed, 255); // turn on orange LED
      else if (currentLED == 1) //
        ledcWrite(channelBlueLed, 255); // turn on blue LED
      else if (currentLED == 2) 
        ledcWrite(channelRedLed, 255); // turn on red LED

      // Move to next LED
      currentLED++;
      if (currentLED > 2)
        currentLED = 0; // reset to first LED after reaching to last LED
      lastTime = millis(); // update lastTime
    }
}
void mode3()
{
  ledcWrite(channelBlueLed, 255);   // turn on blue LED
  ledcWrite(channelRedLed, 255);    // turn on red LED
  ledcWrite(channelOrangeLed, 255); // turn on orange LED
}
void mode4()
{
  if (millis() - lastBlueTime > 10) {
      blueBrightness += blueFade; // increase brightness
      if (blueBrightness <= 0 || blueBrightness >= 255) 
        blueFade = -blueFade; // reverse fade direction at limits
      ledcWrite(channelBlueLed, blueBrightness); // set blue LED brightness
      lastBlueTime = millis(); // update last blue time
    }
    if (millis() - lastRedTime > 35) {
      redBrightness += redFade; // increase brightness
      if (redBrightness <= 0 || redBrightness >= 255) 
        redFade = -redFade; // reverse fade direction at limits
      ledcWrite(channelRedLed, redBrightness); // set red LED brightness
      lastRedTime = millis(); 
    }
    if (millis() - lastOrangeTime > 60) {
      orangeBrightness += orangeFade; // increase brightness
      if (orangeBrightness <= 0 || orangeBrightness >= 255) 
        orangeFade = -orangeFade; // reverse fade direction at limits
      ledcWrite(channelOrangeLed, orangeBrightness); // set orange LED brightness
      lastOrangeTime = millis();
    }
}
void loop()
{
  updateDisplay(); // update display with current mode

  // Mode 1: Both off
  if (mode == 1)
  {
    mode1();
  }

  // Mode 2: Alternate blink (non-blocking)
  else if (mode == 2)
  {
    mode2();
  }

  // Mode 3: Both ON
  else if (mode == 3)
  {
    mode3();
  }

  // Mode 4: PWM Fade
  else if (mode == 4)
  {
    mode4();
  }
}
