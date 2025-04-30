#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

#include <Arduino.h>

class ButtonHandler {
public:
    ButtonHandler(uint8_t pin);
    void init();
    bool isPressed();
    bool isLongPressed();
    void update();

private:
    uint8_t pin;
    bool lastButtonState;
    bool buttonState;
    unsigned long lastDebounceTime;
    unsigned long lastPressTime;
    
    static const unsigned long DEBOUNCE_DELAY = 50;    // milliseconds
    static const unsigned long LONG_PRESS_TIME = 1000; // milliseconds
};

#endif // BUTTONHANDLER_H
