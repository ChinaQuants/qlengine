@echo off

set BOOST_ROOT=D:/dev/boost_1_65_1
set BOOST_LIBRARYDIR=D:/dev/boost_1_65_1/lib/stage
set INCLUDE=%BOOST_ROOT%
set QL_DIR=%CD%\QuantLib
set QLEXT_DIR=%CD%\QuantLib-Ext
set BUILD_TYPE=Debug
set ADDRESS_MODEL=Win64

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
  cmake -G "Visual Studio 14 2015 %ADDRESS_MODEL%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ..
) else (
  cmake -G "Visual Studio 14 2015" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ..
)

msbuild Project.sln /target:QuantLib /m /p:Configuration=%BUILD_TYPE% /p:Platform=%PLATFORM%

cd ..\..\QuantLib-Ext

if exist build (
  rem build folder already exists.
) else (
  mkdir build
)

cd build

if %ADDRESS_MODEL%==Win64 (
  cmake -G "Visual Studio 14 2015 %ADDRESS_MODEL%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ..
) else (
  cmake -G "Visual Studio 14 2015" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ..
)

msbuild QuantLibExt.sln /target:QuantLibExt /m /p:Configuration=%BUILD_TYPE% /p:Platform=%PLATFORM%

cd ..\..\QuantLib-SWIG\Python

python setup.py wrap
python setup.py build
python setup.py bdist_wheel
python setup.py install

cd ..\..

@echo on
