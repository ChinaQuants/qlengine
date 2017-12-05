#!/bin/sh

cd QuantLib
bash autogen.sh
./configure prefix=$PWD CXXFLAGS=-O2 CFLAGS=-O2

export CPLUS_INCLUDE_PATH=$PWD/include:$CPLUS_INCLUDE_PATH
export LIBRARY_PATH=$PWD/lib:$LIBRARY_PATH
export PATH=$PWD/bin:$PATH

make -j 8
make install

cd bin
./quantlib-test-suite --log_level=message --build_info=true

cd ../../QuantLib-Ext
bash autogen.sh
./configure prefix=$PWD CXXFLAGS=-O2 CFLAGS=-O2

export CPLUS_INCLUDE_PATH=$PWD/include:$CPLUS_INCLUDE_PATH
export LIBRARY_PATH=$PWD/lib:$LIBRARY_PATH

make -j 8
make install

cd bin
./quantlibext-test-suite --log_level=message --build_info=true

cd ../../QuantLib-SWIG/Python
python setup.py wrap
python setup.py build
python setup.py install
