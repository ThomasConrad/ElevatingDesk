#ifndef OPTICALENCODER_H
#define OPTICALENCODER_H

#include <Arduino.h>

class OpticalEncoder {
public:
  OpticalEncoder(uint8_t sensorPin, float slitsPerMM = 10.0f);
  void init();
  void update();
  long getPulseCount() const;
  void setPulseCount(long count);
  float getHeightMM() const;
  
  // Configuration methods
  void setSlitsPerMM(float slitsPerMM);
  float getSlitsPerMM() const;
  
  // Calibration and diagnostics
  void resetPosition();
  unsigned long getLastPulseTime() const;
  bool isMoving() const; // Detects if pulses received recently

private:
  uint8_t sensorPin;
  float slitsPerMM; // Number of encoder slits per mm of desk movement
  long pulseCount;
  int lastSensorState;
  unsigned long lastPulseTime;
  
  // Movement detection
  static const unsigned long MOVEMENT_TIMEOUT_MS = 100; // 100ms without pulses = stopped
};

#endif // OPTICALENCODER_H
