/*
 * m5stack_rs485.cpp
 *
 * Header for M5stack Core/Grey ESP32 RS485 communication
 *
 *  Created on: 30.7.2020
 *      Author: vzahradnik
 *
 */


#ifndef M5STACK_RS485_H
#define M5STACK_RS485_H

#ifdef ARDUINO
#include <pins_arduino.h>
#endif

// Check for M5STACK_RS485 flag
#include "user_options.h"

#if defined(ESP32) && defined(M5STACK_RS485)

#include "simplemotion_private.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "simplemotion.h"

smBusdevicePointer m5stackRs485PortOpen(const char *port_device_name, smint32 baudrate_bps, smbool *success);
smint32 m5stackRs485PortRead(smBusdevicePointer busdevicePointer, unsigned char *buf, smint32 size);
smint32 m5stackRs485PortWrite(smBusdevicePointer busdevicePointer, unsigned char *buf, smint32 size);
void m5stackRs485PortClose(smBusdevicePointer busdevicePointer);
smbool m5stackRs485PortMiscOperation(smBusdevicePointer busdevicePointer, BusDeviceMiscOperationType operation);


#ifdef __cplusplus
}
#endif

#endif // M5STACK_RS485

#endif


