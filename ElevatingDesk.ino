#include "DeskController.h"
#include "ButtonHandler.h"
#include "EndStop.h"
#include "RotaryEncoder.h"
#include "MotorControl.h"
#include "HeightDisplay.h"

// Pin definitions
const int UP_BUTTON_PIN = 2;
const int DOWN_BUTTON_PIN = 3;
const int ENDSTOP_PIN = 4;
const int ENCODER_PIN_A = 5;
const int ENCODER_PIN_B = 6;
const int MOTOR_FORWARD_PIN = 9;
const int MOTOR_BACKWARD_PIN = 10;

// Create component instances
ButtonHandler upButton(UP_BUTTON_PIN);
ButtonHandler downButton(DOWN_BUTTON_PIN);
EndStop endStop(ENDSTOP_PIN);
RotaryEncoder encoder(ENCODER_PIN_A, ENCODER_PIN_B);
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
