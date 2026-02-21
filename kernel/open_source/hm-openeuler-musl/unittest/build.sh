#!/usr/bin/env bash
# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: The unittest build shell script
# Author: Huawei OS Kernel Lab
# Create: Fri Jan 29 10:04:40 2021

set -e

# Description:
# 1. This is a shell to automatically build ut testcase and its dependence
#    to executable file(such as hmut_musl_string.elf)
# 2. At present, this script only supports the build of the armbe platform.

# default: armeb
ARCH=arm
BE=y
SUBARCH=be
ARCH_NAME=armeb
CROSS_COMPILE=armeb-eabi-
CC=${CROSS_COMPILE}gcc

SRCDIR=$(cd "$(dirname $0)";pwd)
HM_LIBC=$(cd "${SRCDIR}/.."; pwd)
HMUT_BUILD_OUTPUT=build
HMLIBC_BUILD_OUTPUT=build/hm-libc

CMOCKA_DIR=${SRCDIR}/cmocka/${ARCH_NAME}
CMOCKA_LIB_DIR=${CMOCKA_DIR}/usr/lib
CMOCKA_TAR="cmocka.tar.gz"
CMOCKA_LIB="libcmocka-static.a"

HM_SERVER="http://kernel.archive.rnd.huawei.com/archive/distfiles"
HMUT_SERVER=""${HM_SERVER}"/cmocka"
HMSDK_SERVER="http://kernel.archive.rnd.huawei.com/archive/hongmengsdk"

# get libcmocka-static.a
cd ${SRCDIR}
if [ ! -d "${CMOCKA_LIB_DIR}" ];then
	mkdir -p ${CMOCKA_LIB_DIR}
fi
if [ ! -f "${CMOCKA_LIB_DIR}/${CMOCKA_LIB}" ];then
	wget -O ./${CMOCKA_TAR} ${HMUT_SERVER}/${CMOCKA_TAR} --no-proxy --no-check-certificate
	tar -xvf ./${CMOCKA_TAR}
fi

# mkdir build directories
if [ ! -d "${HMUT_BUILD_OUTPUT}" ];then
	mkdir -p ${HMUT_BUILD_OUTPUT}
fi
if [ ! -d "${HMLIBC_BUILD_OUTPUT}" ];then
	mkdir -p ${HMLIBC_BUILD_OUTPUT}
fi

# HMSDK
HMSDKVERSION="latest"
hmsysroot=$(curl -s -k -L "${HMSDK_SERVER}"/hmsdk-sysroot.sh -o - | bash -s "${ARCH}" "${BE}" "sysroot")
hmtarget=$(curl -s -k -L "${HMSDK_SERVER}"/hmsdk-sysroot.sh -o - | bash -s "${ARCH}" "${BE}" "target")

cd ${SRCDIR}/${HMUT_BUILD_OUTPUT}
# not empty for CI SDK, empty for local downloading SDK
if [ "${_HMSDKSYSROOTPATH}" ]; then
	echo "existed sdk"
	HMSYSROOT_ABS_PATH=${_HMSDKSYSROOTPATH}
else
	if [ ! -d "./.SDK/sysroots/${hmsysroot}" ]; then
		echo "downloading sdk"
		echo "hmsysroot is ${hmsysroot}"
		echo "hmtarget is ${hmtarget}"
# HMLIBC_BUILD_OUTPUT=build/hm-libc
		(curl -s -k -L "${HMSDK_SERVER}"/hmsdk-prepare.sh -o - | bash -s "${HMSDKVERSION}" "${hmtarget}" "./.SDK/sysroots/${hmsysroot}") || true
	fi
	HMSYSROOT_ABS_PATH=${SRCDIR}/${HMUT_BUILD_OUTPUT}/.SDK/sysroots/${hmsysroot}
fi

# ut_sysycall hm-libc
cd ${SRCDIR}/${HMLIBC_BUILD_OUTPUT}
make -C ${HM_LIBC}/hm UT=1 ARCH=arm SUBARCH=be V=1 OUTPUT=$(pwd) HMSDKSYSROOTPATH=${HMSYSROOT_ABS_PATH} install-headers -j

cd ${SRCDIR}/${HMUT_BUILD_OUTPUT}
LDFLAGS=""
cmake	-DARCH=${ARCH} \
	-DSUBARCH=${SUBARCH} \
	-DCMAKE_SYSTEM_PROCESSOR=${ARCH} \
	-DCMAKE_C_COMPILER=${CC} \
	-DCROSS_COMPILE=${CROSS_COMPILE} \
	-DCMAKE_EXE_LINKER_FLAGS="-nostdlib" \
	-DCMAKE_BUILD_DIR=${HMUT_BUILD_OUTPUT} \
	-DHMLIBC_BUILD_DIR=${HMLIBC_BUILD_OUTPUT} \
	-DUNIX=1 \
	-DCMAKE_MODULE_PATH=${HMSYSROOT_ABS_PATH}/usr/share/cmake/Modules \
	-DHMSDK=${HMSYSROOT_ABS_PATH} \
	-DHMSDKLIB=${HMSYSROOT_ABS_PATH}/usr/lib \
	-DHMSDKINCLUDE=${HMSYSROOT_ABS_PATH}/usr/include \
	-DUTCMOCKALIB=${CMOCKA_DIR}/usr/lib \
	-DUTCMOCKAINCLUDE=${CMOCKA_DIR}/usr/include \
	${SRCDIR};

make VERBOSE=1 -j

echo "Build Success."
