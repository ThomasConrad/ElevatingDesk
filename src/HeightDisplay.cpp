#include "HeightDisplay.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

HeightDisplay::HeightDisplay() 
  : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
    currentMode(NORMAL),
    lastUpdate(0),
    animationToggle(false) {}

void HeightDisplay::init() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    return;
  }

  showBootScreen();
  delay(1000);
  currentMode = NORMAL;
}

void HeightDisplay::update() {
  if (shouldAnimate()) {
    updateAnimations();
    lastUpdate = millis();
  }
}

void HeightDisplay::showHeight(float heightMM, bool isMoving) {
  clearDisplay();
  currentMode = NORMAL;
  
  // Show large height number in center
  char heightStr[10];
  snprintf(heightStr, sizeof(heightStr), "%.1f", (double)heightMM);
  
  // Large height display (text size 3)
  display.setTextSize(3);
  int textWidth = strlen(heightStr) * 18; // Each char is ~18 pixels wide at size 3
  int x = (SCREEN_WIDTH - textWidth) / 2;
  display.setCursor(x, 20);
  display.print(heightStr);
  
  // Small "mm" unit below
  display.setTextSize(1);
  centerText("mm", 50, 1);
  
  // Movement indicator if moving
  if (isMoving) {
    display.setTextSize(2);
    if (animationToggle) {
      centerText(heightMM > 0 ? "UP" : "DOWN", 2, 2);
    }
  } else {
    // Show "DESK" label when static
    display.setTextSize(1);
    centerText("DESK", 2, 1);
  }
  
  display.display();
}

void HeightDisplay::showPresetMode(uint8_t presetNumber, float presetHeight) {
  clearDisplay();
  currentMode = PRESET_MODE;
  
  // Large "PRESET" at top
  display.setTextSize(2);
  centerText("PRESET", 5, 2);
  
  // Huge preset number in center
  char presetStr[3];
  snprintf(presetStr, sizeof(presetStr), "%d", presetNumber);
  display.setTextSize(4);
  centerText(presetStr, 25, 4);
  
  // Height value below
  char heightStr[10];
  snprintf(heightStr, sizeof(heightStr), "%.1f mm", (double)presetHeight);
  display.setTextSize(1);
  centerText(heightStr, 55, 1);
  
  display.display();
}

void HeightDisplay::showCalibrationMode(float currentHeight, bool showInstructions) {
  clearDisplay();
  currentMode = HEIGHT_CALIBRATION;
  
  // Title at top
  display.setTextSize(1);
  centerText("CALIBRATION", 2, 1);
  
  if (showInstructions && animationToggle) {
    // Large instruction text
    display.setTextSize(2);
    centerText("UP/DOWN", 20, 2);
    centerText("ADJUST", 40, 2);
  } else {
    // Large height display
    char heightStr[10];
    snprintf(heightStr, sizeof(heightStr), "%.1f", (double)currentHeight);
    display.setTextSize(3);
    int textWidth = strlen(heightStr) * 18;
    int x = (SCREEN_WIDTH - textWidth) / 2;
    display.setCursor(x, 25);
    display.print(heightStr);
    
    display.setTextSize(1);
    centerText("mm", 50, 1);
  }
  
  display.display();
}

void HeightDisplay::showStatusMessage(const char* message, bool isSuccess) {
  clearDisplay();
  currentMode = STATUS_MESSAGE;
  
  // Large status icon at top
  display.setTextSize(3);
  if (isSuccess) {
    centerText("OK", 8, 3);
  } else {
    centerText("!!", 8, 3);
  }
  
  // Message below in large text
  display.setTextSize(2);
  centerText(message, 35, 2);
  
  display.display();
}

void HeightDisplay::showError(const char* errorMessage) {
  clearDisplay();
  currentMode = ERROR;
  
  // Large ERROR text
  display.setTextSize(2);
  centerText("ERROR", 10, 2);
  
  // Error message below
  display.setTextSize(1);
  centerText(errorMessage, 35, 1);
  
  display.display();
}

void HeightDisplay::showBootScreen() {
  clearDisplay();
  
  // Large title
  display.setTextSize(2);
  centerText("DESK", 15, 2);
  centerText("CTRL", 35, 2);
  
  // Version at bottom
  display.setTextSize(1);
  centerText("v1.0", 55, 1);
  
  display.display();
}

void HeightDisplay::clearDisplay() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

// Simplified helper methods for 128x64 display

void HeightDisplay::centerText(const char* text, int y, int textSize) {
  display.setTextSize(textSize);
  int textWidth = strlen(text) * 6 * textSize;
  int x = (SCREEN_WIDTH - textWidth) / 2;
  display.setCursor(x, y);
  display.print(text);
}

void HeightDisplay::updateAnimations() {
  animationToggle = !animationToggle;
  
  // Redraw current screen if it has animations
  if (currentMode == HEIGHT_CALIBRATION || 
      (currentMode == NORMAL && animationToggle)) {
    // Trigger a refresh in the main controller
  }
}

bool HeightDisplay::shouldAnimate() {
  return (millis() - lastUpdate) >= ANIMATION_INTERVAL;
}

// Legacy compatibility methods
void HeightDisplay::showMessage(const char* message) {
  showStatusMessage(message, true);
}