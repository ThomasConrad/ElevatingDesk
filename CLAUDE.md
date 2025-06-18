# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an Arduino Nano-based standing desk controller with sophisticated features including height tracking, preset positions, and safety mechanisms. The system uses a component-based architecture with clear separation of concerns.

## Build and Development Commands

```bash
# Build the project
pio run

# Upload to Arduino Nano
pio run --target upload

# Monitor serial output
pio device monitor --baud 115200

# Clean build artifacts
pio run --target clean

# Build specific environment
pio run -e nano
```

## Architecture

### Core Components
- **DeskController**: Main orchestrator that coordinates all components and implements the state machine
- **DeskState**: Manages system state, height tracking, presets, and EEPROM persistence
- **MotorControl**: Handles PWM motor control with ramping for smooth operation
- **ButtonHandler**: Debounced button input with support for short/long/very-long press detection
- **RotaryEncoder**: Height tracking via encoder pulses
- **EndStop**: Safety limit switch handling
- **HeightDisplay**: OLED display management (SSD1306)

### State Machine
The system operates through clearly defined states:
- `IDLE`: Normal operation, waiting for input
- `MOVING_UP/DOWN`: Active movement with motor control
- `CALIBRATING`: Initial position calibration sequence
- `CALIBRATING_HEIGHT`: Height offset adjustment mode
- `PRESET_MODE`: Preset selection and navigation
- `PRESET_EDIT_MODE`: Preset height setting mode

### Memory Management
Arduino Nano has limited resources (2KB SRAM, 32KB Flash):
- Uses `uint8_t` instead of `int` where possible
- EEPROM storage for persistent data (calibration, presets)
- Optimized buffer sizes and memory usage patterns

### Hardware Configuration
- Target: Arduino Nano (ATmega328P)
- Motors: Dual PWM-controlled motors with ramping
- Encoders: Rotary encoders for height tracking
- Display: SSD1306 OLED via I2C
- Buttons: Interrupt-capable pins for responsive input
- Safety: End stops and overcurrent protection

## Key Design Patterns

### Component Composition
The `DeskController` uses dependency injection, receiving all hardware components via constructor. This enables easy testing and modularity.

### State-Driven Logic
All major functionality is driven by the central state machine in `DeskState`, ensuring predictable behavior and preventing race conditions.

### Safety-First Design
Multiple safety mechanisms:
- Emergency stop via simultaneous button press
- End stop limit switches
- Motor current monitoring
- Software height limits
- Timeout-based preset mode exit

## Development Notes

- Pin assignments are defined in `ElevatingDesk.cpp` main file
- EEPROM layout is carefully managed in `DeskState` to prevent data corruption
- Motor ramping prevents mechanical stress and provides smooth operation
- Button handling supports multiple press types (short/long/very-long) for UI navigation
- Serial debugging available at 115200 baud
- Enhanced OLED UI with visual hierarchy, animations, and mode-specific layouts

## Testing Hardware

When developing, ensure:
1. Motors are properly connected and powered
2. Encoders are wired correctly for direction sensing
3. End stops are normally-open switches
4. OLED display is connected via I2C (usually A4/A5 on Nano)
5. Power supply can handle motor current draw