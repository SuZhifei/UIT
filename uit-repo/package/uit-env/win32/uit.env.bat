@echo off

::uit path
set UIT_RT_HOME_PATH=%~dp0
set UIT_RT_HOME_PATH=%UIT_RT_HOME_PATH:~0,-4%
set UIT_RT_LIB_PATH=%UIT_RT_HOME_PATH%\lib
set UIT_RT_BIN_PATH=%UIT_RT_HOME_PATH%\bin
set UIT_RT_ETC_PATH=%UIT_RT_HOME_PATH%\etc
set UIT_RT_DATA_PATH=%UIT_RT_HOME_PATH%\data
set UIT_RT_RES_PATH=%UIT_RT_HOME_PATH%\resource
set UIT_RT_SHARE_PATH=%UIT_RT_HOME_PATH%\share
set UIT_RT_TEMP_PATH=%UIT_RT_HOME_PATH%\temp

::someip
set COMMONAPI_CONFIG=%UIT_RT_ETC_PATH%\commonapi.ini
set VSOMEIP_CONFIGURATION=%UIT_RT_ETC_PATH%\vsomeip-dataservice.json

set PATH=%PATH%;%UIT_RT_LIB_PATH%\debug;%UIT_RT_BIN_PATH%\debug;%UIT_RT_BIN_PATH%\debug\simulator