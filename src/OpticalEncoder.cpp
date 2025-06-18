#include "OpticalEncoder.h"

OpticalEncoder::OpticalEncoder(uint8_t sensorPin, float slitsPerMM)
    : sensorPin(sensorPin), 
      slitsPerMM(slitsPerMM),
      pulseCount(0),
      lastSensorState(LOW),
      lastPulseTime(0) {}

void OpticalEncoder::init() {
  pinMode(sensorPin, INPUT_PULLUP); // Use pullup for optical sensor
  lastSensorState = digitalRead(sensorPin);
  pulseCount = 0;
  lastPulseTime = millis();
}

void OpticalEncoder::update() {
  int currentState = digitalRead(sensorPin);
  
  // Detect rising edge (light -> dark transition as slit passes)
  if (currentState == LOW && lastSensorState == HIGH) {
    pulseCount++;
    lastPulseTime = millis();
  }
  
  lastSensorState = currentState;
}

long OpticalEncoder::getPulseCount() const {
  return pulseCount;
}

void OpticalEncoder::setPulseCount(long count) {
  pulseCount = count;
}

float OpticalEncoder::getHeightMM() const {
  // Convert pulse count to height: pulses / (slits per mm) = mm
  return static_cast<float>(pulseCount) / slitsPerMM;
}

void OpticalEncoder::setSlitsPerMM(float slitsPerMM) {
  this->slitsPerMM = slitsPerMM;
}

float OpticalEncoder::getSlitsPerMM() const {
  return slitsPerMM;
}

void OpticalEncoder::resetPosition() {
  pulseCount = 0;
  lastPulseTime = millis();
}

unsigned long OpticalEncoder::getLastPulseTime() const {
  return lastPulseTime;
}

bool OpticalEncoder::isMoving() const {
  return (millis() - lastPulseTime) < MOVEMENT_TIMEOUT_MS;
}