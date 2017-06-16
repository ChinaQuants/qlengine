#!/bin/sh

cd QuantLib
bash autogen.sh
./configure prefix=$PWD

export CPLUS_INCLUDE_PATH=$PWD:$CPLUS_INCLUDE_PATH
export LD_LIBRARY_PATH=$PWD/lib:$LD_LIBRARY_PATH

make -j 8
make install

cd ../QuantLib-SWIG/Python
python setup.py wrap
python setup.py build
python setup.py install
