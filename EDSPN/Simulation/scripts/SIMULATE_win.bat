:: 	Starten der verteilten DSPN Simulation unter Windows

@echo off

set currentdir=%cd%

set distr="null"
set distr_string="null"
set method="null"
set meth_string="null"
set trans="null"
set trans_string="null"
set maxsamples="null"
set maxtime="null"
set max_execution_time="null"
set nice=4
set BIN_DIR=%TNETHOME%\EDSPN\Simulation\bin
set SIM_DIR=%TNETHOME%\EDSPN\Simulation
set SHARED_BIN_DIR=%TNETHOME%\EDSPN\Shared\bin

set usage="Usage : SIMULATE model -{D|S} -{SV|CV|RS|MT} -{Ton|Toff} <conf.level> <errPercent> <epsilon> <seed> <max.samples> <min_firings> <max.model time> <max.real time> -{DelTrue|DelFalse}"

if %1 == "" (
  echo %usage%
  GOTO END_SCRIPT
) else (
  set NETNAME=%1
)

set NET=%MODELDIR%\\%NETNAME%

:: check netname
if NOT EXIST "%NET%.TN" (
        echo "No such net %NET%.TN"
  GOTO END_SCRIPT
)

:: set parameters
set distr=%2
set method=%3
set trans=%4
set level=%5
set errPercent=%6
set epsilon=%7
set seed=%8
set maxsamples=%9

:: shift parameter %10 to %9
shift 
set min_firings=%9
shift
set maxtime=%9
shift
set max_execution_time=%9
shift
set del_files=%9

::This parameter was removed from the call on the script.
set Visu_On="-Voff"


if "%distr%" == "-S" (
	set distr_string=Sequential 
	set distr="S"
)
if "%distr%" == "-D" (
	set distr_string=Parallel 
	set distr="D"
)

if "%trans%" == "-Ton" (
	set trans_string=on.
	set trans="Ton"
)
if "%trans%" == "-Toff" (
	set trans_string=off.
	set trans="Toff"
)

if "%method%" == "-RS" (
         set meth_string=RESTART with spectral variance analysis
         set method=-RS
)
if "%method%" == "-SV" (
	set meth_string=spectral variance analysis
	set method=-SV
)
if "%method%" == "-CV" (
	set meth_string=control variates variance reduction
	set method=-CV
)

if "%method%" neq "-MT" (
echo.
echo %distr_string%Simulation with %meth_string% of net:
echo %NET%
echo.
)

if "%method%" == "-MT" (
echo.
echo Multi-Trajecotry Simulation of net:
echo %NET%
echo. 
)


:: remove old results
if EXIST "%NET%.RESULTS" (
echo Removing previous results "%NET%.RESULTS"
del "%NET%.RESULTS"
)
if EXIST "%NET%.dir\%NETNAME%.RESULTS" (
echo Removing previous results "%NET%.dir\%NETNAME%.RESULTS"
del "%NET%.dir\%NETNAME%.RESULTS"
)

if "%method%" neq "-MT" (
echo.
echo Detection of the initial transient period is %trans_string%
)

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

if "%method%" == "-CV" (
    gcc -o "%NET%_slave.exe"  "%NET%_MDF.obj" "%NET%_ERC.obj" "%BIN_DIR%\libsim_cv_slave_win.a" -L"%BIN_DIR%" -lsim_utils_win -L"%SHARED_BIN_DIR%" -lsysdep_win -lnetio_win -lws2_32
    gcc -o "%NET%_master.exe" "%NET%_RDY.obj" "%BIN_DIR%\libsim_cv_master_win.a" -L"%BIN_DIR%" -lsim_utils_win -L"%SHARED_BIN_DIR%" -lsysdep_win -lnetio_win -lws2_32
)

if "%method%" == "-SV" (
    gcc -o "%NET%_slave.exe"  "%NET%_MDF.obj" "%NET%_ERC.obj" "%BIN_DIR%\libsim_spec_slave_win.a" -L"%BIN_DIR%" -lsim_utils_win -L"%SHARED_BIN_DIR%" -lsysdep_win -lnetio_win -lws2_32
    gcc -o "%NET%_master.exe" "%NET%_RDY.obj" "%BIN_DIR%\libsim_spec_master_win.a" -L"%BIN_DIR%" -lsim_utils_win -L"%SHARED_BIN_DIR%" -lsysdep_win -lnetio_win -lws2_32
)

if "%method%" == "-RS" (
	if NOT EXIST "lpsolve55.dll" (
		copy "%BIN_DIR%\lpsolve55.dll" "lpsolve55.dll" >NUL 2>&1
	)
	gcc -c "%NET%_ImpFct.c" -I"%SIM_DIR%\include" -o "%NET%_ImpFct.obj"
	gcc -o "%NET%_slave.exe" "%NET%_ImpFct.obj" "%NET%_MDF.obj" "%NET%_ERC.obj" "%BIN_DIR%\libsim_rst_slave_win.a" -L"%BIN_DIR%" -lsim_utils_win -llpsolve55  -L"%SHARED_BIN_DIR%" -lsysdep_win -lnetio_win -lws2_32
    gcc -o "%NET%_master.exe" "%NET%_RDY.obj" "%BIN_DIR%\libsim_rst_master_win.a" -L"%BIN_DIR%" -lsim_utils_win -L"%SHARED_BIN_DIR%" -lsysdep_win -lnetio_win -lws2_32
)

if "%method%" == "-MT" (
    gcc -o "%NET%_slave.exe" "%NET%_MDF.obj" "%NET%_ERC.obj" "%BIN_DIR%\libsim_mt_slave_win.a" -L"%BIN_DIR%" -lsim_utils_win -L"%SHARED_BIN_DIR%" -lsysdep_win -lnetio_win -lws2_32
    gcc -o "%NET%_master.exe" "%NET%_RDY.obj" "%BIN_DIR%\libsim_mt_master_win.a" -L"%BIN_DIR%" -lsim_utils_win -L"%SHARED_BIN_DIR%" -lsysdep_win -lnetio_win -lws2_32
)

echo.
echo Starting simulation

"%NET%_master.exe" %NETNAME% %level% %epsilon% %seed% %maxsamples% %maxtime% %max_execution_time% %distr% %trans% %min_firings% %errPercent% %Visu_On%

:END_SCRIPT

if "%del_files%" == "-DelFalse" (
GOTO MOVE_FILES
)

echo Removing temporary files

if EXIST "lpsolve55.dll" (
del "lpsolve55.dll" >NUL 2>&1
)
if EXIST "%NET%_ERC.c" (
del "%NET%_ERC.c" >NUL 2>&1
)
if EXIST "%NET%_RDY.c" (
del "%NET%_RDY.c" >NUL 2>&1
)
if EXIST "%NET%_MDF.c" (
del "%NET%_MDF.c" >NUL 2>&1
)
if EXIST "%NET%_ImpFct.c" (
del "%NET%_ImpFct.c" >NUL 2>&1
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
if EXIST "%NET%_ImpFct.obj" (
del "%NET%_ImpFct.obj" >NUL 2>&1
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

GOTO END_END

:MOVE_FILES
echo Moving temporary files

if NOT EXIST "%NET%.dir" (
  md "%NET%.dir"
)

if EXIST "lpsolve55.dll" (
move "lpsolve55.dll" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%_ERC.c" (
move "%NET%_ERC.c" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%_RDY.c" (
move "%NET%_RDY.c" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%_MDF.c" (
move "%NET%_MDF.c" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%_ImpFct.c" (
move "%NET%_ImpFct.c" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%_ERC.obj" (
move "%NET%_ERC.obj" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%_RDY.obj" (
move "%NET%_RDY.obj" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%_MDF.obj" (
move "%NET%_MDF.obj" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%_ImpFct.obj" (
move "%NET%_ImpFct.obj" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.DEFINFO" (
move "%NET%.DEFINFO" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%_master.exe" (
move "%NET%_master.exe" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%_slave.exe" (
move "%NET%_slave.exe" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.sim_curves" (
move "%NET%.sim_curves" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.temp_sim_curves" (
move "%NET%.temp_sim_curves" "%NET%.dir" >NUL 2>&1
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



:END_END
cd %currentdir%
