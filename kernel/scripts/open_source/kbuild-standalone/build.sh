#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2024-2033. All rights reserved.

set -e
set -x

source $TOPDIR/scripts/common.sh

pushd $TOPDIR/open_source/build_tools

mkdir $TOPDIR/kbuild-standalone_build
cd $TOPDIR/kbuild-standalone_build

make -C $TOPDIR/open_source/build_tools/kbuild-standalone/ O=$(pwd) -j 32
mkdir $TOPDIR/output/tools/usr/bin
cp ./kconfig/conf $TOPDIR/output/tools/usr/bin/kbuild-conf
cp ./kconfig/mconf $TOPDIR/output/tools/usr/bin/kbuild-mconf

rm -rf $TOPDIR/kbuild-standalone_build
