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
        CALIBRATING,
        PRESET_MODE,
        PRESET_EDIT_MODE
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
    uint8_t getCurrentPreset() const;
    void setCurrentPreset(uint8_t index);
    void cyclePreset(bool forward);
    static const uint8_t MAX_PRESETS = 3;
    
    // EEPROM operations
    void saveToEEPROM();
    void loadFromEEPROM();

private:
    State currentState;
    float currentHeight;
    bool calibrationStatus;
    float presets[MAX_PRESETS];
    uint8_t currentPreset;
    
    static const int EEPROM_START_ADDRESS = 0;
    static const int CALIBRATION_FLAG_ADDRESS = EEPROM_START_ADDRESS;
    static const int HEIGHT_ADDRESS = CALIBRATION_FLAG_ADDRESS + sizeof(bool);
    static const int PRESETS_ADDRESS = HEIGHT_ADDRESS + sizeof(float);
    static const int CURRENT_PRESET_ADDRESS = PRESETS_ADDRESS + (MAX_PRESETS * sizeof(float));
};

#endif // DESKSTATE_H
