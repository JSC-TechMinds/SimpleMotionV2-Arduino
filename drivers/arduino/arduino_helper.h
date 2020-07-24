/*
 * arduino_helper.h
 *
 * Header for Arduino support inside SimpleMotion library.
 * The goal is to move all Arduino/C++-specific code here,
 * and to build the core library in pure C mode.
 *
 *  Created on: 25.7.2020
 *      Author: vzahradnik
 *
 */


#ifndef ARDUINO_HELPER_H
#define ARDUINO_HELPER_H

#ifdef ARDUINO
#include "simplemotion_private.h"
#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "simplemotion.h"

/* OS independent sleep function for SM internal use
 *
 * SM lib has implementation for unix/win systems (incl linux & mac). For other systems, please add your own smSleepMs implementation in your application.
 * i.e. write function in your main.c or any other .c file that get's compiled and linked:
 *
 * void smSleepMs(int millisecs)
 * {
 *    // do your delay here
 * }
 */
// extern void smSleepMs(int millisecs);

/** Set stream where debug output is written. By default nothing is written.
smVerbosityLevel:
 * SMDebugOff=no debug prints (default)
 * SMDebugLow=only some excepetion/errors printed
 * SMDebugMid=some common function calls printed
 * SMDebugHigh=more details of function calls/bus communication printed
 * SMDebugTrace=print all raw RX/TX data and parsed read values of RX data
 *
 * NOTE: for debug prints to work, SM library must be compiled with ENABLE_DEBUG_PRINTS defined (i.e. uncomment
 * that definition from simplemotion.h or define it application wide with compiler flag, i.e. -DENABLE_DEBUG_PRINTS).
 * Enabling it may slow down & grow binary significantly especially on MCU systems.
 *
 * Also note the Print*, which expects a pointer to the Arduino Serial object.
 */
LIB void smSetDebugOutput( smVerbosityLevel level );

//smDebug: prints debug info to smDebugOut stream. If no handle available, set it to -1, or if wish to print as raw text, set handle to DEBUG_PRINT_RAW.
//set verbositylevel according to frequency of prints made.
//I.e SMDebugLow=low frequency, so it gets displayed when global verbosity level is set to at least Low or set it to Trace which gets filtered
//out if global verbisity level is set less than SMDebugTrace
#ifdef ENABLE_DEBUG_PRINTS
void arduinoPrintMessage(const char* message);
#endif

#ifdef __cplusplus
}
#endif

#endif // Arduino

#endif // ARDUINO_HELPER_H


