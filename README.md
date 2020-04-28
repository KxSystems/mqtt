# `mqtt` - MQTT for kdb+

`mqtt` is a thin wrapper for kdb+ around the [eclipse/paho.mqtt.c](https://github.com/eclipse/paho.mqtt.c) C API for the Message Queuing Telemetry Transport [(MQTT)](http://mqtt.org/) ISO publish-subscribe messaging protocol.

This is part of the [_Fusion for kdb+_](http://code.kx.com/v2/interfaces/fusion/) interface collection.

Please report problems related to this interface in the issues section [here](https://github.com/KxSystems/mqtt/issues).

See [code.kx.com/v2/interfaces/mqtt](https://github.com/cmccarthy1/docs/blob/mqtt/docs/interfaces/mqtt.md) for full documentation. (set to current holding location for now will be updated)

## Build Instructions

The following steps will allow a user to build both the required mqtt eclipse c api and the kdb+ interface.

### General

#### Step 1

Install the latest version of `paho.mqtt.c` and follow the instructions build instructions for your system architecture [here](https://github.com/eclipse/paho.mqtt.c#build-instructions-for-gnu-make). The minimum tested version is v1.3.2.

It is recommended that `mqtt` and `pahot.mqtt.c` are located at the same directory level with the build being run from the parent directory (e.g. using `make --directory`).  Greater flexibility in the expected location will be added moving forward.

For Mac users
*  Building using the instructions outlined in the above link will produce the appropriate shared objects however due to mac conventions these must be renamed using the following commands to remove trailing `.0`
```
$ mv /usr/local/lib/libpaho-mqtt3a.so.1.0 /usr/local/lib/libpaho-mqtt3a.so.1
$ mv /usr/local/lib/libpaho-mqtt3as.so.1.0 /usr/local/lib/libpaho-mqtt3as.so.1
$ mv /usr/local/lib/libpaho-mqtt3c.so.1.0 /usr/local/lib/libpaho-mqtt3c.so.1
$ mv /usr/local/lib/libpaho-mqtt3cs.so.1.0 /usr/local/lib/libpaho-mqtt3cs.so.1
```

#### Step 2

Compile and install a shared object file for mqtt

```bash
// produce the shared object
$make
// copy shared object to `$QHOME/<arch>`
$make install
// remove shared object from build location
$make clean
```

### Docker - Linux

Samples docker files are provided in `docker_linux to` create a Centos environment with both `mqtt` and `paho.mqtt.c` 

The source directories are specified at the top of `mqtt_build.bat` which sets up the environment specified in `Dockerfile.build` and invokes `mqtt_build.sh` to build the libraries.

### Windows

A VS2019 solution is provided in the subdirectory `vs2019` to build mqtt for x64 Windows.  It assumes that `mqtt`, `paho.mqtt.c` and `kdb` (downloaded from [here](https://github.com/kxsystems/kdb)) have been installed at the same directory level with `paho.mqtt.c` being built using `cbuild.bat`, located in the root of the`paho.mqtt.c` repository.  These can be changed by updating the Include and Library directories in the project.

Alternatively the simple script `build/build.bat` performs the build from the command line, with the MSVC and `paho.mqtt.c` directories specified at the top of the file.

## Examples

Basic q producer and receiver examples are available in the `examples` subdirectory, which connect to the MQTT broker at port 1883.  [Mosquitto](https://mosquitto.org/download/) can be used to install a local MQTT instance as a Windows service, which can be connected to from the Windows host or from a docker container (updating the connection address as appropriate).

These require the built libraries to either be installed on the system or be present in the `examples` directory.  To assist with this `mqtt_build.sh` used by the docker build runs `make install ` which copies `mqtt.so` to `$QHOME/$QARCH`.  On Windows, `vs2019/mqtt_install.bat` does likewise for `mqtt.dll` and is run both by the VS2019 solution (as a post-install step) and `build/build.bat`.  However, the `paho.mqtt.c` DLLs also need to present so `examples\symlink_paho.bat` can be used to symlink them to the `examples` directory.

## Documentation

See [code.kx.com/v2/interfaces/mqtt](http://code.kx.com/v2/interfaces/mqtt/).

