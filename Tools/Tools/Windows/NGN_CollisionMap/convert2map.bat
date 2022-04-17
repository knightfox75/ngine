echo off
cls

set IN=""
set OUT=""
set TS=16

for /f "delims==" %%F in ('dir in /b *.png') do set IN=in/%%~nxF & set OUT=out/%%~nF & call :RUN
goto :end

:RUN
cls
echo For the file %IN%
set /p TS="Enter the size of the tile [%TS%]: "
echo The size of the tile is %TS% pixels. 

NGN_CollisionMap.exe %IN% -o %OUT% -ts %TS%

pause
goto :END

:END