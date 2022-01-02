@echo off

REM https://youtu.be/HXSuNxpCzdM?t=3199
REM https://www.youtube.com/watch?v=HyVc0X9JKpg
REM https://people.sc.fsu.edu/~jburkardt/data/obj/obj.html

SET CODE_DIR=code
SET OUTPUT_DIR=bin

call "%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64

rmdir /S /Q %OUTPUT_DIR%
mkdir %OUTPUT_DIR%
pushd %OUTPUT_DIR%

SET COMMON_COMPILER_FLAGS=-MT -nologo -Gm- -GR- -EHa- -Oi -WX -W4 -wd4100 -wd4201 -FC -Z7 /EHsc /O2 -Fm

REM 64-bit build

REM Build the platform independent library
REM using the '/c' flag to skip linking and create only the '.obj' file
cl.exe %COMMON_COMPILER_FLAGS% /c ..\%CODE_DIR%\gentle_giant.cpp

REM use the 'lib.exe' tool to create a lib file from the obj files
lib.exe -nologo gentle_giant.obj

REM Build the platform dependent library
REM using the '/c' flag to skip linking and create only the '.obj' file
cl.exe %COMMON_COMPILER_FLAGS% /c ..\%CODE_DIR%\gentle_giant_win32.cpp

REM use the 'lib.exe' tool to create a lib file from the obj files
lib.exe -nologo gentle_giant_win32.obj

REM copy the library header files to the output directory
xcopy ..\%CODE_DIR%\*.hpp .

SET COMMON_LINKER_FLAGS=-opt:ref user32.lib Gdi32.lib winmm.lib
REM Build tests
cl.exe %COMMON_COMPILER_FLAGS% ..\%CODE_DIR%\tests\unit_tests.cpp /link %COMMON_LINKER_FLAGS% gentle_giant.lib

REM build the demo using the gentle_giant.lib
cl.exe %COMMON_COMPILER_FLAGS% ..\%CODE_DIR%\demo\game.cpp /link %COMMON_LINKER_FLAGS% gentle_giant.lib gentle_giant_win32.lib

xcopy ..\%CODE_DIR%\demo\teapot.obj .

popd
