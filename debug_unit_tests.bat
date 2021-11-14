REM VS 2015:
REM SET DEV_ENV="%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\Common7\IDE\devenv.exe"

REM VS 2017:
REM SET DEV_ENV="%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Professional\Common7\IDE\devenv.exe"

REM VS 2019:
SET DEV_ENV="%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Professional\Common7\IDE\devenv.exe"

%DEV_ENV% .\bin\unit_tests.exe