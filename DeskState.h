#ifndef DESKSTATE_H
#define DESKSTATE_H

#include <Arduino.h>
#include <EEPROM.h>

class DeskState {
public:
    enum State {
        IDLE,
        MOVING_UP,
        MOVING_DOWN,
        CALIBRATING
    };

    DeskState();
    void init();
    
    // State management
    State getState() const;
    void setState(State newState);
    
    // Position management
    float getCurrentHeight() const;
    void updateHeight(float height);
    bool isCalibrated() const;
    void setCalibrated(bool calibrated);
    
    // Preset management
    void savePreset(uint8_t index, float height);
    float getPreset(uint8_t index) const;
    static const uint8_t MAX_PRESETS = 4;
    
    // EEPROM operations
    void saveToEEPROM();
    void loadFromEEPROM();

private:
    State currentState;
    float currentHeight;
    bool calibrationStatus;
    float presets[MAX_PRESETS];
    
    static const int EEPROM_START_ADDRESS = 0;
    static const int CALIBRATION_FLAG_ADDRESS = EEPROM_START_ADDRESS;
    static const int HEIGHT_ADDRESS = CALIBRATION_FLAG_ADDRESS + sizeof(bool);
    static const int PRESETS_ADDRESS = HEIGHT_ADDRESS + sizeof(float);
};

#endif // DESKSTATE_H
