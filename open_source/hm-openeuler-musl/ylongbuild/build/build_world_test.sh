#!/bin/bash
# Copyright Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
set -e

source /opt/buildtools/ylongc/hpe/setenv.sh "$ARCHI" hpe > /dev/null
CMAKE_BUILD_DIR="$ROOT_DIR"/"cmake_build_hpetest"

if [ -d "$CMAKE_BUILD_DIR" ]; then
    rm -rf "$CMAKE_BUILD_DIR"
fi

mkdir "$CMAKE_BUILD_DIR"
cd "$CMAKE_BUILD_DIR"

cmake -DCMAKE_INSTALL_PREFIX="$OUTPUT_DIR" -DMODULE_TYPE:STRING="$MODULE" -DCMAKE_TOOLCHAIN_FILE="$ROOT_DIR"/build/cmake/toolchain/toolchain-"$ARCHI".cmake "$ROOT_DIR"/
make -j$(getconf _NPROCESSORS_CONF) O="$BUILD_DIR" VERBOSE=1
make install -j$(getconf _NPROCESSORS_CONF)

cd ~-
