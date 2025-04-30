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
    
    handleButtons();
    handleMovement();
    handleCalibration();
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

void DeskController::updateDisplay() {
    if (state.isCalibrated()) {
        display.showHeight(state.getCurrentHeight());
    }
}
