@echo off
setlocal

windres ".\resources\app.rc" ".\build-out\app.res" || exit /b 1
set rayInclude="..\raylib\include"
set rayLib="..\raylib\zig-out\lib\raylib.lib"
zig c++  .\src\main.cpp build-out\app.res -o .\build-out\DVDScreenSaver.exe -I "%rayInclude%" "%rayLib%"  -lopengl32 -lgdi32 -lwinmm || exit /b 1
.\build-out\DVDScreenSaver.exe

