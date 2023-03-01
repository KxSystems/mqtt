# ![mqtt](../mqtt.png) Using MQTT with kdb+


Message Queueing Telemetry Transport [(MQTT)](http://mqtt.org/) is a machine-to-machine/IOT connectivity protocol. It is designed to be lightweight, offering functionality for publish/subscribe messaging transport. 

The protocol is commonly used for constrained devices with low bandwidth, high latency or on unreliable networks.

## Use cases

This messaging protocol is used extensively where a small software footprint is required, e.g.

-   communication with edge devices/sensors
-   home automation
-   mobile applications

:globe_with_meridians:
[MQTT: Real-world applications](https://en.wikipedia.org/wiki/MQTT#Real-world_applications "Wikipedia")

## Kdb+/MQTT integration

This interface lets you communicate with an MQTT broker from a kdb+ session. 
The interface follows closely the [PAHO MQTT C API](https://github.com/eclipse/paho.mqtt.c). 
Exposed functionality includes

-   connect to a MQTT broker
-   subscribe to topics
-   publish to a broker

:point_right:
[Function reference](reference.md), [example implementations](examples.md)
<br>
:point_right:
[Install guide](../README.md#installation)

> Tip: To run the examples you will need a MQTT broker [installed and running locally](https://mosquitto.org/download/).

