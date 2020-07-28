#include <Arduino.h>
#include <simplemotion.h>

smbus handle;

void setup() {
  /* Initialize serial port for debug messages. */
  Serial.begin(9600);
  smSetDebugOutput(SMDebugLow, &Serial);
  handle = smOpenBus("Serial3");

  if (handle == -1) {
    Serial.println("SimpleMotion init error!");
  } else {
    Serial.println("SimpleMotion RS485 initialized...");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
