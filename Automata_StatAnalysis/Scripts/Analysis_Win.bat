@echo off

:: running structural analysis
"%TNETHOME%\Automata_StatAnalysis\bin\AutomataStatAnalysis.exe" %ANALYSISTYPE% %NETNAME% %MODELDIR%

if ERRORLEVEL 1 (
  echo.
  echo ERROR occurred while AutomataStatAnalysis.exe execution.
  echo SOLUTION OF MODEL %NETNAME% FAILED.
  echo.
  GOTO END_SCRIPT
)


:: verschiebe *.SSP
echo move files

set NET=%MODELDIR%\%NETNAME%

if NOT EXIST "%NET%.dir" (
  md "%NET%.dir"
)

if EXIST "%NET%.SSP" (
move "%NET%.SSP" "%NET%.dir" >NUL 2>&1
)

:END_SCRIPT