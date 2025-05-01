#include "HeightDisplay.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

HeightDisplay::HeightDisplay() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

void HeightDisplay::init() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    return;
  }

  display.clearDisplay();
  display.setTextSize(TEXT_SIZE);
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void HeightDisplay::showHeight(float heightMM) {
  clearDisplay();

  char buffer[MAX_MESSAGE_LENGTH];
  snprintf(buffer, MAX_MESSAGE_LENGTH, "%.1f mm", (double)heightMM);

  display.setCursor(0, 0);
  display.print(buffer);
  display.display();
}

void HeightDisplay::showMessage(const char* message) {
  clearDisplay();

  char buffer[MAX_MESSAGE_LENGTH];
  strncpy(buffer, message, MAX_MESSAGE_LENGTH - 1);
  buffer[MAX_MESSAGE_LENGTH - 1] = '\0';

  display.setCursor(0, 0);
  display.print(buffer);
  display.display();
}

void HeightDisplay::clearDisplay() {
  display.clearDisplay();
  display.setTextSize(TEXT_SIZE);
  display.setTextColor(SSD1306_WHITE);
}
