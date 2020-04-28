SETLOCAL

SET PAHO_MQTT_BUILD="C:\Git\paho.mqtt.c\build.paho\src"

DEL paho-mqtt3a.dll paho-mqtt3as.dll paho-mqtt3c.dll paho-mqtt3cs.dll

MKLINK paho-mqtt3a.dll %PAHO_MQTT_BUILD%\paho-mqtt3a.dll
MKLINK paho-mqtt3as.dll %PAHO_MQTT_BUILD%\paho-mqtt3as.dll
MKLINK paho-mqtt3c.dll %PAHO_MQTT_BUILD%\paho-mqtt3c.dll
MKLINK paho-mqtt3cs.dll %PAHO_MQTT_BUILD%\paho-mqtt3cs.dll

ENDLOCAL
