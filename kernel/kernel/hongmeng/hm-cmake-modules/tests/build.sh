#!/usr/bin/env bash

# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: Shell script for test
# Author: Huawei OS Kernel Lab
# Create: Wed Sep 29 10:44:39 2021

set -e

SOURCE_PATH=$(dirname $0)
TEST_DIR=$(
    cd ${SOURCE_PATH}
    pwd
)
CMAKE_MODULES_SRC_DIR="${TEST_DIR}/.."
SYSROOT_PATH="${TEST_DIR}/sysroot"
TEST_MODULE_PATH="${TEST_DIR}"

MODULE_DIR=/usr/share/cmake/Modules
EXECUTE_LOG="${TEST_DIR}/execute_log.txt"

# check the validation of #args
usage() {
    echo "Options: -d[SYSROOT, default is tests/sysroot]"
    echo "Examples:"
    echo "  ./build.sh -d /"
    exit 0
}

while getopts "d:" opt; do
    case "${opt}" in
    d)
        SYSROOT_PATH="$OPTARG"
        ;;
    ?)
        usage
        ;;
    esac
done

install_cmake_modules() {
    cd ${CMAKE_MODULES_SRC_DIR}
    rm -rf build
    mkdir build && cd build
    cmake -DMODULE_INSTALL_DIR=${MODULE_DIR} ..
    make test VERBOSE=1
    DESTDIR=${SYSROOT_PATH} cmake --install .
    INSTALL_MODULE_DIR=${SYSROOT_PATH}/${MODULE_DIR}
}

if [ ! -d ${SYSROOT_PATH} ]; then
    mkdir -p ${SYSROOT_PATH}
fi

install_cmake_modules
# before run test
export TEST_WORK_PATH=${TEST_DIR}
export TEST_SYS_ROOT_PATH=${SYSROOT_PATH}
export TEST_CMAKE_MODULE_INSTALL_PATH=${INSTALL_MODULE_DIR}
export PYTHONPATH=${TEST_MODULE_PATH}

source ${TEST_DIR}/execute.sh 2>&1 | tee ${EXECUTE_LOG}
[ $(grep -E "FAILED|uncaught|unhandled|aborting|Illegal|exception" ${EXECUTE_LOG} | wc -l) -eq 0 ] || exit 1
