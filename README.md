# Elevating Desk Controller

An Arduino Nano-based controller for motorized standing desks with precise height tracking and calibration capabilities.

## Features
- Height tracking with rotary encoder (0.3mm precision)
- OLED display showing current height
- Automatic calibration using endstop
- Smooth motor control with PWM
- State persistence in EEPROM
- Debounced button inputs
- Long-press detection for calibration
- Clean, modular architecture

## Hardware Requirements
- Arduino Nano
- SSD1306 OLED Display (128x32)
- 2x Push Buttons (Up/Down)
- Rotary Encoder (10 steps/revolution)
- Endstop Switch
- DC Motor with H-Bridge or similar driver
- Power supply appropriate for your motor

## Pin Configuration
```
Digital Pins:
- Up Button: D2 (INPUT_PULLUP)
- Down Button: D3 (INPUT_PULLUP)
- Endstop: D4 (INPUT_PULLUP)
- Encoder A: D5 (INPUT)
- Encoder B: D6 (INPUT)
- Motor Forward: D9 (PWM OUTPUT)
- Motor Backward: D10 (PWM OUTPUT)

I2C Pins (OLED Display):
- SDA: A4
- SCL: A5
```

## Dependencies
- Adafruit GFX Library
- Adafruit SSD1306
- Arduino EEPROM Library (built-in)

## Installation
1. Install the required libraries using the Arduino Library Manager
2. Connect the hardware according to the pin configuration
3. Upload the code to your Arduino Nano
4. Calibrate the desk by holding the down button until it hits the endstop

## Usage
- **Normal Operation**: Press and hold up/down buttons to move the desk
- **Calibration**: Hold down button for 1 second when uncalibrated
- **Height Display**: Current height is shown in millimeters on the OLED
- **State Persistence**: Calibration and height are saved in EEPROM

## Implementation Details

### Core Components
1. **DeskController**: Main controller coordinating all components
   - Manages state transitions
   - Handles button inputs
   - Controls motor movement
   - Updates display
   - Manages calibration sequence

2. **DeskState**: State management with EEPROM persistence
   - Tracks current height
   - Manages calibration status
   - Handles state transitions (IDLE/MOVING/CALIBRATING)
   - Stores/loads data from EEPROM

3. **ButtonHandler**: Debounced button input
   - Software debouncing (50ms)
   - Long press detection (1000ms)
   - Active-low with internal pullup

4. **RotaryEncoder**: Position tracking
   - 10 steps per revolution
   - 3mm travel per revolution
   - 0.3mm precision
   - Quadrature decoding

5. **MotorControl**: PWM motor control
   - Forward/backward control
   - Variable speed (0-255)
   - Smooth stop function

6. **HeightDisplay**: OLED interface
   - Height display in millimeters
   - Status messages
   - 128x32 resolution

7. **EndStop**: Calibration sensor
   - Active-low with pullup
   - Used for zero-point calibration

## Contributing
Feel free to submit issues and pull requests.

## License
MIT License - feel free to use this code in your own projects.
