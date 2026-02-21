#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Fri April 10 10:42:37 2020

set -e

# hack for aarch64 -> arm64
if [ "${ARCH}" = "aarch64" ]; then
	export ARCH="arm64"
fi

# copy module sources to output dir
if [ "${EXTLIB_OUTPUT}" != "${EXTLIB_SRC}" ]; then
	if [ ! -d "${EXTLIB_OUTPUT}" ]; then
		if [ -e "${EXTLIB_OUTPUT}" ]; then
			rm -r ${EXTLIB_OUTPUT}
		fi
		mkdir ${EXTLIB_OUTPUT}
	fi
	cp --preserve -RL ${EXTLIB_SRC}/* ${EXTLIB_OUTPUT}/
fi

# make modules
${MAKE} -C ${LIBLINUX_KDIR}/${LIBLINUX_VERSION}/build \
	M=${EXTLIB_OUTPUT} -j

if [ "${CONFIG_STRIP_OPTION}" != "" ]; then
	for f in $(find ${EXTLIB_OUTPUT} -type f -name "*.ko"); do
		${STRIP} ${CONFIG_STRIP_OPTION} ${f}
	done
fi
