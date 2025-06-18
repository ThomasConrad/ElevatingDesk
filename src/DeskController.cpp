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
  handlePresetMode();
  updateDisplay();
}

void DeskController::handleButtons() {
  // Simple button state detection
  bool upPressed = upButton.isPressed();
  bool downPressed = downButton.isPressed();
  bool bothPressed = upPressed && downPressed;
  bool downLong = downButton.isLongPressed();
  bool bothLong = upButton.isBothLongPressed(downButton);
  bool bothVeryLong = upButton.isBothVeryLongPressed(downButton);
  
  switch (state.getState()) {
    case DeskState::IDLE:
      handleIdleButtons(upPressed, downPressed, downLong, bothLong, bothVeryLong);
      break;
      
    case DeskState::PRESET_MODE:
      handlePresetButtons(upPressed, downPressed, bothPressed, bothLong);
      break;
      
    default:
      // For all other states, return to IDLE when no buttons pressed
      if (!upPressed && !downPressed && state.getState() != DeskState::CALIBRATING) {
        state.setState(DeskState::IDLE);
      }
      break;
  }
}

void DeskController::handleIdleButtons(bool up, bool down, bool downLong, bool bothLong, bool bothVeryLong) {
  if (bothVeryLong) {
    // Priority 1: Both buttons very long press -> Full Calibration (encoder + height)
    state.setState(DeskState::CALIBRATING);
    display.showStatusMessage("Entering Calibration", true);
  } else if (bothLong) {
    // Priority 2: Both buttons long press -> Preset mode
    state.setState(DeskState::PRESET_MODE);
    display.showStatusMessage("Entering Presets", true);
  } else if (down && downLong && !state.isCalibrated()) {
    // Priority 3: Down long press when uncalibrated -> Calibration
    state.setState(DeskState::CALIBRATING);
  } else if (up && !down) {
    // Priority 4: Up button alone -> Move up
    state.setState(DeskState::MOVING_UP);
  } else if (down && !up) {
    // Priority 5: Down button alone -> Move down
    state.setState(DeskState::MOVING_DOWN);
  }
}

void DeskController::handlePresetButtons(bool up, bool down, bool both, bool bothLong) {
  static unsigned long lastButtonPress = 0;
  
  if (bothLong) {
    // Save current preset
    saveCurrentPreset();
    state.setState(DeskState::IDLE);
    char message[20];
    snprintf(message, sizeof(message), "Preset %d Saved", state.getCurrentPreset() + 1);
    display.showStatusMessage(message, true);
  } else if (both && !bothLong) {
    // Go to current preset
    moveToPreset(state.getCurrentPreset());
  } else if (up && !down) {
    // Cycle preset forward
    state.cyclePreset(true);
    lastButtonPress = millis();
  } else if (down && !up) {
    // Cycle preset backward
    state.cyclePreset(false);
    lastButtonPress = millis();
  } else if (!up && !down) {
    // No buttons - check timeout
    if (lastButtonPress == 0) {
      lastButtonPress = millis();
    } else if (millis() - lastButtonPress > PRESET_TIMEOUT) {
      state.setState(DeskState::IDLE);
      display.showStatusMessage("Normal Mode", true);
      lastButtonPress = 0;
    }
  } else {
    lastButtonPress = millis();
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
    static uint8_t step = 0; // 0=start height, 1=end height, 2=results
    static float startHeight = 700.0f; // Default start height
    static float endHeight = 800.0f;   // Default end height 
    static long startPulseCount = 0;
    static bool initialized = false;
    
    // Initialize on first entry
    if (!initialized) {
      step = 0;
      // Load current settings if they exist, otherwise use defaults
      float currentSlitsPerMM = state.getEncoderSlitsPerMM();
      if (currentSlitsPerMM > 0.1f && state.isCalibrated()) {
        // Has existing calibration - use current height as start
        startHeight = state.getCurrentHeight();
      } else {
        // No calibration - use default
        startHeight = 700.0f;
      }
      endHeight = startHeight + 100.0f;
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
        // Calculate and save slits per mm + set height offset
        long totalPulses = currentPulseCount - startPulseCount;
        float heightDiff = endHeight - startHeight;
        
        if (heightDiff != 0 && totalPulses != 0) {
          float slitsPerMM = abs(totalPulses) / abs(heightDiff);
          state.setEncoderSlitsPerMM(slitsPerMM);
          encoder.setSlitsPerMM(slitsPerMM);
          
          // Set height offset so current position shows as startHeight
          float currentEncoderHeight = encoder.getHeightMM();
          float heightOffset = startHeight - currentEncoderHeight;
          state.setHeightOffset(heightOffset);
          
          state.setCalibrated(true);
          step = 2;
          
          // Show results for 2 seconds then exit
          delay(2000);
          state.setState(DeskState::IDLE);
          display.showStatusMessage("Calibrated!", true);
          delay(1500);
        }
        
        // Reset for next time
        initialized = false;
        step = 0;
      }
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
      
      case DeskState::CALIBRATING:
        // Handled separately in handleCalibration()
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

