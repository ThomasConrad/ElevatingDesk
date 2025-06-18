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
  
  // Draw header with desk icon
  drawHeader("DESK HEIGHT");
  
  // Draw main height display
  drawHeightDisplay(heightMM, true);
  
  // Draw movement indicator
  if (isMoving) {
    drawMovementIndicator(true, heightMM > 0); // Assume positive change is up
  }
  
  // Draw footer with units
  drawFooter("millimeters");
  
  display.display();
}

void HeightDisplay::showPresetMode(uint8_t presetNumber, float presetHeight) {
  clearDisplay();
  currentMode = PRESET_MODE;
  
  // Draw header
  drawHeader("PRESET MODE");
  
  // Draw preset number prominently
  display.setTextSize(2);
  display.setCursor(MARGIN_X, MAIN_CONTENT_Y);
  display.print("P");
  display.print(presetNumber);
  
  // Draw preset height
  char heightStr[10];
  snprintf(heightStr, sizeof(heightStr), "%.1fmm", (double)presetHeight);
  display.setTextSize(1);
  display.setCursor(50, MAIN_CONTENT_Y + 4);
  display.print(heightStr);
  
  // Draw preset indicators
  for (uint8_t i = 1; i <= 3; i++) {
    drawPresetIndicator(i, i == presetNumber);
  }
  
  drawFooter("Up/Down: Select | Both: Go");
  display.display();
}

void HeightDisplay::showCalibrationMode(float currentHeight, bool showInstructions) {
  clearDisplay();
  currentMode = HEIGHT_CALIBRATION;
  
  drawHeader("HEIGHT CAL");
  
  if (showInstructions && animationToggle) {
    centerText("Up/Down to", MAIN_CONTENT_Y, 1);
    centerText("adjust height", MAIN_CONTENT_Y + 8, 1);
  } else {
    drawHeightDisplay(currentHeight, false);
  }
  
  drawFooter("Hold Both: Save & Exit");
  display.display();
}

void HeightDisplay::showStatusMessage(const char* message, bool isSuccess) {
  clearDisplay();
  currentMode = STATUS_MESSAGE;
  
  // Draw icon based on success/failure
  if (isSuccess) {
    drawIcon(MARGIN_X, MAIN_CONTENT_Y, ">");
  } else {
    drawIcon(MARGIN_X, MAIN_CONTENT_Y, "!");
  }
  
  // Center the message
  display.setTextSize(1);
  display.setCursor(20, MAIN_CONTENT_Y + 2);
  display.print(message);
  
  display.display();
}

void HeightDisplay::showError(const char* errorMessage) {
  clearDisplay();
  currentMode = ERROR;
  
  drawHeader("ERROR");
  drawIcon(MARGIN_X, MAIN_CONTENT_Y, "X");
  
  display.setTextSize(1);
  display.setCursor(20, MAIN_CONTENT_Y + 2);
  display.print(errorMessage);
  
  display.display();
}

void HeightDisplay::showBootScreen() {
  clearDisplay();
  
  // Draw logo/title
  display.setTextSize(1);
  centerText("ELEVATING DESK", 8, 1);
  centerText("Controller v1.0", 18, 1);
  
  // Draw progress bar
  drawProgressBar(20, 28, 88, 3, 1.0);
  
  display.display();
}

void HeightDisplay::clearDisplay() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void HeightDisplay::drawHeader(const char* title) {
  display.setTextSize(1);
  display.setCursor(MARGIN_X, 0);
  display.print(title);
  
  // Draw underline
  display.drawLine(0, 8, SCREEN_WIDTH, 8, SSD1306_WHITE);
}

void HeightDisplay::drawFooter(const char* text) {
  display.setTextSize(1);
  display.setCursor(MARGIN_X, FOOTER_Y);
  display.print(text);
}

void HeightDisplay::drawProgressBar(int x, int y, int width, int height, float progress) {
  // Draw border
  display.drawRect(x, y, width, height, SSD1306_WHITE);
  
  // Draw fill
  int fillWidth = (int)(width * progress);
  if (fillWidth > 0) {
    display.fillRect(x + 1, y + 1, fillWidth - 2, height - 2, SSD1306_WHITE);
  }
}

void HeightDisplay::drawIcon(int x, int y, const char* icon) {
  display.setTextSize(2);
  display.setCursor(x, y);
  display.print(icon);
}

void HeightDisplay::drawHeightDisplay(float height, bool large) {
  char heightStr[15];
  snprintf(heightStr, sizeof(heightStr), "%.1f", (double)height);
  
  if (large) {
    display.setTextSize(2);
    display.setCursor(MARGIN_X, MAIN_CONTENT_Y);
    display.print(heightStr);
    
    display.setTextSize(1);
    display.setCursor(MARGIN_X + strlen(heightStr) * 12, MAIN_CONTENT_Y + 8);
    display.print("mm");
  } else {
    display.setTextSize(1);
    centerText(heightStr, MAIN_CONTENT_Y, 1);
    centerText("mm", MAIN_CONTENT_Y + 8, 1);
  }
}

void HeightDisplay::drawMovementIndicator(bool isMoving, bool isUp) {
  if (isMoving && animationToggle) {
    display.setCursor(SCREEN_WIDTH - 15, MAIN_CONTENT_Y);
    display.setTextSize(1);
    display.print(isUp ? "^" : "v");
  }
}

void HeightDisplay::drawPresetIndicator(uint8_t presetNumber, bool isActive) {
  int x = 90 + (presetNumber - 1) * 12;
  int y = MAIN_CONTENT_Y;
  
  if (isActive) {
    display.fillCircle(x, y, 3, SSD1306_WHITE);
  } else {
    display.drawCircle(x, y, 3, SSD1306_WHITE);
  }
  
  display.setTextSize(1);
  display.setCursor(x - 2, y - 3);
  display.print(presetNumber);
}

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