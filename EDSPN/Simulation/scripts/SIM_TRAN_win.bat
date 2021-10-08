:: 	Starten der verteilten DSPN Simulation unter Windows

@echo off

set currentdir=%cd%

set distr="null"
set distr_string="null"
set method="null"
set meth_string="null"
set resmon="null"
set resmon_string="null"
set trans="null"
set trans_string="null"
set rule="null"
set rule_string="null"
set transient_time="null"
set abort_condition="null"
set no_sampling_points="null"
set maxsamples="null"
set maxtime="null"
set max_execution_time="null"
set nice=4
set BIN_DIR=%TNETHOME%\EDSPN\Simulation\bin
set SIM_DIR=%TNETHOME%\EDSPN\Simulation
set SHARED_BIN_DIR=%TNETHOME%\EDSPN\Shared\bin

set usage="Usage : SIM_TRAN_win model -{D|S} -{Pon|Poff} -{Gon|Goff} <transient_time> <abort_cond> <no_sampling_points> <conf.level> <epsilon> <seed> <max.real time>"

if "%1" == "" (
  echo %usage%
  GOTO END_SCRIPT
) else (
  set NETNAME=%1
)

if "%MODELDIR%" == "" (
  set NET=%cd%\%NETNAME%
) else (
  set NET=%MODELDIR%\%NETNAME%
)


:: check netname
if NOT EXIST "%NET%.TN" (
  echo "No such net"
  GOTO END_SCRIPT
)

:: set parameters
if "%2" == "" (
  set distr=-S
) else (
  set distr=%2
)
if "%3" == "" (
  set rule=-Pon
) else (
  set rule=%3
)


if "%4" == "" (
  set resmon=-Gon
) else (
  set resmon=%4
)


if "%5" == "" (
  set transient_time=100.0
) else (
  set transient_time=%5
)
if "%~6" == "" (
  set abort_condition=0
) else (
  set abort_condition=%6
)
if "%7" == "" (
  set no_sampling_points=100
) else (
  set no_sampling_points=%7
)
if "%8" == "" (
  set level=95
) else (
  set level=%8
)
if "%9" == "" (
  set epsilon=10
) else (
  set epsilon=%9
)
shift /1
if "%9" == "" (
  set seed=1
) else (
  set seed=%9
)
shift /1
if "%9" == "" (
  set max_execution_time=300.0
) else (
  set max_execution_time=%9
)

:: Defaults
set trans = "Toff"
set method = "-STR"
:: Dummys
set maxsamples=0;
set maxtime=0.0;

if "%distr%" == "-S" (
	set distr_string=Sequential
	set distr="S"
)
if "%distr%" == "-D" (
	set distr_string=Parallel
	set distr="D"
)

if "%rule%" == "-Pon" (
	set rule_string=on.
	set rule="Pon"
)
if "%rule%" == "-Toff" (
	set rule_string=off.
	set rule="Poff"
)


if "%resmon%" == "-Gon" (
	set resmon_string=on.
	set resmon="Gon"
)
if "%resmon%" == "-Goff" (
	set resmon_string=off.
	set resmon="Goff"
)


echo.
echo %distr_string% transient Simulation of net:
echo %NET%

"%SHARED_BIN_DIR%\proc_tn.exe" -r "RUC (%abort_condition:"=%);" -s %NETNAME%

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
gcc -c "%NET%_RUC.c" -I"%SIM_DIR%\include" -I"%SIM_DIR%\sim_utils\include" -o "%NET%_RUC.obj"

gcc -o "%NET%_slave.exe" "%NET%_RUC.obj" "%NET%_MDF.obj" "%NET%_ERC.obj" "%BIN_DIR%\libsim_str_slave_win.a" -L"%BIN_DIR%" -lsim_utils_win -L"%SHARED_BIN_DIR%" -lsysdep_win -lnetio_win -lws2_32
gcc -o "%NET%_master.exe" "%NET%_RDY.obj" "%BIN_DIR%\libsim_str_master_win.a" -L"%BIN_DIR%" -lsim_utils_win -L"%SHARED_BIN_DIR%" -lsysdep_win -lnetio_win -lws2_32

:: check executables
if NOT EXIST "%NET%_master.exe" (
  echo Could not build the simulator, for errors see above.
  GOTO END_SCRIPT
)
if NOT EXIST "%NET%_slave.exe" (
  echo Could not build the simulator, for errors see above.
  GOTO END_SCRIPT
)

"%NET%_master.exe" %NETNAME% %level% %epsilon% %seed% %maxsamples% %maxtime% %max_execution_time% %distr% %trans% %transient_time% %no_sampling_points% %rule% %% %resmon%

:END_SCRIPT

echo Removing temporary files

if EXIST "%NET%.DEFINFO" (
del /Q "%NET%.DEFINFO" >NUL 2>&1
)
if EXIST "%NET%_master.exe" (
del /Q "%NET%_master.exe" >NUL 2>&1
)
if EXIST "%NET%_master.ilk" (
del /Q "%NET%_master.ilk" >NUL 2>&1
)
if EXIST "%NET%_master.pdb" (
del /Q "%NET%_master.pdb" >NUL 2>&1
)
if EXIST "%NET%_slave.exe" (
del /Q "%NET%_slave.exe" >NUL 2>&1
)
if EXIST "%NET%_slave.ilk" (
del /Q "%NET%_slave.ilk" >NUL 2>&1
)
if EXIST "%NET%_slave.pdb" (
del /Q "%NET%_slave.pdb" >NUL 2>&1
)
if EXIST "%NET%.sim_curves" (
del /Q "%NET%.sim_curves" >NUL 2>&1
)
if EXIST "%NET%.temp_sim_curves" (
del /Q "%NET%.temp_sim_curves" >NUL 2>&1
)

if EXIST "%NET%_ERC.c" (
del /Q "%NET%_ERC.c" >NUL 2>&1
)
if EXIST "%NET%_RDY.c" (
del /Q "%NET%_RDY.c" >NUL 2>&1
)
if EXIST "%NET%_MDF.c" (
del /Q "%NET%_MDF.c" >NUL 2>&1
)
if EXIST "%NET%_RUC.c" (
del /Q "%NET%_RUC.c" >NUL 2>&1
)
if EXIST "%NET%_erc.obj" (
del /Q "%NET%_erc.obj" >NUL 2>&1
)
if EXIST "%NET%_rdy.obj" (
del /Q "%NET%_rdy.obj" >NUL 2>&1
)
if EXIST "%NET%_mdf.obj" (
del /Q "%NET%_mdf.obj" >NUL 2>&1
)
if EXIST "%NET%_ruc.obj" (
del /Q "%NET%_ruc.obj" >NUL 2>&1
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

cd %currentdir%
