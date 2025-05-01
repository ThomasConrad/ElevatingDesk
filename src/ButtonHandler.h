#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

#include <Arduino.h>

class ButtonHandler {
public:
  ButtonHandler(uint8_t pin);
  void init();
  bool isPressed() const;
  bool isLongPressed() const;
  bool isVeryLongPressed() const;
  bool isBothPressed(const ButtonHandler& other) const;
  bool isBothLongPressed(const ButtonHandler& other) const;
  bool isBothVeryLongPressed(const ButtonHandler& other) const;
  void update();

private:
  uint8_t pin;
  bool lastButtonState;
  bool buttonState;
  unsigned long lastDebounceTime;
  unsigned long lastPressTime;

  static const unsigned long DEBOUNCE_DELAY = 50;         // milliseconds
  static const unsigned long LONG_PRESS_TIME = 1000;      // milliseconds
  static const unsigned long BOTH_BUTTONS_TIME = 2000;    // milliseconds for both buttons
  static const unsigned long VERY_LONG_PRESS_TIME = 5000; // milliseconds
};

#endif // BUTTONHANDLER_H
