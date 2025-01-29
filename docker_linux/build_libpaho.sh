#!/bin/sh

OLD_DIR=`pwd`
mkdir /tmp/paho && cd /tmp/paho
wget https://github.com/eclipse-paho/paho.mqtt.c/archive/refs/tags/v1.3.14.tar.gz -O paho_src.tar.gz
mkdir paho_src && tar xvf paho_src.tar.gz -C ./paho_src --strip-components=1 && cd paho_src
mkdir build && cd build 
cmake -DPAHO_BUILD_STATIC=FALSE -DPAHO_BUILD_SHARED=TRUE -DCMAKE_BUILD_TYPE=Release -DPAHO_WITH_SSL=TRUE -DOPENSSL_ROOT_DIR= -DPAHO_BUILD_DOCUMENTATION=FALSE -DPAHO_BUILD_SAMPLES=FALSE -DPAHO_HIGH_PERFORMANCE=TRUE -DCMAKE_INSTALL_LIBDIR=lib .. && cmake --build . && cpack --verbose
cp Eclipse*.tar.gz $OLD_DIR && rm -rf /tmp/paho 
