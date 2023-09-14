@echo off
setlocal enabledelayedexpansion

REM Number of times to run the program
set num_runs=100

REM Arguments for the program
set arg1=8
set arg2=1000
set arg3=10000
set arg4=0.5
set arg5=0.25
set arg6=0.25

REM Loop to run the program multiple times
for /l %%i in (1,1,%num_runs%) do (
    .\one_mutex.exe !arg1! !arg2! !arg3! !arg4! !arg5! !arg6!
)

endlocal