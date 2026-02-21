#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2024-2033. All rights reserved.

set -e

source $TOPDIR/scripts/common.sh

cd $TOPDIR/self_source/hm-ulibs

rm -rf $TOPDIR/self_source/hm-ulibs/build

#metalibs
mkdir -p $TOPDIR/self_source/hm-ulibs/build/metalibs
cd $TOPDIR/self_source/hm-ulibs/build/metalibs
cp $TOPDIR/self_source/hm-ulibs/libs/metalibs/.config .
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
          $TOPDIR/self_source/hm-ulibs/libs/metalibs
cmake --build $TOPDIR/self_source/hm-ulibs/build/metalibs --verbose -- -j 32
DESTDIR="${TOPDIR}/output/aarch64" cmake --install $TOPDIR/self_source/hm-ulibs/build/metalibs


#ulibs
mkdir -p $TOPDIR/self_source/hm-ulibs/build/ulibs
cd $TOPDIR/self_source/hm-ulibs/build/ulibs
cp $TOPDIR/self_source/hm-ulibs/ulibs/.config .
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
          $TOPDIR/self_source/hm-ulibs/ulibs
cmake --build $TOPDIR/self_source/hm-ulibs/build/ulibs --verbose -- -j 32
DESTDIR="${TOPDIR}/output/aarch64" cmake --install $TOPDIR/self_source/hm-ulibs/build/ulibs

#devmgr-libs
mkdir $TOPDIR/self_source/hm-ulibs/build/devmgr
cd ${TOPDIR}/self_source/hm-ulibs/build/devmgr
cp $TOPDIR/self_source/hm-ulibs/devmgr/.config .config
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
      ${TOPDIR}/self_source/hm-ulibs/devmgr/libs
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(${TOOLCHAIN_CC} -print-file-name=plugin)
cmake --build ${TOPDIR}/self_source/hm-ulibs/build/devmgr --verbose -- -j 32
DESTDIR="${TOPDIR}/output/aarch64" cmake --install ${TOPDIR}/self_source/hm-ulibs/build/devmgr
