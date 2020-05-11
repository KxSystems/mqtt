@echo off

IF "%QHOME%"=="" (
    ECHO ERROR: Enviroment variable QHOME is NOT defined 
    EXIT /B
)

IF NOT EXIST %QHOME%\w64 (
    ECHO ERROR: Installation destination %QHOME%\w64 does not exist
    EXIT /B
)

IF NOT EXIST lib (
    ECHO ERROR: Directory 'lib' does not exist. Please run from release package
    EXIT /B
)

IF NOT EXIST script (
    ECHO ERROR: Directory 'script' does not exist. Please run from release package
    EXIT /B
)

ECHO Copying scripts to %QHOME%
COPY script\* %QHOME%
ECHO Copying dll to %QHOME%\w64
COPY lib\* %QHOME%\w64\

ECHO Installation complete