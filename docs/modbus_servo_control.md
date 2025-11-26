# Modbus Servo Control Implementation for ESP32

## Overview

This implementation provides a comprehensive Modbus servo control system for ESP32 that handles 5 servo data registers (20-24) with complete communication flow. The system processes HEX commands from master devices through the Modbus library, stores decoded values in holding registers, executes servo control via `servo.write()` function, and includes a detailed working example.

## System Architecture

### Hardware Components

- ESP32 microcontroller
- 5 servo motors connected to GPIO pins 2, 4, 12, 14, and 16
- Modbus RTU communication via Serial2 (RS232/485 interface)

### Software Components

- ModbusRTU library for Modbus communication
- Servo library for servo motor control
- Custom implementation for register handling and servo control

## Register Mapping

| Register Address | Description     | Data Format            | Usage           |
| ---------------- | --------------- | ---------------------- | --------------- |
| 20               | Servo 0 Control | 16-bit value           | Control servo 0 |
| 21               | Servo 1 Control | 16-bit value (example) | Control servo 1 |
| 22               | Servo 2 Control | 16-bit value           | Control servo 2 |
| 23               | Servo 3 Control | 16-bit value           | Control servo 3 |
| 24               | Servo 4 Control | 16-bit value           | Control servo 4 |

### Register Data Format

Each register uses a 16-bit value with the following structure:

- **Bits 0-7 (Lower 8 bits)**: Target angle (0-180 degrees)
- **Bits 8-15 (Upper 8 bits)**: Control command (0=stop, 1=start, 2=move)

## Communication Flow

1. **Command Reception**: Master device sends Modbus write command to registers 20-24
2. **Data Processing**: ESP32 decodes the 16-bit value into angle and command
3. **Register Storage**: Values are stored in holding registers for monitoring
4. **Servo Actuation**: `servo.write()` function is called to move servos to target positions
5. **Response**: System acknowledges command and updates status

## Implementation Details

### Key Functions

#### `processServoControl()`

- Monitors changes in servo control registers (20-24)
- Triggers appropriate servo control actions when values change
- Handles all 5 servos with individual control

#### `handleServoControl(servoIndex, controlValue)`

- Decodes 16-bit control value into angle and command
- Executes appropriate servo action based on command
- Supports stop, start, and move commands

#### `moveServo1ToAngle(angle)`

- Example function demonstrating how to write to register #21
- Shows how to format values for Modbus communication

### Example Usage

To move servo 1 (register 21) to 90 degrees:

1. Master sends Modbus write command to register 21
2. Value written: 0x015A (0x01 = move command, 0x5A = 90 degrees)
3. ESP32 processes the command and moves servo to 90 degrees

## Code Structure

### main.cpp Components

1. **Initialization**:

   - Sets up Modbus communication on Serial2
   - Initializes 5 servo motors on GPIO pins 2, 4, 12, 14, 16
   - Creates holding registers 0-24 with appropriate initial values

2. **Main Loop**:

   - Processes Modbus requests
   - Calls servo control processing function
   - Maintains system responsiveness

3. **Servo Control Functions**:
   - `processServoControl()`: Main servo control logic
   - `handleServoControl()`: Individual servo command handling
   - `moveServo1ToAngle()`: Example function for register #21

## Operational Flow

1. **System Initialization**:

   - Modbus master is configured with slave ID 1
   - Servo motors are attached to GPIO pins
   - All servos are initialized to 90 degrees (neutral position)
   - Holding registers 0-24 are created with appropriate values

2. **Command Processing**:

   - Modbus requests are processed in the main loop
   - Servo control registers are monitored for changes
   - When a change is detected, the appropriate servo action is executed

3. **Servo Execution**:
   - Angle values are validated (0-180 degrees)
   - Servo motors are moved using `servo.write()`
   - System logs control actions for debugging

## Testing and Verification

To test the implementation:

1. **Hardware Setup**:

   - Connect 5 servo motors to GPIO pins 2, 4, 12, 14, 16
   - Configure Modbus master device to communicate with ESP32

2. **Register Testing**:

   - Write to register 21 with value 0x015A (move servo 1 to 90 degrees)
   - Verify servo moves to correct position
   - Write to register 21 with value 0x0000 (stop servo)
   - Verify servo stops at current position

3. **System Monitoring**:
   - Monitor Serial output for control logs
   - Verify holding registers contain correct values
   - Test all 5 servo registers for proper operation

## Error Handling

The implementation includes:

- Angle validation (0-180 degrees)
- Command validation (0-2)
- Register boundary checking
- Serial logging for debugging

## Future Enhancements

1. Add support for more servo control parameters
2. Implement position feedback registers
3. Add safety limits and emergency stop functionality
4. Include calibration routines for servo motors
5. Add status registers for servo health monitoring
