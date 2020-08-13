/**
 * This is an example showing how to change baudrate on SimpleMotion bus.
 * Note: Default baudrate is 460,800 bps. IONI devices set this baudrate after each boot.
 * In cases that you're unable to communicate with the driver, please power-cycle your drive.
 */

#include <Arduino.h>
#include <simplemotion.h>

#define MONITOR_BAUD_RATE 115200
#define DEVICE_PORT "Serial3" // Controllino RS485
#define IONI_DEVICE_ID 1
#define SMBUS_DEFAULT_BAUDRATE 460800 // DO NOT CHANGE
#define SMBUS_BAUDRATE 1000000
smbus handle;

void setup() {
  /* Initialize serial port for debug messages. */
  Serial.begin(MONITOR_BAUD_RATE);
  smSetDebugOutput(SMDebugLow, &Serial);

  if (initSmBus() != SM_OK) {
    while(1);
  }
}

void loop() {
  // Try to read some data to confirm IONI is communicating on a new baudrate
  smint32 serialNr;
  SM_STATUS status = smRead1Parameter(handle, IONI_DEVICE_ID, SMP_SERIAL_NR, &serialNr);

  if (status != SM_OK) {
    Serial.println("Device is not communicating");
  } else {
    Serial.print("Device sent serial number: ");
    Serial.println(serialNr);
  }
}

SM_STATUS initSmBus() {
    SM_STATUS status = SM_OK;

    handle = smOpenBus(DEVICE_PORT);
    if (handle == -1) {
        Serial.println("SimpleMotion init error!");
    }
    
    if (SMBUS_BAUDRATE != SMBUS_DEFAULT_BAUDRATE) {
        status |= setBaudRate(SMBUS_BAUDRATE);
    }

    return status;
}

SM_STATUS setBaudRate(const long baudrate) {
    SM_STATUS status;
    Serial.println("Changing SMBus baudrate");

    // Step 1: Send command to slave devices to change baudrate
    // Note: DEVICE ID 0 means that baudrate will be set on all devices
    status = smSetParameter(handle, /*device id*/ 0, SMP_BUS_SPEED, baudrate);
    if (status == SM_OK) {
        // Step 2: Close SMBus
        status |= smCloseBus(handle);

        // Step 3: Set new baudrate
        smSetBaudrate(baudrate);

        // Step 4: Re-open SMBus
        handle = smOpenBus(DEVICE_PORT);

        if (handle == -1) {
            Serial.println("Changing baudrate failed. Please power-cycle connected IONI drives.");
            while(1);
        }
    }

    return status;
}
