# Standing Desk Controller

Arduino Nano-based standing desk controller with optical encoder height tracking, OLED display, and memory presets.

## Features

- **Motor Control**: PWM-controlled motors with ramping for smooth operation  
- **Height Tracking**: Precise height measurement using optical encoder with slotted wheel
- **OLED Display**: Large, readable height display on 128x64 SSD1306 screen
- **Memory Presets**: 3 programmable height positions stored in EEPROM
- **Safety Features**: 
  - End stop limit switches
  - Smooth motor ramping
  - Height bounds checking
- **Unified Calibration**: Simple two-button workflow for encoder and height setup

## Hardware Requirements

- Arduino Nano (ATmega328P)
- SSD1306 OLED Display (128x64, I2C)
- 2x Momentary Push Buttons  
- Optical Encoder (slotted wheel + light sensor)
- Motor Driver (PWM-capable)
- End Stop Switches
- Standing Desk Motors

## Button Controls

### Normal Operation
- **Up Button**: Move desk up
- **Down Button**: Move desk down
- **Both Buttons (Long Press)**: Enter preset mode
- **Both Buttons (Very Long Press)**: Enter calibration mode

### First Time Setup
- **Down Button (Long Press)**: Enter calibration mode (when uncalibrated)

### Preset Mode
- **Up/Down Buttons**: Navigate through presets (1, 2, 3)
- **Both Buttons (Short Press)**: Move to selected preset
- **Both Buttons (Long Press)**: Save current height as selected preset
- **No Input (5 seconds)**: Exit to normal mode

### Calibration Mode
1. **Step 1 - Set Start Height**:
   - Up/Down: Adjust start height value
   - Both buttons: Confirm and proceed to step 2

2. **Step 2 - Set End Height**:
   - Move desk physically to new position
   - Up/Down: Adjust end height value  
   - Both buttons: Save calibration and exit

## Calibration Process

The unified calibration sets both encoder scaling and height reference in one workflow:

1. Enter calibration mode (both buttons very long press)
2. Set the current height using up/down buttons, confirm with both buttons
3. Physically move the desk up or down to a different position
4. Set the new height using up/down buttons, confirm with both buttons
5. System automatically calculates encoder scaling and saves all settings

This eliminates the need for separate encoder and height calibration procedures.

## Build Commands

```bash
# Build project
pio run

# Upload to Arduino
pio run --target upload

# Monitor serial output
pio device monitor --baud 115200
```

## Features

- **Height Display**: Large, readable text on 128x64 OLED
- **Memory Presets**: 3 programmable height positions
- **Optical Encoder**: Precise height tracking with user-configurable calibration
- **Safety**: End stop switches and smooth motor ramping
- **Persistence**: All settings saved to EEPROM
