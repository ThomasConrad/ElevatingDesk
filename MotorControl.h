#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <Arduino.h>

class MotorControl {
public:
    MotorControl(int forwardPin, int backwardPin);
    void init();
    void forward(int speed);
    void backward(int speed);
    void stop();
    void setSpeed(int speed);
    int getSpeed() const;
    void update();  // Call this regularly to handle ramping

private:
    void rampToSpeed(int targetSpeed);
    
    int forwardPin;
    int backwardPin;
    int currentSpeed;
    int targetSpeed;
    unsigned long lastRampTime;
    bool isMovingForward;
    bool isMovingBackward;
    
    static const int MIN_SPEED = 0;
    static const int MAX_SPEED = 255;
    static const unsigned long RAMP_TIME_MS = 1000;  // 1 second ramp time
    static const int RAMP_STEP = 5;  // Speed change per update
};

#endif // MOTORCONTROL_H
