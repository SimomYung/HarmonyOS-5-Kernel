#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Fri April 10 10:42:37 2020

set -e

if [ "${EXTLIB_OUTPUT}" = "${EXTLIB_SRC}" ]; then
	find ${EXTLIB_OUTPUT} \
		   -iname "*.ko" \
		-o -iname "*.o" \
		-o -iname "*.mod.*" \
		-o -iname "*.mod" \
		-o -iname "*.symvers" \
		-o -iname "*.order" \
		-o -iname ".*.cmd" \
		-type f | xargs rm -f
elif [ -e "${EXTLIB_OUTPUT}" ]; then
	rm -r ${EXTLIB_OUTPUT}
fi
