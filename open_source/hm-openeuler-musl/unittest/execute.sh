#!/usr/bin/env bash
# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: The unittest execute shell script
# Author: Huawei OS Kernel Lab
# Create: Fri Jan 29 10:04:40 2021

set -e

QEMU_ARMEB="qemu-armeb"
UTDIR=$(cd "$(dirname $0)";pwd)

BUILD_DIR=${UTDIR}/build
LCOV_DIR=${UTDIR}/lcov_out

if [ ! -d "${LCOV_DIR}" ];then
	mkdir -p ${LCOV_DIR}
fi

TARGET_NAME="hmut_musl_string.elf"
for target in ${TARGET_NAME}
do
	${QEMU_ARMEB} $BUILD_DIR/${target}
done | tee result.txt

#Gen coverage info
lcov --capture --directory ${BUILD_DIR} --output-file ${LCOV_DIR}/coverage.info
lcov -l ${LCOV_DIR}/coverage.info
genhtml ${LCOV_DIR}/coverage.info --output-directory ${LCOV_DIR}
