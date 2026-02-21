#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2024-2033. All rights reserved.
set -e

source $TOPDIR/scripts/common.sh

cd $TOPDIR/self_source/hm-cmake-modules

cp $TOPDIR/open_source/build_tools/yocto/layers/meta-hongmeng-ng/recipes-devtools/hm-cmake-modules/files/cmakeng_toolchain/CMakeLists.txt .
cp $TOPDIR/open_source/build_tools/yocto/layers/meta-hongmeng-ng/recipes-devtools/hm-cmake-modules/files/cmakeng_toolchain/toolchain.cmake.in .
cp $TOPDIR/open_source/build_tools/yocto/layers/meta-hongmeng-ng/recipes-devtools/hm-cmake-modules/files/cmakeng_toolchain/itrustee-toolchain.cmake.in .

export CMAKE_C_COMPILER=${CROSS_COMPILE}gcc
export ARCH=${ARCH}
export SRCTREE=$PWD
export OBJTREE=$PWD/build/

cmake --install $PWD
