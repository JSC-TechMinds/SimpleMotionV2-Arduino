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
    // TODO: Open RS485 communication
    Serial2.begin(baudrate_bps, SERIAL_8N1, RX_PIN, TX_PIN);

    *success=smtrue;

    // Serial device is available as a global object. No need to pass a reference.
    return (smBusdevicePointer) NULL;
}


smint32 arduinoRs485PortRead(smBusdevicePointer busdevicePointer, smuint8 *buf, smint32 size)
{
    smint32 n = 0;
    n = Serial2.readBytes(buf, size);

    return n;
}



smint32 arduinoRs485PortWrite(smBusdevicePointer busdevicePointer, unsigned char *buf, smint32 size)
{
    smint32 n = Serial2.write(buf, size);
    Serial2.flush(); // wait until the trasmission is complete

    return n;
}

smbool arduinoRs485PortMiscOperation(smBusdevicePointer busdevicePointer, BusDeviceMiscOperationType operation)
{
    switch(operation) {
        case MiscOperationPurgeRX:
            //flush any stray bytes from device receive buffer that may reside in it
            while (Serial2.available() > 0) {
                // There is no method to flush buffer. We'll keep reading until it's empty.
                Serial2.read();
            }

            return smtrue;
            break;
    case MiscOperationFlushTX:
        Serial2.flush();
        return smtrue;
        break;
    default:
        return smfalse;
        break;
    }
}

void arduinoRs485PortClose(smBusdevicePointer busdevicePointer)
{
    Serial2.end();
}
