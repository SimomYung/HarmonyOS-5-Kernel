#!/usr/bin/env bash
# Make some changes to sysroot.
# Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.

# Error out on error
set -e

# Use "make V=1" to debug this script
case "${KBUILD_VERBOSE}" in
*1*)
	set -x
	;;
esac

CONFIG_ARCH="$1"
CONFIG_BIG_ENDIAN="$2"

# Feel free to change following code according to SDK improvement
case "${CONFIG_ARCH}" in
"arm")
	case "${CONFIG_BIG_ENDIAN}" in
	"y")
		tune_arch=armeb
		tune_pkgarch=armv7ab
		;;
	*)
		tune_arch=arm
		tune_pkgarch=armv7a
		;;
	esac
	abi_extension=eabi
	;;
"aarch64")
	case "${CONFIG_BIG_ENDIAN}" in
	"y")
		tune_arch=aarch64_be
		tune_pkgarch=aarch64_be
		;;
	*)
		tune_arch=aarch64
		tune_pkgarch=aarch64
		;;
	esac
	abi_extension=elf
	;;
*)
	# default aarch64 little endian
	tune_arch=aarch64
	tune_pkgarch=aarch64
	abi_extension=elf
	;;
esac

target_vendor=-euler

case "x$3" in
"xtarget")
	echo -n ${tune_arch}${target_vendor}-${abi_extension}
	;;
"xgccplugin")
	echo -n "usr/lib/gcc/${tune_arch}${target_vendor}-${abi_extension}/7.3.0/plugin"
	;;
*)
	echo -n ${tune_pkgarch}${target_vendor}-${abi_extension}
	;;
esac
