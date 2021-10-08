@echo off

if "%1" == "" (
  echo "Usage: startResultMonitor <portnumber>"
) else (
  pushd "%CD%"
  cd /d "%0\..\.."
  java -cp TimeNET.jar StatisticClientStart %1
  popd
)
