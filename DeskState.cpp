#include "DeskState.h"

DeskState::DeskState() 
    : currentState(IDLE)
    , currentHeight(0.0f)
    , calibrationStatus(false) {
    for (uint8_t i = 0; i < MAX_PRESETS; i++) {
        presets[i] = 0.0f;
    }
}

void DeskState::init() {
    loadFromEEPROM();
}

DeskState::State DeskState::getState() const {
    return currentState;
}

void DeskState::setState(State newState) {
    currentState = newState;
}

float DeskState::getCurrentHeight() const {
    return currentHeight;
}

void DeskState::updateHeight(float height) {
    currentHeight = height;
    saveToEEPROM();
}

bool DeskState::isCalibrated() const {
    return calibrationStatus;
}

void DeskState::setCalibrated(bool calibrated) {
    calibrationStatus = calibrated;
    saveToEEPROM();
}

void DeskState::savePreset(uint8_t index, float height) {
    if (index < MAX_PRESETS) {
        presets[index] = height;
        saveToEEPROM();
    }
}

float DeskState::getPreset(uint8_t index) const {
    if (index < MAX_PRESETS) {
        return presets[index];
    }
    return 0.0f;
}

void DeskState::saveToEEPROM() {
    EEPROM.put(CALIBRATION_FLAG_ADDRESS, calibrationStatus);
    EEPROM.put(HEIGHT_ADDRESS, currentHeight);
    EEPROM.put(PRESETS_ADDRESS, presets);
}

void DeskState::loadFromEEPROM() {
    EEPROM.get(CALIBRATION_FLAG_ADDRESS, calibrationStatus);
    EEPROM.get(HEIGHT_ADDRESS, currentHeight);
    EEPROM.get(PRESETS_ADDRESS, presets);
}
