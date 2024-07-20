/**
 * This is an example showing how to use SimpleMotionV2-Arduino with Arduino board without explicit support.
 * You need to implement a few callback methods. For inspiration please take a look at existing Arduino drivers:
 * https://github.com/JSC-TechMinds/SimpleMotionV2-Arduino/tree/main/src/drivers/arduino
 */

#include <Arduino.h>
#include <simplemotion.h>
#define RX_PIN 16
#define TX_PIN 17

#define MONITOR_BAUD_RATE 115200
// It might be anything. We're handling data transfer in our code
#define DEVICE_PORT "My Arduino board"
#define IONI_DEVICE_ID 1
smbus handle;

void setup() {
  /* Initialize serial port for debug messages. */
  Serial.begin(MONITOR_BAUD_RATE);
  smSetDebugOutput(SMDebugLow, &Serial);

  handle = smOpenBusWithCallbacks(DEVICE_PORT, arduinoRs485PortOpen, arduinoRs485PortClose, arduinoRs485PortRead, arduinoRs485PortWrite, arduinoRs485PortMiscOperation);

  if (handle == -1) {
    Serial.println("SimpleMotion init error!");
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

smBusdevicePointer arduinoRs485PortOpen(const char * port_device_name, smint32 baudrate_bps, smbool *success)
{
    Serial.print("Opening bus with baudrate ");
    *success=smtrue;

    // Serial device is available as a global object. No need to pass a reference.
    return (smBusdevicePointer) NULL;
}


smint32 arduinoRs485PortRead(smBusdevicePointer busdevicePointer, smuint8 *buf, smint32 size)
{   
    Serial.println("Reading data from SimpleMotion");
    return 0;
}



smint32 arduinoRs485PortWrite(smBusdevicePointer busdevicePointer, unsigned char *buf, smint32 size)
{
    Serial.println("Writing data to SimpleMotion");
    return 0;
}

smbool arduinoRs485PortMiscOperation(smBusdevicePointer busdevicePointer, BusDeviceMiscOperationType operation)
{
    switch(operation) {
        case MiscOperationPurgeRX:
            Serial.println("Flushing SimpleMotion RX data");
            return smtrue;
            break;
    case MiscOperationFlushTX:
        Serial.println("Flushing SimpleMotion TX data");
        return smtrue;
        break;
    default:
        return smfalse;
        break;
    }
}

void arduinoRs485PortClose(smBusdevicePointer busdevicePointer)
{
    Serial.println("Closing SimpleMotion bus");
}
