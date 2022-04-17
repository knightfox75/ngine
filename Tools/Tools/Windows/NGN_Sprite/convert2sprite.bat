echo off
cls

set IN=""
set OUT=""
set WIDTH=32
set HEIGHT=32



for /f "delims==" %%F in ('dir in /b *.png') do set IN=in/%%~nxF & set OUT=out/%%~nF & call :RUN
goto :end

:RUN
cls
echo For the file %IN%

set /p WIDTH="Enter the WIDTH of the sprite [%WIDTH%]: "
echo The WIDTH of the sprite is %WIDTH%.

set /p HEIGHT="Enter the HEIGHT of the sprite [%HEIGHT%]: "
echo The HEIGHT of the sprite is %HEIGHT%. 

NGN_Sprite.exe %IN% -o %OUT% -w %WIDTH% -h %HEIGHT% -s

pause
goto :END

:END