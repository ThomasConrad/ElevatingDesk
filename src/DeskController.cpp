#include "DeskController.h"

DeskController::DeskController(ButtonHandler& upButton, ButtonHandler& downButton, EndStop& endStop,
                               RotaryEncoder& encoder, MotorControl& motor, HeightDisplay& display)
    : upButton(upButton), downButton(downButton), endStop(endStop), encoder(encoder), motor(motor), display(display),
      state() {}

void DeskController::init() {
  state.init();
  motor.init();
  display.init();

  if (!state.isCalibrated()) {
    display.showStatusMessage("Please calibrate", false);
  }
}

void DeskController::update() {
  upButton.update();
  downButton.update();
  encoder.update();
  motor.update();
  display.update(); // Update display animations

  handleButtons();
  handleMovement();
  handleCalibration();
  handleHeightCalibration();
  handlePresetMode();
  updateDisplay();
}

void DeskController::handleButtons() {
  if (state.getState() == DeskState::IDLE) {
    if (upButton.isPressed()) {
      state.setState(DeskState::MOVING_UP);
    } else if (downButton.isPressed()) {
      if (downButton.isLongPressed() && !state.isCalibrated()) {
        state.setState(DeskState::CALIBRATING);
      } else {
        state.setState(DeskState::MOVING_DOWN);
      }
    } else if (upButton.isBothVeryLongPressed(downButton)) {
      state.setState(DeskState::CALIBRATING_HEIGHT);
      display.showStatusMessage("Entering Height Cal", true);
    } else if (upButton.isBothLongPressed(downButton)) {
      state.setState(DeskState::PRESET_MODE);
      display.showStatusMessage("Entering Presets", true);
    }
  } else if (state.getState() == DeskState::PRESET_MODE) {
    if (upButton.isBothLongPressed(downButton)) {
      saveCurrentPreset();
      state.setState(DeskState::IDLE);
      char message[20];
      snprintf(message, sizeof(message), "Preset %d Saved", state.getCurrentPreset() + 1);
      display.showStatusMessage(message, true);
    } else if (upButton.isBothPressed(downButton) && !upButton.isLongPressed()) {
      moveToPreset(state.getCurrentPreset());
    } else if (upButton.isPressed() && !downButton.isPressed()) {
      state.cyclePreset(true);
      // Display handled in updateDisplay() method
    } else if (downButton.isPressed() && !upButton.isPressed()) {
      state.cyclePreset(false);
      // Display handled in updateDisplay() method
    } else if (!upButton.isPressed() && !downButton.isPressed()) {
      // Exit preset mode without saving if no buttons are pressed for 5 seconds
      static unsigned long lastButtonPress = 0;
      if (lastButtonPress == 0) {
        lastButtonPress = millis();
      } else if (millis() - lastButtonPress > 5000) {
        state.setState(DeskState::IDLE);
        display.showStatusMessage("Normal Mode", true);
        lastButtonPress = 0;
      }
    } else {
      lastButtonPress = millis();
    }
  } else if (!upButton.isPressed() && !downButton.isPressed()) {
    if (state.getState() != DeskState::CALIBRATING) {
      state.setState(DeskState::IDLE);
    }
  }
}

void DeskController::handleMovement() {
  float currentHeight = encoder.getHeightMM();
  state.updateHeight(currentHeight);

  switch (state.getState()) {
  case DeskState::MOVING_UP:
    motor.forward(MOTOR_SPEED);
    break;

  case DeskState::MOVING_DOWN:
    motor.backward(MOTOR_SPEED);
    break;

  case DeskState::IDLE:
    motor.stop();
    break;

  case DeskState::CALIBRATING:
    // Handled in handleCalibration()
    break;

  case DeskState::CALIBRATING_HEIGHT:
    // Handled in handleHeightCalibration()
    motor.stop();
    break;

  case DeskState::PRESET_MODE:
    motor.stop();
    break;

  case DeskState::PRESET_EDIT_MODE:
    motor.stop();
    break;
  }
}

void DeskController::handleCalibration() {
  if (state.getState() == DeskState::CALIBRATING) {
    if (endStop.isTriggered()) {
      motor.stop();
      encoder.setPosition(0); // Reset position to 0 at bottom
      state.setCalibrated(true);
      state.setState(DeskState::IDLE);
      display.showStatusMessage("Calibrated!", true);
    } else {
      motor.backward(MOTOR_SPEED);
    }
  }
}

void DeskController::handlePresetMode() {
  if (state.getState() == DeskState::PRESET_MODE) {
    // Check if we should save the current preset
    if (upButton.isBothLongPressed(downButton)) {
      saveCurrentPreset();
      char message[20];
      snprintf(message, sizeof(message), "Preset %d Saved", state.getCurrentPreset() + 1);
      display.showStatusMessage(message, true);
    }
  }
}

void DeskController::moveToPreset(uint8_t presetIndex) {
  float targetHeight = state.getPreset(presetIndex);
  float currentHeight = state.getCurrentHeight();

  if (targetHeight > currentHeight) {
    state.setState(DeskState::MOVING_UP);
    motor.forward(PRESET_MOVE_SPEED);
  } else if (targetHeight < currentHeight) {
    state.setState(DeskState::MOVING_DOWN);
    motor.backward(PRESET_MOVE_SPEED);
  }

  // The movement will be stopped by handleMovement() when we reach the target
}

void DeskController::saveCurrentPreset() {
  state.savePreset(state.getCurrentPreset(), state.getCurrentHeight());
}

void DeskController::updateDisplay() {
  if (state.isCalibrated()) {
    switch (state.getState()) {
      case DeskState::PRESET_MODE:
        display.showPresetMode(state.getCurrentPreset() + 1, state.getPreset(state.getCurrentPreset()));
        break;
      
      case DeskState::CALIBRATING_HEIGHT:
        // Handled separately in handleHeightCalibration()
        break;
        
      case DeskState::MOVING_UP:
      case DeskState::MOVING_DOWN:
        display.showHeight(state.getCurrentHeight(), true);
        break;
        
      default:
        display.showHeight(state.getCurrentHeight(), false);
        break;
    }
  }
}

void DeskController::handleHeightCalibration() {
  if (state.getState() == DeskState::CALIBRATING_HEIGHT) {
    static bool showingInstructions = false;
    static unsigned long lastToggleTime = 0;

    // Toggle display every 3 seconds
    if (millis() - lastToggleTime > 3000) {
      lastToggleTime = millis();
      showingInstructions = !showingInstructions;
    }

    // Show calibration screen with alternating content
    display.showCalibrationMode(state.getCurrentHeight(), showingInstructions);

    // Handle button presses for adjustment
    if (upButton.isPressed() && !downButton.isPressed()) {
      state.adjustHeightOffset(0.5);
      showingInstructions = false; // Show height immediately after adjustment
      lastToggleTime = millis();
    } else if (downButton.isPressed() && !upButton.isPressed()) {
      state.adjustHeightOffset(-0.5);
      showingInstructions = false; // Show height immediately after adjustment
      lastToggleTime = millis();
    }

    // Exit calibration mode with long press of both buttons
    if (upButton.isBothLongPressed(downButton)) {
      state.setState(DeskState::IDLE);
      display.showStatusMessage("Cal Saved!", true);
      delay(1500); // Show the message for 1.5 seconds
    }
  }
}
