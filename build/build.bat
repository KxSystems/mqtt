@echo off
echo.

IF "%MQTT_INCLUDE%"=="" SET /P MQTT_INCLUDE="Please add full path to location of 'paho.mqtt.c/': "
IF "%VS_BAT%"=="" SET /P VS_BAT="Please add path to your vcvars64.bat file, usuall contained in C:\Program Files (x86)\Microsoft Visual Studio: "

:: Standalone build
curl -fskSL -o ../k.h   https://github.com/KxSystems/kdb/raw/master/c/c/k.h      || goto :error
curl -fskSL -o ../q.lib https://github.com/KxSystems/kdb/raw/master/w64/q.lib    || goto :error

::keep original PATH, PATH may get too long otherwise
set OP=%PATH%
call "%VS_BAT%"
cl /LD /DKXVER=3 /DWIN32 /I%MQTT_INCLUDE% /Felibmqtt.dll /O2 ../mqtt.c ../q.lib
set PATH=%OP%

exit /b 0
:error
exit /b %errorLevel%
