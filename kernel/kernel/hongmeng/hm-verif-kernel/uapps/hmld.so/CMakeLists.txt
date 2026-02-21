# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: CMakeLists.txt for hmld.so
# Author: Huawei OS Kernel Lab
# Create: Tue Feb 08 16:04:58 2022

cmake_minimum_required(VERSION 3.16)
project(HMLDSO LANGUAGES NONE)
include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/PreHmbuild)
hmbuild_config_target(hmld.so.elf PROPERTY HMBUILD_LINK_WHOLE_BUILTIN  TRUE)

if (CONFIG_ARM)
    set(ARCH arm)
    set(HOSTCFLAG -D__arm__)
elseif(CONFIG_AARCH64)
    set(ARCH aarch64)
    set(HOSTCFLAG -D__aarch64__)
else()
    message(FATAL_ERROR "Only support aarch64 and arm")
endif()
set(HOSTCC gcc)
set(HM_VERIFY_KERNEL ${CMAKE_CURRENT_SOURCE_DIR}/../..)
# generate <BINARY_DIR>/build/syscall/<ARCH>/cap_numbers.h
# syscall.S needs it
execute_process(COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/syscall/${ARCH})
execute_process(
    COMMAND ${HOSTCC} ${CMAKE_CURRENT_SOURCE_DIR}/syscall/${ARCH}/gen_cap_numbers.c
            -o ${CMAKE_CURRENT_BINARY_DIR}/syscall/${ARCH}/gen_cap_numbers
            -I ${HM_VERIFY_KERNEL}/kernel/include/mapi/uapi
            ${HOSTCFLAG}
)
execute_process(
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/syscall/${ARCH}/gen_cap_numbers
    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/syscall/${ARCH}/cap_numbers.h
)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/syscall/${ARCH})
include(HongmengCMakeNG/HMbuild)

# vim:ts=4:sw=4:expandtab
