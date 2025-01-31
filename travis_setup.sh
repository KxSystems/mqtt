#!/bin/bash

mkdir cbuild

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
  wget https://github.com/eclipse-paho/paho.mqtt.c/releases/download/v1.3.14/Eclipse-Paho-MQTT-C-1.3.14-Darwin.tar.gz
  tar xvf Eclipse-Paho-MQTT-C-1.3.14-Darwin.tar.gz -C ./cbuild --strip-components=1
elif [ "$TRAVIS_OS_NAME" == "linux" ]; then
  /bin/sh docker_linux/build_libpaho.sh
  tar xvf Eclipse-Paho-MQTT-C-1.3.14-Linux.tar.gz -C ./cbuild --strip-components=1
elif [ "$TRAVIS_OS_NAME" == "windows" ]; then
  wget https://github.com/eclipse-paho/paho.mqtt.c/releases/download/v1.3.14/eclipse-paho-mqtt-c-win64-1.3.14.zip
   7z x -ocbuild eclipse-paho-mqtt-c-win64-1.3.14.zip
else
  echo "$TRAVIS_OS_NAME is currently not supported"  
fi
