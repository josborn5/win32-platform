REM VS 2015:
REM SET DEV_ENV="%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\Common7\IDE\devenv.exe"

REM VS 2017:
REM SET DEV_ENV="%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe"

REM VS 2019:
REM SET DEV_ENV="%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\Common7\IDE\devenv.exe"

REM VS 2022:
call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

devenv.exe .\bin\unit_tests.exe