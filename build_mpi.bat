
@echo off
setlocal

REM Load Microsoft C++ compiler environment
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b 1

REM Compile the active MPI C++ file
cl.exe /Zi /EHsc /nologo ^
 /I"%MSMPI_INC%." ^
 /I"%MSMPI_INC%x64" ^
 "%~1" ^
 /link ^
 /LIBPATH:"%MSMPI_LIB64%." ^
 msmpi.lib ^
 /OUT:"%~dpn1.exe"

if errorlevel 1 exit /b 1

echo.
echo MPI build successful!
