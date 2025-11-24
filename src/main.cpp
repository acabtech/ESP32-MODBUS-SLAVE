#include <Arduino.h>
#include <ModbusRTU.h>

ModbusRTU mb;

void setup() {
  // Use Serial2 for CP210x USB-to-UART bridge on COM4
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX, TX pins for Serial2 on ESP32
  mb.begin(&Serial2);
  mb.slave(1); // Set slave ID to 1

  // Add holding registers (addresses 0-9)
  for (int i = 0; i < 10; i++) {
    mb.addHreg(i, i * 10); // Initialize with values 0, 10, 20, ..., 90
  }

  // Add coils (addresses 0-9)
  for (int i = 0; i < 10; i++) {
    mb.addCoil(i, false); // Initialize all coils to false
  }
}

void loop() {
  mb.task();
  delay(10); // Small delay to prevent overwhelming the processor
}