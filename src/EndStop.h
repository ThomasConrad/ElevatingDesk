#ifndef ENDSTOP_H
#define ENDSTOP_H

#include <Arduino.h>

class EndStop {
public:
    EndStop(uint8_t pin);
    void init();
    bool isTriggered() const;
    
private:
    uint8_t pin;
};

#endif // ENDSTOP_H
