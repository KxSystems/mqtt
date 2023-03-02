# Building MQTT kdb+ library from source

-> Tip: Prebuilt libraries are available from the [release page](https://github.com/KxSystems/mqtt/releases)

## Linux/macOS

1.  Download the latest release of the [MQTT C API](https://github.com/eclipse/paho.mqtt.c/releases) (`paho.mqtt.c`).
2.  Set an environment variable `MQTT_INSTALL_DIR` pointing to the location of the installed and unzipped release where `include` and `lib` are located. This environmental variable will link the library to the MQTT-kdb+ interface.

    ```bash
    ]$ mkdir paho_mqtt_c
    ]$ tar xzf Eclipse-Paho-MQTT-C-1.3.11-Linux.tar.gz -C paho_mqtt_c/ --strip-components=1
    ]$ cd paho_mqtt_c/
    paho_mqtt_c]$ export MQTT_INSTALL_DIR=$(pwd)
    ```

3.  For macOS add the `lib` directory which includes `include` and `lib` to the `DYLD_LIBRARY_PATH` environment variable e.g. if unzipped to `/Users/jim/Eclipse-Paho-MQTT-C/`, run:

    ```bash
    export DYLD_LIBRARY_PATH=/Users/jim/Eclipse-Paho-MQTT-C/lib/:$DYLD_LIBRARY_PATH
    ```

4.  Download a source [release](https://github.com/KxSystems/mqtt/releases) or clone the MQTT-kdb+ repository and build with `cmake`.

    ```bash
    ]$ git clone https://github.com/KxSystems/mqtt.git
    ]$ cd mqtt
    mqtt]$ mkdir build && cd build
    build]$ cmake ..
    build]$ cmake --build . --target install
    ```

**Note:** `cmake --build . --target install` as used in the Linux/macOS builds installs the required share object and q files to the `QHOME/[ml]64` and `QHOME` directories respectively. If you do not wish to install these files directly, you can execute `cmake --build .` instead of `cmake --build . --target install` and move the files from their build location at `build/mqttkdb`.

## Windows

1.  Download the latest release of the [MQTT C API](https://github.com/eclipse/paho.mqtt.c/releases) (`paho.mqtt.c`).
2.  Set an environment variable `MQTT_INSTALL_DIR` pointing to the location of the installed and unzipped release where `include` and `lib` are located. This environmental variable will link the library to the MQTT-kdb+ interface.
3.  Create links to the paho DLLs in the `%QHOME%\w64` directory.

    ```bat
    > mkdir paho_mqtt_c
    > 7z x eclipse-paho-mqtt-c-win64-1.3.11.zip -opaho_mqtt_c
    > cd paho_mqtt_c
    paho_mqtt_c> set MQTT_INSTALL_DIR=%cd%
    paho_mqtt_c> cd %QHOME%\w64
    w64> MKLINK paho-mqtt3c.dll %MQTT_INSTALL_DIR%\lib\paho-mqtt3c.dll
    w64> MKLINK paho-mqtt3cs.dll %MQTT_INSTALL_DIR%\lib\paho-mqtt3cs.dll
    ```

4.  Download a source [release](https://github.com/KxSystems/mqtt/releases) or clone the MQTT-kdb+ repository and build with `cmake`. Building the interface from source requires Visual Studio (assuming `-G "Visual Studio 15 2017 Win64"` is not necessary).

    ```bat
    > git clone https://github.com/KxSystems/mqtt.git
    > cd mqtt
    mqtt> mkdir build && cd build
    build> cmake --config Release ..
    build> cmake --build . --config Release --target install
    ```

**Notes:** 

1.  `cmake --build . --config Release --target install` installs the required share object and q files to the `QHOME\w64` and `QHOME` directories respectively. If you do not wish to install these files directly, you can execute `cmake --build . --config Release` instead of `cmake --build . --config Release --target install` and move the files from their build location at `build/mqttkdb`.
2.  You can use flag `cmake -G "Visual Studio 16 2019" -A Win32` if building 32-bit version.

## Docker – Linux

There are two sample Docker files provided in the `docker_linux` directory.

### CentOS 7

A sample Docker file is provided in the `docker_linux` directory to create a CentOS 7 environment (including downloading the `paho.mqtt.c` 64-bit Linux release) before building and installing the kdb+ `mqtt` interface.

The `MQTT_INSTALL_DIR` and `QHOME` directories are specified at the top of `mqtt_build.bat`, which sets up the environment specified in `Dockerfile.centos7` and invokes `mqtt_build.sh` to build the library.

### Alpine

A lightweight Alpine environment is also provided. The file `Dockerfile.alpine` specifies the environment and build steps. This image will have a smaller footprint than the above and may be more suited to smaller IOT devices. 

Note if this is to be built for a ARM64 device (on a non-ARM device), be sure to have multi-platform builds available. See this blog post for more information: 
https://www.docker.com/blog/multi-platform-docker-builds/

e.g. Building on x86-64 for ARM:

```bash
docker buildx build --platform linux/arm64 -t mqtt-kdb-arm64 --load --file docker_linux/Dockerfile.alpine .
```
