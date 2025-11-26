# Modbus Servo Control System for ESP32

A comprehensive Modbus servo control implementation for ESP32 that handles 5 servo data registers (20-24) with complete communication flow.

## Features

- **Modbus RTU Communication**: Full Modbus RTU protocol support using the ModbusMaster library
- **Servo Control**: Control up to 5 servo motors via Modbus registers
- **Register Mapping**: Dedicated registers 20-24 for servo control
- **HEX Command Processing**: Process HEX commands from master devices
- **Servo Actuation**: Execute servo control via `servo.write()` function
- **Complete Example**: Detailed working example for register #21 servo address

## Hardware Requirements

- ESP32 microcontroller
- 5 servo motors
- RS232/485 communication interface
- Modbus master device (PC, PLC, HMI, etc.)

## Software Requirements

- Arduino IDE with PlatformIO support
- ModbusMaster library (included in platformio.ini)
- Servo library (included in platformio.ini)

## Project Structure

```
ModbusTesESP32/
├── platformio.ini          # Project configuration
├── src/
│   └── main.cpp           # Main implementation
├── docs/
│   └── modbus_servo_control.md  # Detailed documentation
└── examples/
    └── servo_control_example.ino  # Complete example code
```

## Register Mapping

| Register | Description     | Usage                     |
| -------- | --------------- | ------------------------- |
| 20       | Servo 0 Control | Control servo 0           |
| 21       | Servo 1 Control | Control servo 1 (example) |
| 22       | Servo 2 Control | Control servo 2           |
| 23       | Servo 3 Control | Control servo 3           |
| 24       | Servo 4 Control | Control servo 4           |

### Register Data Format

Each register uses a 16-bit value:

- **Bits 0-7**: Target angle (0-180 degrees)
- **Bits 8-15**: Control command (0=stop, 1=start, 2=move)

## Usage Example

To move servo 1 (register 21) to 90 degrees:

1. Send Modbus write command to register 21
2. Value: 0x015A (0x01 = move command, 0x5A = 90 degrees)
3. ESP32 processes command and moves servo to 90 degrees

## Implementation Details

### Communication Flow

1. **Command Reception**: Master device sends Modbus write command to registers 20-24
2. **Data Processing**: ESP32 decodes the 16-bit value into angle and command
3. **Register Storage**: Values are stored in holding registers for monitoring
4. **Servo Actuation**: `servo.write()` function is called to move servos to target positions
5. **Response**: System acknowledges command and updates status

### Key Functions

- `processServoControl()`: Monitors servo control registers and triggers actions
- `handleServoControl()`: Processes individual servo commands
- `moveServo1ToAngle()`: Example function for register #21

## Installation

1. Clone this repository
2. Open with PlatformIO IDE or Arduino IDE
3. Connect hardware components
4. Upload to ESP32
5. Configure Modbus master device to communicate with ESP32

## Testing

1. Connect 5 servo motors to GPIO pins 2, 4, 12, 14, and 16
2. Configure Modbus master device to communicate with ESP32 (slave ID 1)
3. Send Modbus write commands to registers 20-24
4. Verify servo movements and system logs

## Documentation

- [Complete Implementation Guide](docs/modbus_servo_control.md)
- [Example Usage](examples/servo_control_example.ino)

## License

MIT License
