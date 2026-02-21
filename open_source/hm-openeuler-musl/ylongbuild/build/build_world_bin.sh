#!/bin/bash
# Copyright Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
set -e

source /opt/buildtools/ylongc/hpe/setenv.sh "$ARCHI" hpk> /dev/null

CMAKE_BUILD_DIR="$ROOT_DIR"/"cmake_build_hlibc"

if [ -d "$CMAKE_BUILD_DIR" ]; then
    rm -rf "$CMAKE_BUILD_DIR"
fi

mkdir "$CMAKE_BUILD_DIR"
cd "$CMAKE_BUILD_DIR"

cmake -DCMAKE_INSTALL_PREFIX="$OUTPUT_DIR" -DCPU_TYPE:STRING="$CPU_TYPE" -DCMAKE_TOOLCHAIN_FILE="$ROOT_DIR"/build/cmake/toolchain/toolchain-"$ARCHI".cmake "$ROOT_DIR"/
if [ "$HLIBC_FEATURE" = 'cov' ]; then
source /mnt/hgfs/share_vmware/linux_avatar_64/HLLT_init.sh hpk
fi
make -j$(getconf _NPROCESSORS_CONF) O="$BUILD_DIR"
if [ "$HLIBC_FEATURE" = 'cov' ]; then
source /mnt/hgfs/share_vmware/linux_avatar_64/HLLT_close.sh
fi

make install -j$(getconf _NPROCESSORS_CONF)
if [ $? != 0 ]; then
    echo "${APP_NAME} Compile failure."
    exit 1
fi

if [ "$CPU_TYPE" == "hi5651" -o "$CPU_TYPE" == "qemu" ]; then
    copy_target "$ROOT_DIR"/config/runtime/hi5651/config.hpp
elif [ "$CPU_TYPE" == "hi1213" ]; then
    copy_target "$ROOT_DIR"/config/runtime/hi1213/config.hpp
fi

copy_target "$ROOT_DIR"/config/runtime/hlogd/hlog.conf
copy_target "$ROOT_DIR"/config/runtime/hlogd/hlogidstring.conf

cd ~-
