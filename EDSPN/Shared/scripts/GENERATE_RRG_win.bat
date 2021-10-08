:: generate reduced reachability graph

@echo off

set usage="Usage: GENERATE_RRG_win [-t] <net-name>"
set trans=0
set SHARED_BIN_DIR=%TNETHOME%\EDSPN\Shared\bin

if "%1" == "-t" (
  set trans=1
  shift
)  

if "%1" == "" (
  echo usage
  GOTO END_SCRIPT
) else (
  set NETNAME=%1
)

set NET=%MODELDIR%\%NETNAME%

if NOT EXIST "%NET%.TN" (
  echo %NET%.TN: No such net
  echo %usage%  
  GOTO END_SCRIPT
)  

:: generate marking dependent functions
"%TNETHOME%\EDSPN\Shared\bin\proc_tn.exe" %NETNAME%

if ERRORLEVEL 1 (
  echo.
  echo ERROR occurred while proc_TN execution.
  echo SOLUTION OF MODEL %NETNAME% FAILED.
  echo.
  GOTO END_SCRIPT
)

:: running structural analysis
"%TNETHOME%\EDSPN\Shared\bin\struct_tn.exe" -i %NETNAME%

if ERRORLEVEL 1 (
  echo.
  echo ERROR occurred while struct_tn execution.
  echo SOLUTION OF MODEL %NETNAME% FAILED.
  echo.
  GOTO END_SCRIPT
)

echo GENERATING REDUCED REACHABILITY GRAPH

gcc -c "%NET%_MDF.c" -I"%SIM_DIR%\include" -I"%SIM_DIR%\sim_utils\include" -o "%NET%_MDF.obj"
gcc -o "%NETNAME%_genrrg.exe" "%NET%_MDF.obj" "%SHARED_BIN_DIR%\libgenrrg_win.a" -L"%SHARED_BIN_DIR%" -lsysdep_win -lws2_32

if "%trans%" == "1" (
  "%NETNAME%_genrrg.exe" -t "%NETNAME%"
) else (
  "%NETNAME%_genrrg.exe" "%NETNAME%"
) 

:END_SCRIPT
echo Removing temporary files

if EXIST "%NET%_MDF.obj" (
del /Q "%NET%_MDF.obj" >NUL 2>&1
)
if EXIST "%NET%_MDF.c" (
del /Q "%NET%_MDF.c" >NUL 2>&1
)
