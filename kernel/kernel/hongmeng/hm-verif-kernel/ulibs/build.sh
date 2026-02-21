#!/usr/bin/env bash
# Pass compile options to cmake.
# Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.

# Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
# Description: Build script for ulibs.
# Author: Huawei OS Kernel Lab
# Create: Wed Oct 16 17:58:08 2019

# Error out on error
set -e

mkdir -p ulibs/cmake-build
cd ulibs/cmake-build

LDFLAGS=""

cmake \
	-DARCH=${ARCH} \
	-DCROSS_COMPILE=${CROSS_COMPILE} \
	-DHMSDKPATH=${HMSDKSYSROOTPATH}/../.. \
	-DHMSDKINCLUDE=${HMSDKSYSROOTPATH}/usr/include \
	-DHMSDKLIB=${HMSDKSYSROOTPATH}/usr/lib \
	-DCMAKE_MODULE_PATH=${HMSDKNATIVESYSROOTPATH}/usr/share/cmake/Modules \
	-DKCONFIG_PATH=${KCONFIG_CONFIG} \
	-DCMAKE_C_COMPILER_WORKS=TRUE \
	-DCMAKE_CXX_COMPILER_WORKS=TRUE \
	-DCMAKE_EXE_LINKER_FLAGS="-nostdlib" \
	-DHMULIBS_INSTALL_LIB_DIR=${objtree} \
	-DLIBGCC_PATH=${LIBGCC_PATH} \
	-DCMAKE_C_FLAGS="${CMAKE_CFLAGS}" \
	-DCMAKE_C_LINK_FLAGS="${CMAKE_LDFLAGS}" \
	-DCONFIG_KASAN_RUNTIME="${CONFIG_KASAN_RUNTIME}" \
	-DCONFIG_STRIP_OPTION="${CONFIG_STRIP_OPTION}" \
	-DCONFIG_AUDIT="${CONFIG_AUDIT}" \
	${srctree}/ulibs

if [ "$1" == "shared-only" ]; then
	cmake --build . --target hmulibs
	cmake -DCOMPONENT=hmulibs_install -P cmake_install.cmake
else
	cmake --build . --target install
fi
