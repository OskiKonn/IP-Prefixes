@echo off

if not exist build (
    mkdir build
)

cmake -B build -S .
cmake --build build

build\Debug\app.exe

pause