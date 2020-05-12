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

* kdb+ ≥ 3.5 64-bit(Linux/MacOS/Windows) and 32-bit ARM
* [paho.mqtt.c](https://github.com/eclipse/paho.mqtt.c) ≥ 1.3.2

### Third-Party Library Installation

#### Linux/MacOS/Windows 64-bit

Linux, MacOS and Windows users should complete the following steps

1. Download the latest release of the `paho.mqtt.c` C api for your system architecture, available [here](https://github.com/eclipse/paho.mqtt.c/releases).
2. Unzip this release to a location appropriate for your system.
3. Set an environment variable `$PAHO_HOME` / `%PAHO_HOME%` pointing to the location of the installed and unzipped release.
4. Make the paho.mqtt.c api available to kdb.

For Linux and MacOS, add the location of the 'lib' directory to `LD_LIBRARY_PATH`/`DYLD_LIBRARY_PATH` as appropriate
```
## Linux
export LD_LIBRARY_PATH=$PAHO_HOME/lib/:$LD_LIBRARY_PATH

## MacOS
export DYLD_LIBRARY_PATH=$PAHO_HOME/lib/:$DYLD_LIBRARY_PATH
```
For Windows, create links to the paho dll's in the `%QHOME%\w64` directory.
e.g.
```
cd %QHOME%\w64
MKLINK paho-mqtt3a.dll %PAHO_HOME%\lib\paho-mqtt3a.dll
MKLINK paho-mqtt3as.dll %PAHO_HOME%\lib\paho-mqtt3as.dll
MKLINK paho-mqtt3c.dll %PAHO_HOME%\lib\paho-mqtt3c.dll
MKLINK paho-mqtt3cs.dll %PAHO_HOME%\lib\paho-mqtt3cs.dll
```

#### ARM 32 build

For 32-bit arm systems, there are currently no prebuilt releases available. As such, a user is required to build the PAHO C api from source following the instructions [here](https://github.com/eclipse/paho.mqtt.c/blob/master/README.md#cross-compilation).

### Installing a release

It is recommended that a user install this interface through a release. This is completed in a number of steps

1. Ensure you have downloaded/installed the `paho.mqtt.c` C api following the instructions [here](#third-party-library-installation)
2. Download a release from [here](https://github.com/KxSystems/mqtt/releases) for your system architecture.
3. Install script `hdf5.q` to `$QHOME`, and binary file `lib/libhdf5.(so|dll)` to `$QHOME/[mlw](64)`, by executing the following from the Release directory
```
## Linux/MacOS
chmod +x install.sh && ./install.sh

## Windows
install.bat
```
  
### Building and installing from source

In order to successfully build and install this interface from source, the following environment variables must be set

1. `PAHO_HOME` = Location of a paho mqtt C api release
2. `QHOME` = Q installation directory (directory containing `q.k`)

#### Linux/MacOS/ARM 32-bit

- Create an out-of-source directory for the CMake and object files

```bash
mkdir cmake && cd cmake
```

- Generate the makefile (defaults to a generator for the native build system with a release target type)

```bash
cmake ..
```

- Build the interface shared object

```bash
make
```

- Create the installation package into sub-directory `mqtt`

```bash
make install
```

- Install the package (copies the shared object to`$QHOME/[ml](64|32)` )

```bash
cd mqtt && ./install.sh
```

#### Windows

From a Visual Studio command prompt:

- Create an out-of-source directory for the CMake and object files.

```bash
mkdir cmake && cd cmake
```

- Generate the VS solution

```bash
cmake ..
```

- Build the interface DLL and create the installation package into sub-directory `mqtt`

```bash
MSBuild.exe INSTALL.vcxproj /p:Configuration=Release /p:Platform=x64
```

- Install the package (copies the shared object to`%QHOME%/w64` )

```bash
cd mqtt && install.bat
```

#### Docker - Linux

A sample docker file is provided in the `docker_linux` directory to create a CentOS 7 environment (including downloading the `paho.mqtt.c` 64 bit Linux release) before building and installing the kdb+ `mqtt` interface.

The `PAHO_HOME` and `QHOME` directories are specified at the top of `mqtt_build.bat`, which sets up the environment specified in `Dockerfile.build` and invokes `mqtt_build.sh` to build the library.

## Documentation

Documentation outlining the functionality available for this interface can be found [here](http://code.kx.com/q/interfaces/mqtt/).

## Status

The mqtt interface is provided here under an Apache 2.0 license.

If you find issues with the interface or have feature requests, please consider raising an issue [here](https://github.com/KxSystems/mqtt/issues).

If you wish to contribute to this project, please follow the contributing guide [here](CONTRIBUTING.md).
