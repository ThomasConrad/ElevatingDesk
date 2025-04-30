# Elevating Desk Controller

A sophisticated Arduino-based controller for an electric standing desk, featuring precise height control, preset positions, and smooth operation.

## Features

- **Dual Motor Control**: Independent control of two motors for precise desk movement
- **Height Tracking**: Accurate height measurement using rotary encoders
- **Preset Positions**: Three customizable preset heights
- **Smooth Operation**: Ramped motor control for gentle starts and stops
- **Safety Features**: 
  - Emergency stop functionality
  - Collision detection
  - Overcurrent protection
  - Height limit enforcement

## Hardware Requirements

- Arduino Nano
- Two DC motors with encoders
- Two motor drivers (e.g., L298N)
- Two push buttons
- Two rotary encoders
- RGB LED for status indication
- Power supply (24V recommended)

## Pin Configuration

### Motor Control
- Motor 1 PWM: Pin 2
- Motor 1 Direction: Pin 3
- Motor 2 PWM: Pin 4
- Motor 2 Direction: Pin 5

### Encoders
- Encoder 1 A: Pin A0 (14)
- Encoder 1 B: Pin A1 (15)
- Encoder 2 A: Pin A2 (16)
- Encoder 2 B: Pin A3 (17)

### Buttons
- Up Button: Pin 6
- Down Button: Pin 7

### LED
- Red: Pin 8
- Green: Pin 9
- Blue: Pin 10

## Operation

### Basic Controls
- **Up Button**: Press to move desk up
- **Down Button**: Press to move desk down
- **Both Buttons**: Press simultaneously to stop desk movement

### Preset Mode
- **Enter Preset Mode**: Hold both buttons for 2 seconds
- **Cycle Presets**: Press either up or down button to cycle through presets (1-3)
- **Select Preset**: Press both buttons to move desk to selected preset height
- **Exit Preset Mode**: Hold both buttons for 2 seconds

### Setting Presets
1. Enter Preset Mode
2. Select the preset you want to modify
3. Move desk to desired height
4. Hold both buttons for 2 seconds to save current height as selected preset
5. Preset will be saved and persist across power cycles

### LED Indicators
- **Normal Mode**:
  - Green: Desk is at target height
  - Blue: Desk is moving up
  - Red: Desk is moving down
- **Preset Mode**:
  - Slow Blink: In preset selection mode
  - Color indicates selected preset:
    - Red: Preset 1
    - Green: Preset 2
    - Blue: Preset 3
- **Preset Save**:
  - Quick double blink of selected color when preset is saved

## Safety Features

1. **Emergency Stop**:
   - Press both buttons simultaneously to stop all movement
   - System will enter a safe state until reset

2. **Collision Detection**:
   - System monitors motor current
   - Stops movement if resistance is detected
   - Prevents damage to desk or objects

3. **Height Limits**:
   - Software-enforced minimum and maximum heights
   - Prevents desk from moving beyond safe range

4. **Overcurrent Protection**:
   - Monitors motor current
   - Reduces power or stops if current exceeds safe limits

## Installation

1. Connect all hardware components according to pin configuration
2. Upload the sketch to Arduino Nano
3. Calibrate the height sensors if needed
4. Set your preferred preset heights

## Calibration

The system includes automatic calibration on startup:
1. Desk moves to minimum height
2. Height sensors are zeroed
3. System is ready for operation

## Troubleshooting

- If desk doesn't move, check motor connections and power supply
- If height tracking is inaccurate, check encoder connections
- If presets aren't saving, check EEPROM functionality
- If emergency stop triggers unexpectedly, check current sensors

## License

This project is open source and available under the MIT License.
