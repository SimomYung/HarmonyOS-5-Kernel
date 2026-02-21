#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2024-2033. All rights reserved.

set -e

source $TOPDIR/scripts/common.sh

cd $TOPDIR/self_source/hm-vendor-drivers

# prepare
mkdir $TOPDIR/self_source/hm-vendor-drivers/build/kbuild
cd  $TOPDIR/self_source/hm-vendor-drivers/build/kbuild
cp $TOPDIR/self_source/hm-vendor-drivers/toolchain.cmake .
unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS
make ARCH=arm64 CROSS_COMPILE=aarch64-hongmeng-musl-      \
     -C $TOPDIR/open_source/hm-openeuler-kernel O=$TOPDIR/self_source/hm-vendor-drivers/build/kbuild LOCALVERSION="" liblinux_sd5151t_defconfig

make ARCH=arm64 CROSS_COMPILE=aarch64-hongmeng-musl-      \
     -C $TOPDIR/open_source/hm-openeuler-kernel O=$TOPDIR/self_source/hm-vendor-drivers/build/kbuild LOCALVERSION="" -j 32 modules_prepare

ln -s $TOPDIR/open_source/hm-openeuler-kernel/include/linux $TOPDIR/self_source/hm-vendor-drivers/build/kbuild/include
(                                                                   \
        cd $TOPDIR/self_source/hm-vendor-drivers/build/kbuild;                                          \
	make ARCH=arm64 INSTALL_HDR_PATH=$(pwd)/uapi       \
                   -j 32 headers_install                                      \
    )

#alternative
mkdir $TOPDIR/self_source/hm-vendor-drivers/build/alternative
cd  $TOPDIR/self_source/hm-vendor-drivers/build/alternative
cp $TOPDIR/self_source/hm-vendor-drivers/toolchain.cmake .
cmake \
          -G 'Unix Makefiles' -DCMAKE_MAKE_PROGRAM=make \
          $oecmake_sitefile \
          -DCMAKE_INSTALL_PREFIX:PATH=/usr \
          -DCMAKE_INSTALL_BINDIR:PATH=bin \
          -DCMAKE_INSTALL_SBINDIR:PATH=sbin \
          -DCMAKE_INSTALL_LIBEXECDIR:PATH=libexec \
          -DCMAKE_INSTALL_SYSCONFDIR:PATH=/etc \
          -DCMAKE_INSTALL_SHAREDSTATEDIR:PATH=../com \
          -DCMAKE_INSTALL_LOCALSTATEDIR:PATH=/var \
          -DCMAKE_INSTALL_LIBDIR:PATH=lib \
          -DCMAKE_INSTALL_INCLUDEDIR:PATH=include \
          -DCMAKE_INSTALL_DATAROOTDIR:PATH=share \
          -DPYTHON_EXECUTABLE:PATH=/usr/bin/python3 \
          -DPython_EXECUTABLE:PATH=/usr/bin/python3 \
          -DPython3_EXECUTABLE:PATH=/usr/bin/python3 \
          -DLIB_SUFFIX= \
          -DCMAKE_INSTALL_SO_NO_EXE=0 \
          -DCMAKE_TOOLCHAIN_FILE=$TOPDIR/self_source/hm-vendor-drivers/build/alternative/toolchain.cmake \
          -DCMAKE_NO_SYSTEM_FROM_IMPORTED=1 \
          -DCMAKE_EXPORT_NO_PACKAGE_REGISTRY=ON \
          -DFETCHCONTENT_FULLY_DISCONNECTED=ON \
          -DCMAKE_SYSTEM_NAME=Generic                     -DUNIX=1                        \
          -DHMSDKSYSROOTPATH=$TOPDIR/output/aarch64                     \
          -DHMSDKPATH=$TOPDIR/output/aarch64                    \
          -DHMSDKINCLUDE=$TOPDIR/output/aarch64/usr/include                     \
          -DNATIVE_BINDIR=$TOPDIR/output/tools/usr/bin                         \
          -DHMSDKLIB=$TOPDIR/output/aarch64/usr/lib                     \
          -DCMAKE_MODULE_PATH=$TOPDIR/output/aarch64/usr/share/cmake/Modules  \
          -DHMBUILD_LEGACY_CMAKE_LIBRAR=TRUE            \
          -DARCH=aarch64     -DLINUX_KERNEL_ARCH=arm64    \
          -DDESTDIR=$TOPDIR/output/aarch64/usr/share/vendor-targets    \
          -Wno-dev \
          -DCMAKE_C_COMPILER_WORKS=TRUE \
          -DCMAKE_CXX_COMPILER_WORKS=TRUE \
          -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY        \
          -B $TOPDIR/self_source/hm-vendor-drivers/build/alternative \
          -S $TOPDIR/self_source/hm-vendor-drivers
DESTDIR="${TOPDIR}/output/aarch64" VERBOSE=1 cmake --build $TOPDIR/self_source/hm-vendor-drivers/build/alternative --target alternative_headers_sd5151t_5.10
cmake -DCOMPONENT=alternative_headers_iot_install -P $TOPDIR/self_source/hm-vendor-drivers/build/alternative/cmake_install.cmake
cp -r $TOPDIR/self_source/hm-vendor-drivers/build/alternative/targets/sd5151t_5.10/include/alternative ../kbuild/include/

# compile
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(${TOOLCHAIN_CC} -print-file-name=plugin)
cd $TOPDIR/self_source/hm-vendor-drivers/build
cp $TOPDIR/self_source/hm-vendor-drivers/toolchain.cmake .
sed -i "s|sysroot=|sysroot=$TOPDIR/output/aarch64 -L$TOPDIR/output/aarch64/usr/lib -L$TOPDIR/output/aarch64/lib|g" $TOPDIR/self_source/hm-vendor-drivers/build/toolchain.cmake

cmake \
          -G 'Unix Makefiles' -DCMAKE_MAKE_PROGRAM=make \
          $oecmake_sitefile \
          -DCMAKE_INSTALL_PREFIX:PATH=/usr \
          -DCMAKE_INSTALL_BINDIR:PATH=bin \
          -DCMAKE_INSTALL_SBINDIR:PATH=sbin \
          -DCMAKE_INSTALL_LIBEXECDIR:PATH=libexec \
          -DCMAKE_INSTALL_SYSCONFDIR:PATH=/etc \
          -DCMAKE_INSTALL_SHAREDSTATEDIR:PATH=../com \
          -DCMAKE_INSTALL_LOCALSTATEDIR:PATH=/var \
          -DCMAKE_INSTALL_LIBDIR:PATH=lib \
          -DCMAKE_INSTALL_INCLUDEDIR:PATH=include \
          -DCMAKE_INSTALL_DATAROOTDIR:PATH=share \
          -DPYTHON_EXECUTABLE:PATH=/usr/bin/python3 \
          -DPython_EXECUTABLE:PATH=/usr/bin/python3 \
          -DPython3_EXECUTABLE:PATH=/usr/bin/python3 \
          -DLIB_SUFFIX= \
          -DCMAKE_INSTALL_SO_NO_EXE=0 \
          -DCMAKE_TOOLCHAIN_FILE=$TOPDIR/self_source/hm-vendor-drivers/build/toolchain.cmake \
          -DCMAKE_NO_SYSTEM_FROM_IMPORTED=1 \
          -DCMAKE_EXPORT_NO_PACKAGE_REGISTRY=ON \
          -DFETCHCONTENT_FULLY_DISCONNECTED=ON \
          -DCMAKE_SYSTEM_NAME=Generic                     -DUNIX=1                        \
          -DHMSDKSYSROOTPATH=$TOPDIR/output/aarch64                     \
          -DHMSDKPATH=$TOPDIR/output/aarch64                    \
          -DHMSDKINCLUDE=$TOPDIR/output/aarch64/usr/include                     \
          -DNATIVE_BINDIR=$TOPDIR/output/tools/usr/bin                         \
          -DHMSDKLIB=$TOPDIR/output/aarch64/usr/lib                     \
          -DCMAKE_MODULE_PATH=$TOPDIR/output/aarch64/usr/share/cmake/Modules  \
          -DHMBUILD_LEGACY_CMAKE_LIBRAR=TRUE            \
	  -DARCH=aarch64     -DLINUX_KERNEL_ARCH=arm64     -DLINUX_KERNEL_VERSION=5.10.0-sd5151t     -DCROSS_COMPILE=aarch64-hongmeng-musl- \
	  -DLINUX_BUILD_DIR=$TOPDIR/self_source/hm-vendor-drivers/build/kbuild -DMODLIB=$TOPDIR/output/aarch64/lib/modules/linux-5.10.0-sd5151t  \
	  -DTARGET_BOARD_PLATFORM=sd5151t_5.10     -DLINUX_HEADERS_DIR=/$TOPDIR/self_source/hm-vendor-drivers/build/kbuild/uapi \
	  -DDESTDIR=$TOPDIR/output/aarch64/usr/share/vendor-targets     -DBINS_INSTALL_DIR=$TOPDIR/output/aarch64/usr/bin     -DLIBS_INSTALL_DIR=$TOPDIR/output/aarch64/usr/lib   \
          -Wno-dev \
	  -DCMAKE_C_COMPILER_WORKS=TRUE \
	  -DCMAKE_CXX_COMPILER_WORKS=TRUE \
	  -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY	\
	  -B $TOPDIR/self_source/hm-vendor-drivers/build \
          -S $TOPDIR/self_source/hm-vendor-drivers
echo "begin compile sd5151t_5.10"
DESTDIR="${TOPDIR}/output/aarch64" VERBOSE=1 cmake --build $TOPDIR/self_source/hm-vendor-drivers/build --target sd5151t_5.10
echo "end compile sd5151t_5.10"
cmake -DCOMPONENT=sd5151t_5.10_builtin_install -P $TOPDIR/self_source/hm-vendor-drivers/build/cmake_install.cmake
cmake -DCOMPONENT=sd5151t_5.10_modules_install -P $TOPDIR/self_source/hm-vendor-drivers/build/cmake_install.cmake
cmake -DCOMPONENT=libs_install -P $TOPDIR/self_source/hm-vendor-drivers/build/cmake_install.cmake
#cmake -DCOMPONENT=bins_install -P $TOPDIR/self_source/hm-vendor-drivers/build/cmake_install.cmake
