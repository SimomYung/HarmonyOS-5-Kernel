#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2024-2033. All rights reserved.
set -e

source $TOPDIR/scripts/common.sh

cd $TOPDIR/self_source/hmld.so

export CMAKE_C_COMPILER=${CROSS_COMPILE}gcc
export ARCH=${ARCH}
export SRCTREE=$PWD
export OBJTREE=$PWD/build/
export KCONFIG_CONFIG=$TOPDIR/self_source/hmld.so/.config

rm -rf $TOPDIR/self_source/hmld.so/build
cp $TOPDIR/scripts/self_source/hmld.so/.config $TOPDIR/self_source/hmld.so/
mkdir $TOPDIR/self_source/hmld.so/build
cd $TOPDIR/self_source/hmld.so/build
cp $TOPDIR/self_source/hmld.so/.config $TOPDIR/self_source/hmld.so/build/
sed -i "s|sysroot=|sysroot=$TOPDIR/output/aarch64|g" $TOPDIR/self_source/hmld.so/build/.config
if [ "${TOOLCHAIN_CC}" = "clang" ]; then
    sed -i "s|-isystem|-isystem\ $TOPDIR/output/aarch64/usr/include|g" .config
    CMAKE_OPTIONS_LLVM=" -DCMAKE_LINKER=$TOPDIR/output/tools/bin/ld.lld -DCMAKE_STRIP=$TOPDIR/output/tools/bin/llvm-strip "
fi

cmake -DCMAKE_MODULE_PATH:PATH=$TOPDIR/output/aarch64/usr/share/cmake/Modules \
          -DHMNATIVESDKPATH:PATH=$TOPDIR/output/tools \
          -DCMAKE_SYSROOT:PATH=$TOPDIR/output/aarch64 \
          -DCMAKE_TOOLCHAIN_FILE=$TOPDIR/output/aarch64/usr/share/cmake/Modules/$TOOLCHAIN_FILE \
          -DBUILD_USE_HMBUILD=TRUE \
          $CMAKE_OPTIONS_LLVM \
          $TOPDIR/self_source/hmld.so
cmake --build $TOPDIR/self_source/hmld.so/build --verbose
DESTDIR="${TOPDIR}/output/aarch64" cmake --install $TOPDIR/self_source/hmld.so/build

#rm -rf $TOPDIR/self_source/hmld.so/build
#cp ./build/libdh.so ./build/libhmulibs.so ${HMSDKSYSROOTPATH}/usr/lib
