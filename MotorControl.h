#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <Arduino.h>

class MotorControl {
public:
    MotorControl(uint8_t forwardPin, uint8_t backwardPin);
    void init();
    void forward(uint8_t speed);
    void backward(uint8_t speed);
    void stop();
    void setSpeed(uint8_t speed);
    uint8_t getSpeed() const;
    void update();  // Call this regularly to handle ramping

private:
    void rampToSpeed(uint8_t targetSpeed);
    
    uint8_t forwardPin;
    uint8_t backwardPin;
    uint8_t currentSpeed;
    uint8_t targetSpeed;
    unsigned long lastRampTime;
    bool isMovingForward;
    bool isMovingBackward;
    
    static const uint8_t MIN_SPEED = 0;
    static const uint8_t MAX_SPEED = 255;
    static const unsigned long RAMP_TIME_MS = 500;  // Reduced from 1000ms to 500ms
    static const uint8_t RAMP_STEP = 10;  // Increased from 5 to 10 for faster ramping
};

#endif // MOTORCONTROL_H
