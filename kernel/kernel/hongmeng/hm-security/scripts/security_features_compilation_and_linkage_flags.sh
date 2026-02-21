#!/bin/bash

# Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Wed June 3 17:58:08 2020
set -e

usage() {
  echo "Usage:"
  echo "  ./security_features_compilation_and_linkage_flags.sh -t <target> -a <arch> [-b] [-p <workpath> | -s <hmsdksysrootpath>] -m <method>"
  echo ""
  echo "  This script generates the compilation options of the target."
  echo "  If the SDK is used for compilation, use the -s parameter to specify the sdksysroot path."
  echo "  If you use the yocto tool for compilation, use the -p parameter to specify the yocto tool directory of the current module."
  echo "  List of targets: kcfi, shadow-stack"
  echo "  List of archs: arm, aarch64"
  echo "  List of methods: reg, mem"
}

GCCVERSION=""
ARCH=""
BE="n"
WORKPATH=""
HMSDKSYSROOT=""
METHOD=""
HMSDKINCLUDE=""
ARCH_TARGET=""
SECURITY_PLUGIN_DIR=""
SECURITY_SHADOW_STACK_FIXED_X18="-ffixed-x18"
SECURITY_SHADOW_STACK="-fplugin=libshadow_stack"
SECURITY_SHADOW_STACK_METHOD_REG="-fplugin-arg-libshadow_stack-method=reg"
SECURITY_SHADOW_STACK_METHOD_MEM="-fplugin-arg-libshadow_stack-method=mem"
SECURITY_SHADOW_STACK_OFFSET=""
SECURITY_KCFI_CFLAGS="-fplugin=libkcfi.so"

while getopts "t:a:bp:s:m:" opt
do
    case "${opt}" in
        t) TARGET="$OPTARG";;
        a) ARCH="$OPTARG";;
        b) BE="y";;
        p) WORKPATH="$OPTARG";;
        s) HMSDKSYSROOT="$OPTARG";;
        m) METHOD="$OPTARG";;
        ?) usage;;
    esac
done

if [ "${ARCH}x" == "aarch64x" -a "${BE}x" == "yx" ]; then
    ARCH_TARGET="aarch64_be-euler-elf"
elif [ "${ARCH}x" == "aarch64x" -a "${BE}x" == "nx" ]; then
    ARCH_TARGET="aarch64-euler-elf"
elif [ "${ARCH}x" == "armx" -a "${BE}x" == "yx" ]; then
    ARCH_TARGET="armeb-euler-eabi"
elif [ "${ARCH}x" == "armx" -a "${BE}x" == "nx" ]; then
    ARCH_TARGET="arm-euler-eabi"
else
    usage
    exit 1
fi

if [ ! -z "${WORKPATH}" -a -d "${WORKPATH}" ]; then
    GCCVERSION=$(${WORKPATH}/recipe-sysroot-native/usr/bin/${ARCH_TARGET}-gcc --version | sed -ne 's:.*gcc (.*) \([0-9\.]*\).*:\1:p;')
    HMSDKINCLUDE="${WORKPATH}/recipe-sysroot/usr/include"
    HMSDKPLUGINDIR="${WORKPATH}/recipe-sysroot-native/usr/lib/gcc/${ARCH_TARGET}/${GCCVERSION}/plugin"
elif [ ! -z "${HMSDKSYSROOT}" -a -d "${HMSDKSYSROOT}" ]; then
    GCCVERSION=$(${HMSDKSYSROOT}/x86_64-eulersdk-linux/usr/bin/${ARCH_TARGET}-gcc --version | sed -ne 's:.*gcc (.*) \([0-9\.]*\).*:\1:p;')
    HMSDKINCLUDE="${HMSDKSYSROOT}/${ARCH_TARGET}/usr/include"
    HMSDKPLUGINDIR="${HMSDKSYSROOT}/x86_64-eulersdk-linux/usr/lib/gcc/${ARCH_TARGET}/${GCCVERSION}/plugin"
else
    echo "Error: please input valid yocto work path or hongmeng sdk sysroot path"
    exit 1
fi

SECURITY_PLUGIN_DIR="-plugindir=${HMSDKPLUGINDIR}"

if [ "${TARGET}x" = "kcfix" ]; then
    echo "${SECURITY_PLUGIN_DIR} ${SECURITY_KCFI_CFLAGS}"
elif [ "${TARGET}x" = "shadow-stackx" ]; then
    if [ "${METHOD}x" = "regx" ]; then
        echo "${SECURITY_PLUGIN_DIR} ${SECURITY_SHADOW_STACK} ${SECURITY_SHADOW_STACK_METHOD_REG} ${SECURITY_SHADOW_STACK_FIXED_X18}"
    elif [ "${METHOD}x" = "memx" ]; then
        OFFSETHEADER="${HMSDKINCLUDE}/shadow_stack_offset.h"
        if [ -f "${OFFSETHEADER}" ]; then
            offset=$(sed 's:^#define \([A-Z_]\+\) \([0-9]\+\) .*:\2:' ${OFFSETHEADER})
            SECURITY_SHADOW_STACK_OFFSET="-fplugin-arg-libshadow_stack-offset=${offset}"
            echo "${SECURITY_PLUGIN_DIR} ${SECURITY_SHADOW_STACK} ${SECURITY_SHADOW_STACK_METHOD_MEM} ${SECURITY_SHADOW_STACK_OFFSET}"
        else
            echo "Error: not find shadow stack offset header file."
            exit 1
        fi
    else
        echo "Error: -m <method> is mandatory. List of methods: reg, mem"
        exit 1
    fi
else
    echo "Error: invalid target. List of targets: kcfi, shadow-stack"
    exit 1
fi
