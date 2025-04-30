#include "MotorControl.h"

MotorControl::MotorControl(int forwardPin, int backwardPin) 
  : forwardPin(forwardPin), backwardPin(backwardPin) {
  currentSpeed = MAX_SPEED;
}

void MotorControl::init() {
  // Set up the motor control pins as outputs
  pinMode(forwardPin, OUTPUT);
  pinMode(backwardPin, OUTPUT);
  
  // Ensure motors are stopped at initialization
  stop();
}

void MotorControl::forward(int speed) {
  if (speed >= 0) {
    setSpeed(speed);
  }
  
  // Ensure backward pin is off first to prevent short-circuiting
  analogWrite(backwardPin, 0);
  
  // Apply PWM to forward pin
  analogWrite(forwardPin, currentSpeed);
}

void MotorControl::backward(int speed) {
  if (speed >= 0) {
    setSpeed(speed);
  }
  
  // Ensure forward pin is off first to prevent short-circuiting
  analogWrite(forwardPin, 0);
  
  // Apply PWM to backward pin
  analogWrite(backwardPin, currentSpeed);
}

void MotorControl::stop() {
  // Set both motor pins to LOW
  analogWrite(forwardPin, 0);
  analogWrite(backwardPin, 0);
}

void MotorControl::setSpeed(int speed) {
  // Constrain speed between MIN_SPEED and MAX_SPEED
  currentSpeed = constrain(speed, MIN_SPEED, MAX_SPEED);
}

int MotorControl::getSpeed() const {
  return currentSpeed;
}
