@echo off

set currentdir=%cd%

set BIN_DIR=%TNETHOME%\EDSPN\StatAnalysis\bin
set SHARED_BIN_DIR=%TNETHOME%\EDSPN\Shared\bin


GOTO END_USAGE

# show usage for help
:USAGE
  echo.
  echo Usage:
  echo RECHABILITY_GRAPH_win netname
  echo.
  GOTO END_SOLVE
:END_USAGE

if "%1" == "" (
  echo %usage%
  GOTO END_SCRIPT
) else (
  set NETNAME=%1
)

set NET=%MODELDIR%\%NETNAME%.dir\%NETNAME%

if NOT EXIST "%NET%.TN" (
  echo %NET%.TN: No such file, please run a Stationary Analysis.
  GOTO END_SCRIPT
)  

if NOT EXIST "%NET%.RG" (
  echo %NET%.RG: No such file, please run a Stationary Analysis.
  GOTO END_SCRIPT
)  

:: build reachability graph visualisation

echo Creating Reachability Graph Visualisation...

"%TNETHOME%\EDSPN\StatAnalysis\bin\showRG.exe" "%NET%"

:END_SCRIPT

cd %currentdir%
