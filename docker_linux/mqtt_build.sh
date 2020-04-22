#/bin/bash

make --directory /source/paho.mqtt.c
make install --directory /source/paho.mqtt.c
make --directory /source/mqtt

ln -s -f /source/mqtt/mqtt.so /source/mqtt/examples/mqtt.so

/bin/bash
