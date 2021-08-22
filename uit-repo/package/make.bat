@echo off
setlocal ENABLEDELAYEDEXPANSION

set UIT_PKGS=uit-doc uit-env uit-3rdparty uit-lib-core uit-fw-data uit-fw-screen uit-uid-kanzi uit-uid-qt

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
set clean_flag=%args:~0,6%
set clean_target=%args:~7,20%
if "%args%" == "" 			call :targets & goto :eof
if "%args%" == "all"		call :targets & goto :eof
if "%args%" == "clean"		call :clean & goto :eof
if "%args%" == "publish"	call :publish & goto :eof
if "%args%" == "clone"		call :clone & goto :eof
if "%args%" == "update"		call :update & goto :eof
if "%clean_flag%" == "clean_" call :clean_one %clean_target% & goto :eof

call :targets %args% & goto :eof
::::::::::::args to function:::::::::

::build
:targets
if "%~1"=="" (set targets_temp=%UIT_PKGS%) else (set targets_temp=%args%)
:loop_targets
for /f "tokens=1*" %%a in ("%targets_temp%") do (

	::check if package is in UIT_PKGS
	echo %UIT_PKGS% | findstr "%%a">nul
	if not !errorlevel! == 0 ( echo [%%a] is not a supported package of [%UIT_PKGS%] & goto :end)

	set target=%%a
	if not exist !target! (git clone %UIT_REPO_DOMAIN_PREFIX%/!target!.git -b %UIT_BASELINE%)
	::check depends and build depends
	call :build_depends !target!
	
	set build_dir=!target!\%UIT_BUILD_DIR%
	if not exist "!build_dir!" md "!build_dir!"

	cd !build_dir! & %UIT_CMAKE% ../..
	if exist INSTALL.vcxproj (
		@%UIT_COMPILER_EXE% INSTALL.vcxproj /p:Configuration="debug"
		@%UIT_COMPILER_EXE% INSTALL.vcxproj /p:Configuration="release"
	) else ( if exist ALL_BUILD.vcxproj (
		@%UIT_COMPILER_EXE% ALL_BUILD.vcxproj /p:Configuration="debug"
		@%UIT_COMPILER_EXE% ALL_BUILD.vcxproj /p:Configuration="release"
		) else (
			echo [!target!] can't find vs project, maybe CMAKE gen fail, please check [!target!]'s CmakeLists.txt..."
		)
	)
	set targets_temp=%%b
	cd %curdir%
)
if defined targets_temp goto :loop_targets
goto :eof

:build_depends
cd "%1%"
if not exist BuildSystem.Depends (cd %curdir% & goto :eof)
for /f "delims=" %%i in (BuildSystem.Depends) do (
	cd %curdir%
	if not exist %%i (git clone %UIT_REPO_DOMAIN_PREFIX%/%%i.git -b %UIT_BASELINE%)
	
	set build_dir=%%i\%UIT_BUILD_DIR%
	if not exist "!build_dir!" md "!build_dir!"

	cd !build_dir! & %UIT_CMAKE% ../..
	if exist INSTALL.vcxproj (
		@%UIT_COMPILER_EXE% INSTALL.vcxproj /p:Configuration="debug"
		@%UIT_COMPILER_EXE% INSTALL.vcxproj /p:Configuration="release"
	) else ( if exist ALL_BUILD.vcxproj (
		@%UIT_COMPILER_EXE% ALL_BUILD.vcxproj /p:Configuration="debug"
		@%UIT_COMPILER_EXE% ALL_BUILD.vcxproj /p:Configuration="release"
		) else (
			echo [%%i] can't find vs project, maybe CMAKE gen fail, please check [%%i]'s CmakeLists.txt..."
		)
	)
	cd %curdir%
)
cd %curdir%
goto :eof

::clean
:clean
set clean_temp=%UIT_PKGS%
:loop_clean
for /f "tokens=1*" %%a in ("%clean_temp%") do (
	echo del [%%a\%UIT_BUILD_DIR%]
	if exist "%%a\%UIT_BUILD_DIR%" ( rd /s /q "%%a\%UIT_BUILD_DIR%" )
	cd %curdir%
	set clean_temp=%%b
)
if defined clean_temp goto :loop_clean
goto :eof

::publish
:publish
call :targets
%UIT_ZIP_EXT% a %UIT_DIST_DIR%\%UIT_TARGET%.zip %UIT_DIST_DIR%\%UIT_TARGET%\*
goto :eof

::clone
:clone
set clone_temp=%UIT_PKGS%
:loop_clone
for /f "tokens=1*" %%a in ("%clone_temp%") do (
	git clone %UIT_REPO_DOMAIN_PREFIX%/%%a.git
	set clone_temp=%%b
)
if defined clone_temp goto :loop_clone
goto :eof

::update
:update
set update_temp=%UIT_PKGS%
:loop_update
for /f "tokens=1*" %%a in ("%update_temp%") do (
	if exist "%curdir%\%%a" (echo git pull [%%a] & git --git-dir=%curdir%\%%a\.git --work-tree=%curdir%\%%a pull 1>nul )
	set update_temp=%%b
)
if defined update_temp goto :loop_update
goto :eof

::clean one
:clean_one
echo del [%1\%UIT_BUILD_DIR%]
if exist "%1\%UIT_BUILD_DIR%" (
	rd /s /q "%1\%UIT_BUILD_DIR%" 
)
goto :eof

:end

