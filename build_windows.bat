@echo off

@REM set INCLUDE=%BOOST_ROOT%;C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include
call :set_variable QL_DIR %CD%\QuantLib %BUILD_TYPE%
call :set_variable QLEXT_DIR %CD%\QuantLib-Ext %BUILD_TYPE%
call :set_variable BUILD_TYPE Release %BUILD_TYPE%
call :set_variable ADDRESS_MODEL Win64 %ADDRESS_MODEL%
call :set_variable MSVC_RUNTIME static %MSVC_RUNTIME%
call :set_variable MSVC_VERSION "Visual Studio 16 2019" %MSVC_VERSION%

if "%ADDRESS_MODEL%"=="Win64" (
  set PLATFORM=x64
) else (
  if "%MSVC_VERSION%"=="Visual Studio 16 2019" (
    set PLATFORM=x64
  ) else (
    set PLATFORM=Win32
  )
)

if "%ADDRESS_MODEL%"=="Win64" (
  if "%MSVC_VERSION%"=="Visual Studio 16 2019" (
    set ADDRESS_MODEL=
  )
)

echo BUILD_TYPE: %BUILD_TYPE%
echo QL_DIR: %QL_DIR%
echo QLEXT_DIR: %QLEXT_DIR%
echo ADDRESS_MODEL: %ADDRESS_MODEL%
echo MSVC_RUNTIME: %MSVC_RUNTIME%
echo MSVC_VERSION: %MSVC_VERSION%

cd QuantLib

if exist build (
  rem build folder already exists.
) else (
  mkdir build
)

cd build

if "%ADDRESS_MODEL%"=="Win64" (
  cmake -G "%MSVC_VERSION% %ADDRESS_MODEL%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%QL_DIR% -DMSVC_RUNTIME=%MSVC_RUNTIME% ..
) else (
  cmake -G "%MSVC_VERSION%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%QL_DIR% -DMSVC_RUNTIME=%MSVC_RUNTIME% ..
)

if %errorlevel% neq 0 exit /b 1

msbuild QuantLib.sln /m:%NUMBER_OF_PROCESSORS% /p:Configuration=%BUILD_TYPE% /p:Platform=%PLATFORM%
msbuild INSTALL.vcxproj /m:%NUMBER_OF_PROCESSORS% /p:Configuration=%BUILD_TYPE% /p:Platform=%PLATFORM%

if %errorlevel% neq 0 exit /b 1

cd ..\bin

rem quantlib-test-suite --log_level=message --build_info=true

if %errorlevel% neq 0 exit /b 1

cd ..\..\QuantLib-Ext

if exist build (
  rem build folder already exists.
) else (
  mkdir build
)

cd build

if "%ADDRESS_MODEL%"=="Win64" (
  cmake -G "%MSVC_VERSION% %ADDRESS_MODEL%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%QLEXT_DIR% -DMSVC_RUNTIME=%MSVC_RUNTIME% ..
) else (
  cmake -G "%MSVC_VERSION%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%QLEXT_DIR% -DMSVC_RUNTIME=%MSVC_RUNTIME% ..
)

if %errorlevel% neq 0 exit /b 1

msbuild QuantLibExt.sln /m:%NUMBER_OF_PROCESSORS% /p:Configuration=%BUILD_TYPE% /p:Platform=%PLATFORM%
msbuild INSTALL.vcxproj /m:%NUMBER_OF_PROCESSORS% /p:Configuration=%BUILD_TYPE% /p:Platform=%PLATFORM%

if %errorlevel% neq 0 exit /b 1

cd ..\bin

quantlibext-test-suite --log_level=message --build_info=true

if %errorlevel% neq 0 exit /b 1

cd ..\..\examples\c++

if exist build (
  rem build folder already exists.
) else (
  mkdir build
)

cd build

if "%ADDRESS_MODEL%"=="Win64" (
  cmake -G "%MSVC_VERSION% %ADDRESS_MODEL%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%QLEXT_DIR% -DMSVC_RUNTIME=%MSVC_RUNTIME% ..
) else (
  cmake -G "%MSVC_VERSION%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%QLEXT_DIR% -DMSVC_RUNTIME=%MSVC_RUNTIME%  ..
)

if %errorlevel% neq 0 exit /b 1

msbuild Examples.sln /m:%NUMBER_OF_PROCESSORS% /p:Configuration=%BUILD_TYPE% /p:Platform=%PLATFORM%

if %errorlevel% neq 0 exit /b 1

cd ..\..\..\QuantLib-SWIG\Python

python setup.py wrap

if %MSVC_RUNTIME% neq static (
  if %BUILD_TYPE% neq Release (
    python setup.py build --debug
  ) else (
    python setup.py build
  )
) else (
  if %BUILD_TYPE% neq Release (
    python setup.py build --static --debug
  ) else (
    python setup.py build --static
  )
)

python setup.py bdist_wheel

if %errorlevel% neq 0 exit /b 1

cd ..\..

@echo on

:set_variable
set %~1=%~2
EXIT /B 0
