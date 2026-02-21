#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2024-2033. All rights reserved.

set -e

source $TOPDIR/scripts/common.sh

compile_securec() {
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(${TOOLCHAIN_CC} -print-file-name=plugin)
	cd $TOPDIR/platform/hw-securec/build
	ln -s $TOPDIR/output/aarch64/usr/lib/libc.a  $TOPDIR/platform/hw-securec/build/libc.a
        ln -s $TOPDIR/output/aarch64/usr/lib/libc.so $TOPDIR/platform/hw-securec/build/libc.so
	make -C ../ O=$TOPDIR/platform/hw-securec/build CFLAGS_USR="          -D__hmspif_enable__      " LDFLAGS="     -Wl,-z,relro     -Wl,-z,now     -Wl,-z,noexecstack     -Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed    -L$TOPDIR/platform/hw-securec/build"
	make -C ../ O=$TOPDIR/platform/hw-securec/build CFLAGS_USR="          -D__hmspif_enable__     " install DESTDIR=$TOPDIR/output/aarch64 includedir=/usr/include libdir=/usr/lib
}

compile_ksecurec() {
	if [ "$ARCH" == "aarch64" ]; then
		ARCH=arm64
	fi

	LDK_BUILD=$(readconf liblinux.build)
	unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS MACHINE
	make ARCH=${ARCH} CROSS_COMPILE=$CROSS_COMPILE INCDIR=$PWD/include \
		M=$PWD/src -C ${HMSDKSYSROOTPATH}/lib/modules/${LDK_BUILD}/build -j 32
	mkdir -p ${HMSDKSYSROOTPATH}/lib/modules/${LDK_BUILD}/build/include/linux
	cp -pPR ./include/* ${HMSDKSYSROOTPATH}/lib/modules/${LDK_BUILD}/build/include/linux
}

compile_securec
#compile_ksecurec
