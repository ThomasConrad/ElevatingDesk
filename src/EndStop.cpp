#include "EndStop.h"

EndStop::EndStop(uint8_t pin) : pin(pin) {}

void EndStop::init() {
  pinMode(pin, INPUT_PULLUP);
}

bool EndStop::isTriggered() const {
  return digitalRead(pin) == LOW; // Active LOW
}
