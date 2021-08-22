@echo off
setlocal ENABLEDELAYEDEXPANSION

set UIT_PKGS=package 

if "%UIT_ROOT%" == "" echo please source env.sh	& pause & goto :eof

set curdir=%~dp0
::::::::::::args to function:::::::::
set args=%1 %2 %3 %4 %5 %6 %7 %8 %9
::trim right
:delright
if "%args:~-1%"==" " (
	set args=%args:~0,-1%
	goto delright
)
if "%args%" == "" 			call :targets & goto :eof
if "%args%" == "all"		call :targets & goto :eof
if "%args%" == "clean"		call :clean & goto :eof
if "%args%" == "publish"	call :publish & goto :eof
if "%args%" == "clone"		call :clone & goto :eof
if "%args%" == "update"		call :update & goto :eof
if "%args%" == "dist_clean"	call :dist_clean & goto :eof
call :targets %args% & goto :eof
::::::::::::args to function:::::::::

::build
:targets
if "%~1"=="" (set targets_temp=%UIT_PKGS%) else (set targets_temp=%args%)
:loop_targets
for /f "tokens=1*" %%a in ("%targets_temp%") do (
	if not exist %%a ( echo [%%a] doesn't exist, please check %curdir% && goto :eof)
	
	cd %%a & call make.bat & cd %curdir%
	set targets_temp=%%b
)
if defined targets_temp goto :loop_targets
goto :eof

::clean
:clean
set clean_temp=%UIT_PKGS%
:loop_clean
for /f "tokens=1*" %%a in ("%clean_temp%") do (
	cd %%a & call make.bat clean & cd %curdir%
	set clean_temp=%%b
)
if defined clean_temp goto :loop_clean
goto :eof

::publish
:publish
set public_temp=%UIT_PKGS%
:loop_publish
for /f "tokens=1*" %%a in ("%clean_temp%") do (
	cd %%a & call make.bat clean & cd %curdir%
	set clean_temp=%%b
)
if defined clean_temp goto :loop_publish
goto :eof

::clone
:clone
set clone_temp=%UIT_PKGS%
:clone_loop
for /f "tokens=1*" %%a in ("%clone_temp%") do (
	cd %%a & call make.bat clone & cd %curdir%
	set clone_temp=%%b
)
if defined clone_temp goto :clone_loop
goto :eof

::update
:update
set update_temp=%UIT_PKGS%
:update_loop
for /f "tokens=1*" %%a in ("%update_temp%") do (
	cd %%a & call make.bat update & cd %curdir%
	set update_temp=%%b
)
if defined update_temp goto :update_loop
goto :eof

::clean dist
:dist_clean
if exist "%UIT_DIST_DIR%" (
	echo del [%UIT_DIST_DIR%]
	rd /s /q "%UIT_DIST_DIR%"
)
goto :eof
