#include "MotorControl.h"

MotorControl::MotorControl(uint8_t forwardPin, uint8_t backwardPin)
    : forwardPin(forwardPin), backwardPin(backwardPin), currentSpeed(0),
      targetSpeed(0), lastRampTime(0), isMovingForward(false),
      isMovingBackward(false) {}

void MotorControl::init() {
  // Set up the motor control pins as outputs
  pinMode(forwardPin, OUTPUT);
  pinMode(backwardPin, OUTPUT);

  // Ensure motors are stopped at initialization
  stop();
}

void MotorControl::forward(uint8_t speed) {
  targetSpeed = speed;
  isMovingForward = true;
  isMovingBackward = false;
}

void MotorControl::backward(uint8_t speed) {
  targetSpeed = speed;
  isMovingForward = false;
  isMovingBackward = true;
}

void MotorControl::stop() {
  targetSpeed = 0;
  isMovingForward = false;
  isMovingBackward = false;
  currentSpeed = 0;
  analogWrite(forwardPin, 0);
  analogWrite(backwardPin, 0);
}

void MotorControl::setSpeed(uint8_t speed) { targetSpeed = speed; }

uint8_t MotorControl::getSpeed() const { return currentSpeed; }

void MotorControl::update() {
  unsigned long currentTime = millis();
  if (currentTime - lastRampTime >= 20) { // Update every 20ms
    lastRampTime = currentTime;

    if (currentSpeed != targetSpeed) {
      if (currentSpeed < targetSpeed) {
        currentSpeed = min(currentSpeed + RAMP_STEP, targetSpeed);
      } else {
        currentSpeed = max(currentSpeed - RAMP_STEP, targetSpeed);
      }

      if (isMovingForward) {
        analogWrite(backwardPin, 0);
        analogWrite(forwardPin, currentSpeed);
      } else if (isMovingBackward) {
        analogWrite(forwardPin, 0);
        analogWrite(backwardPin, currentSpeed);
      }
    }
  }
}
