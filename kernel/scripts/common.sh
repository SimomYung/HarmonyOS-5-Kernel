#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2024-2033. All rights reserved.
set -e
mod_path() {
	if [ -d "scripts/platform/$1" ]; then
		echo "platform/$1"
	elif [ -d "scripts/open_source/$1" ]; then
		echo "open_source/$1"
	else
		echo "self_source/$1"
	fi
}

readconf() {
	set +e
	value=$(grep -e "^\s*$1=" ${TOPDIR}/${CONFIG} | grep -v "#" | awk -F\= '{printf $2}')
	set -e
	echo ${value}
}
