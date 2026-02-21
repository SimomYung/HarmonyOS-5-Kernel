# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Master cmake for klibs
# Author: Huawei OS Kernel Lab
# Create: Mon Jul 12 14:54:52 2021

cmake_minimum_required(VERSION 3.16)
project(HMKLibs LANGUAGES NONE)
include(HongmengCMakeNG/Kconfig)
set(HMCMAKENG_HMBUILD_GLOBAL_INITED "1")
add_compile_definitions($<TARGET_PROPERTY:HMBase::Sysroot,INTERFACE_COMPILE_DEFINITIONS>)

if(CONFIG_HMPSF)
set(__HMPSF_INCLUDE_DIR ${CMAKE_SYSROOT}/usr/include/nostdinc)
endif()

set(KLIBS_GLOBAL_INCLUDE
      ${__HMPSF_INCLUDE_DIR}
      ${__HMXVM_INCLUDE_DIR}
)
include_directories(${KLIBS_GLOBAL_INCLUDE})

include(HongmengCMakeNG/HMbuild)
