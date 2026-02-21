#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2024-2033. All rights reserved.
set -e

source $TOPDIR/scripts/common.sh

cd $TOPDIR/self_source/devhost

rm -rf $TOPDIR/self_source/devhost/build
if [ "${TOOLCHAIN_CC}" = "clang" ]; then
cp $TOPDIR/scripts/self_source/devhost/.config_llvm $TOPDIR/self_source/devhost/.config
cp $TOPDIR/scripts/self_source/devhost/backends/liblinux/.config_llvm $TOPDIR/self_source/devhost/backends/liblinux/.config
else
cp $TOPDIR/scripts/self_source/devhost/.config_gcc $TOPDIR/self_source/devhost/.config
cp $TOPDIR/scripts/self_source/devhost/backends/liblinux/.config_gcc $TOPDIR/self_source/devhost/backends/liblinux/.config
fi
mkdir $TOPDIR/self_source/devhost/build
cd $TOPDIR/self_source/devhost/build
cp $TOPDIR/self_source/devhost/.config .
sed -i "s|sysroot=|sysroot=$TOPDIR/output/aarch64|g" .config
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
          $TOPDIR/self_source/devhost

cmake --build $TOPDIR/self_source/devhost/build -j 32

DESTDIR="${TOPDIR}/output/aarch64" cmake --install $TOPDIR/self_source/devhost/build

#lnxbase
rm -rf $TOPDIR/self_source/devhost/build/lnxbase
mkdir -p $TOPDIR/self_source/devhost/build/lnxbase
cd $TOPDIR/self_source/devhost/build/lnxbase
cp $TOPDIR/self_source/devhost/backends/liblinux/.config .
sed -i "s|sysroot=|sysroot=$TOPDIR/output/aarch64|g" .config
if [ "${TOOLCHAIN_CC}" = "clang" ]; then
    sed -i "s|-isystem|-isystem\ $TOPDIR/output/aarch64/usr/include|g" .config
fi

cmake -DCMAKE_MODULE_PATH:PATH=$TOPDIR/output/aarch64/usr/share/cmake/Modules \
          -DHMNATIVESDKPATH:PATH=$TOPDIR/output/tools \
          -DCMAKE_SYSROOT:PATH=$TOPDIR/output/aarch64 \
          -DCMAKE_TOOLCHAIN_FILE=$TOPDIR/output/aarch64/usr/share/cmake/Modules/$TOOLCHAIN_FILE \
          -DBUILD_USE_HMBUILD=TRUE \
          $CMAKE_OPTIONS_LLVM \
          $TOPDIR/self_source/devhost/backends/liblinux

cmake --build $TOPDIR/self_source/devhost/build/lnxbase -j 32

DESTDIR="${TOPDIR}/output/aarch64" cmake --install $TOPDIR/self_source/devhost/build/lnxbase
if [ -f "$TOPDIR/self_source/devhost/devhost/backends/liblinux/scripts/ksymtab.lds" ]; then
   install -D $TOPDIR/self_source/devhost/devhost/backends/liblinux/scripts/ksymtab.lds ${TOPDIR}/output/aarch64/lib/ksymtab.lds
fi
cp $TOPDIR/output/aarch64/debug/lib/libdh-lnxbase.unstripped.so $TOPDIR/output/aarch64/lib/libdh-lnxbase.so.1.0

#ldk_ext
cp -f $TOPDIR/scripts/self_source/devhost/ldk_ext/* $TOPDIR/self_source/devhost/ldk_ext/
cp $TOPDIR/self_source/devhost/.config $TOPDIR/self_source/devhost/ldk_ext/
rm -rf $TOPDIR/self_source/devhost/build/ldk_ext
mkdir -p $TOPDIR/self_source/devhost/build/ldk_ext
cd $TOPDIR/self_source/devhost/build/ldk_ext
cp $TOPDIR/self_source/devhost/.config .
sed -i "s|sysroot=|sysroot=$TOPDIR/output/aarch64|g" .config
if [ "${TOOLCHAIN_CC}" = "clang" ]; then
    sed -i "s|-isystem|-isystem\ $TOPDIR/output/aarch64/usr/include|g" .config
fi

cmake -DCMAKE_MODULE_PATH:PATH=$TOPDIR/output/aarch64/usr/share/cmake/Modules \
          -DHMNATIVESDKPATH:PATH=$TOPDIR/output/tools \
          -DCMAKE_SYSROOT:PATH=$TOPDIR/output/aarch64 \
          -DCMAKE_TOOLCHAIN_FILE=$TOPDIR/output/aarch64/usr/share/cmake/Modules/$TOOLCHAIN_FILE \
          -DBUILD_USE_HMBUILD=TRUE \
          $CMAKE_OPTIONS_LLVM \
          $TOPDIR/self_source/devhost/ldk_ext

cmake --build $TOPDIR/self_source/devhost/build/ldk_ext -j 32

DESTDIR="${TOPDIR}/output/aarch64" cmake --install $TOPDIR/self_source/devhost/build/ldk_ext

