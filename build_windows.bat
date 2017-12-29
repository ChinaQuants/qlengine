@echo off

set BOOST_ROOT=D:/dev/boost_1_65_1
set BOOST_LIBRARYDIR=D:/dev/boost_1_65_1/lib/stage
set INCLUDE=%BOOST_ROOT%;D:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include
set QL_DIR=%CD%\QuantLib
set QLEXT_DIR=%CD%\QuantLib-Ext
set BUILD_TYPE=Release
set ADDRESS_MODEL=Win64
set MSVC_RUNTIME=dynamic

cd QuantLib

if exist build (
  rem build folder already exists.
) else (
  mkdir build
)

if %ADDRESS_MODEL%==Win64 (
  set PLATFORM=x64
) else (
  set PLATFORM=Win32
)

cd build


if %ADDRESS_MODEL%==Win64 (
  cmake -G "Visual Studio 14 2015 %ADDRESS_MODEL%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%QL_DIR% -DMSVC_RUNTIME=%MSVC_RUNTIME% --target install ..
) else (
  cmake -G "Visual Studio 14 2015" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%QL_DIR% -DMSVC_RUNTIME=%MSVC_RUNTIME% --target install ..
)

if %errorlevel% neq 0 exit /b 1

msbuild QuantLib.sln /m:%NUMBER_OF_PROCESSORS% /p:Configuration=%BUILD_TYPE% /p:Platform=%PLATFORM%
msbuild INSTALL.vcxproj /m:%NUMBER_OF_PROCESSORS% /p:Configuration=%BUILD_TYPE% /p:Platform=%PLATFORM%

if %errorlevel% neq 0 exit /b 1

cd ..\bin

quantlib-test-suite --log_level=message --build_info=true

if %errorlevel% neq 0 exit /b 1

cd ..\..\QuantLib-Ext

if exist build (
  rem build folder already exists.
) else (
  mkdir build
)

cd build

if %ADDRESS_MODEL%==Win64 (
  cmake -G "Visual Studio 14 2015 %ADDRESS_MODEL%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%QLEXT_DIR% -DMSVC_RUNTIME=%MSVC_RUNTIME% --target install ..
) else (
  cmake -G "Visual Studio 14 2015" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%QLEXT_DIR% -DMSVC_RUNTIME=%MSVC_RUNTIME% --target install ..
)

if %errorlevel% neq 0 exit /b 1

msbuild QuantLibExt.sln /m:%NUMBER_OF_PROCESSORS% /p:Configuration=%BUILD_TYPE% /p:Platform=%PLATFORM%
msbuild INSTALL.vcxproj /m:%NUMBER_OF_PROCESSORS% /p:Configuration=%BUILD_TYPE% /p:Platform=%PLATFORM%

if %errorlevel% neq 0 exit /b 1

cd ..\bin

quantlibext-test-suite --log_level=message --build_info=true

if %errorlevel% neq 0 exit /b 1

cd ..\..\QuantLib-SWIG\Python

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
