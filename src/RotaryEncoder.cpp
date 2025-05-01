#include "RotaryEncoder.h"

const float RotaryEncoder::MM_PER_STEP = MM_PER_REVOLUTION / STEPS_PER_REVOLUTION;

RotaryEncoder::RotaryEncoder(uint8_t pinA, uint8_t pinB) : pinA(pinA), pinB(pinB), lastStateA(0), position(0) {}

void RotaryEncoder::init() {
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  lastStateA = digitalRead(pinA);
}

void RotaryEncoder::update() {
  int stateA = digitalRead(pinA);
  int stateB = digitalRead(pinB);

  if (stateA != lastStateA) {
    // If the A state changed, check B state to determine direction
    if (stateB != stateA) {
      position++; // Clockwise
    } else {
      position--; // Counter-clockwise
    }
  }

  lastStateA = stateA;
}

long RotaryEncoder::getPosition() const {
  return position;
}

void RotaryEncoder::setPosition(long newPosition) {
  position = newPosition;
}

float RotaryEncoder::getHeightMM() const {
  return position * MM_PER_STEP;
}
