#!/bin/bash -

git submodule init
git submodule update

export QL_DIR=$PWD/QuantLib
export QLEXT_DIR=$PWD/QuantLib-Ext
export NUM_CORES="$(nproc)"

cd QuantLib
export WORK_DIR=$PWD

export LD_LIBRARY_PATH=$WORK_DIR/lib:$LD_LIBRARY_PATH
export LIBRARY_PATH=$WORK_DIR/lib:$LIBRARY_PATH
export CPLUS_INCLUDE_PATH=$WORK_DIR/include:$CPLUS_INCLUDE_PATH

bash autogen.sh
./configure --prefix=$WORK_DIR

make -j $NUM_CORES
make install

cd ./bin
./quantlib-test-suite --log_level=message --build_info=true

cd ../../QuantLib-Ext
export WORK_DIR=$PWD
mkdir build
cd build

cmake -DCMAKE_INSTALL_PREFIX=$WORK_DIR ..

export LD_LIBRARY_PATH=$WORK_DIR/lib:$LD_LIBRARY_PATH
export LIBRARY_PATH=$WORK_DIR/lib:$LIBRARY_PATH
export CPLUS_INCLUDE_PATH=$WORK_DIR/include:$CPLUS_INCLUDE_PATH 

make -j $NUM_CORES
make install

cd ../bin
./quantlibext-test-suite --log_level=message --build_info=true

cd ../../examples/c++
export WORK_DIR=$PWD
mkdir build
cd build

cmake -DCMAKE_INSTALL_PREFIX=$WORK_DIR ..
make -j $NUM_CORES
make install

echo $LD_LIBRARY_PATH

cd ../../../QuantLib-SWIG/Python
python setup.py wrap
python setup.py build
python setup.py install --user
