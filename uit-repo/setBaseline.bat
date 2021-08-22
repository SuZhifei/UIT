@echo off
set UIT_BASELINE=%1
setlocal ENABLEDELAYEDEXPANSION

set curdir=%cd%

set valid=0

call :setBaseline %1

goto :eof

:isValidBaseline
for /f %%b in ('git --git-dir=%1/.git --work-tree=%1 branch -r') do (
	set v=%%b
	set bl=!v:~7,10!
	if "!bl!" == "%2" (
		if "!bl!" == "master" (
			set "valid=0"
		) else if "!bl!" == "HEAD" (
			set "valid=0"
		) else (
			set "valid=1"
			goto :eof
		)
	) else (
		set "valid=0"
	)
)
goto :eof

:setBaselineOne
call :isValidBaseline %1 %2
if %valid%==0 (
	echo error: %1 has no baseline %2!
) else (
	echo setBaseline of %1 to %2
	git --git-dir=%1/.git --work-tree=%1 checkout %2 1>nul
)
goto :eof

:setBaseline
call :setBaselineOne %curdir% %1
if %valid%==0 (goto :eof)

for /d %%i in (%curdir%\package\*) do (
	call :setBaselineOne %%i %1
	)
goto :eof
