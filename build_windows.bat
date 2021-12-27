@echo off

set BOOST_ROOT=D:/dev/boost_1_74_0
set BOOST_LIBRARYDIR=D:/dev/boost_1_74_0/lib64-msvc-14.2
set INCLUDE=%BOOST_ROOT%;C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include
set QL_DIR=%CD%\QuantLib
set QLEXT_DIR=%CD%\QuantLib-Ext
set BUILD_TYPE=Release
set ADDRESS_MODEL=Win64
set VS_VERSION=Visual Studio 16 2019

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

cmake -G "%VS_VERSION%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%QL_DIR% -DMSVC_RUNTIME=%MSVC_RUNTIME% --target install ..


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

cmake -G "%VS_VERSION%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%QLEXT_DIR% --target install ..

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
cmake -G "%VS_VERSION%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%QLEXT_DIR% --target install ..

if %errorlevel% neq 0 exit /b 1

msbuild Examples.sln /m:%NUMBER_OF_PROCESSORS% /p:Configuration=%BUILD_TYPE% /p:Platform=%PLATFORM%

if %errorlevel% neq 0 exit /b 1

cd ..\..\..\QuantLib-SWIG\Python

python setup.py wrap
if %BUILD_TYPE% neq Release (
    python setup.py build --debug
) else (
    python setup.py build
)
python setup.py bdist_wheel

if %errorlevel% neq 0 exit /b 1

cd ..\..

@echo on
