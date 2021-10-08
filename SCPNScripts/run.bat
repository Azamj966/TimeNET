@echo off
REM The parameters for this batch are:
REM First parameter : JavaScript file name
REM Second parameter : first additional parameter for JavaScript (accessible by arguments[0])
REM Third parameter : second additional parameter for JavaScript (accessible by arguments[1])
REM SET SCRIPT_OLDHOME=%CD%
REM cd ..
REM SET SCRIPT_TNETHOME=%CD%
REM cd %SCRIPT_OLDHOME%
REM SET SCRIPT_DIRNAME=%SCRIPT_TNETHOME%\GPSC

cd ..
java -Xms32m -Xmx512m -cp TimeNET.jar -DTNETHOME=. -Dlog4j.configuration="etc\gpsc_conf\log4j.props" gpsc.Host -i "etc\gpsc_conf\integration.props" -m "..\Models\SCPN" "%1" "%2" "%3"
cd SCPNScripts
