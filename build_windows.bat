@echo off

set "BOOST_ROOT=D:/dev/boost_1_64_0"
set INCLUDE=%BOOST_ROOT%
set QL_DIR=%CD%\QuantLib

cd QuantLib

if exist build (
  rem build folder already exists.
) else (
  mkdir build
)

mkdir build
cd build

cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=Release ..

msbuild Project.sln /target:QuantLib /m /p:Configuration=Release /p:Platform=x64

cd ..\..\QuantLib-SWIG\Python

python setup.py wrap
python setup.py build
python setup.py install

cd ..\..

@echo on
