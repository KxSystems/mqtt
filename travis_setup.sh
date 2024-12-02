#!/bin/bash

mkdir cbuild

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
  wget https://github.com/eclipse/paho.mqtt.c/releases/download/v1.3.13/Eclipse-Paho-MQTT-C-1.3.13-Darwin.tar.gz
  tar xvf Eclipse-Paho-MQTT-C-1.3.13-Darwin.tar.gz -C ./cbuild --strip-components=1
elif [ "$TRAVIS_OS_NAME" == "linux" ]; then
  wget https://github.com/eclipse/paho.mqtt.c/releases/download/v1.3.13/Eclipse-Paho-MQTT-C-1.3.13-Linux.tar.gz
  tar xvf Eclipse-Paho-MQTT-C-1.3.13-Linux.tar.gz -C ./cbuild --strip-components=1
elif [ "$TRAVIS_OS_NAME" == "windows" ]; then
  wget https://github.com/eclipse/paho.mqtt.c/releases/download/v1.3.13/eclipse-paho-mqtt-c-win64-1.3.13.zip
   7z x -ocbuild eclipse-paho-mqtt-c-win64-1.3.13.zip
else
  echo "$TRAVIS_OS_NAME is currently not supported"  
fi
