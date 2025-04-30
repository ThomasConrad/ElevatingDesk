#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

#include <Arduino.h>

class ButtonHandler {
public:
    ButtonHandler(uint8_t pin);
    void init();
    bool isPressed();
    bool isLongPressed();
    bool isBothPressed(const ButtonHandler& other);
    bool isBothLongPressed(const ButtonHandler& other);
    void update();

private:
    uint8_t pin;
    bool lastButtonState;
    bool buttonState;
    unsigned long lastDebounceTime;
    unsigned long lastPressTime;
    
    static const unsigned long DEBOUNCE_DELAY = 50;    // milliseconds
    static const unsigned long LONG_PRESS_TIME = 1000; // milliseconds
    static const unsigned long BOTH_BUTTONS_TIME = 2000; // milliseconds for both buttons
};

#endif // BUTTONHANDLER_H
