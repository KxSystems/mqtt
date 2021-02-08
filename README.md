# `mqtt` - MQTT for kdb+

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/kxsystems/mqtt?include_prereleases)](https://github.com/kxsystems/mqtt/releases) [![Travis (.org) branch](https://img.shields.io/travis/kxsystems/mqtt/master?label=travis%20build)](https://travis-ci.org/kxsystems/mqtt/branches)

## Introduction

This interface provides kdb+ users with the ability to interact with the Message Queuing Telemetry Transport [(MQTT)](http://mqtt.org/) messaging protocol. The interface is a thin wrapper for kdb+ around the [eclipse/paho.mqtt.c](https://github.com/eclipse/paho.mqtt.c) C api.

This is part of the [_Fusion for kdb+_](http://code.kx.com/q/interfaces/fusion/) interface collection.

## New to kdb+ ?

Kdb+ is the world's fastest time-series database, optimized for ingesting, analyzing and storing massive amounts of structured data. To get started with kdb+, please visit https://code.kx.com/q/learn/ for downloads and developer information. For general information, visit https://kx.com/

## New to MQTT ?

Message Queueing Telemetry Transport (MQTT) is a machine-to-machine/IOT connectivity protocol. It is designed to provide a lightweight publish/subscribe messaging protocol.

MQTT is used commonly for constrained devices with low-bandwidth, high-latency or unreliable networks.

## Installation

### Requirements

* kdb+ >= 3.5 64-bit(Linux/MacOS/Windows) and 32-bit ARM
* [paho.mqtt.c](https://github.com/eclipse/paho.mqtt.c) >= 1.3.2
* CMake >= 3.1 [^1]

[^1]: Required when building from source

### Installing a release

1. Ensure [MQTT C api](https://github.com/eclipse/paho.mqtt.c/releases) (`paho.mqtt.c`) is installed.
2. Make the MQTT library available from kdb+:
   - Linux: Add the lib directory which includes `include` and `lib` to the `LD_LIBRARY_PATH` environment variable e.g. if unzipped to `/usr/local/Eclipse-Paho-MQTT-C/`, run:
        ```bash

        $ export LD_LIBRARY_PATH=/usr/local/Eclipse-Paho-MQTT-C/lib/:$LD_LIBRARY_PATH
        
        ```
   - MacOS: Add the lib directory which includes `include` and `lib`to the `DYLD_LIBRARY_PATH` environment variable e.g. if unzipped to `/Users/jim/eclipse-paho-mqtt-c/`, run:
        ```bash

        $ export DYLD_LIBRARY_PATH=/Users/jim/Eclipse-Paho-MQTT-C/lib/:$DYLD_LIBRARY_PATH
      
        ```
   - Windows: Add the `paho-mqtt3a.dll`, `paho-mqtt3as.dll`, `paho-mqtt3c.dll` and `paho-mqtt3cs.dll` to the kdb+ lib directory e.g. `C:\q\w64` for 64-bit.
3. Download the latest release of the mqtt interface from our [releases page](https://github.com/KxSystems/mqtt). To install shared library and q files, use:

        # Linux/MacOS
        $ ./install.sh

        # Windows
        > install.bat

    or copy `mqtt.q` to `QHOME`, then copy `mqttkdb.so` or `mqttkdb.dll` into `QHOME/[l|m|w]64`

### Building Interface From Source

#### Linux/MacOS

1. Download the latest release of the [MQTT C api](https://github.com/eclipse/paho.mqtt.c/releases) (`paho.mqtt.c`).
2. Set an environment variable `MQTT_INSTALL_DIR` pointing to the location of the installed and unzipped release where `include` and `lib` are located. This environmental variable will be used to link the library to MQTT-kdb+ interface.

     ```bash

     ]$ mkdir paho_mqtt_c
     ]$ tar xzf Eclipse-Paho-MQTT-C-1.3.8-Linux.tar.gz -C paho_mqtt_c/ --strip-components=1
     ]$ cd paho_mqtt_c/
     paho_mqtt_c]$ export MQTT_INSTALL_DIR=$(pwd)

     ```

3. For macOSX add the lib directory which includes `include` and `lib` to the `DYLD_LIBRARY_PATH` environment variable e.g. if unzipped to `/Users/jim/Eclipse-Paho-MQTT-C/`, run:
   
     ```bash

     $ export DYLD_LIBRARY_PATH=/Users/jim/Eclipse-Paho-MQTT-C/lib/:$DYLD_LIBRARY_PATH
     
     ```

4. Clone MQTT-kdb+ repository and build with `cmake`.

     ```bash

     ]$ git clone https://github.com/KxSystems/mqtt.git
     ]$ cd mqtt
     mqtt]$ mkdir build && cd build
     build]$ cmake ..
     build]$ cmake --build . --target install

     ```

**Note:** `cmake --build . --target install` as used in the Linux/MacOS builds installs the required share object and q files to the `QHOME/[ml]64` and `QHOME` directories respectively. If you do not wish to install these files directly, you can execute `cmake --build .` instead of `cmake --build . --target install` and move the files from their build location at `build/mqttkdb`.

#### Windows

1. Download the latest release of the [MQTT C api](https://github.com/eclipse/paho.mqtt.c/releases) (`paho.mqtt.c`).
2. Set an environment variable `MQTT_INSTALL_DIR` pointing to the location of the installed and unzipped release where `include` and `lib` are located. This environmental variable will be used to link the library to MQTT-kdb+ interface.
3. Create links to the paho dll's in the `%QHOME%\w64` directory.

     ```bat

     > mkdir paho_mqtt_c
     > 7z x eclipse-paho-mqtt-c-win64-1.3.8.zip -opaho_mqtt_c
     > cd paho_mqtt_c
     paho_mqtt_c> set MQTT_INSTALL_DIR=%cd%
     paho_mqtt_c> cd %QHOME%\w64
     w64> MKLINK paho-mqtt3a.dll %MQTT_INSTALL_DIR%\lib\paho-mqtt3a.dll
     w64> MKLINK paho-mqtt3as.dll %MQTT_INSTALL_DIR%\lib\paho-mqtt3as.dll
     w64> MKLINK paho-mqtt3c.dll %MQTT_INSTALL_DIR%\lib\paho-mqtt3c.dll
     w64> MKLINK paho-mqtt3cs.dll %MQTT_INSTALL_DIR%\lib\paho-mqtt3cs.dll

     ```

4. Clone MQTT-kdb+ repository and build with `cmake`. Building the interface from source requires Visual Studio (assuming `-G "Visual Studio 15 2017 Win64"` is not necessary).

     ```bat

     > git clone https://github.com/KxSystems/mqtt.git
     > cd mqtt
     mqtt> mkdir build && cd build
     build> cmake --config Release ..
     build> cmake --build . --config Release --target install

     ```

**Notes:** 

1. `cmake --build . --config Release --target install` installs the required share object and q files to the `QHOME\w64` and `QHOME` directories respectively. If you do not wish to install these files directly, you can execute `cmake --build . --config Release` instead of `cmake --build . --config Release --target install` and move the files from their build location at `build/mqttkdb`.
2. You can use flag `cmake -G "Visual Studio 16 2019" -A Win32` if building 32-bit version.

#### Docker - Linux

A sample docker file is provided in the `docker_linux` directory to create a CentOS 7 environment (including downloading the `paho.mqtt.c` 64 bit Linux release) before building and installing the kdb+ `mqtt` interface.

The `MQTT_INSTALL_DIR` and `QHOME` directories are specified at the top of `mqtt_build.bat`, which sets up the environment specified in `Dockerfile.build` and invokes `mqtt_build.sh` to build the library.

## Quick Start

### Requirements

- [Mosquitto broker](https://mosquitto.org/download/)

### Start Mosquitto

Start mosquitto on default port `localhost:1883`.

```bash

$ sudo systemctl start mosquitto

```

### Launch Consumer

We assume you are in the source directory of MQTT-kdb+ interface.

```bash

mqtt]$ cd q
q]$ q ../examples/consumer.q 

```

### Launch Producer

In the same directory as the one launching a consumer, run:

```bash

q]$ q ../examples/producer.q

```

Then pubish a message by a timer, say every second:

```q

q)\t 1000

```

Going back to the console of consumer, now you can see received messages in `.mqtt.tab`:

```q

q).mqtt.tab
topic  msg_sent                      msg_recv                      received_m..
-----------------------------------------------------------------------------..
topic1 2021.01.06D13:24:46.267356000 2021.01.06D13:24:46.267718000 topic1_3  ..
topic2 2021.01.06D13:24:46.267450000 2021.01.06D13:24:46.368412000 topic2_3  ..
topic1 2021.01.06D13:24:47.267366000 2021.01.06D13:24:47.268676000 topic1_4  ..
topic2 2021.01.06D13:24:47.267624000 2021.01.06D13:24:47.372268000 topic2_4  ..
topic1 2021.01.06D13:24:48.276623000 2021.01.06D13:24:48.277677000 topic1_5  ..
topic2 2021.01.06D13:24:48.276801000 2021.01.06D13:24:48.377926000 topic2_5  ..
topic1 2021.01.06D13:24:49.267324000 2021.01.06D13:24:49.268228000 topic1_6  ..
topic2 2021.01.06D13:24:49.267482000 2021.01.06D13:24:49.368805000 topic2_6  ..
topic1 2021.01.06D13:24:50.267684000 2021.01.06D13:24:50.268125000 topic1_7  ..
topic2 2021.01.06D13:24:50.267804000 2021.01.06D13:24:50.368889000 topic2_7  ..

```

## Documentation

Documentation outlining the functionality available for this interface can be found [here](http://code.kx.com/q/interfaces/mqtt/).

## Status

The mqtt interface is provided here under an Apache 2.0 license.

If you find issues with the interface or have feature requests, please consider raising an issue [here](https://github.com/KxSystems/mqtt/issues).

If you wish to contribute to this project, please follow the contributing guide [here](CONTRIBUTING.md).
