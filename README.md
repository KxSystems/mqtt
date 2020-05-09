# `mqtt` - MQTT for kdb+

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/kxsystems/mqtt)](https://github.com/kxsystems/mqtt/releases) [![Travis (.org) branch](https://img.shields.io/travis/kxsystems/mqtt/master?label=travis%20build)](https://travis-ci.org/kxsystems/mqtt/branches)

`mqtt` is a thin wrapper for kdb+ around the [eclipse/paho.mqtt.c](https://github.com/eclipse/paho.mqtt.c) C API for the Message Queuing Telemetry Transport [(MQTT)](http://mqtt.org/) ISO publish-subscribe messaging protocol.

This is part of the [_Fusion for kdb+_](http://code.kx.com/q/interfaces/fusion/) interface collection.

Please report problems related to this interface in the issues section [here](https://github.com/KxSystems/mqtt/issues).

See [code.kx.com/q/interfaces/mqtt](https://code.kx.com/q/interfaces/mqtt) for full documentation.

## Requirements

- kdb+ ≥ 3.5 64-bit(Linux/MacOS/Windows) and 32-bit Arm
- [paho.mqtt.c](https://github.com/eclipse/paho.mqtt.c) ≥ 1.3.2 

## Installation

The following steps will allow a user to install both the required paho mqtt eclipse c api and the kdb+ interface.

### Third-Party Library Installation

#### Linux/MacOS/Windows 64-bit

Linux, MacOS and Windows users should complete the following steps

1. Download the latest release of the `paho.mqtt.c` C api for MQTT available [here](https://github.com/eclipse/paho.mqtt.c/releases) for your system architecture.
2. Unzip this release and move to a location appropriate for your system.
3. Set an environment variable `$PAHO_HOME/%PAHO_HOME%` which points to the location of the installed and unzipped release.

#### ARM 32 build

For 32-bit arm builds there is currently no prebuilt releases available, as such a user is required to build the interface paho C api from source following the instructions [here](https://github.com/eclipse/paho.mqtt.c/blob/master/README.md#cross-compilation)

### Interface build and install

#### Linux/MacOS/Arm 32-bit

In order to successfully build and install this interface the following environment variables must be set

1. PAHO_HOME = Location of a paho mqtt C api release
2. QHOME = Location of the q executable

* Create the interface shared object

```bash
make
```

* Copy the shared object to $QHOME/[ml](64|32)

```bash
make install
```

* Remove the shared object from the build location

Please ensure that you have "PAHO_HOME" set as an environment variable, this should be the location of the git repository installed above.

```bash
make clean
```

#### Windows

Two methods are provided to allow users to build a Windows version of this repository

1. A VS2019 solution is provided in the subdirectory `vs2019` to build mqtt for x64 Windows.
2. The script `builds/build.bat` allows for build to be initialised from command line

**Visual Studio 2019**

- This assumes that `mqtt`, `paho.mqtt.c` and `kdb` (downloaded from [here](https://github.com/kxsystems/kdb)) have been installed at the same directory level with `paho.mqtt.c` being built using `cbuild.bat`, located in the root of the`paho.mqtt.c` repository.  These can be changed by updating the Include and Library directories in the project.

**Command line**

- The script `build/build.bat` allows a user to install from command line. In order to ensure that the install is successful please modify the `MSVC` and `paho.mqtt.c` directories at the top of this file to support your environment.

#### Docker - Linux

A sample docker file is provided in the `docker_linux` directory to create a CentOS 7 environment with both the kdb+ `mqtt` interface and `paho.mqtt.c` api built and installed.

The source directories are specified at the top of `mqtt_build.bat` which sets up the environment specified in `Dockerfile.build` and invokes `mqtt_build.sh` to build the libraries.

## Examples

Basic q producer and receiver examples are available in the `examples` subdirectory, which connect to the MQTT broker at the Mosquitto brokers default port 1883.

It is assumed that the user has installed the `mqtt.so` or `mqtt.dll` binary in `$QHOME/[wlm](32|64)`.

You can install a local MQTT instance from Mosquitto by following the instructions [here](https://mosquitto.org/download/). This can be connected to locally or from a docker container (updating the connection address as appropriate).

The following shows the initialization of the example script and expected output.

```
$q producer.q

Type `\t 100` to publish a message every 100ms up to 200 messages, to stop type `\t 0`
q)\t 100
q)(`msgsent;1)
(`msgsent;2)
(`msgsent;3)
(`msgsent;4)

// Stop the producer
q)\t 0
```

The following code block shows the expected behaviour when a consumer is listening to the port supplied by the above producer

```
$q consumer.q
q)"Message received"
"Message received"
"Message received"
"Message received"

// Interrogate the table being serviced by the example script
q).mqtt.tab
topic  msg_sent                      msg_recv                      received_m..
-----------------------------------------------------------------------------..
topic1 2020.04.24D09:03:06.719866000 2020.04.24D09:03:06.721215000 topic1_0  ..
topic2 2020.04.24D09:03:06.720178000 2020.04.24D09:03:06.824406000 topic2_0  ..
topic1 2020.04.24D09:03:06.823581000 2020.04.24D09:03:06.924166000 topic1_1  ..
topic2 2020.04.24D09:03:06.923756000 2020.04.24D09:03:07.025692000 topic2_1  ..
```

### Docker - Linux

Samples docker files are provided in the `docker_linux` directory to create a Centos environment with both `mqtt` and `paho.mqtt.c` installed

The source directories are specified at the top of `mqtt_build.bat` which sets up the environment specified in `Dockerfile.build` and invokes `mqtt_build.sh` to build the libraries.

### Windows

Two methods are provided to allow users to build a Windows version of this repository

1. A VS2019 solution is provided in the directory `vs2019` to build mqtt for x64 Windows.
2. The script `builds/build.bat` allows for build to be initialised from command line

#### Visual Studio 2019

- This assumes that `mqtt`, `paho.mqtt.c` and `kdb` (downloaded from [here](https://github.com/kxsystems/kdb)) have been installed at the same directory level with `paho.mqtt.c` being built using `cbuild.bat`, located in the root of the`paho.mqtt.c` repository.  These can be changed by updating the Include and Library directories in the project.

#### Command line

- The script `build/build.bat` allows a user to install from command line. In order to ensure that the install is successful please modify the `MSVC` and `paho.mqtt.c` directories at the top of this file to support your environment.

## Examples

Basic q producer and receiver examples are available in the `examples` subdirectory, which connect to the MQTT broker at the default port 1883.

It is assumed that the user has installed the `mqtt.so` or `mqtt.dll` binary in `$QHOME/<arch>`.

You can install a local MQTT instance from Mosquitto by following the instructions [here](https://mosquitto.org/download/). This can be connected to locally or from a docker container (updating the connection address as appropriate).

The following shows the initialization of the example script and expected output.

```
$q producer.q

Type `\t 100` to publish a message every 100ms up to 200 messages, to stop type `\t 0`
q)\t 100
q)(`msgsent;1)
(`msgsent;2)
(`msgsent;3)
(`msgsent;4)

// Stop the producer
q)\t 0
```

The following code block shows the expected behaviour when a consumer is listening to the port supplied by the above producer

```
$q consumer.q
q)"Message received"
"Message received"
"Message received"
"Message received"

// Interrogate the table being serviced by the example script
q).mqtt.tab
topic  msg_sent                      msg_recv                      received_m..
-----------------------------------------------------------------------------..
topic1 2020.04.24D09:03:06.719866000 2020.04.24D09:03:06.721215000 topic1_0  ..
topic2 2020.04.24D09:03:06.720178000 2020.04.24D09:03:06.824406000 topic2_0  ..
topic1 2020.04.24D09:03:06.823581000 2020.04.24D09:03:06.924166000 topic1_1  ..
topic2 2020.04.24D09:03:06.923756000 2020.04.24D09:03:07.025692000 topic2_1  ..
```

## Documentation

Documentation outlining the functionality available for this interface can be found [here](http://code.kx.com/v2/interfaces/mqtt/).

## Status

The mqtt interface is still in development and is provided here as a beta release under an Apache 2.0 license.

If you find issues with the interface or have feature requests please consider raising an issue [here](https://github.com/KxSystems/mqtt/issues).

If you wish to contribute to this project please follow the contributing guide [here](CONTRIBUTING.md).
