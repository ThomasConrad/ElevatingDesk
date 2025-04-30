#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

#include <Arduino.h>

class RotaryEncoder {
public:
    RotaryEncoder(uint8_t pinA, uint8_t pinB);
    void init();
    void update();
    long getPosition() const;
    void setPosition(long position);
    float getHeightMM() const;
    
private:
    uint8_t pinA;
    uint8_t pinB;
    int lastStateA;
    long position;
    
    // Constants for height calculation
    static constexpr float MM_PER_REVOLUTION = 3.0f;  // 3mm per revolution
    static const int STEPS_PER_REVOLUTION = 10;   // 10 steps per revolution
    static const float MM_PER_STEP;               // Calculated in cpp file
};

#endif // ROTARYENCODER_H
