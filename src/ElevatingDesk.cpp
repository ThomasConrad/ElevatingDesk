#include <Arduino.h>

#include "ButtonHandler.h"
#include "DeskController.h"
#include "EndStop.h"
#include "HeightDisplay.h"
#include "MotorControl.h"
#include "OpticalEncoder.h"

// Pin definitions for Arduino Nano
const int UP_BUTTON_PIN = 2;       // D2 - Interrupt capable pin for button
const int DOWN_BUTTON_PIN = 3;     // D3 - Interrupt capable pin for button
const int ENDSTOP_PIN = 4;         // D4 - End stop switch
const int ENCODER_PIN_A = 5;       // D5 - Optical encoder sensor pin
// ENCODER_PIN_B no longer needed for optical encoder
const int MOTOR_FORWARD_PIN = 9;   // D9 - PWM capable pin
const int MOTOR_BACKWARD_PIN = 10; // D10 - PWM capable pin

// Note: Arduino Nano has limited memory (2KB SRAM, 32KB Flash)
// - Using PROGMEM for static strings
// - Optimizing buffer sizes
// - Using uint8_t instead of int where possible

// Create component instances
ButtonHandler upButton(UP_BUTTON_PIN);
ButtonHandler downButton(DOWN_BUTTON_PIN);
EndStop endStop(ENDSTOP_PIN);
// Optical encoder with default 10 slits per mm (configurable via calibration)
OpticalEncoder encoder(ENCODER_PIN_A, 10.0f);
MotorControl motor(MOTOR_FORWARD_PIN, MOTOR_BACKWARD_PIN);
HeightDisplay display;

// Create the desk controller
DeskController controller(upButton, downButton, endStop, encoder, motor, display);

void setup() {
  // Initialize serial for debugging
  Serial.begin(9600);

  // Initialize components
  upButton.init();
  downButton.init();
  endStop.init();
  encoder.init();

  // Initialize the controller
  controller.init();
}

void loop() {
  // Update the controller (which updates all components)
  controller.update();

  // Small delay to prevent overwhelming the system
  delay(10);
}
