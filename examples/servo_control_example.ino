/*
 * Modbus Servo Control Example
 * 
 * This example demonstrates how to control servos using Modbus commands
 * 
 * To use this example:
 * 1. Upload this code to your ESP32
 * 2. Connect a Modbus master device (like a PC with Modbus software)
 * 3. Send Modbus write commands to registers 20-24 to control servos
 * 
 * Example commands:
 * - Write register 21 with value 0x015A to move servo 1 to 90 degrees
 * - Write register 21 with value 0x0000 to stop servo 1
 * - Write register 21 with value 0x0290 to move servo 1 to 144 degrees
 */

#include <Arduino.h>
#include <ModbusRTU.h>
#include <Servo.h>

// Create Modbus master instance
ModbusRTU mb;

// Create servo instances for 5 servos
Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Define servo control registers (20-24)
#define SERVO_REG_20 20  // Servo 0 control register
#define SERVO_REG_21 21  // Servo 1 control register (example)
#define SERVO_REG_22 22  // Servo 2 control register
#define SERVO_REG_23 23  // Servo 3 control register
#define SERVO_REG_24 24  // Servo 4 control register

// Define servo pin assignments
#define SERVO_PIN_0 2
#define SERVO_PIN_1 4
#define SERVO_PIN_2 12
#define SERVO_PIN_3 14
#define SERVO_PIN_4 16

// Define servo angle ranges
#define MIN_ANGLE 0
#define MAX_ANGLE 180

// Define servo control values
#define SERVO_STOP 0
#define SERVO_START 1
#define SERVO_MOVE 2

// Global variables to store servo control data
uint16_t servoControlData[5] = {0, 0, 0, 0, 0};  // For registers 20-24

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

  // Initialize all servos to neutral position
  servo0.write(90);
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);

  // Add holding registers (addresses 0-25)
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
  Serial.println("Modbus Servo Control Example Initialized");
  Serial.println("Servo registers 20-24 ready for control");
  Serial.println("Example commands:");
  Serial.println("- Write register 21 with value 0x015A to move servo 1 to 90 degrees");
  Serial.println("- Write register 21 with value 0x0290 to move servo 1 to 144 degrees");
  Serial.println("- Write register 21 with value 0x0000 to stop servo 1");
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
  // Read values from servo control registers (20-24)
  uint16_t reg20 = mb.Hreg(SERVO_REG_20);
  uint16_t reg21 = mb.Hreg(SERVO_REG_21);
  uint16_t reg22 = mb.Hreg(SERVO_REG_22);
  uint16_t reg23 = mb.Hreg(SERVO_REG_23);
  uint16_t reg24 = mb.Hreg(SERVO_REG_24);
  
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
}

/**
 * Handle servo control based on register value
 * @param servoIndex The servo index (0-4)
 * @param controlValue The control value from the register
 */
void handleServoControl(uint8_t servoIndex, uint16_t controlValue) {
  // Decode the control value (assuming it's a 16-bit value with:
  // - Bits 0-7: Target angle (0-180)
  // - Bits 8-15: Control command (0=stop, 1=start, 2=move)
  
  uint8_t angle = controlValue & 0xFF;  // Lower 8 bits = angle
  uint8_t command = (controlValue >> 8) & 0xFF;  // Upper 8 bits = command
  
  // Validate angle
  if (angle > MAX_ANGLE) {
    angle = MAX_ANGLE;
  }
  
  switch(command) {
    case SERVO_STOP:
      // Stop servo movement
      Serial.print("Servo ");
      Serial.print(servoIndex);
      Serial.println(" stopped");
      break;
      
    case SERVO_START:
      // Start servo at specific angle
      Serial.print("Servo ");
      Serial.print(servoIndex);
      Serial.print(" started at angle ");
      Serial.println(angle);
      switch(servoIndex) {
        case 0: servo0.write(angle); break;
        case 1: servo1.write(angle); break;
        case 2: servo2.write(angle); break;
        case 3: servo3.write(angle); break;
        case 4: servo4.write(angle); break;
      }
      break;
      
    case SERVO_MOVE:
      // Move servo to specific angle
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
      }
      break;
      
    default:
      // Unknown command, do nothing
      Serial.print("Unknown command for servo ");
      Serial.print(servoIndex);
      Serial.println(": ");
      Serial.println(command);
      break;
  }
}