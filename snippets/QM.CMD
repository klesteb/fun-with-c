@REM +++Date last modified: 05-Jul-1997

@echo off
if exist $qmtemp.cmd del $qmtemp.cmd > nul
qmenup
if exist $qmtemp.cmd $qmtemp
