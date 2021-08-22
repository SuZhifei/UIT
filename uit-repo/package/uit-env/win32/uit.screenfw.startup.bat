@echo off

call uit.env.bat

start uit.svc.screen.exe
::start uit.test.screen-listener.exe
@ping 127.0.0.1 -n 1 >nul
::start uit.test.screen-controller.exe