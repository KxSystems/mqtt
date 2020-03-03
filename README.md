# `mqtt` - MQTT for kdb+

`mqtt` is a thin wrapper for kdb+ around [eclipse/paho.mqtt.c](https://github.com/eclipse/paho.mqtt.c)
C API for the Message Queuing Telemetry Transport [(MQTT)](http://mqtt.org/) ISO publish-subscribe messaging protocol.

It is part of the [_Fusion for kdb+_](http://code.kx.com/v2/interfaces/fusion/) interface collection.

Please [report issues](https://github.com/KxSystems/mqtt/issues) in this repository.

See [code.kx.com/v2/interfaces/mqtt](http://code.kx.com/v2/interfaces/mqtt/) for full documentation.

## Build Instructions

The following steps will allow 

### Step 1

Install the latest version of `paho.mqtt.c` and follow the instructions build instructions for your system architecture [here](https://github.com/eclipse/paho.mqtt.c#build-instructions-for-gnu-make). The minimum tested version is v1.3.0.

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
