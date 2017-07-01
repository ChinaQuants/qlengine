# qlengine

**qlengine**是基于QuantLib和QuantLib-SWIG项目的扩展。目的是通过增加新模块的方法，使得在不破坏QuantLib原有项目的结构的前提下，添加自定义功能，满足特定的需求。

该项目的特点：

* 非侵入式的更改QuantLib主项目；

* 在QuantLib-Ext项目下增加新的功能；

* 更加容易使用的安装脚本。


## 依赖

* boost >= 1.58.0 
 
    编译安装只需要``boost``库头文件。如果需要运行单元测试，需要链接``unit_test_frame_work``。

* SWIG >= 3.0.12

    生成需要的python wrapper。

* cmake >= 3.0.0

    在Windows下生成所需的 Visual Studio 项目文件。

* QuantLib >= 1.10.0

* QuantLib-SWIG >= 1.10.0

其中QuantLib以及QuantLib-SWIG已经作为依赖的模块加入。

## 下载

源代码可以使用如下方式下载：

```bash
git clone https://github.com/ChinaQuants/qlengine.git
```

该项目有子模块的依赖，所以还需要完成下面的步骤，下载子模块：

```bash
cd qlengine
git submodule init
git submodule update
```

## 安装

### 从安装包安装

对于在windows上的python 3.6用户，在prebuilt文件夹下已经放置了预编译完成的安装包。在prebuilt文件夹下运行：

```bash
pip install QuantLib_Python-1.11-cp36-cp36m-win_amd64.whl
```

完成安装。

### 从源码安装

#### Windows

在windows上我们使用了静态库链接。

* 打开``build_windows.bat``文件，将环境变量``BOOST_ROOT``修改为正确的值，``BOOST_ROOT``需要指向boost库的根目录；

* 运行``build_windows.bat``。

#### Linux

在Linux上我们使用了动态库链接，所以需要在安装完成后，将必要的库目录加入``LD_LIBRARY_PATH``：

* 运行``build_linux.sh``。

* 将以下两个路径加入``LD_LIBRARY_PATH``：

    * QuantLib/lib

    * QuantLib-Ext/lib

## 运行

安装完成以后，可以在python环境中运行如下语句：

```python
from QuantLib import *
```
