#ifndef DESKCONTROLLER_H
#define DESKCONTROLLER_H

#include "DeskState.h"
#include "ButtonHandler.h"
#include "EndStop.h"
#include "RotaryEncoder.h"
#include "MotorControl.h"
#include "HeightDisplay.h"

class DeskController {
public:
    DeskController(
        ButtonHandler& upButton,
        ButtonHandler& downButton,
        EndStop& endStop,
        RotaryEncoder& encoder,
        MotorControl& motor,
        HeightDisplay& display
    );
    
    void init();
    void update();
    
private:
    void handleButtons();
    void handleMovement();
    void handleCalibration();
    void updateDisplay();
    
    ButtonHandler& upButton;
    ButtonHandler& downButton;
    EndStop& endStop;
    RotaryEncoder& encoder;
    MotorControl& motor;
    HeightDisplay& display;
    DeskState state;
    
    static const int MOTOR_SPEED = 200;  // ~78% of max speed
};

#endif // DESKCONTROLLER_H
