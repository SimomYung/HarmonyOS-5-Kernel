# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Mon Oct 11 20:32:13 2021

cmake_minimum_required(VERSION 3.16)

project(test_kconfig_suffix)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/defconfig
        ${CMAKE_CURRENT_BINARY_DIR}/.config
        COPYONLY
)

include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/HMbuild)
