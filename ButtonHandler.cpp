#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(uint8_t pin)
    : pin(pin)
    , lastButtonState(HIGH)
    , buttonState(HIGH)
    , lastDebounceTime(0)
    , lastPressTime(0) {
}

void ButtonHandler::init() {
    pinMode(pin, INPUT_PULLUP);
}

void ButtonHandler::update() {
    int reading = digitalRead(pin);

    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        if (reading != buttonState) {
            buttonState = reading;
            if (buttonState == LOW) { // Button pressed (active low)
                lastPressTime = millis();
            }
        }
    }

    lastButtonState = reading;
}

bool ButtonHandler::isPressed() {
    return buttonState == LOW;
}

bool ButtonHandler::isLongPressed() {
    return isPressed() && (millis() - lastPressTime) >= LONG_PRESS_TIME;
}
