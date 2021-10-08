@echo off

if "%1" == "" (
  echo "Usage: startResultMonitor <port number> <model path>"
) else (
  if "%2" == "" (
    echo "Usage: startResultMonitor <port number> <model path>"
  ) else (
    pushd "%CD%"
    cd /d "%0\..\.."
    java -cp TimeNET.jar server.Server $1 "$2"
    popd
  )
)
