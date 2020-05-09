SETLOCAL

SET QARCH=%2

SET ERROR=0
IF NOT DEFINED QHOME (
    ECHO QHOME is not set
    SET ERROR=1
)
IF ["%QARCH%"]==[""] (
    ECHO QARCH is not set
    SET ERROR=2
)
IF %ERROR% NEQ 0 GOTO :finished

SET QINSTALLDIR="%QHOME%\%QARCH%"
IF NOT EXIST %QINSTALLDIR% (
    ECHO Install directory does not exist: %QINSTALLDIR%
    SET ERROR=3
    GOTO :finished
)

ECHO Copying mqtt.dll to: %QINSTALLDIR% ...
COPY /Y %1 %QINSTALLDIR% > NUL
SET ERROR=%ERRORLEVEL%


:finished
IF %ERROR% EQU 0 (
    ECHO Installation suceeded
) ELSE (
    ECHO Installation failed
)

ENDLOCAL & EXIT /B %ERROR%

