#include <Arduino.h>
#include <simplemotion.h>

#define MONITOR_BAUD_RATE 115200
#define DEVICE_PORT "Serial3" // Controllino RS485
#define IONI_DEVICE_ID 1
#define COUNTER_LIMIT 1000 // How many samples to read before calculating samples/second
smbus handle;

unsigned long counter = 0;
unsigned long start_time;
unsigned long current_time;

void setup() {
  /* Initialize serial port for debug messages. */
  Serial.begin(MONITOR_BAUD_RATE);
  smSetDebugOutput(SMDebugLow, &Serial);
  
  handle = smOpenBus(DEVICE_PORT);

  if (handle == -1) {
    Serial.println("SimpleMotion init error!");
    while(1);
  }

  start_time = millis();
}

void loop() {
  // Read as fast as possible. Library calls are blocking.
  smint32 torque;
  readTorque(&torque);
  current_time = millis();
  counter++;

  // Time to calculate samples/second
  if (counter > COUNTER_LIMIT) {
    unsigned long time_diff = abs(current_time - start_time);
    Serial.print("Test finished after ");
    Serial.print(time_diff);
    Serial.println(" seconds.");
    Serial.print("Samples per second: ");
    Serial.println((double)counter/time_diff);

    // Reset counter and run test again
    counter = 0;
    start_time = millis();
  }
}

SM_STATUS readTorque(smint32 *torque) {
    SM_STATUS status;
    status = smRead1Parameter(handle, IONI_DEVICE_ID, SMP_ACTUAL_TORQUE, torque);
    return status;

}
