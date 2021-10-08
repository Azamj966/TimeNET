:: 	Starten der verteilten DSPN Simulation unter Windows

@echo off

set currentdir=%cd%

set restart_distr="null"
set restart_distr_string="null"
set restart_trans="null"
set restart_trans_string="null"
set restart_maxsamples="null"
set restart_maxtime="null"
set restart_first_threshold="null"
set restart_last_threshold="null"
set restart_max_thresholds="null"
set restart_max_real_time="null"
set restart_if_mode="null"
set restart_splitting_factor="null"
set nice=4
set BIN_DIR=%TNETHOME%\EDSPN\Simulation\bin
set SHARED_BIN_DIR=%TNETHOME%\EDSPN\Shared\bin

set usage="Usage : RESTART model -{D|S} <restart_if_mode> <restart_first_threshold> <restart_last_threshold> <restart_max_thresholds> <-Ton|Toff> <conf.restart_level> <restart_epsilon> <restart_seed> <max.samples> <restart_min_firings> <max.modelpath time> <max.real time> <splitting_factor>"

if %1 == "" (
  echo %usage%
  GOTO END_SCRIPT
) else (
  set RNETNAME=%1
)

:: DO NOT USE %NET% and %NETNAME% BECAUSE IT IS CHANGED IN SIMULATE_win.bat
set RNET=%MODELDIR%\\%RNETNAME%
ECHO "%RNET%.TN"
:: check netname
if NOT EXIST "%RNET%.TN" (
	echo "No such net"
	GOTO END_SCRIPT
)

:: set parameters
set restart_distr=%2
set restart_if_mode=%3
set restart_first_threshold=%4
set restart_last_threshold=%5
set restart_max_thresholds=%6
set restart_trans=%7
set restart_level=%8
set restart_epsilon=%9

:: shift parameter %10 to %9
shift
set restart_seed=%9
shift
set restart_maxsamples=%9
shift 
set restart_min_firings=%9
shift
set restart_maxtime=%9
shift
set restart_max_real_time=%9
shift
set restart_splitting_factor=%9

if "%restart_distr%" == "-S" (
	set restart_distr_string=Sequential 
	set restart_distr=S
)
if "%restart_distr%" == "-D" (
	set restart_distr_string=Parallel 
	set restart_distr=D
)

if "%restart_trans%" == "-Ton" (
	set restart_trans_string=on.
	set restart_trans=Ton
)
if "%restart_trans%" == "-Toff" (
	set restart_trans_string=off.
	set restart_trans=Toff
)

echo.

"%SHARED_BIN_DIR%\proc_tn.exe" -s %RNETNAME%

if ERRORLEVEL 1 (
  echo.
  echo ERROR occurred while proc_TN execution.
  echo SOLUTION OF MODEL %RNETNAME% FAILED.
  echo.
  GOTO END_SCRIPT
)

"%SHARED_BIN_DIR%\struct_tn.exe" -i %RNETNAME%

if ERRORLEVEL 1 (
  echo.
  echo ERROR occurred while struct_tn execution.
  echo SOLUTION OF MODEL %RNETNAME% FAILED.
  echo.
  GOTO END_SCRIPT
)

"%BIN_DIR%\sim_rst_set.exe" "%RNET%" %restart_first_threshold% %restart_last_threshold% %restart_max_thresholds% %restart_if_mode% %restart_splitting_factor%

if ERRORLEVEL 1 (
   echo could not finish threshold-setting of RESTART
   GOTO END_SCRIPT
)

echo.
echo Threshold-setting of RESTART finished.
echo.
echo Starting main simulation

"%TNETHOME%\EDSPN\Simulation\scripts\SIMULATE_win.bat" %RNETNAME% -%restart_distr% -RS -%restart_trans% %restart_level% 50 %restart_epsilon% %restart_seed% %restart_maxsamples% %restart_min_firings% %restart_maxtime% %restart_max_real_time% -DelTrue

if ERRORLEVEL 1 (
   echo "could not finish main simulation of RESTART"
   GOTO END_SCRIPT
)

echo.
echo Main simulation of RESTART finished.


:END_SCRIPT

echo Removing temporary files

if EXIST "%RNET%_ERC.c" (
del "%RNET%_ERC.c" >NUL 2>&1
)
if EXIST "%RNET%_RDY.c" (
del "%RNET%_RDY.c" >NUL 2>&1
)
if EXIST "%RNET%_MDF.c" (
del "%RNET%_MDF.c" >NUL 2>&1
)
if EXIST "%RNET%_ImpFct.c" (
del "%RNET%_ImpFct.c"  >NUL 2>&1
)
if EXIST "%RNET%.DEFINFO" (
del "%RNET%.DEFINFO"
)
if EXIST "%RNET%_RST.TN" (
del "%RNET%_RST.TN"
)
if EXIST "%RNET%_RST.RESULTS" (
del "%RNET%_RST.RESULTS"
)
if EXIST "%RNET%.PARAM" (
del "%RNET%.PARAM" 
)


if NOT EXIST "%RNET%.dir" (
	md "%RNET%.dir"
)

if EXIST "%RNET%.ECS" (
move "%RNET%.ECS" "%RNET%.dir"
)
if EXIST "%RNET%.INV" (
move "%RNET%.INV" "%RNET%.dir"
)
if EXIST "%RNET%.STRUCT" (
move "%RNET%.STRUCT" "%RNET%.dir"
)
if EXIST "%RNET%.TN" (
move "%RNET%.TN" "%RNET%.dir"
)

cd %currentdir%