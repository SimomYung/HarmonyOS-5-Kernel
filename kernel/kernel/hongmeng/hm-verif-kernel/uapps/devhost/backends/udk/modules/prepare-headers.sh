#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Thu Jul 04 11:12:21 2019

set -eu

HEADER_LIST=${1:-/dev/null}

if [ -e "${EXTLIB_OUTPUT}/include" ]; then
	rm -r ${EXTLIB_OUTPUT}/include
fi
mkdir -p ${EXTLIB_OUTPUT}/include

grep -vE '^(\s*$|\s*#)' ${HEADER_LIST} | while read record; do
	pair=($(echo ${record} | perl -n -e '/^(\S+)\s+(\S+)/ && print "$1 $2"'))
	mkdir -p ${EXTLIB_OUTPUT}/include/$(dirname ${pair[1]})
	if [ ${pair[0]} = "-" ]; then
		ln -sf /dev/null ${EXTLIB_OUTPUT}/include/${pair[1]}
	else
		ln -sf ${HMSDKSYSROOTPATH}/${pair[0]} ${EXTLIB_OUTPUT}/include/${pair[1]}
	fi
done
