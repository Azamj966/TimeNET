:: start transient solution

@echo off

set currentdir=%cd%

set BIN_DIR=%TNETHOME%\EDSPN\TransientSolution\bin
set SHARED_DIR=%TNETHOME%\EDSPN\Shared
set SHARED_BIN_DIR=%TNETHOME%\EDSPN\Shared\bin

set Usage="Usage: TransSolve_win model [-g graphics][-t trans_time] [-s stepsize] [-o point | duration] [-r result_stepsize] [-c cluster_size ] [-p precision] [-k] [-x] [-O] [-a]"
set args=
set NETNAME=

:: ARGUMENT PARSING LOOP

:PARSE

if "%1" == "-g" (
  set args=%1 %2
  shift
  shift
  GOTO PARSE
)

if "%1" == "-t" (
  set args=%args% %1 %2
  shift
  shift
  GOTO PARSE
)

if "%1" == "-s" (
  set args=%args% %1 %2
  shift
  shift
  GOTO PARSE
) 

if "%1" == "-o" (
  set args=%args% %1 %2
  shift
  shift
  GOTO PARSE
) 

if "%1" == "-r" (
  set args=%args% %1 %2
  shift
  shift
  GOTO PARSE
) 

if "%1" == "-c" (
  set args=%args% %1 %2
  shift
  shift
  GOTO PARSE
) 

if "%1" == "-p" (
  set args=%args% %1 %2
  shift
  shift
  GOTO PARSE
) 

if "%1" == "-k" (
  set args=%args% %1
  shift
  GOTO PARSE
) 

if "%1" == "-x" (
  set args=%args% %1
  shift
  GOTO PARSE
) 

if "%1" == "-O" (
  set args=%args% %1
  shift
  GOTO PARSE
) 

if "%1" == "-a" (
  set args=%args% %1
  shift
  GOTO PARSE
) 

if NOT "%1" == "" (
  if "%NETNAME%" == "" (
    set NETNAME=%1
	shift
    GOTO PARSE
  ) else (
    echo "unexpected argument: %1"
	echo %Usage%
    GOTO END_SCRIPT
  )
)

:: END OF ARGUMENT PARSING LOOP


if "%NETNAME%" == "" (
  echo Missing netname.
  echo %Usage%
  GOTO END_SCRIPT
)

set NET=%MODELDIR%\%NETNAME%

:: check netname
if NOT EXIST "%NET%.TN" (
  echo "No such net %NET%.TN"
  GOTO END_SCRIPT
)

:: generate reduced reachability graph
call "%SHARED_DIR%\scripts\GENERATE_RRG_win" -t %NETNAME%
echo.

echo COMPILE %NETNAME%_transSolve.exe File and execute it.
echo.
gcc -c "%NET%_ERC.c" -I"%SIM_DIR%\include" -I"%SIM_DIR%\sim_utils\include" -o "%NET%_ERC.obj"
gcc -o "%NET%_transSolve.exe" "%NET%_ERC.obj" "%BIN_DIR%\libtrans_solve_win.a" -L"%SHARED_BIN_DIR%" -lnetio_win -lsysdep_win -lws2_32

:: echo "%NET%_transSolve.exe" %args% %NETNAME%
"%NET%_transSolve.exe" %args% %NETNAME%

:END_SCRIPT
echo Removing temporary files

if EXIST "%NET%_transSolve.exe" (
del /Q "%NET%_transSolve.exe" >NUL 2>&1
)
if EXIST "%NET%_genrrg.exe" (
del /Q "%NET%_genrrg.exe" >NUL 2>&1
)
if EXIST "%NET%_ERC.obj" (
del /Q "%NET%_ERC.obj" >NUL 2>&1
)
if EXIST "%NET%_ERC.c" (
del /Q "%NET%_ERC.c" >NUL 2>&1
)
if EXIST "%NET%.tempcurves" (
del /Q "%NET%.tempcurves" >NUL 2>&1
)

if NOT EXIST "%NET%.dir" (
  md "%NET%.dir"
)

if EXIST "%NET%.curves" (
move "%NET%.curves" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.RESULTS" (
move "%NET%.RESULTS" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.pmf" (
move "%NET%.pmf" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.rrg" (
move "%NET%.rrg" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.tmark" (
move "%NET%.tmark" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.DEFINFO" (
move "%NET%.DEFINFO" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.STRUCT" (
move "%NET%.STRUCT" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.ECS" (
move "%NET%.ECS" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.INV" (
move "%NET%.INV" "%NET%.dir" >NUL 2>&1
)
if EXIST "%NET%.TN" (
move "%NET%.TN" "%NET%.dir" >NUL 2>&1
)

cd %currentdir%
