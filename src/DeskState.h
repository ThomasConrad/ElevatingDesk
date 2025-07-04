#ifndef DESKSTATE_H
#define DESKSTATE_H

#include <Arduino.h>
#include <EEPROM.h>

class DeskState {
public:
  enum State { IDLE, MOVING_UP, MOVING_DOWN, CALIBRATING, PRESET_MODE, PRESET_EDIT_MODE };

  DeskState();
  void init();

  // State management
  State getState() const;
  void setState(State newState);

  // Position management
  float getCurrentHeight() const;
  void updateHeight(float height);
  float getHeightOffset() const;
  void setHeightOffset(float offset);
  void adjustHeightOffset(float delta);
  bool isCalibrated() const;
  void setCalibrated(bool calibrated);

  // Preset management
  void savePreset(uint8_t index, float height);
  float getPreset(uint8_t index) const;
  uint8_t getCurrentPreset() const;
  void setCurrentPreset(uint8_t index);
  void cyclePreset(bool forward);
  static const uint8_t MAX_PRESETS = 3;

  // Encoder configuration
  void setEncoderSlitsPerMM(float slitsPerMM);
  float getEncoderSlitsPerMM() const;
  
  // EEPROM operations
  void saveToEEPROM();
  void loadFromEEPROM();

private:
  State currentState;
  float currentHeight;
  float heightOffset; // Offset to adjust displayed height
  bool calibrationStatus;
  float presets[MAX_PRESETS];
  uint8_t currentPreset;
  
  // Encoder configuration
  float encoderSlitsPerMM;

  static const int EEPROM_START_ADDRESS = 0;
  static const int CALIBRATION_FLAG_ADDRESS = EEPROM_START_ADDRESS;
  static const int HEIGHT_ADDRESS = CALIBRATION_FLAG_ADDRESS + sizeof(bool);
  static const int HEIGHT_OFFSET_ADDRESS = HEIGHT_ADDRESS + sizeof(float);
  static const int PRESETS_ADDRESS = HEIGHT_OFFSET_ADDRESS + sizeof(float);
  static const int CURRENT_PRESET_ADDRESS = PRESETS_ADDRESS + (MAX_PRESETS * sizeof(float));
  static const int ENCODER_SLITS_PER_MM_ADDRESS = CURRENT_PRESET_ADDRESS + sizeof(uint8_t);
};

#endif // DESKSTATE_H
