#ifndef HEIGHTDISPLAY_H
#define HEIGHTDISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

class HeightDisplay {
public:
  enum DisplayMode {
    NORMAL,
    PRESET_MODE,
    CALIBRATION,
    HEIGHT_CALIBRATION,
    STATUS_MESSAGE,
    ERROR
  };

  HeightDisplay();
  void init();
  
  // Enhanced display methods
  void showHeight(float heightMM, bool isMoving = false);
  void showPresetMode(uint8_t presetNumber, float presetHeight);
  void showCalibrationMode(float currentHeight, bool showInstructions = false);
  void showEncoderCalibrationMode(uint8_t step, float startHeight, float endHeight, long pulseCount);
  void showStatusMessage(const char* message, bool isSuccess = true);
  void showError(const char* errorMessage);
  void showBootScreen();
  void update(); // Call regularly for animations
  
  // Legacy compatibility method
  void showMessage(const char* message);

private:
  static const int SCREEN_WIDTH = 128;
  static const int SCREEN_HEIGHT = 64;
  static const int OLED_RESET = -1;
  static const int SCREEN_ADDRESS = 0x3C;
  
  // UI Layout constants for 128x64 display
  static const int HEADER_HEIGHT = 12;
  static const int MAIN_CONTENT_Y = 16;
  static const int SECONDARY_CONTENT_Y = 40;
  static const int FOOTER_Y = 56;
  static const int MARGIN_X = 4;
  
  Adafruit_SSD1306 display;
  DisplayMode currentMode;
  unsigned long lastUpdate;
  bool animationToggle;
  
  // UI rendering methods
  void clearDisplay();
  void centerText(const char* text, int y, int textSize = 1);
  
  // Animation helpers
  void updateAnimations();
  bool shouldAnimate();
  
  // Memory optimization
  static const int MAX_MESSAGE_LENGTH = 20;
  static const unsigned long ANIMATION_INTERVAL = 500; // 500ms
};

#endif // HEIGHTDISPLAY_H