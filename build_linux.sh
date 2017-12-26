#!/bin/sh

cd QuantLib
export WORK_DIR=$PWD
mkdir build
cd build

cmake -DCMAKE_INSTALL_PREFIX=$WORK_DIR ..

export CPLUS_INCLUDE_PATH=$WORK_DIR/include:$CPLUS_INCLUDE_PATH
export LIBRARY_PATH=$WORK_DIR/lib:$LIBRARY_PATH
export PATH=$WORK_DIR/bin:$PATH

make -j 8
make install

cd ../bin
./quantlib-test-suite --log_level=message --build_info=true

cd ../../QuantLib-Ext
export WORK_DIR=$PWD
mkdir build
cd build

cmake -DCMAKE_INSTALL_PREFIX=$WORK_DIR ..

export CPLUS_INCLUDE_PATH=$WORK_DIR/include:$CPLUS_INCLUDE_PATH
export LIBRARY_PATH=$WORK_DIR/lib:$LIBRARY_PATH

make -j 8
make install

cd ../bin
./quantlibext-test-suite --log_level=message --build_info=true

cd ../../QuantLib-SWIG/Python
python setup.py wrap
python setup.py build
python setup.py install
