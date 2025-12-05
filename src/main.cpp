#include <Arduino.h>
#include <ModbusRTU.h>
#include <Servo.h>

// Create Modbus master instance
ModbusRTU mb;

// Create servo instances for 6 servos
Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

// Define servo control registers (20-25)
#define SERVO_REG_20 20  // Servo 0 control register
#define SERVO_REG_21 21  // Servo 1 control register (example)
#define SERVO_REG_22 22  // Servo 2 control register
#define SERVO_REG_23 23  // Servo 3 control register
#define SERVO_REG_24 24  // Servo 4 control register
#define SERVO_REG_25 25  // Servo 5 control register

// Define servo pin assignments
#define SERVO_PIN_0 2
#define SERVO_PIN_1 4
#define SERVO_PIN_2 12
#define SERVO_PIN_3 14
#define SERVO_PIN_4 18
#define SERVO_PIN_5 19

// Define servo angle ranges
#define MIN_ANGLE 0
#define MAX_ANGLE 180

// Global variables to store servo control data
uint16_t servoControlData[6] = {0, 0, 0, 0, 0, 0};  // For registers 20-25

// Function prototypes
void handleServoControl(uint8_t servoIndex, uint16_t controlValue);
void processServoControl();

void setup() {
  // Use Serial2 for CP210x USB-to-UART bridge on COM4
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX, TX pins for Serial2 on ESP32
  mb.begin(&Serial2);
  mb.slave(1); // Set slave ID to 1

  // Initialize servo pins
  servo0.attach(SERVO_PIN_0);
  servo1.attach(SERVO_PIN_1);
  servo2.attach(SERVO_PIN_2);
  servo3.attach(SERVO_PIN_3);
  servo4.attach(SERVO_PIN_4);
  servo5.attach(SERVO_PIN_5);

  // Initialize all servos to neutral position
  servo0.write(90);
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);
  servo5.write(90);

  // Add holding registers (addresses 0-25)
  // We'll add registers 0-19 for general use, and 20-24 for servo control
  for (int i = 0; i < 25; i++) {
    if (i < 20) {
      mb.addHreg(i, i * 10); // Initialize with values 0, 10, 20, ..., 190
    } else {
      mb.addHreg(i, 0); // Initialize servo control registers to 0
    }
  }

  // Add coils (addresses 0-9)
  for (int i = 0; i < 10; i++) {
    mb.addCoil(i, false); // Initialize all coils to false
  }
  
  // Print initialization message
  Serial.println("Modbus Servo Control System Initialized");
  Serial.println("Servo registers 20-24 ready for control");
}

void loop() {
  // Process Modbus requests
  mb.task();
  
  // Process servo control commands
  processServoControl();
  
  delay(10); // Small delay to prevent overwhelming the processor
}

/**
 * Process servo control commands from Modbus registers
 * This function checks for changes in servo control registers and acts accordingly
 */
void processServoControl() {
  // Read values from servo control registers (20-25)
  uint16_t reg20 = mb.Hreg(SERVO_REG_20);
  uint16_t reg21 = mb.Hreg(SERVO_REG_21);
  uint16_t reg22 = mb.Hreg(SERVO_REG_22);
  uint16_t reg23 = mb.Hreg(SERVO_REG_23);
  uint16_t reg24 = mb.Hreg(SERVO_REG_24);
  uint16_t reg25 = mb.Hreg(SERVO_REG_25);
  
  // Process register 20 - Servo 0 control
  if (reg20 != servoControlData[0]) {
    servoControlData[0] = reg20;
    handleServoControl(0, reg20);
  }
  
  // Process register 21 - Servo 1 control (example)
  if (reg21 != servoControlData[1]) {
    servoControlData[1] = reg21;
    handleServoControl(1, reg21);
  }
  
  // Process register 22 - Servo 2 control
  if (reg22 != servoControlData[2]) {
    servoControlData[2] = reg22;
    handleServoControl(2, reg22);
  }
  
  // Process register 23 - Servo 3 control
  if (reg23 != servoControlData[3]) {
    servoControlData[3] = reg23;
    handleServoControl(3, reg23);
  }
  
  // Process register 24 - Servo 4 control
  if (reg24 != servoControlData[4]) {
    servoControlData[4] = reg24;
    handleServoControl(4, reg24);
  }
  
  // Process register 25 - Servo 5 control
  if (reg25 != servoControlData[5]) {
    servoControlData[5] = reg25;
    handleServoControl(5, reg25);
  }
}

/**
 * Handle servo control based on register value
 * @param servoIndex The servo index (0-5)
 * @param controlValue The control value from the register (lower 8 bits = angle)
 */
void handleServoControl(uint8_t servoIndex, uint16_t controlValue) {
  // Extract angle from lower 8 bits of the control value
  uint8_t angle = controlValue & 0xFF;  // Lower 8 bits = angle
  
  // Validate angle
  if (angle > MAX_ANGLE) {
    angle = MAX_ANGLE;
  }
  
  // Move servo to the specified angle
  Serial.print("Servo ");
  Serial.print(servoIndex);
  Serial.print(" moving to angle ");
  Serial.println(angle);
  
  switch(servoIndex) {
    case 0: servo0.write(angle); break;
    case 1: servo1.write(angle); break;
    case 2: servo2.write(angle); break;
    case 3: servo3.write(angle); break;
    case 4: servo4.write(angle); break;
    case 5: servo5.write(angle); break;
  }
}

/**
 * Example function to demonstrate how to write to register #21 (servo 1)
 * This would be called from a Modbus master device
 *
 * Example HEX command to move servo 1 to 90 degrees:
 * - Write register 21 with value 0x005A (0x5A = 90 degrees)
 *
 * @param angle The angle to move servo to (0-180)
 */
void moveServo1ToAngle(uint8_t angle) {
  // Validate angle
  if (angle > MAX_ANGLE) {
    angle = MAX_ANGLE;
  }
  
  // Set register 21 to angle (no command bits needed)
  uint16_t value = angle;
  mb.Hreg(SERVO_REG_21, value);
  
  Serial.print("Set servo 1 to angle ");
  Serial.println(angle);
}
