echo off
cls

set IN=""
set OUT=""
set TS=32
set LV=2


for /f "delims==" %%F in ('dir in /b *.png') do set IN=in/%%~nxF & set OUT=out/%%~nF & call :RUN
goto :end

:RUN
cls
echo For the file %IN%

set /p TS="Enter the size of the tile [%TS%]: "
echo The size of the tile is %TS% pixels.

set /p LV="Enter the optimitzation level [%LV%]: "
echo Level %LV% selected.  

NGN_TileMap.exe %IN% -o %OUT% -ts %TS% -lv %LV% -ef

pause
goto :END

:END