@echo off

:vs2015
	set px="C:\Program Files (x86)\MSBuild\14.0\Bin\msbuild.exe"
	if not exist px	(
	set "UIT_SYSTEM_SDK_PATH=C:\Program Files (x86)\Microsoft Visual Studio 14.0"
	set UIT_COMPILER_EXE=%px%
	set UIT_COMPILER_VERSION=VC14(vs2015^)
	) else (echo can't find compiler [%px%])
	
	set UIT_CMAKE=%UIT_CMAKE% -G "Visual Studio 14 2015"
	goto :eof
	