#!/bin/sh

cd QuantLib
bash autogen.sh
./configure prefix=$PWD

export CPLUS_INCLUDE_PATH=$PWD/include:$CPLUS_INCLUDE_PATH
export LIBRARY_PATH=$PWD/lib:$LIBRARY_PATH

make -j 8
make install

cd ../QuantLib-Ext
bash autogen.sh
./configure prefix=$PWD

export CPLUS_INCLUDE_PATH=$PWD/include:$CPLUS_INCLUDE_PATH
export LIBRARY_PATH=$PWD/lib:$LIBRARY_PATH

make -j 8
make install

cd ../QuantLib-SWIG/Python
python setup.py wrap
python setup.py build
python setup.py install
