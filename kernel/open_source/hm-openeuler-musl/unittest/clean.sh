#!/usr/bin/env bash
# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: The unittest clean shell script
# Author: Huawei OS Kernel Lab
# Create: Fri Jan 29 10:04:40 2021

set -e

SRCDIR=$(cd "$(dirname $0)";pwd)
HMUT_BUILD_OUTPUT=build

# Clean ouputs of cmake build
if [ -f "${HMUT_BUILD_OUTPUT}/Makefile" ]; then
	# In case the generated Makefile is broken
	cd ${HMUT_BUILD_OUTPUT}
	make clean
	cd ..
fi

# Clean metadata of cmake
if [ -d "${HMUT_BUILD_OUTPUT}/CMakeFiles" ]; then
    rm -r ${HMUT_BUILD_OUTPUT}/CMakeFiles
fi
if [ -f "${HMUT_BUILD_OUTPUT}/Makefile" ]; then
    rm -r ${HMUT_BUILD_OUTPUT}/Makefile
fi
if [ -f "${HMUT_BUILD_OUTPUT}/cmake_install.cmake" ]; then
    rm -r ${HMUT_BUILD_OUTPUT}/cmake_install.cmake
fi
