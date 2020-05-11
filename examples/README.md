# kdb+ interface for MQTT (Examples)

The demonstration documented below provides users with a simple example of interfacing kdb+ with the MQTT messaging protocol.

## Requirements

It is assumed that the user has installed the `mqtt.so` or `mqtt.dll` binary in `$QHOME/[wlm](32|64)`, following the instructions outlined [here](../README.md)

This demonstration requires that a MQTT broker is running on port 1883. This is the default running port for a Mosquitto broker running locally. You can install a local MQTT instance from Mosquitto by following the instructions [here](https://mosquitto.org/download/). This can be connected to locally or from a docker container (updating the connection address as appropriate).

## Example

* The following shows the initialization of the example script and expected output.

```
$ q producer.q

Type `\t 100` to publish a message every 100ms up to 200 messages, to stop type `\t 0`
q)\t 100
q)(`msgsent;1)
(`msgsent;2)
(`msgsent;3)
(`msgsent;4)

// Stop the producer
q)\t 0
```

* The following code block shows the expected behaviour when a consumer is listening to the port supplied by the above producer using the `consumer.q` script

```
$ q consumer.q

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
