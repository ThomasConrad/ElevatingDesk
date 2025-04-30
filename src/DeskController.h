#ifndef DESKCONTROLLER_H
#define DESKCONTROLLER_H

#include "ButtonHandler.h"
#include "DeskState.h"
#include "EndStop.h"
#include "HeightDisplay.h"
#include "MotorControl.h"
#include "RotaryEncoder.h"

class DeskController {
public:
  DeskController(ButtonHandler &upButton, ButtonHandler &downButton,
                 EndStop &endStop, RotaryEncoder &encoder, MotorControl &motor,
                 HeightDisplay &display);

  void init();
  void update();

private:
  void handleButtons();
  void handleMovement();
  void handleCalibration();
  void handlePresetMode();
  void updateDisplay();
  void moveToPreset(uint8_t presetIndex);
  void saveCurrentPreset();

  ButtonHandler &upButton;
  ButtonHandler &downButton;
  EndStop &endStop;
  RotaryEncoder &encoder;
  MotorControl &motor;
  HeightDisplay &display;
  DeskState state;
  unsigned long lastButtonPress;

  static const uint8_t MOTOR_SPEED = 200;       // ~78% of max speed
  static const uint8_t PRESET_MOVE_SPEED = 150; // Slower speed for preset moves
  static const unsigned long PRESET_TIMEOUT =
      5000; // 5 seconds timeout for preset mode
};

#endif // DESKCONTROLLER_H
