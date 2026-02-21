# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Thu Jan 06 02:03:25 2022

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(exclude_project_test)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/defconfig
        ${CMAKE_CURRENT_BINARY_DIR}/.config
        COPYONLY
)

include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/HMbuild)
