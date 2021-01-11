REM https://youtu.be/HXSuNxpCzdM?t=3199
REM https://www.youtube.com/watch?v=HyVc0X9JKpg
REM https://people.sc.fsu.edu/~jburkardt/data/obj/obj.html

REM VS 2015:
REM call "%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64

REM VS 2017:
call "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
@echo off

SET CODE_DIR=code
SET OUTPUT_DIR=bin

call "%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64

rmdir /S /Q %OUTPUT_DIR%
mkdir %OUTPUT_DIR%
pushd %OUTPUT_DIR%

SET COMMON_COMPILER_FLAGS=-MT -nologo -Gm- -GR- -EHa- -Oi -WX -W4 -wd4100 -wd4201 -FC -Z7 -Fmwin32_platform.map /EHsc /O2

REM 64-bit build
cl.exe %COMMON_COMPILER_FLAGS% ..\%CODE_DIR%\unit_tests.cpp

REM using the '/c' flag to skip linking and create only the '.obj' file
cl.exe %COMMON_COMPILER_FLAGS% /c ..\%CODE_DIR%\gentle_giant.cpp

REM use the 'lib.exe' tool to create a lib file from the bj files
lib.exe -nologo gentle_giant.obj

REM copy the library header files to the output director
xcopy ..\%CODE_DIR%\*.hpp .

REM build the game.exe file using the gentle_giant.lib
SET COMMON_LINKER_FLAGS=-opt:ref user32.lib Gdi32.lib winmm.lib
cl.exe %COMMON_COMPILER_FLAGS% ..\%CODE_DIR%\game.cpp /link %COMMON_LINKER_FLAGS% gentle_giant.lib

xcopy ..\%CODE_DIR%\teapot.obj .

popd
