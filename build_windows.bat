@echo off

cd QuantLib

set "INCLUDE=d:\dev\boost_1_63_0;%INCLUDE%"
set "QL_DIR=%cd%\QuantLib"
set UseEnv=true
msbuild QuantLib.sln /target:QuantLib /m /p:Configuration=Release /p:Platform=x64

cd ..\QuantLib-SWIG\Python

python setup.py wrap
python setup.py install

cd ..\..

@echo on
