/*
 * controllino_rs485.cpp
 *
 * Header for Controllino Mega/Maxi RS485 communication
 *
 *  Created on: 24.7.2020
 *      Author: vzahradnik
 *
 */



#include "controllino_rs485.h"
#include "user_options.h"
#include "simplemotion_private.h" //needed for timeout variable

#ifdef ARDUINO
#include "arduino_helper.h"
#endif

#if defined(CONTROLLINO_MAXI) || defined(CONTROLLINO_MEGA)
#include <Controllino.h>

smBusdevicePointer controllinoRs485PortOpen(const char * port_device_name, smint32 baudrate_bps, smbool *success)
{
    // check if devicename is correct format
    if (strncmp(port_device_name,"Serial3", 7) != 0)
        return SMBUSDEVICE_RETURN_ON_OPEN_FAIL;

    // Initialize CONTROLLINO RS485 direction control DE/RE pins and Serial3
    Controllino_RS485Init(baudrate_bps);
    Controllino_RS485RxEnable();

    *success=smtrue;

    // Serial device is available as a global object. No need to pass a reference.
    return (smBusdevicePointer) NULL;
}


smint32 controllinoRs485PortRead(smBusdevicePointer busdevicePointer, smuint8 *buf, smint32 size)
{
    smint32 n = 0;
    n = Serial3.readBytes(buf, size);

    return n;
}



smint32 controllinoRs485PortWrite(smBusdevicePointer busdevicePointer, unsigned char *buf, smint32 size)
{
    // Turn on transmission
    Controllino_RS485TxEnable();

    smint32 n = Serial3.write(buf, size);
    Serial3.flush(); // wait until the trasmission is complete

    // Switch back to receiving data
    Controllino_RS485RxEnable();

    return n;
}

smbool controllinoRs485PortMiscOperation(smBusdevicePointer busdevicePointer, BusDeviceMiscOperationType operation)
{
    switch(operation) {
        case MiscOperationPurgeRX:
            //flush any stray bytes from device receive buffer that may reside in it
            while (Serial3.available() > 0) {
                // There is no method to flush buffer. We'll keep reading until it's empty.
                Serial3.read();
            }

            return smtrue;
            break;
    case MiscOperationFlushTX:
        Serial3.flush();
        return smtrue;
        break;
    default:
        //FIXME smDebug( -1, SMDebugLow, "Serial port error: given MiscOperataion not implemented\n");
        return smfalse;
        break;
    }
}

void controllinoRs485PortClose(smBusdevicePointer busdevicePointer)
{
    Serial3.end();
}
#endif
