:: 	Starten des Token Game unter Windows

@echo off

set nice=4
set BIN_DIR=%TNETHOME%\EDSPN\Simulation\bin
set SIM_DIR=%TNETHOME%\EDSPN\Simulation
set SHARED_BIN_DIR=%TNETHOME%\EDSPN\Shared\bin

set usage="Usage : TOKENGAME_win model <seed>"

if "%1" == "" (
  echo %usage%
  GOTO END_SCRIPT
) else (
  set NETNAME=%1
)

set NET=%MODELDIR%\%NETNAME%

:: check netname
if NOT EXIST "%NET%.TN" (
        echo "No such net %NET%.TN"
  GOTO END_SCRIPT
)

:: set default parameters
set distr=S
set distr_string=Sequential
set method=-S
set meth_string=spectral variance analysis
set trans=-Ton
set level=95
set errPercent=50
set epsilon=10
:: changed the fixed value to be set by parameter 2
set seed=%2
set maxsamples=0
set min_firings=50
set maxtime=0.0
set max_execution_time=0.0
set Visu_On=-Voff
set tg=tg

echo %distr_string% Simulation with %meth_string% of net:
echo %NET%
echo Detection of the initial transient period is on.

"%SHARED_BIN_DIR%\proc_tn.exe" -s %NETNAME%

if ERRORLEVEL 1 (
  echo.
  echo ERROR occurred while proc_TN execution.
  echo SOLUTION OF MODEL %NETNAME% FAILED.
  echo.
  GOTO END_SCRIPT
)

gcc -c "%NET%_MDF.c" -I"%SIM_DIR%\include" -I"%SIM_DIR%\sim_utils\include" -o "%NET%_MDF.obj"
gcc -c "%NET%_ERC.c" -I"%SIM_DIR%\include" -I"%SIM_DIR%\sim_utils\include" -o "%NET%_ERC.obj"
gcc -c "%NET%_RDY.c" -I"%SIM_DIR%\include" -I"%SIM_DIR%\sim_utils\include" -o "%NET%_RDY.obj"

:: compile spectral variance analysis
gcc -o "%NET%_slave.exe" "%NET%_MDF.obj" "%NET%_ERC.obj" "%BIN_DIR%\libsim_spec_slave_win.a" -L"%BIN_DIR%" -lsim_utils_win -L"%SHARED_BIN_DIR%" -lsysdep_win -lnetio_win -lws2_32
gcc -o "%NET%_master.exe" "%NET%_RDY.obj" "%BIN_DIR%\libsim_spec_master_win.a" -L"%BIN_DIR%" -lsim_utils_win -L"%SHARED_BIN_DIR%" -lsysdep_win -lnetio_win -lws2_32

"%NET%_master.exe" %NETNAME% %level% %epsilon% %seed% %maxsamples% %maxtime% %max_execution_time% %distr% %trans% %min_firings% %errPercent% %Visu_On% %tg%



:END_SCRIPT

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
if EXIST "%NET%_ERC.obj" (
del "%NET%_ERC.obj" >NUL 2>&1
)
if EXIST "%NET%_RDY.obj" (
del "%NET%_RDY.obj" >NUL 2>&1
)
if EXIST "%NET%_MDF.obj" (
del "%NET%_MDF.obj" >NUL 2>&1
)
if EXIST "%NET%.DEFINFO" (
del "%NET%.DEFINFO" >NUL 2>&1
)
if EXIST "%NET%_master.exe" (
del "%NET%_master.exe" >NUL 2>&1
)
if EXIST "%NET%_slave.exe" (
del "%NET%_slave.exe" >NUL 2>&1
)
if EXIST "%NET%.sim_curves" (
del "%NET%.sim_curves" >NUL 2>&1
)
if EXIST "%NET%.temp_sim_curves" (
del "%NET%.temp_sim_curves" >NUL 2>&1
)

if NOT EXIST "%NET%.dir" (
  md "%NET%.dir"
)

if EXIST "%NET%.pid" (
move "%NET%.pid" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.RESULTS" (
move "%NET%.RESULTS" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.STAT_OUT" (
move "%NET%.STAT_OUT" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.TN" (
move "%NET%.TN" "%NET%.dir" >NUL 2>&1
)
