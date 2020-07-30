/*
 * m5stack_rs485.cpp
 *
 * Header for M5stack Core/Grey ESP32 RS485 communication
 *
 *  Created on: 30.7.2020
 *      Author: vzahradnik
 *
 */



#include "m5stack_rs485.h"
#include "user_options.h"
#include "simplemotion_private.h" //needed for timeout variable

#ifdef ARDUINO
#include "arduino_helper.h"
#endif

#if defined(ESP32) && defined(M5STACK_RS485)
#include <M5Stack.h>
#define RX_PIN 16
#define TX_PIN 17

smBusdevicePointer m5stackRs485PortOpen(const char * port_device_name, smint32 baudrate_bps, smbool *success)
{
    // check if devicename is correct format
    if (strncmp(port_device_name,"Serial2", 7) != 0)
        return SMBUSDEVICE_RETURN_ON_OPEN_FAIL;

    M5.begin();
    M5.Power.begin();
    Serial2.begin(baudrate_bps, SERIAL_8N1, RX_PIN, TX_PIN);

    *success=smtrue;

    // Serial device is available as a global object. No need to pass a reference.
    return (smBusdevicePointer) NULL;
}


smint32 m5stackRs485PortRead(smBusdevicePointer busdevicePointer, smuint8 *buf, smint32 size)
{
    smint32 n = 0;
    n = Serial2.readBytes(buf, size);

    return n;
}



smint32 m5stackRs485PortWrite(smBusdevicePointer busdevicePointer, unsigned char *buf, smint32 size)
{
    smint32 n = Serial2.write(buf, size);
    Serial2.flush(); // wait until the trasmission is complete

    return n;
}

smbool m5stackRs485PortMiscOperation(smBusdevicePointer busdevicePointer, BusDeviceMiscOperationType operation)
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
        //FIXME smDebug( -1, SMDebugLow, "Serial port error: given MiscOperataion not implemented\n");
        return smfalse;
        break;
    }
}

void m5stackRs485PortClose(smBusdevicePointer busdevicePointer)
{
    Serial2.end();
}
#endif
