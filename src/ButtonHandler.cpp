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

bool ButtonHandler::isPressed() const {
    return buttonState == LOW;
}

bool ButtonHandler::isLongPressed() const {
    return isPressed() && (millis() - lastPressTime) >= LONG_PRESS_TIME;
}

bool ButtonHandler::isBothPressed(const ButtonHandler& other) const {
    return isPressed() && other.isPressed();
}

bool ButtonHandler::isBothLongPressed(const ButtonHandler& other) const {
    return isBothPressed(other) && 
           (millis() - lastPressTime) >= BOTH_BUTTONS_TIME && 
           (millis() - other.lastPressTime) >= BOTH_BUTTONS_TIME;
}
