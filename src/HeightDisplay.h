#ifndef HEIGHTDISPLAY_H
#define HEIGHTDISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

class HeightDisplay {
public:
  HeightDisplay();
  void init();
  void showHeight(float heightMM);
  void showMessage(const char* message);

private:
  static const int SCREEN_WIDTH = 128;
  static const int SCREEN_HEIGHT = 32;
  static const int OLED_RESET = -1;
  static const int SCREEN_ADDRESS = 0x3C;

  Adafruit_SSD1306 display;
  void clearDisplay();

  // Memory optimization
  static const int TEXT_SIZE = 1;           // Reduced from 2 to save memory
  static const int MAX_MESSAGE_LENGTH = 16; // Limit message length
};

#endif // HEIGHTDISPLAY_H
