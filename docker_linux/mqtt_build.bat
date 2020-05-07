SETLOCAL

SET MQTT_SOURCE="C:\Users\guest\Development\mqtt"
SET QHOME_LINUX="C:\q"

docker build -f Dockerfile.build -t mqtt-dev .
docker run --rm -it -v %MQTT_SOURCE%:/source/mqtt -v %QHOME_LINUX%:/q mqtt-dev /bin/bash -c /source/mqtt_build.sh

ENDLOCAL
