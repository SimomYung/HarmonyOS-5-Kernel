#!/bin/bash

# please use this sh copy from src code to build dir
# example: ./copy_src.sh $path
# Copyright (c) Huawei Technologies Co., Ltd. 2024-2033. All rights reserved.

set -e

export TOPDIR=$(cd $(dirname $0); pwd)
export SRC_DIR=$1

echo "current dir = $TOPDIR"
echo "src_dir = $SRC_DIR"

HMKERNEL_SRC_DIR="$SRC_DIR/kernel/hongmeng"

rm -rf $TOPDIR/self_source
mkdir -p $TOPDIR/self_source

modules=(hm-cmake-modules hm-compatible-headers hm-libvsync)
for mod in ${modules[@]}; do
	if [ -e "$TOPDIR/self_source/$mod" ];then
		rm -rf $TOPDIR/self_source/$mod
	fi
	cp -rf $HMKERNEL_SRC_DIR/$mod $TOPDIR/self_source/$mod
done


if [ -e "$TOPDIR/self_source/hm-filesystems-libs" ];then
	rm -rf $TOPDIR/self_source/hm-filesystems-libs
fi
cp -rf $HMKERNEL_SRC_DIR/hm-filesystems $TOPDIR/self_source/hm-filesystems-libs

if [ -e "$TOPDIR/self_source/hm-network-libs" ];then
	rm -rf $TOPDIR/self_source/hm-network-libs
fi
cp -rf $HMKERNEL_SRC_DIR/hm-network $TOPDIR/self_source/hm-network-libs

if [ -e "$TOPDIR/self_source/hm-security-libs" ];then
	rm -rf $TOPDIR/self_source/hm-security-libs
fi
cp -rf $HMKERNEL_SRC_DIR/hm-security $TOPDIR/self_source/hm-security-libs


rm -rf $TOPDIR/self_source/hm-ulibs
mkdir $TOPDIR/self_source/hm-ulibs

if [ -e "$TOPDIR/self_source/hm-ulibs/devmgr" ];then
	rm -rf $TOPDIR/self_source/hm-ulibs/devmgr
fi
cp -rf $HMKERNEL_SRC_DIR/hm-verif-kernel/uapps/devmgr $TOPDIR/self_source/hm-ulibs/

hmlibs_modules=(arch include kernel klibs libs modules scripts sysmgr ulibs tools)
for mod in ${hmlibs_modules[@]}; do
	if [ -e "$TOPDIR/self_source/hm-ulibs/$mod" ];then
		rm -rf $TOPDIR/self_source/hm-ulibs/$mod
	fi
	cp -rf $HMKERNEL_SRC_DIR/hm-verif-kernel/$mod $TOPDIR/self_source/hm-ulibs/$mod
done
ln -s $TOPDIR/self_source/hm-ulibs/ulibs/arch/aarch64/ulibs $TOPDIR/self_source/hm-ulibs/arch/aarch64/ulibs
ln -s $TOPDIR/self_source/hm-ulibs/ulibs/libvdso/arch/aarch64/vdso $TOPDIR/self_source/hm-ulibs/arch/aarch64/vdso
ln -s $TOPDIR/self_source/hm-ulibs/modules $TOPDIR/self_source/hm-ulibs/kernel/modules

if [ -e "$TOPDIR/self_source/devhost" ];then
	rm -rf $TOPDIR/self_source/devhost
fi
cp -rf $HMKERNEL_SRC_DIR/hm-verif-kernel/uapps/devhost $TOPDIR/self_source/devhost

if [ -e "$TOPDIR/self_source/hmld.so" ];then
	rm -rf $TOPDIR/self_source/hmld.so
fi
cp -rf $HMKERNEL_SRC_DIR/hm-verif-kernel/uapps/hmld.so $TOPDIR/self_source/hmld.so

if [ -e "$TOPDIR/self_source/hm-ulibs/sec_headers" ];then
	rm -rf $TOPDIR/self_source/hm-ulibs/sec_headers
fi

cp -rf $TOPDIR/scripts/self_source/hm-ulibs/sec_headers $TOPDIR/self_source/hm-ulibs/

if [ -e "$TOPDIR/self_source/hm-ulibs/sysif" ];then
	rm -rf $TOPDIR/self_source/hm-ulibs/sysif
fi
cp -rf $TOPDIR/scripts/self_source/hm-ulibs/sysif $TOPDIR/self_source/hm-ulibs/sysif

if [ -e "$TOPDIR/self_source/hm-ulibs/uapps/tppmgr" ];then
	rm -rf $TOPDIR/self_source/hm-ulibs/uapps
fi
mkdir -p $TOPDIR/self_source/hm-ulibs/uapps/tppmgr
cp -rf $HMKERNEL_SRC_DIR/hm-verif-kernel/uapps/tppmgr $TOPDIR/self_source/hm-ulibs/uapps/

# 拷贝 ldk-kernel；
LTSKERNEL_SRC_DIR="$SRC_DIR/kernel/linux-5.10-lts"
mkdir -p $TOPDIR/open_source/hm-openeuler-kernel
cp -rf $LTSKERNEL_SRC_DIR/* $TOPDIR/open_source/hm-openeuler-kernel

# 创建 lcdkit 软连接
mkdir -p "${TOPDIR}"/vendor/huawei/chipset_common/devkit
ln -s ../../../../open_source/hm-openeuler-kernel/drivers/devkit/lcdkit  "${TOPDIR}"/vendor/huawei/chipset_common/devkit