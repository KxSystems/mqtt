# `mqtt` - MQTT for kdb+

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/kxsystems/mqtt)](https://github.com/kxsystems/mqtt/releases) [![Travis (.org) branch](https://img.shields.io/travis/kxsystems/mqtt/master?label=travis%20build)](https://travis-ci.org/kxsystems/mqtt/branches)

## Introduction

This interface provides kdb+ users with the ability to interact with the Message Queuing Telemetry Transport [(MQTT)](http://mqtt.org/) ISO publish-subscribe messaging protocol. The interface is a thin wrapper for kdb+ around the [eclipse/paho.mqtt.c](https://github.com/eclipse/paho.mqtt.c) C api.

This is part of the [_Fusion for kdb+_](http://code.kx.com/q/interfaces/fusion/) interface collection.

## New to kdb+ ?

Kdb+ is the world's fastest time-series database, optimized for ingesting, analyzing and storing massive amounts of structured data. To get started with kdb+, please visit https://code.kx.com/q/learn/ for downloads and developer information. For general information, visit https://kx.com/

## New to MQTT ?

Message Queueing Telemetry Transport(MQTT) is a machine-to-machine/IOT connectivity protocol. It is designed to be lightweight offering functionality for publish/subscribe messaging transport.

This protocol is used commonly for constrained devices with low-bandwidth, high-latency or unreliable networks.

## Installation

### Requirements

* kdb+ ≥ 3.5 64-bit(Linux/MacOS/Windows) and 32-bit Arm
* [paho.mqtt.c](https://github.com/eclipse/paho.mqtt.c) ≥ 1.3.2

### Third-Party Library Installation

#### Linux/MacOS/Windows 64-bit

Linux, MacOS and Windows users should complete the following steps

1. Download the latest release of the `paho.mqtt.c` C api for your system architecture, available [here](https://github.com/eclipse/paho.mqtt.c/releases).
2. Unzip this release and move to a location appropriate for your system.
3. Set an environment variable `$BUILD_HOME` / `%BUILD_HOME%` pointing to the location of the installed and unzipped release.

#### ARM 32 build

For 32-bit arm systems, there are currently no prebuilt releases available. As such, a user is required to build the PAHO C api from source following the instructions [here](https://github.com/eclipse/paho.mqtt.c/blob/master/README.md#cross-compilation).

### Interface build and install

In order to successfully build and install this interface, the following environment variables must be set

1. `BUILD_HOME` = Location of a paho mqtt C api release
2. `QHOME` = Q installation directory (directory containing `q.k`)

#### Linux/MacOS/Arm 32-bit

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

The `BUILD_HOME` and `QHOME` directories are specified at the top of `mqtt_build.bat`, which sets up the environment specified in `Dockerfile.build` and invokes `mqtt_build.sh` to build the library.

## Documentation

Documentation outlining the functionality available for this interface can be found [here](http://code.kx.com/q/interfaces/mqtt/).

## Status

The mqtt interface is provided here under an Apache 2.0 license.

If you find issues with the interface or have feature requests, please consider raising an issue [here](https://github.com/KxSystems/mqtt/issues).

If you wish to contribute to this project, please follow the contributing guide [here](CONTRIBUTING.md).
