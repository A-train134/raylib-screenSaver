@echo off
setlocal

windres ".\resources\app.rc" ".\build-out\app.res" || exit /b 1

zig c++  .\src\main.cpp build-out\app.res -o .\build-out\DVDScreenSaver.exe -I"..\raylib\include" "..\raylib\zig-out\lib\raylib.lib" -lopengl32 -lgdi32 -lwinmm || exit /b 1
.\build-out\DVDScreenSaver.exe

