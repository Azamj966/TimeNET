@echo off
pushd "%CD%"
cd /d "%0\..\.."
java -classpath GPSC\lib\openadaptor.jar;GPSC\lib\jakarta-oro-2.0.6.jar;GPSC\lib\log4j.jar;GPSC\lib\jdom.jar org.openadaptor.adaptor.editor.AFEditor %1
popd
