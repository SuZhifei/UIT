@echo off
set UIT_REPO_DOMAIN_PREFIX=git@scm-gitlab:uit/uniform/kunp
set UIT_BASELINE=1.0
set UIT_HOST=win32
set UIT_TARGET=win32
set UIT_ROOT=%~dp0
set UIT_ROOT=%UIT_ROOT:~0,-1%
set UIT_TOOL_DIR=%UIT_ROOT%\tool\%UIT_HOST%
set UIT_CONF_DIR=%UIT_ROOT%\conf
set UIT_DIST_DIR=%UIT_ROOT%\dist
set UIT_PACKAGE_DIR=%UIT_ROOT%\package
set UIT_COMPILER_VERSION=14.0
set UIT_SYSTEM_SDK_PATH="C:\Program Files (x86)\Microsoft Visual Studio %UIT_COMPILER_VERSION%"
set UIT_COMPILER_EXE="C:\Program Files (x86)\MSBuild\%UIT_COMPILER_VERSION%\Bin\msbuild.exe"
if not exist %UIT_COMPILER_EXE%	(echo can't find VC compiler [%UIT_COMPILER_EXE%], check your Visual Studio install path & goto :eof)
set UIT_CMAKE_EXE=cmake
set UIT_ZIP_EXT=%UIT_TOOL_DIR%\7z.exe
set CAPI_CORE_GEN=%UIT_TOOL_DIR%\commonapi-generator\commonapi-generator-windows-x86_64.exe
set CAPI_DBUS_GEN=%UIT_TOOL_DIR%\commonapi_dbus_generator\commonapi-dbus-generator-windows-x86_64.exe
set CAPI_SOMEIP_GEN=%UIT_TOOL_DIR%\commonapi_someip_generator\commonapi-someip-generator-windows-x86_64.exe
set UIT_BUILD_DIR=build\%UIT_TARGET%
set UIT_INSTALL_DIR=%UIT_DIST_DIR%\%UIT_TARGET%\uit
set UIT_CMAKE="%UIT_CMAKE_EXE%" ^
-DCMAKE_INSTALL_PREFIX:PATH=%UIT_INSTALL_DIR% ^
-DCMAKE_INSTALL_BINDIR:PATH=%UIT_INSTALL_DIR%\bin ^
-DCMAKE_INSTALL_ETCDIR:PATH=%UIT_INSTALL_DIR%\etc ^
-DCMAKE_INSTALL_LIBDIR:PATH=%UIT_INSTALL_DIR%\lib ^
-DCMAKE_INSTALL_INCLUDEDIR:PATH=%UIT_INSTALL_DIR%\include ^
-DCMAKE_INSTALL_DOCDIR:PATH=%UIT_INSTALL_DIR%\doc ^
-DCMAKE_INSTALL_TEMPDIR:PATH=%UIT_INSTALL_DIR%\temp ^
-DCMAKE_INSTALL_SHAREDIR:PATH=%UIT_INSTALL_DIR%\share ^
-DCMAKE_INSTALL_RESOURCEDIR:PATH=%UIT_INSTALL_DIR%\resource ^
-DCMAKE_INSTALL_EXAMPLEDIR:PATH=%UIT_INSTALL_DIR%\example ^
-Wno-dev -G "Visual Studio 14 2015"

::call %UIT_CONF_DIR%\%UIT_TARGET%.bat %1 %2

echo --------------------------- Development Environment ---------------------------
echo   UIT Root      	: %UIT_ROOT%
echo   UIT Baseline		: %UIT_BASELINE%
echo   Build Target		: %UIT_TARGET%
echo   System SDK		: %UIT_SYSTEM_SDK_PATH:~1,-1%
echo   Compiler Exe		: %UIT_COMPILER_EXE:~1,-1%
echo   Compiler Version	: %UIT_COMPILER_VERSION%
echo   Install Path		: %UIT_INSTALL_DIR%
echo -------------------------------------------------------------------------------
