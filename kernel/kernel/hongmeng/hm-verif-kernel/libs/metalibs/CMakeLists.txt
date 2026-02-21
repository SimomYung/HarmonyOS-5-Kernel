# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: CMakeLists.txt for metalibs
# Author: Huawei OS Kernel Lab
# Create: Mon May 23 16:06:31 2022

cmake_minimum_required(VERSION 3.16)
project(metalibs LANGUAGES NONE)
include(HongmengCMakeNG/Kconfig)

set(FIND_INCLUDE_PATH "${CMAKE_SYSROOT}/usr/${CONFIG_HMBUILD_FINDPATH_INCLUDE}")

if (DEFINED CONFIG_HMBUILD_MAKE_BINARIES)
    set(CAP_INCLUDE_PATH "${FIND_INCLUDE_PATH}/generated")
    include_directories(BEFORE ${CAP_INCLUDE_PATH})
endif()

if (DEFINED CONFIG_HMBUILD_MAKE_HEADERS)
    if (DEFINED CONFIG_AARCH64)
        set(ARCH "aarch64")
    elseif (DEFINED CONFIG_ARM)
        set(ARCH "arm")
    endif()

    set(HOSTCC gcc)
    set(CREATE_CAP_COMMAND "${HOSTCC} ${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/gen_cap_numbers.c -o ${CMAKE_CURRENT_BINARY_DIR}/gen_cap_numbers -I${FIND_INCLUDE_PATH} -D__${ARCH}__ -Wno-builtin-declaration-mismatch")
    set(CREATE_HEADER_FILE "${CMAKE_CURRENT_BINARY_DIR}/gen_cap_numbers > ${CMAKE_CURRENT_BINARY_DIR}/cap_numbers.h")

    execute_process(
        OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/gen_cap_numbers
        COMMAND sh -c "${CREATE_CAP_COMMAND}"
        )

    execute_process(
        OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/cap_numbers.h
        COMMAND sh -c "${CREATE_HEADER_FILE}"
        )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cap_numbers.h
        DESTINATION "${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/generated/"
        COMPONENT "headers"
        )
endif()

include(HongmengCMakeNG/HMbuild)
