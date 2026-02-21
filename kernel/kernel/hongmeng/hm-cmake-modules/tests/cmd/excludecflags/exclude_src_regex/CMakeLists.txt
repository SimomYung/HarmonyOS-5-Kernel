# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: CMakeLists.txt for testcase exclude_src_regex
# Author: Huawei OS Kernel Lab
# Create: Sun Jul 24 21:30:30 2022

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(exclude_src_regex_test)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/defconfig
        ${CMAKE_CURRENT_BINARY_DIR}/.config
        COPYONLY
)

include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/HMbuild)
