/*
 * arduino_helper.cpp
 *
 * Header for Arduino support inside SimpleMotion library.
 * The goal is to move all Arduino/C++-specific code here,
 * and to build the core library in pure C mode.
 *
 *  Created on: 25.7.2020
 *      Author: vzahradnik
 *
 */

#ifdef ARDUINO

#include "arduino_helper.h"
#include "user_options.h"
#include "simplemotion_private.h" //needed for timeout variable

Print* consoleOut = NULL;

void smSleepMs(int millisecs)
{
    delay(millisecs);
}

LIB void smSetDebugOutput( smVerbosityLevel level, ArduinoSerial serial )
{
    smDebugThreshold=level;
    consoleOut = (Print*) serial;
}

#ifdef ENABLE_DEBUG_PRINTS
void arduinoPrintMessage(const char* message) {
    if (consoleOut != NULL) {
        consoleOut->print(message);
    }
}
#endif // DEBUG PRINTS

#endif
