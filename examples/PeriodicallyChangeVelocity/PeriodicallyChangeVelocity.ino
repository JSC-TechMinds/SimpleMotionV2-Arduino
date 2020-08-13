/**
 * This is an example where Arduino periodically changes velocity in opposite directions.
 * Please make sure your drive is configured to control servo through velocity setpoints.
 * Also make sure you've specified correct IONI device port.
 * Note: For configuring most of the servo parameters, use official Granity app.
 */

#include <Arduino.h>
#include <simplemotion.h>

#define MONITOR_BAUD_RATE 115200
#define DEVICE_PORT "Serial3" // Controllino RS485
#define IONI_DEVICE_ID 1
smbus handle;

void setup() {
  /* Initialize serial port for debug messages. */
  Serial.begin(MONITOR_BAUD_RATE);
  smSetDebugOutput(SMDebugLow, &Serial);
  
  handle = smOpenBus(DEVICE_PORT);

  if (handle == -1) {
    Serial.println("SimpleMotion init error!");
    while(1);
  }
}

void loop() {
  // Write positive setpoint
  Serial.println("Writing setpoint 500 to remote drive...");
  smSetParameter(handle, IONI_DEVICE_ID, SMP_ABSOLUTE_SETPOINT, 500);

  // Wait 1 second before changing speed
  delay(1000);

  // Write negative setpoint
  Serial.println("Writing setpoint -500 to remote drive...");
  smSetParameter(handle, IONI_DEVICE_ID, SMP_ABSOLUTE_SETPOINT, -500);

  delay(1000);
}
