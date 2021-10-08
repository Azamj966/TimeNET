:: start statespace estimation  

@echo off

set SHARED_BIN_DIR=%TNETHOME%\EDSPN\Shared\bin


set usage="Usage: ESTIMSE_win <netname>"

if "%1" == "" (
  echo %usage%
  GOTO END_SCRIPT
) else (
  set NETNAME=%1
)

set NET=%MODELDIR%\%NETNAME%

:: check netname
if NOT EXIST "%NET%.TN" (
  echo "No such net"
  GOTO END_SCRIPT
)

:: Simulation mode to accept WEIBULL and other distributions
"%SHARED_BIN_DIR%\proc_tn.exe" -s "%NETNAME%"

if ERRORLEVEL 1 (
  echo.
  echo ERROR occurred while proc_TN execution.
  echo SOLUTION OF MODEL %NETNAME% FAILED.
  echo.
  GOTO END_SCRIPT
)

"%SHARED_BIN_DIR%\estimSE.exe" "%NETNAME%"

if ERRORLEVEL 1 (
  echo ERROR while estimSE execution.
  GOTO END_SCRIPT
)  

:END_SCRIPT
echo Removing temporary files

if EXIST "%NET%_MDF.c" (
del /Q "%NET%_MDF.c" >NUL 2>&1
)
if EXIST "%NET%_ERC.c" (
del /Q "%NET%_ERC.c" >NUL 2>&1
)
if EXIST "%NET%_RDY.c" (
del /Q "%NET%_RDY.c" >NUL 2>&1
)
if EXIST "%NET%.pmf" (
del /Q "%NET%.pmf" >NUL 2>&1
)
if EXIST "loopcount.c" (
del /Q "loopcount.c" >NUL 2>&1
)
if EXIST "loopcount.exe" (
del /Q "loopcount.exe" >NUL 2>&1
)

if NOT EXIST "%NET%.dir" (
  md "%NET%.dir"
)

if EXIST "%NET%.ese" (
move "%NET%.ese" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.INV" (
move "%NET%.INV" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.ECS" (
move "%NET%.ECS" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.DEFINFO" (
move "%NET%.DEFINFO" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.TN" (
move "%NET%.TN" "%NET%.dir" >NUL 2>&1
)
if EXIST "count_result" (
move "count_result" "%NET%.dir" >NUL 2>&1
)
