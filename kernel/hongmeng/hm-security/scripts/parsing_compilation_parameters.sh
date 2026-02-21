#!/bin/bash

# Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Wed June 3 17:58:08 2020

set -e

C_COMPILER=""
TOOLCHAIN=""
HMSDKVERSION="latest"
SOURCE_PATH=$(dirname $0)
ABS_SOURCE_PATH=$(cd ${SOURCE_PATH};pwd)
BUILD_PATH="${ABS_SOURCE_PATH}/Build"

if [ "${ARCH}x" == "aarch64x" -a "${BE}x" == "yx" ]
then
    TARGET_PREFIX="aarch64_be-euler-elf-"
    C_COMPILER="${TARGET_PREFIX}gcc"
    TOOLCHAIN="gcc_aarch64_toolchain.cmake"
elif [ "${ARCH}x" == "aarch64x" -a "${BE}x" == "nx" ]
then
    TARGET_PREFIX="aarch64-euler-elf-"
    C_COMPILER="${TARGET_PREFIX}gcc"
    TOOLCHAIN="gcc_aarch64_toolchain.cmake"
elif [ "${ARCH}x" == "armx" -a "${BE}x" == "yx" ]
then
    TARGET_PREFIX="armeb-euler-eabi-"
    C_COMPILER="${TARGET_PREFIX}gcc"
    TOOLCHAIN="gcc_armeb_toolchain.cmake"
elif [ "${ARCH}x" == "armx" -a "${BE}x" == "nx" ]
then
    TARGET_PREFIX="arm-euler-eabi-"
    C_COMPILER="${TARGET_PREFIX}gcc"
    TOOLCHAIN="gcc_arm_toolchain.cmake"
elif [[ "$*"x =~ "clean"x ]]
then
    #make sure "build" directory exists
    if [ -d "${BUILD_PATH}" ]
    then
        rm -rf ${BUILD_PATH}
    fi
    exit 0
else
    usage
    exit 1
fi

if [ -n "${DESIRED_C_COMPILER}" ]
then
    C_COMPILER="${DESIRED_C_COMPILER}"
fi
