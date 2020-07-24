/*
 * controllino_rs485.h
 *
 * Header for Controllino Mega/Maxi RS485 communication
 *
 *  Created on: 24.7.2020
 *      Author: vzahradnik
 *
 */


#ifndef CONTROLLINO_RS485_H
#define CONTROLLINO_RS485_H

#include "simplemotion_private.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "simplemotion.h"

smBusdevicePointer controllinoRs485PortOpen(const char *port_device_name, smint32 baudrate_bps, smbool *success);
smint32 controllinoRs485PortRead(smBusdevicePointer busdevicePointer, unsigned char *buf, smint32 size);
smint32 controllinoRs485PortWrite(smBusdevicePointer busdevicePointer, unsigned char *buf, smint32 size);
void controllinoRs485PortClose(smBusdevicePointer busdevicePointer);
smbool controllinoRs485PortMiscOperation(smBusdevicePointer busdevicePointer, BusDeviceMiscOperationType operation);


#ifdef __cplusplus
}
#endif

#endif


