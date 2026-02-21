#!/bin/bash  
# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.    
# version V1.0
# used for ldso compile
set -e
COMPILE_MODE=$1
# source toolenv for building libc base
source /opt/RTOS/setenv.sh "$ARCHI"> /dev/null

echo "-------------------- Compile musl START --------------------"

cd $ROOT_DIR/src/base/

mkdir -p $ROOT_DIR/output/hpe-toolchain

# build hpe toolchain
if [ ${COMPILE_MODE} == "_ylong_debug"  ];then
    ./configure --enable-debug  CROSS_COMPILE=aarch64-linux-gnu- --prefix=/opt/buildtools/ylongc/hpe/aarch64
else
    ./configure  CROSS_COMPILE=aarch64-linux-gnu- --prefix=/opt/buildtools/ylongc/hpe/aarch64
fi

make clean
make ARCH=aarch64 -j$(getconf _NPROCESSORS_CONF) O="$BUILD_DIR" VERBOSE=1

# install hpe toolchain into rpm rootpath
make install -j$(getconf _NPROCESSORS_CONF) DESTDIR=$ROOT_DIR/output/hpe-toolchain
cp $ROOT_DIR/src/base/tools/setenv.sh $ROOT_DIR/output/hpe-toolchain/opt/buildtools/ylongc/hpe

# install hpe toolchain into docker path
#cp -rf $ROOT_DIR/output/hpe-toolchain/opt/buildtools/ylongc/* /opt/buildtools/ylongc/

echo "-------------------- Compile musl END --------------------"
cd ~-

