# `mqtt` - MQTT for kdb+

`mqtt` is a thin wrapper for kdb+ around the [eclipse/paho.mqtt.c](https://github.com/eclipse/paho.mqtt.c) C API for the Message Queuing Telemetry Transport [(MQTT)](http://mqtt.org/) ISO publish-subscribe messaging protocol.

This is part of the [_Fusion for kdb+_](http://code.kx.com/v2/interfaces/fusion/) interface collection.

Please report problems related to this interface in the issues section [here](https://github.com/KxSystems/mqtt/issues).

See [code.kx.com/v2/interfaces/mqtt](http://code.kx.com/v2/interfaces/mqtt/) for full documentation.

## Build Instructions

The following steps will allow a user to build both the required mqtt eclipse c api and the kdb+ interface.

### Step 1

Install the latest version of `paho.mqtt.c` and follow the instructions build instructions for your system architecture [here](https://github.com/eclipse/paho.mqtt.c#build-instructions-for-gnu-make). The minimum tested version is v1.3.0.

The eclipse mqtt repository is at present expected to be located in the root of this repository at present in order to successfully build the interface. Greater flexibility in the expected location will be added moving forward.

For Mac users
*  Building using the instructions outlined in the above link will produce the appropriate shared objects however due to mac conventions these must be renamed using the following commands to remove training `.0`
```
$ mv /usr/local/lib/libpaho-mqtt3a.so.1.0 /usr/local/lib/libpaho-mqtt3a.so.1
$ mv /usr/local/lib/libpaho-mqtt3as.so.1.0 /usr/local/lib/libpaho-mqtt3as.so.1
$ mv /usr/local/lib/libpaho-mqtt3c.so.1.0 /usr/local/lib/libpaho-mqtt3c.so.1
$ mv /usr/local/lib/libpaho-mqtt3cs.so.1.0 /usr/local/lib/libpaho-mqtt3cs.so.1
```

### Step 2

Compile and install a shared object file for mqtt

```bash
// produce the shared object
$make
// copy shared object to `$QHOME/<arch>`
$make install
// remove shared object from build location
$make clean
```

## Documentation

See [code.kx.com/v2/interfaces/mqtt](http://code.kx.com/v2/interfaces/mqtt/).
