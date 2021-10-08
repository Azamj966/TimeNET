@echo off

set SHARED_DIR=%TNETHOME%\EDSPN\Shared

del /Q "loopcount.exe" >NUL 2>&1
del /Q "count_result" >NUL 2>&1

gcc -O3 loopcount.c -x c "%SHARED_DIR%\scripts\timer.c" -o loopcount.exe

"loopcount.exe" > "count_result"
