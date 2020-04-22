SETLOCAL
echo.

SET PAHO_MQTT="C:\Git\paho.mqtt.c"
SET VS_BAT="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"


:: Standalone build
curl -fskSL -o ../k.h   https://github.com/KxSystems/kdb/raw/master/c/c/k.h      || goto :error
curl -fskSL -o ../q.lib https://github.com/KxSystems/kdb/raw/master/w64/q.lib    || goto :error

SET PAHO_MQTT_INCLUDE=%PAHO_MQTT%"\src"
SET PAHO_MQTT_LIB=%PAHO_MQTT%"\build.paho\src"
SET PAHO_MQTT_INCLUDE=%PAHO_MQTT_INCLUDE:""=%
SET PAHO_MQTT_LIB=%PAHO_MQTT_LIB:""=%

::keep original PATH, PATH may get too long otherwise
set OP=%PATH%
call %VS_BAT%
set OLDLIB=%LIB%
set LIB=%PAHO_MQTT_LIB%;%LIB%
cl /LD /DKXVER=3 /DWIN32 /I%PAHO_MQTT_INCLUDE% /Femqtt.dll /O2 ../mqtt.c ../q.lib
set LIB=%OLDLIB%
set PATH=%OP%

ENDLOCAL
exit /b 0

:error
ENDLOCAL
exit /b %errorLevel%
