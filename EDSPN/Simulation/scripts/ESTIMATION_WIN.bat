:: 	Starten der verteilten DSPN Simulation unter Windows

@echo off

set currentdir=%cd%

set transition="null"

set BIN_DIR=%TNETHOME%\EDSPN\Simulation\bin
set SHARED_BIN_DIR=%TNETHOME%\EDSPN\Shared\bin

set usage="Usage : ESTIMATION model <transition>" 

if %1 == "" (
  echo %usage%
  GOTO END_SCRIPT
) else (
  set NETNAME=%1
)

set NET=%MODELDIR%\\%NETNAME%

:: check netname
if NOT EXIST "%NET%.TN" (
	echo "No such net"
	GOTO END_SCRIPT
)

:: set parameters
set transition=%2


"%SHARED_BIN_DIR%\proc_tn.exe" -s %NETNAME%

if ERRORLEVEL 1 (
  echo.
  echo ERROR occurred while proc_TN execution.
  echo SOLUTION OF MODEL %NETNAME% FAILED.
  echo.
  GOTO END_SCRIPT
)

"%SHARED_BIN_DIR%\struct_tn.exe" -i %NETNAME%

if ERRORLEVEL 1 (
  echo.
  echo ERROR occurred while struct_tn execution.
  echo SOLUTION OF MODEL %NETNAME% FAILED.
  echo.
  GOTO END_SCRIPT
)

echo.
echo STARTING ESTIMATION.
echo.

"%BIN_DIR%\sim_rst_estimation.exe" "%NET%" %transition%

if ERRORLEVEL 1 (
   echo ERROR occurred while estimation execution
   GOTO END_SCRIPT
)

echo.
echo ESTMIATION FINISHED.


:END_SCRIPT
echo.
echo Removing temporary files

if EXIST "%NET%_ERC.c" (
del "%NET%_ERC.c" >NUL 2>&1
)
if EXIST "%NET%_RDY.c" (
del "%NET%_RDY.c" >NUL 2>&1
)
if EXIST "%NET%_MDF.c" (
del "%NET%_MDF.c" >NUL 2>&1
)
if EXIST "%NET%.DEFINFO" (
del "%NET%.DEFINFO" >NUL 2>&1
)
if EXIST "%NET%_RST.TN" (
del "%NET%_RST.TN" >NUL 2>&1
)
if EXIST "%NET%_RST.RESULTS" (
del "%NET%_RST.RESULTS" >NUL 2>&1
)
if EXIST "%NET%.PARAM" (
del "%NET%.PARAM" >NUL 2>&1 
)


if NOT EXIST "%NET%.dir" (
	md "%NET%.dir"
)

if EXIST "%NET%.ECS" (
move "%NET%.ECS" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.INV" (
move "%NET%.INV" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.STRUCT" (
move "%NET%.STRUCT" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.TN" (
move "%NET%.TN" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.ESTIM" (
move "%NET%.ESTIM" "%NET%.dir" >NUL 2>&1
)

cd %currentdir%