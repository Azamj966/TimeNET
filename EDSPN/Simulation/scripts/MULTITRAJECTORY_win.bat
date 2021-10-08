:: Multi Trajectory Simulation

@echo off

set usage="Usage : MULTITRAJECTORY_win model <conf.level> <epsilon> <seed> <max.samples> <max.model time> <max.real time> -{DelTrue|DelFalse} -{IniTranStrict|IniTranModerate|IniTranLight} <min.particle.weight (1e-VAL, 0=>DBL_MIN)> <max.particle.count (1eVAL)> -{SortNo|SortHDif|SortLDif|SortHDev|SortLDev|SortHVar|SortLVar} -{SplitAlways|SplitTrLessAvrg|SplitSelective}"

if %1 == "" (
  echo %usage%
  GOTO END_SCRIPT
) else (
  set NETNAME=%1
)
set NET=%MODELDIR%\\%NETNAME%


:: SIMULATE parameters
:: call SIMULATE_win %NETNAME% %distr% %method% %trans% %level% %errPercent% %epsilon% %seed% %maxsamples% %min_firings% %maxtime% %max_execution_time% %del_files%
set distr=-S
set method=-MT
set trans=-Toff
set level=%2
set errPercent=%3
set epsilon=%3
set seed=%4
set maxsamples=%5
set min_firings=0
set maxtime=%6
set max_execution_time=%7
set del_files=%8

echo level %level%
echo errPercent %errPercent%
echo epsilon %epsilon%


:: MT parameters
set ini_tran=%9
shift
set min_part_weight=%9
shift
set max_part_count=%9
shift
set sort=%9
shift
set split=%9

set MT_PARAM="%NET%_mt.param"
del %MT_PARAM% >NUL 2>&1

if "%ini_tran%" == "-IniTranStrict" ( 
  echo 0 >> %MT_PARAM% 
) else if "%ini_tran%" == "-IniTranModerate" ( 
  echo 1 >> %MT_PARAM% 
) else if "%ini_tran%" == "-IniTranLight" ( 
  echo 2 >> %MT_PARAM% 
) else (
  echo -1 >> %MT_PARAM% 
)


echo %min_part_weight% >> %MT_PARAM%
echo %max_part_count% >> %MT_PARAM%

if "%sort%" == "-SortNo" ( 
  echo 0 >> %MT_PARAM% 
) else if "%sort%" == "-SortHDif" ( 
  echo 1 >> %MT_PARAM% 
) else if "%sort%" == "-SortLDif" ( 
  echo 2 >> %MT_PARAM% 
) else if "%sort%" == "-SortHDev" ( 
  echo 3 >> %MT_PARAM% 
) else if "%sort%" == "-SortLDev" ( 
  echo 4 >> %MT_PARAM% 
) else if "%sort%" == "-SortHVar" ( 
  echo 5 >> %MT_PARAM% 
) else if "%sort%" == "-SortLVar" ( 
  echo 6 >> %MT_PARAM% 
) else (
  echo -1 >> %MT_PARAM%
)

if "%split%" == "-SplitAlways" ( 
  echo 0 >> %MT_PARAM% 
) else if "%split%" == "-SplitTrLessAvrg" ( 
  echo 1 >> %MT_PARAM% 
) else if "%split%" == "-SplitSelective" ( 
  echo 2 >> %MT_PARAM% 
) else (
  echo -1 >> %MT_PARAM%
)



::::::::::::::::::::::::::::::::::::::::: LOG ::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Used to generate log of the simulation, for developing 
GOTO SKIP_GENERATE_LOG_1
set FLG_MST=%TNETHOME%\EDSPN\Simulation\sim_mt_master\scripts\_flag_M.txt
set FLG_SLV=%TNETHOME%\EDSPN\Simulation\sim_mt_master\scripts\_flag_S.txt
set EMPTY_F=%TNETHOME%\EDSPN\Simulation\sim_mt_master\scripts\_empty.txt
set LOG_MST=%TNETHOME%\EDSPN\Simulation\sim_mt_master\_log_master.txt
set LOG_SLV=%TNETHOME%\EDSPN\Simulation\sim_mt_slave\_log_slave.txt

echo %LOG_MST%>> %MT_PARAM%
echo %FLG_MST%>> %MT_PARAM%
echo %LOG_SLV%>> %MT_PARAM%
echo %FLG_SLV%>> %MT_PARAM%


set STARTTIME=%TIME%
echo Start Time = %STARTTIME%

del %FLG_MST% >NUL 2>&1
del %FLG_SLV% >NUL 2>&1
copy /Y %EMPTY_F% %LOG_MST% >NUL 2>&1
copy /Y %EMPTY_F% %LOG_SLV% >NUL 2>&1


set CALL="call %MODELDIR% %NETNAME% %level% %epsilon% %ini_tran% min_w=%min_part_weight% max_p=%max_part_count% %sort% %split%"

echo %CALL%
echo %CALL% >> %LOG_MST%
echo ...  >> %LOG_MST%
echo %CALL% >> %LOG_SLV%
echo ...  >> %LOG_SLV%
:SKIP_GENERATE_LOG_1
::::::::::::::::::::::::::::::::::::::::: LOG ::::::::::::::::::::::::::::::::::::::::::::::::::::::


call "%TNETHOME%\EDSPN\Simulation\scripts\SIMULATE_win.bat" %NETNAME% %distr% %method% %trans% %level% %errPercent% %epsilon% %seed% %maxsamples% %min_firings% %maxtime% %max_execution_time% %del_files%

:END_SCRIPT

if NOT EXIST "%NET%.dir" (
  md "%NET%.dir"
)

if EXIST "%MT_PARAM%" (
move "%MT_PARAM%" "%NET%.dir" >NUL 2>&1
)


::::::::::::::::::::::::::::::::::::::::: LOG ::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Used to generate log of the simulation, for developing 
GOTO SKIP_GENERATE_LOG_2

set ENDTIME=%TIME%

:: Change formatting for the start and end times
for /F "tokens=1-4 delims=:.," %%a in ("%STARTTIME%") do (
   set /A "start=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

for /F "tokens=1-4 delims=:.," %%a in ("%ENDTIME%") do (
   set /A "end=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

:: Calculate the elapsed time by subtracting values
set /A elapsed=end-start
    
:: Format the results for output
set /A hh=elapsed/(60*60*100), rest=elapsed%%(60*60*100), mm=rest/(60*100), rest%%=60*100, ss=rest/100, cc=rest%%100
if %hh% lss 10 set hh=0%hh%
if %mm% lss 10 set mm=0%mm%
if %ss% lss 10 set ss=0%ss%
if %cc% lss 10 set cc=0%cc%

set DURATION=%hh%:%mm%:%ss%.%cc%

echo .  >> %LOG_MST%
echo .  >> %LOG_MST%
echo .  >> %LOG_MST%
echo Duration : %DURATION%  >> %LOG_MST%
echo .  >> %LOG_SLV%
echo .  >> %LOG_SLV%
echo .  >> %LOG_SLV%
echo Duration : %DURATION%  >> %LOG_SLV%

copy "%LOG_MST%" "%NET%.dir" >NUL 2>&1
copy "%LOG_SLV%" "%NET%.dir" >NUL 2>&1
:SKIP_GENERATE_LOG_2
::::::::::::::::::::::::::::::::::::::::: LOG ::::::::::::::::::::::::::::::::::::::::::::::::::::::





