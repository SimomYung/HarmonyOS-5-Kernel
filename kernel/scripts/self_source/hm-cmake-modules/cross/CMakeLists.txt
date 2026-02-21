# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Generate hongmeng toolchain cmake file with cmake
# Author: Huawei OS Kernel Lab
# Create: Tue Mar 16 22:42:59 2021

project(hmcmakeng-toolchain LANGUAGES)
cmake_minimum_required(VERSION 3.16)
include(GNUInstallDirs)

# GNU toolchain

if ("${TARGET_PREFIX}" MATCHES "^aarch64-euler-elf")
    configure_file("llvm_toolchain.cmake.in" "${TARGET_PREFIX}toolchain.cmake" @ONLY)
    add_custom_target(${TARGET_PREFIX}toolchain.cmake)
    install(FILES "${CMAKE_BINARY_DIR}/${TARGET_PREFIX}toolchain.cmake"
            DESTINATION "${HMCMAKENG_MODULE_DIR}")
endif()

if ("${TARGET_PREFIX}" MATCHES "^aarch64-hongmeng-musl")
    configure_file("toolchain.cmake.in" "${TARGET_PREFIX}toolchain.cmake" @ONLY)
    add_custom_target(${TARGET_PREFIX}toolchain.cmake)
    install(FILES "${CMAKE_BINARY_DIR}/${TARGET_PREFIX}toolchain.cmake"
            DESTINATION "${HMCMAKENG_MODULE_DIR}")
endif()

# For aarch64, iTrustee toolchain needs to be generated.
# Let package variant select toolchain
if ("${TARGET_PREFIX}" MATCHES "^aarch64")
    configure_file("itrustee-toolchain.cmake.in" "${TARGET_PREFIX}itrustee-toolchain.cmake" @ONLY)
    add_custom_target(${TARGET_PREFIX}itrustee-toolchain.cmake)
    install(FILES "${CMAKE_BINARY_DIR}/${TARGET_PREFIX}itrustee-toolchain.cmake"
            DESTINATION "${HMCMAKENG_MODULE_DIR}")
endif()

# vim:ts=4:sw=4:expandtab
