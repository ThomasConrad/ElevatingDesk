#include "DeskController.h"

DeskController::DeskController(
    ButtonHandler& upButton,
    ButtonHandler& downButton,
    EndStop& endStop,
    RotaryEncoder& encoder,
    MotorControl& motor,
    HeightDisplay& display
)
    : upButton(upButton)
    , downButton(downButton)
    , endStop(endStop)
    , encoder(encoder)
    , motor(motor)
    , display(display)
    , state() {
}

void DeskController::init() {
    state.init();
    motor.init();
    display.init();
    
    if (!state.isCalibrated()) {
        display.showMessage("Please calibrate");
    }
}

void DeskController::update() {
    upButton.update();
    downButton.update();
    encoder.update();
    motor.update();
    
    handleButtons();
    handleMovement();
    handleCalibration();
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
        } else if (upButton.isBothLongPressed(downButton)) {
            state.setState(DeskState::PRESET_MODE);
            display.showMessage("Preset Mode");
        }
    } else if (state.getState() == DeskState::PRESET_MODE) {
        if (upButton.isBothLongPressed(downButton)) {
            saveCurrentPreset();
            state.setState(DeskState::IDLE);
            display.showMessage("Preset " + String(state.getCurrentPreset() + 1) + " Saved");
        } else if (upButton.isBothPressed(downButton) && !upButton.isLongPressed(downButton)) {
            moveToPreset(state.getCurrentPreset());
        } else if (upButton.isPressed() && !downButton.isPressed()) {
            state.cyclePreset(true);
            display.showMessage("Preset " + String(state.getCurrentPreset() + 1));
        } else if (downButton.isPressed() && !upButton.isPressed()) {
            state.cyclePreset(false);
            display.showMessage("Preset " + String(state.getCurrentPreset() + 1));
        } else if (!upButton.isPressed() && !downButton.isPressed()) {
            // Exit preset mode without saving if no buttons are pressed for 5 seconds
            static unsigned long lastButtonPress = 0;
            if (lastButtonPress == 0) {
                lastButtonPress = millis();
            } else if (millis() - lastButtonPress > 5000) {
                state.setState(DeskState::IDLE);
                display.showMessage("Normal Mode");
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
            
        case DeskState::PRESET_MODE:
            motor.stop();
            break;
    }
}

void DeskController::handleCalibration() {
    if (state.getState() == DeskState::CALIBRATING) {
        if (endStop.isTriggered()) {
            motor.stop();
            encoder.setPosition(0);  // Reset position to 0 at bottom
            state.setCalibrated(true);
            state.setState(DeskState::IDLE);
            display.showMessage("Calibrated");
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
            display.showMessage("Preset " + String(state.getCurrentPreset() + 1) + " Saved");
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
        if (state.getState() == DeskState::PRESET_MODE) {
            display.showMessage("Preset " + String(state.getCurrentPreset() + 1));
        } else {
            display.showHeight(state.getCurrentHeight());
        }
    }
}
