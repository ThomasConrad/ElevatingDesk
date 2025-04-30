#include "MotorControl.h"

MotorControl::MotorControl(int forwardPin, int backwardPin) 
    : forwardPin(forwardPin)
    , backwardPin(backwardPin)
    , currentSpeed(0)
    , targetSpeed(0)
    , lastRampTime(0)
    , isMovingForward(false)
    , isMovingBackward(false) {
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
        targetSpeed = constrain(speed, MIN_SPEED, MAX_SPEED);
        isMovingForward = true;
        isMovingBackward = false;
    }
}

void MotorControl::backward(int speed) {
    if (speed >= 0) {
        targetSpeed = constrain(speed, MIN_SPEED, MAX_SPEED);
        isMovingForward = false;
        isMovingBackward = true;
    }
}

void MotorControl::stop() {
    targetSpeed = 0;
    isMovingForward = false;
    isMovingBackward = false;
    currentSpeed = 0;
    analogWrite(forwardPin, 0);
    analogWrite(backwardPin, 0);
}

void MotorControl::setSpeed(int speed) {
    targetSpeed = constrain(speed, MIN_SPEED, MAX_SPEED);
}

int MotorControl::getSpeed() const {
    return currentSpeed;
}

void MotorControl::update() {
    unsigned long currentTime = millis();
    if (currentTime - lastRampTime >= 20) {  // Update every 20ms
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
