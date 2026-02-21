# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: cmake for test dynamic exec
# Author: Huawei OS Kernel Lab
# Create: Mon Oct 11 20:32:13 2021

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(test_extra_ldflags LANGUAGES NONE)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/defconfig
    ${CMAKE_CURRENT_BINARY_DIR}/.config
    COPYONLY
)
include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/HMbuild)

