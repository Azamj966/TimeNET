@echo off
pushd "%CD%"
cd /d "%0\..\.."
java -classpath GPSC\lib\openadaptor.jar;GPSC\lib\log4j.jar;GPSC\lib\jakarta-oro-2.0.6.jar;GPSC\lib\jdom.jar org.openadaptor.adaptor.RunAdaptor %1 "%2"
popd
