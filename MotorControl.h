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

private:
    int forwardPin;
    int backwardPin;
    int currentSpeed;
    static const int MIN_SPEED = 0;
    static const int MAX_SPEED = 255;
};

#endif // MOTORCONTROL_H
