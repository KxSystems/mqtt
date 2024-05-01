# ![MQTT](mqtt.png) `mqtt` - MQTT for kdb+

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/kxsystems/mqtt?include_prereleases)](https://github.com/kxsystems/mqtt/releases) [![Travis (.org) branch](https://img.shields.io/travis/kxsystems/mqtt/master?label=travis%20build)](https://travis-ci.org/kxsystems/mqtt/branches)

This interface lets you communicate with an MQTT broker from a kdb+ session. 
The interface follows closely the [PAHO MQTT C API](https://github.com/eclipse/paho.mqtt.c). 
Exposed functionality includes

-   connect to a MQTT broker
-   subscribe to topics
-   publish to a broker

This is part of the [_Fusion for kdb+_](http://code.kx.com/q/interfaces#fusion/) interface collection.

## New to kdb+ ?

Kdb+ is the world’s fastest timeseries database, optimized for ingesting, analyzing and storing massive amounts of structured data. To get started with kdb+, please visit https://code.kx.com/q/learn/ for downloads and developer information. For general information, visit https://kx.com/

## New to MQTT ?

Message Queueing Telemetry Transport (MQTT) is a machine-to-machine/IOT connectivity protocol. It is designed to provide a lightweight publish/subscribe messaging protocol.

MQTT is used commonly for constrained devices with low-bandwidth, high-latency or unreliable networks.

### Use cases

This messaging protocol is used extensively where a small software footprint is required, e.g.

-   communication with edge devices/sensors
-   home automation
-   mobile applications

:globe_with_meridians:
[MQTT: Real-world applications](https://en.wikipedia.org/wiki/MQTT#Real-world_applications "Wikipedia")

## API Documentation

:point_right: [`API reference`](docs/reference.md)

## Installation Documentation

:point_right: [`Install guide`](docs/install.md)

## Debugging/Tracing

:point_right: The underlying Paho MQTT library provides the ability to log diagnostic information to standard out or log files via an [environment variable](https://en.wikipedia.org/wiki/Environment_variable) as detailed [here](https://eclipse.dev/paho/files/mqttdoc/MQTTClient/html/tracing.html)

## Building From Source

:point_right: [`Building guide`](docs/build.md)

## Quick start

### Requirements

To run the examples you will need a MQTT broker e.g.

- [Mosquitto broker](https://mosquitto.org/download/)

### Start Mosquitto

Start Mosquitto on default port `localhost:1883`.

```bash
sudo systemctl start mosquitto
```

### Launch consumer

We assume you are in the source directory of the MQTT-kdb+ interface.

```bash
mqtt]$ cd q
q]$ q ../examples/consumer.q 
```


### Launch producer

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


## Status

The interface is currently available under an Apache 2.0 license and is supported on a best-efforts basis by the KX Fusion team. The interface is currently in active development, with additional functionality released on an ongoing basis.

:point_right:
[Issues and feature requests](../../../issues) 

:point_right:
[Guide to contributing](../CONTRIBUTING.md)
