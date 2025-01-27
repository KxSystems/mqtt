# MQTT kdb+ library installation

## Requirements

* kdb+ ≥ 3.5 64-bit(Linux/MacOS/Windows) and 32-bit ARM
* [`paho.mqtt.c`](https://github.com/eclipse/paho.mqtt.c) >= 1.3.11
* CMake ≥ 3.1 [^1]

[^1]: Required when building from source

## Installing a release

1.  Ensure [MQTT C API](https://github.com/eclipse/paho.mqtt.c/releases) (`paho.mqtt.c`) is installed.

    -    Linux: Paho MQTT provides a pre-build binary which may be built against a more recent version of glibc that may be available on your system, resulting in an error such as `'/lib64/libc.so.6: version `GLIBC_2.38' not found`. To get around this, build from the paho mqtt library from the source code package. See docker_linux/build_libpaho.sh as an example of how to download and build into a `tar.gz` package.

2.  Make the MQTT library available from kdb+:

    -   Linux: Add the `lib` directory, which includes `include` and `lib` to the `LD_LIBRARY_PATH` environment variable e.g. if unzipped to `/usr/local/Eclipse-Paho-MQTT-C/`, run:

        ```bash
        export LD_LIBRARY_PATH=/usr/local/Eclipse-Paho-MQTT-C/lib/:$LD_LIBRARY_PATH
        ```
        
    -   macOS: Add the `lib` directory, which includes `include` and `lib`to the `DYLD_LIBRARY_PATH` environment variable e.g. if unzipped to `/Users/jim/eclipse-paho-mqtt-c/`, run:
  
        ```bash
        export DYLD_LIBRARY_PATH=/Users/jim/Eclipse-Paho-MQTT-C/lib/:$DYLD_LIBRARY_PATH
        ```

    -   Windows: Add `paho-mqtt3c.dll` and `paho-mqtt3cs.dll` to the kdb+ directory e.g. `C:\q\w64` for 64-bit.

3.  Download the latest release of the MQTT interface from our [releases page](https://github.com/KxSystems/mqtt). To install shared library and q files, use:

    ```shell
    # Linux/MacOS
    ./install.sh
    
    # Windows
    install.bat
    ```

    or copy `mqtt.q` to `QHOME`, then copy `mqttkdb.so` or `mqttkdb.dll` into `QHOME/[l|m|w]64`

