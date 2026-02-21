# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: cmake for test dynamic exec
# Author: Huawei OS Kernel Lab
# Create: Mon Oct 11 20:32:13 2021

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(test_exclude_builtin)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/defconfig
        ${CMAKE_CURRENT_BINARY_DIR}/.config
        COPYONLY
)

configure_file(${CMAKE_SOURCE_DIR}/HMbuild.in
               ${CMAKE_SOURCE_DIR}/HMbuild @ONLY)
include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/HMbuild)

set(hmbuild_file "${CMAKE_SOURCE_DIR}/HMbuild")
configure_file(${CMAKE_SOURCE_DIR}/clean_hm.cmake.in
               ${CMAKE_BINARY_DIR}/clean_hm.cmake @ONLY)

add_custom_target(clean_hm ALL
                  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/clean_hm.cmake
                  COMMENT "Removing HMbuild.")
