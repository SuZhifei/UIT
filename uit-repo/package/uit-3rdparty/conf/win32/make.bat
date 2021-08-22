@echo off
setlocal ENABLEDELAYEDEXPANSION
set UIT_PKGS=boost capicxx-core-runtime vsomeip capicxx-someip-runtime poco qt

set curdir=%~dp0
set curdir=%curdir:~0,-1%
set dist=%curdir%/../../dist/win32
set boost_ver=1.61.0
set boost_pkg=boost_%boost_ver:.=_%
set boost_tar=%boost_pkg%.7z

set qt_ver=5.12.2
set qt_pkg=qt-everywhere-src-%qt_ver%
set qt_tar=%qt_pkg%.zip

git config --add core.compression 1

if not "%curdir%" == "%cd%" echo "please make sure run make.bat on %curdir%" &pause & goto :eof

if "%UIT_ROOT%" == "" echo please source env.sh &pause & goto :eof

::::::::::::args to function:::::::::
set args=%1 %2 %3 %4 %5 %6 %7 %8 %9
::trim right
:delright
if "%args:~-1%"==" " (
	set args=%args:~0,-1%
	goto delright
)

set clean_flag=%args:~0,6%
set clean_target=%args:~6,30%
if "%args%" == "" 			call :boost & call :capicxx-core-runtime & call :vsomeip & call :capicxx-someip-runtime & call :poco & goto :eof
if "%args%" == "clean"		call :clean & goto :eof
if "%clean_flag%" == "clean_" call :clean_one %clean_target% & goto :eof
echo %UIT_PKGS% | findstr "%1">nul
if not %errorlevel% == 0 ( echo [%1] is not a supported package of [%UIT_PKGS%] & goto :end)

call :%1 & goto :eof
::::::::::::args to function:::::::::

:all
set build_temp=%UIT_PKGS%
:loop_all
for /f "tokens=1*" %%a in ("%build_temp%") do (
	call :%%a
	set build_temp=%%b
)
if defined build_temp goto :loop_all
goto :eof


:boost
::			   https://ufpr.dl.sourceforge.net/project/boost/boost/1.61.0/boost_1_61_0.7z
set boost_url="https://ufpr.dl.sourceforge.net/project/boost/boost/%boost_ver%/%boost_tar%"
if not exist "boost" (
	curl --insecure %boost_url% -o %boost_tar% --retry 10000
	%UIT_ZIP_EXT% x %boost_tar% -y
	::decompression fail, download again
	if not exist "%boost_pkg%" (goto :boost)
	if exist "boost" (rd /s /q "boost")
	ren %boost_pkg% boost
)
cd boost & call bootstrap.bat
@ping 127.0.0.1 -n 20 >nul
b2 --buildtype=complete
b2 link=shared runtime-link=shared threading=multi debug release
cd .. & goto :eof

:capicxx-core-runtime
if not exist "capicxx-core-runtime" (
	git clone https://github.com/GENIVI/capicxx-core-runtime.git
	if not exist capicxx-core-runtime goto :capicxx-core-runtime
	cd capicxx-core-runtime & git checkout 2fd0625 & git apply ../capicxx-core-runtime.patch & cd ..
)
call :build capicxx-core-runtime
goto :eof

:vsomeip
if not exist "vsomeip" (
	git clone https://github.com/GENIVI/vsomeip.git
	if not exist vsomeip goto :vsomeip
	cd vsomeip & git checkout 9fb9bee & git apply ../vsomeip.patch & cd ..
)
call :build vsomeip
goto :eof

:capicxx-someip-runtime
if not exist "capicxx-someip-runtime" (
	git clone https://github.com/GENIVI/capicxx-someip-runtime.git
	if not exist capicxx-someip-runtime goto :capicxx-someip-runtime
	cd capicxx-someip-runtime & git checkout 64f0cb0 & git apply ../capicxx-someip-runtime.patch & cd ..
)
call :build capicxx-someip-runtime
goto :eof

:poco
if not exist "poco" (
	git clone https://github.com/pocoproject/poco.git
	if not exist poco goto :poco
	cd poco & git checkout 13eda19 & cd ..
)
call :build poco
goto :eof

:qt
::Qt官网：http://download.qt.io/archive/qt/5.12/5.12.2/single
set qt_url=http://mirrors.ustc.edu.cn/qtproject/archive/qt/%qt_ver:~,-2%/%qt_ver%/single/%qt_tar%
if not exist "qt" (
	curl --insecure %qt_url% -o %qt_tar% --retry 10000
	%UIT_ZIP_EXT% x %qt_tar% -y
	::decompression fail, download again
	if not exist "%qt_pkg%" (goto :boost)
	if exist "boost" (rd /s /q "boost")
	ren %qt_pkg% qt
)
call "%UIT_SYSTEM_SDK_PATH%\Common7\Tools\VsDevCmd.bat"
cd qt
call configure.bat -prefix "%dist%/debug" -opensource -confirm-license -debug -opengl desktop -platform win32-msvc2015 -c++std c++11 -skip qtwebengine -nomake examples -nomake tests 
nmake & nmake install
call configure.bat -prefix "%dist%/release" -opensource -confirm-license -release -opengl desktop -platform win32-msvc2015 -c++std c++11 -skip qtwebengine -nomake examples -nomake tests 
nmake & nmake install
goto :eof

:build
if not exist "%1/%UIT_BUILD_DIR%" md "%1/%UIT_BUILD_DIR%"
set pwd=%~dp0
cd %1/%UIT_BUILD_DIR%
%UIT_CMAKE% -DCMAKE_INSTALL_PREFIX=%dist%/debug ../..
@%UIT_COMPILER_EXE% INSTALL.vcxproj /p:Configuration="debug"
%UIT_CMAKE% -DCMAKE_INSTALL_PREFIX=%dist%/release ../..
@%UIT_COMPILER_EXE% INSTALL.vcxproj /p:Configuration="release"
cd %pwd% & goto :eof

::clean
:clean
echo del [%boost_tar%]
if exist "%boost_tar%" ( del /f /s /q "%boost_tar%" >nul )
set clean_temp=%UIT_PKGS%
:loop_clean
for /f "tokens=1*" %%a in ("%clean_temp%") do (
	echo del [%%a]
	if exist "%%a" ( rd /s /q "%%a" )
	cd %curdir%
	set clean_temp=%%b
)
if defined clean_temp goto :loop_clean
goto :eof

::clean one
:clean_one
echo del [%1]
if exist "%1" ( rd /s /q "%1" )
if "%1" == "boost" (
	if exist "%boost_tar%" ( del /f /s /q "%boost_tar%" >nul )
)
goto :eof


:end
