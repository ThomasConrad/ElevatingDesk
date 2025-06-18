#include "DeskController.h"

DeskController::DeskController(ButtonHandler& upButton, ButtonHandler& downButton, EndStop& endStop,
                               OpticalEncoder& encoder, MotorControl& motor, HeightDisplay& display)
    : upButton(upButton), downButton(downButton), endStop(endStop), encoder(encoder), motor(motor), display(display),
      state() {}

void DeskController::init() {
  state.init();
  motor.init();
  display.init();
  
  // Configure encoder from saved settings
  encoder.setSlitsPerMM(state.getEncoderSlitsPerMM());

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
  handleEncoderCalibration();
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
    } else if (upButton.isBothLongPressed(downButton) && upButton.isVeryLongPressed()) {
      state.setState(DeskState::CALIBRATING_ENCODER);
      display.showStatusMessage("Entering Encoder Cal", true);
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

  case DeskState::CALIBRATING_ENCODER:
    // Handled in handleEncoderCalibration()
    motor.stop();
    break;
  }
}

void DeskController::handleCalibration() {
  if (state.getState() == DeskState::CALIBRATING) {
    if (endStop.isTriggered()) {
      motor.stop();
      encoder.setPulseCount(0); // Reset encoder position to 0 at bottom
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

      case DeskState::CALIBRATING_ENCODER:
        // Handled separately in handleEncoderCalibration()
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

void DeskController::handleEncoderCalibration() {
  if (state.getState() == DeskState::CALIBRATING_ENCODER) {
    static uint8_t step = 0; // 0=start height, 1=end height, 2=results
    static float startHeight = 700.0f; // Default start height
    static float endHeight = 800.0f;   // Default end height 
    static long startPulseCount = 0;
    static bool initialized = false;
    
    // Initialize on first entry
    if (!initialized) {
      step = 0;
      startHeight = 700.0f;
      endHeight = 800.0f;
      initialized = true;
    }
    
    // Show current step
    long currentPulseCount = encoder.getPulseCount();
    display.showEncoderCalibrationMode(step, startHeight, endHeight, currentPulseCount - startPulseCount);
    
    if (step == 0) {
      // Step 1: Set start height
      if (upButton.isPressed() && !downButton.isPressed()) {
        startHeight += 1.0f;
        if (startHeight > 1200.0f) startHeight = 1200.0f;
      } else if (downButton.isPressed() && !upButton.isPressed()) {
        startHeight -= 1.0f;
        if (startHeight < 600.0f) startHeight = 600.0f;
      } else if (upButton.isBothPressed(downButton) && !upButton.isLongPressed()) {
        // Record start position and move to next step
        startPulseCount = encoder.getPulseCount();
        step = 1;
        endHeight = startHeight + 100.0f; // Default 100mm difference
      }
      
    } else if (step == 1) {
      // Step 2: Set end height (after moving desk)
      if (upButton.isPressed() && !downButton.isPressed()) {
        endHeight += 1.0f;
        if (endHeight > 1200.0f) endHeight = 1200.0f;
      } else if (downButton.isPressed() && !upButton.isPressed()) {
        endHeight -= 1.0f;
        if (endHeight < 600.0f) endHeight = 600.0f;
      } else if (upButton.isBothPressed(downButton) && !upButton.isLongPressed()) {
        // Calculate and save slits per mm
        long totalPulses = currentPulseCount - startPulseCount;
        float heightDiff = endHeight - startHeight;
        
        if (heightDiff != 0 && totalPulses > 0) {
          float slitsPerMM = totalPulses / heightDiff;
          state.setEncoderSlitsPerMM(slitsPerMM);
          encoder.setSlitsPerMM(slitsPerMM);
          step = 2;
          
          // Show results for 2 seconds then exit
          delay(2000);
          state.setState(DeskState::IDLE);
          display.showStatusMessage("Encoder Saved!", true);
          delay(1500);
        }
        
        // Reset for next time
        initialized = false;
        step = 0;
      }
    }
  }
}