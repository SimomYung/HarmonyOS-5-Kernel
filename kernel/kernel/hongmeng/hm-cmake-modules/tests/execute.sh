#!/usr/bin/env bash

# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: Shell script for test
# Author: Huawei OS Kernel Lab
# Create: Wed Sep 29 10:44:39 2021

set -e

install_to_sysroot() {
    if [ -d "${TEST_WORK_PATH}/build" ]; then
        rm -rf "${TEST_WORK_PATH}/build"
    fi
    mkdir -p "${TEST_WORK_PATH}/build"

    local tmp_install_path="$2"

    cd "${TEST_WORK_PATH}/build"
    cmake -DCMAKE_MODULE_PATH=${TEST_CMAKE_MODULE_INSTALL_PATH}\
    ${TEST_WORK_PATH}/$1 \
    -DCMAKE_SYSROOT=${TEST_SYS_ROOT_PATH}\
    -Wno-dev
    make install DESTDIR="${TEST_SYS_ROOT_PATH}"
}

install_bass_to_sysroot() {
    # base need to install sysroot
    install_to_sysroot "cmake_modules/base"
}

run_one_test() {
    cd ${TEST_WORK_PATH}
    python3 "$1"
}

install_bass_to_sysroot

cd ${TEST_WORK_PATH}
# need run test tool first
python3 "${TEST_WORK_PATH}/test_tools/run_test_all.py"

# cmd test
python3 "${TEST_WORK_PATH}/cmake_modules/cmd/run_cmd_all.py"

python3 "${TEST_WORK_PATH}/cmake_modules/others/run_others_all.py"

# you can change path to yourself's code to debug
#run_one_test cmake_modules/cmd/include/test_include_base.py
