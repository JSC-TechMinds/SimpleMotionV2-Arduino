SimpleMotionV2 for Arduino
==========================
[![Arduino_CI](https://github.com/JSC-electronics/SimpleMotionV2-Arduino/actions/workflows/arduino_ci.yml/badge.svg)](https://github.com/JSC-electronics/SimpleMotionV2-Arduino/actions/workflows/arduino_ci.yml)
[![License: Apache 2.0](https://img.shields.io/badge/license-Apache--2.0-green.svg)](https://github.com/JSC-electronics/SimpleMotionV2-Arduino/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/JSC-electronics/SimpleMotionV2-Arduino.svg?maxAge=3600)](https://github.com/JSC-electronics/SimpleMotionV2-Arduino/releases)
[![JSC electronics](https://img.shields.io/badge/JSC-electronics-green.svg)](https://www.jsce.cz/)
[![Donate](https://img.shields.io/badge/donate-PayPal-blueviolet.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=SESX9ABM7V8KA&source=url)

This is a SimpleMotion V2 library, which is an API to control motor controller from any programmable platform, such as PC (Linux, Win, Mac), Rasperry Pi, MCU or PLC system.

For main documentation, see:
http://granitedevices.com/wiki/SimpleMotion_V2

# Arduino support
In order to add support for Arduino, we had to fork the original SimpleMotion V2 library. Arduino library specification requires several changes (like adding an `examples` folder, library manifest, etc.). Upstreaming all Arduino changes into a generic, multi-platform repository doesn't make sense. Instead, we'll keep maintaining this version.

## Supported features
All features, which could be ported into Arduino, were adapted. We've modified all compulsory files, and also the `bufferedmotion.c/.h` library for buffered motion stream applications.

## Supported boards
This library supports Controllino Maxi/Mega and M5Stack Core out-of-the-box.

### Controllino Maxi/Mega
To initialize SMbus with Controllino, call this function inside your sketch:
```c
#include <simplemotion.h>

void setup() {
smbus handle = smOpenBus("Serial3");
}
```

`Serial2` is the default port used in Controllino Maxi/Mega, and the [Controllino driver](src/drivers/arduino/controllino_rs485.h) inside the library expects such a name to be activated.

> Note: Default baudrate of SMbus is 460,800 bauds/second. During our testing we've found that Controllino is unable to achieve this baudrate with required precision. As a result, you won't get any response from the IONI drive. Partial solution to this problem could be changing baudrate on the IONI drive with the `SMP_BUS_SPEED` command. However, you need to use another device, which can communicate with IONI (like their USB module with Granity software), and please remember IONI will discard this setting after powering down.

### M5stack Core
If you use M5stack, add the following flag inside your sketch:
```c
#define M5STACK_RS485
```

Alternatively, you can modify the file `user_options.h` [found](src/user_options.h) in the library source. We've successfully tested SMBus communication with IONI using baudrate of 2,000,000 bauds/second.

Initialization works the same as with Controllino, except default port used for RS485 communication is `Serial2`:
```c
#include <simplemotion.h>

void setup() {
smbus handle = smOpenBus("Serial2");
}
```

### Other Arduino boards
Adding support for other boards is relatively simple. Instead of calling `smOpenBus(...)` you need to call `smOpenBusWithCallbacks(...)`, and implement callbacks for opening, closing, reading, and writing from/to RS485. Take a look at the [Controllino driver](src/drivers/arduino/controllino_rs485.h).

## Debugging
You can debug your SimpleMotion communication using function `smSetDebugOutput(/*trace level*/, &Serial)`. By specifying `trace level` you control how much information does the library print to the `Serial`.

**Supported trace levels:**
- SMDebugOff=no debug prints (default)
- SMDebugLow=only some excepetion/errors printed
- SMDebugMid=some common function calls printed
- SMDebugHigh=more details of function calls/bus communication printed
- SMDebugTrace=print all raw RX/TX data and parsed read values of RX data

> Original SMBus library writes debug messages to the file. In Arduino we used serial line for that purpose. Note that debugging messages will make your code slow. You should enable that only in simple sketches where you don't send much data. In production code you can check for `SM_STATUS` values returned by the functions:

```c
// Read a value
SM_STATUS result;
smint32 readValue;
result = smRead1Parameter(handle, 1, SMP_VEL_I, &readValue);

if (result != SM_OK) {
printf("Failed to read a parameter from remote drive!\n");
} else {
printf("Velocity I gain: %d\n", readValue);
}
```

## C++ Issue
Arduino IDE determines whether a library is C-compatible or C++-compatible based on file extensions. In order to compile the main `simplemotion.c` library in C++ mode, we had to rename it to `simplemotion.cpp`. Otherwise, we couldn't call object methods, like the `Serial.println(...)`.

## License
Copyright (c) JSC electronics and Granite Devices.
Licensed under the [Apache-2.0](LICENSE) license.

## Support
We hope our library helped to speed up your project development. You can support our effort to convert coffe and pizza into a code with a small donation. Any support is much appreciated.

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=SESX9ABM7V8KA&source=url)

> Further documentation is from original library as-is.

Files & usage
=============

There are files that are needed for basic SimpleMotion usage, and files that are optional and needed only for certain features. Following may be useful info when porting library to different platforms (such as embedded MCU) to avoid unnecessary work on some files.

Compulsory
----------
Following files are the core library. These must be included in the compiled projects to achieve a working solution.

- simplemotion.c/.h
- sm485.h
- sm_consts.c
- simplemotion_defs.h
- simplemotion_private.h
- busdevice.c/.h

Feature specific
----------------
Following files are not required in the core library, however you might need them depending on your application.

- bufferedmotion.c/.h - Library used for buffered motion stream applications
- devicedeployment.c/.h - Library used for installing firmware and loading settings into target devices. Can be used to configure devices automatically in-system.
- files in drivers/ folder - These are platofrm & hardware specific SM bus interface device drivers

Porting to new platform
-----------------------
Following files need modification if ported to another platform where SimpleMotion communication interface device driver does not yet exist

- If porting to a non-PC system, make sure that ENABLE_BUILT_IN_DRIVERS is not defined at compilation time (see qmake .pri file for clues) and use smOpenBusWithCallbacks instead of smOpenBus
- Write custom communication interface device driver and use smOpenBusWithCallbacks to open bus with custom driver callbacks
  - See existing drivers for example
  - Only four simple functions are needed to write custom port driver: open port, close port, write port and read port

Using SimpleMotion
==================
## In Qt Application
To include SM library in Qt project, simply place the library files under project folder and add the following line in your .pro file:

    include(SimpleMotionV2/SimpleMotionV2.pri)

## Creating shared / dynamic library
It is possible to compile shared and dynamic libraries (i.e. .dll or .a file) from the SM library source package. One of the easiest ways of compiling library is to use Qt Creator, where a ready-to-compile project file is provided. Just open SimpleMotionV2lib.pro in Qt Creator and compile it with the compiler of your choice. The resulting library files may be used in other applications.

Alternatively create a new library project in your favorite programming tool and compile the provided source codes into a libraray. You might need to study workings of the .pri file to succeed.

Windows users notice that a .dll library compiled with MinGW might not be compatible with MSVC. So if you're developing with MSVC, it's best to complile the dll using MSVC compiler. Qt Creator allows using Visual Studio's MSVC as compiler when configured properly.

## In Python, C# & others
Most of programming languages have wrapper solutions that can call C functions. To get started, search some examples from the Internet for your programmign language.

If you write a wrapper library or any form of working demo application to your non-C programming language, feel free to contribute it to us and get credited!

## In Visual Studio C/C++
If you wish to link library statically in other programming environments, i.e. in Visual Studio, it might be easiest to grap the sources and add them into the project so that they will be compiled along your project. This way library will be included in your application and it does not depend on external shared or dynamic libraries.

## In LabView
LabView allows including .dll file in the project and calling the C functions. To do that, first compile a .dll file as instructed in "Creating shared / dynamic library" section above.

Examples
========
For practical usage examples, refer to https://github.com/GraniteDevices/SimpleMotionV2Examples

About different branches
========================
There are multiple branches in this repository. Summary:

* **master:** Latest stable version. The goal here is to keep API stable and backwards compatible with earlier master versions. Master branch will be updated occasionally by merge commit from develop.
* **develop:** Latest development version, may contain unfinished features & changing API. Be warned that your application using under development features might break.
* **feature/nnn:** Under development features that eventually shall be merged to develop.

Usually it's safest to use master version. If latest additions are necessary, then use develop.
