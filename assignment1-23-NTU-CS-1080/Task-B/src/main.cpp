#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


#define blueLed 18 // Setting up GPIO pin for blue Led
#define blueButton 25 // Setting up GPIO pin for blue Button
#define buzzer 27 // Setting up GPIO pin for Buzzer

bool ledState = false; // Current state of the LED
unsigned long pressTime = 0; // Time when the button was pressed
bool buttonPressed = false; // Flag to track button press state
bool longPress = false; // Flag to track if long press was detected

const unsigned long longPressTime = 2000;  // 2 seconds for long press

void displayMessage(const char* message) {
  display.clearDisplay(); // Clear the display
  display.setTextSize(1); // Text size 1
  display.setTextColor(SSD1306_WHITE); // White text
  display.setCursor(0, 10); // Start at top-left corner
  display.println(message); // Print the message
  display.display(); // Update the display
}

void setup() {
  Serial.begin(115200);

  pinMode(blueLed, OUTPUT); // setting up blue LED pin for getting output
  pinMode(buzzer, OUTPUT); // setting up buzzer pin for getting output
  pinMode(blueButton, INPUT_PULLUP); // Initialize blue button pin for taking input

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!"); // OLED not found
    while (true); // Halt execution if OLED is not found
  }
  displayMessage("Initializing...");
}

void loop() {
  bool buttonState = digitalRead(blueButton); // Read button state

  // Detect whether the button is pressed or released
  if (buttonState == LOW && !buttonPressed) {
    buttonPressed = true; // Button is pressed
    pressTime = millis(); // Record the time when button was pressed
    longPress = false; // Reset long press flag
  }

  // Check for long press
  if (buttonState == LOW && buttonPressed) {
    if ((millis() - pressTime > longPressTime)) { // If pressed for more than 2 seconds
      displayMessage("Long Press: buzzer ON"); // Update display

      tone(buzzer, 1000);   // It will play tone of 1 kHz for 1 second
      delay(1000); // Wait for 1 second
      noTone(buzzer); // Stop the tone

      displayMessage("buzzer Done"); // Update display
      delay(300); // Brief delay to show message
      longPress = true; // Set long press flag
    }
  }

  // Check for short press
  if (buttonState == HIGH && buttonPressed) {
    if (!longPress) { // If it was not a long press
      ledState = !ledState; // Toggle LED state
      digitalWrite(blueLed, ledState); // Update LED state
      displayMessage(ledState ? "Short Press: LED ON" : "Short Press: LED OFF"); // Update display
    }

    buttonPressed = false; // Reset button pressed flag
    delay(100);  // Debounce delay
  }
}