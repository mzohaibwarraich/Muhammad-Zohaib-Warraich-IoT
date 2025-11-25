#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Wire.begin(21, 22); // ESP32 default I2C pins

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    for (;;); // Stop if display not found
  }

  display.clearDisplay();

  // --- Cartoon-style Avatar ---

  // Head
  display.drawCircle(64, 22, 12, SSD1306_WHITE);

  // Hairline (simple zigzag)
  display.drawLine(52, 12, 56, 8, SSD1306_WHITE);
  display.drawLine(56, 8, 60, 12, SSD1306_WHITE);
  display.drawLine(60, 12, 64, 8, SSD1306_WHITE);
  display.drawLine(64, 8, 68, 12, SSD1306_WHITE);
  display.drawLine(68, 12, 72, 8, SSD1306_WHITE);
  display.drawLine(72, 8, 76, 12, SSD1306_WHITE);

  // Eyes
  display.fillCircle(59, 22, 2, SSD1306_WHITE);
  display.fillCircle(69, 22, 2, SSD1306_WHITE);

  // Nose
  display.drawLine(64, 23, 64, 27, SSD1306_WHITE);

  // Smile
  display.drawLine(60, 30, 68, 30, SSD1306_WHITE);

  // Neck and shoulders
  display.drawLine(64, 34, 64, 44, SSD1306_WHITE);
  display.drawLine(64, 44, 50, 54, SSD1306_WHITE);
  display.drawLine(64, 44, 78, 54, SSD1306_WHITE);

  // Shirt (filled rectangle)
  display.fillRect(50, 54, 28, 8, SSD1306_WHITE);

  // Optional border box
  display.drawRect(40, 5, 50, 60, SSD1306_WHITE);

  display.display();
}

void loop() {
  // Nothing here
}
