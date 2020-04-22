SETLOCAL

SET MQTT_SOURCE="c:\git\mqtt"
SET PAHO_MQTT="c:\git\paho.mqtt.c"
SET QHOME_LINUX="c:\linux_shared\q"

docker build -f Dockerfile.build -t mqtt-dev .
docker run --rm -it -v %MQTT_SOURCE%:/source/mqtt -v %PAHO_MQTT%:/source/paho.mqtt.c -v %QHOME_LINUX%:/q mqtt-dev /bin/bash -c /source/mqtt/docker_linux/mqtt_build.sh

ENDLOCAL
