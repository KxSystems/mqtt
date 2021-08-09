#!/bin/sh
	
cd /source

wget https://github.com/eclipse/paho.mqtt.c/archive/refs/tags/v1.3.9.tar.gz
tar xvf v1.3.9.tar.gz -C ./paho.mqtt.c --strip-components=1

cd paho.mqtt.c

# This library is not present in the Alpine image, removing it from the compiler args
sed -i 's/GAI_LIB = -lanl/#GAI_LIB = -lanl/g' Makefile
ln -s /usr/lib/libnsl.so.2 /usr/lib/libnsl.so.1

make
make install
mkdir -p  include/lib
cd include/lib

ln -s /usr/local/lib/libpaho-mqtt3c.so libpaho-mqtt3c.a
ln -s /usr/local/lib/libpaho-mqtt3cs.so libpaho-mqtt3cs.a
ln -s /usr/local/lib/libpaho-mqtt3as.so libpaho-mqtt3as.a
ln -s /usr/local/lib/libpaho-mqtt3a.so libpaho-mqtt3a.a

mkdir /source/mqtt/cmake
cd  /source/mqtt/cmake
cmake ..
make
make install