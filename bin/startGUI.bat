@echo off
pushd "%CD%"
cd /d "%0\..\.."
java -Dsun.java2d.d3d=false -Dsun.java2d.ddoffscreen=false -Dsun.java2d.noddraw=true -Xms32m -Xmx512m -jar TimeNET.jar
popd
