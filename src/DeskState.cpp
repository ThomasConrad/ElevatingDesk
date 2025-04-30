#include "DeskState.h"

DeskState::DeskState()
    : currentState(IDLE), currentHeight(0.0f), calibrationStatus(false),
      currentPreset(0) {
  for (uint8_t i = 0; i < MAX_PRESETS; i++) {
    presets[i] = 0.0f;
  }
}

void DeskState::init() { loadFromEEPROM(); }

DeskState::State DeskState::getState() const { return currentState; }

void DeskState::setState(State newState) { currentState = newState; }

float DeskState::getCurrentHeight() const { return currentHeight; }

void DeskState::updateHeight(float height) {
  currentHeight = height;
  saveToEEPROM();
}

bool DeskState::isCalibrated() const { return calibrationStatus; }

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

uint8_t DeskState::getCurrentPreset() const { return currentPreset; }

void DeskState::setCurrentPreset(uint8_t index) {
  if (index < MAX_PRESETS) {
    currentPreset = index;
    saveToEEPROM();
  }
}

void DeskState::cyclePreset(bool forward) {
  if (forward) {
    currentPreset = (currentPreset + 1) % MAX_PRESETS;
  } else {
    currentPreset = (currentPreset == 0) ? MAX_PRESETS - 1 : currentPreset - 1;
  }
  saveToEEPROM();
}

void DeskState::saveToEEPROM() {
  EEPROM.put(CALIBRATION_FLAG_ADDRESS, calibrationStatus);
  EEPROM.put(HEIGHT_ADDRESS, currentHeight);
  EEPROM.put(PRESETS_ADDRESS, presets);
  EEPROM.put(CURRENT_PRESET_ADDRESS, currentPreset);
}

void DeskState::loadFromEEPROM() {
  EEPROM.get(CALIBRATION_FLAG_ADDRESS, calibrationStatus);
  EEPROM.get(HEIGHT_ADDRESS, currentHeight);
  EEPROM.get(PRESETS_ADDRESS, presets);
  EEPROM.get(CURRENT_PRESET_ADDRESS, currentPreset);
}
