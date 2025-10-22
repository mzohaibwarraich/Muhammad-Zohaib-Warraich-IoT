// Name: Muhammad Zohaib Warraich
// Roll no: 23-NTU-CS-1080 

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


#define yellowLED 19 // Setting up GPIO pin for Yellow Led
#define blueButton 25 // Setting up GPIO pin for blue Button
#define buzzer 27 // Setting up GPIO pin for Buzzer

bool ledState = false; // Current state of the LED
unsigned long pressTime = 0; // Time when the button was pressed
bool buttonPressed = false; // Flag to track button press state
bool longPress = false; // Flag to track if long press was detected

const unsigned long longPressTime = 2000;  // 2 seconds for long press

// Simple but visually better display message
void displayMessage(const char* line1, const char* line2 = "") {
  display.clearDisplay();
  // First line will be bold and large
  display.setTextSize(2);  // Large text size
  display.setTextColor(SSD1306_WHITE); // White text
  display.setCursor(15, 10); // Centered cursor
  display.println(line1); // Print first line
  // Second line will be smaller and placed below First line
  if (line2[0] != '\0') { // If second line is not empty
    display.setTextSize(1); // Smaller text size
    display.setCursor(20, 40); // Centered cursor for second line
    display.println(line2); // Print second line
  }
  display.display();
}

void setup() {
  Serial.begin(115200);
  pinMode(yellowLED, OUTPUT); // setting up blue LED pin for getting output
  pinMode(buzzer, OUTPUT); // setting up buzzer pin for getting output
  pinMode(blueButton, INPUT_PULLUP); // Initialize blue button pin for taking input
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!"); // OLED not found
    while (true); // Halt execution if OLED is not found
  }
  displayMessage("Ready", "Press the Button");
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
      displayMessage("Long Press", "Buzzer ON");
      tone(buzzer, 1000);   // It will play tone of 1 kHz for 1 second
      delay(1000);
      noTone(buzzer); // Stop the tone
      displayMessage("Completed", "Buzzer OFF");
      delay(500); // Brief delay to show message
      longPress = true; // Set long press flag
    }
  }

  // Check for short press
  if (buttonState == HIGH && buttonPressed) {
    if (!longPress) { // If it was not a long press
      ledState = !ledState; // Toggle LED state
      digitalWrite(yellowLED, ledState); // Update LED state
      if (ledState==HIGH) { // If LED is on
        displayMessage("LED ON", "Short Press");
      } else { 
        displayMessage("LED OFF", "Short Press");
      }
    }

    buttonPressed = false; // Reset button pressed flag
    delay(300);  // Debounce delay
  }
}
