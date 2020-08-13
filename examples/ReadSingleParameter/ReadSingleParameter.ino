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
  readVelocity();
  delay(1000);
}

void readVelocity() {
  smint32 readValue;
  SM_STATUS status;
  
  status = smRead1Parameter(handle, IONI_DEVICE_ID, SMP_VEL_I, &readValue);
  if (status != SM_OK) {
    Serial.println("Failed to read a parameter from remote drive!");
  } else {
    Serial.print("Velocity I gain: ");
    Serial.println();
  }
}
