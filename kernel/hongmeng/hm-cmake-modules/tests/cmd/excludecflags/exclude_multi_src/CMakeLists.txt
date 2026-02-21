# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: CMakeLists.txt for testcase exclude_multi_src
# Author: Huawei OS Kernel Lab
# Create: Mon Jul 25 12:50:49 2022

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(exclude_multi_src_test)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/defconfig
        ${CMAKE_CURRENT_BINARY_DIR}/.config
        COPYONLY
)

include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/HMbuild)
