@echo off
setlocal

where cl >nul 2>nul
if %ERRORLEVEL% NEQ 0 (echo ERROR: "cl" not found - please run this from the MSVC x64 native tools command prompt.) && exit /b 1
:: Either run this script from an msvc-enabled developer console, invoke the vcvars batch script
:: call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
:: call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64


set CFLAGS=/nologo /TP /EHsc /std:c++20 /W4 /wd4996 /wd4200 /O2

call cl %CFLAGS%  src\*.cpp /FeA2

