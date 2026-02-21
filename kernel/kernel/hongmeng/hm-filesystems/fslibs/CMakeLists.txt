# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: cmake for fslibs
# Author: Huawei OS Kernel Lab
# Create: Mon Oct 11 20:32:13 2021

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(HMFsLibs LANGUAGES NONE)
include(HongmengCMakeNG/Kconfig)

# HMbuild not support etc install, we add manifest install command with headers
if (DEFINED CONFIG_HMBUILD_MAKE_HEADERS)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/../manifest
        COMPONENT config_install
        DESTINATION "${CONFIG_HMBUILD_INSTALL_DIR_CONFDIR}")

    set(our_auto_h "${CMAKE_BINARY_DIR}/include/generated/autoconf.h")

    install(FILES "${our_auto_h}"
        RENAME "autoconf_fslibs.h"
        COMPONENT "headers"
        DESTINATION "${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}")
endif()

# fslibs common include
set(SYSROOT_USR_INCLUDE ${CMAKE_SYSROOT}/usr/include)
set(SDK_SYSIF_INCLUDE ${SYSROOT_USR_INCLUDE}/generated/sysif_client/sysmgr
  ${PROJECT_BINARY_DIR}/include/generated
  ${SYSROOT_USR_INCLUDE}/generated/sysif_client/fs
  ${SYSROOT_USR_INCLUDE}/generated/sysif_client/devhost
  ${SYSROOT_USR_INCLUDE}/generated/sysif_client/devmgr
  ${SYSROOT_USR_INCLUDE}/generated/sysif_server/fs
  ${SYSROOT_USR_INCLUDE}/generated/sysif_base
)
include_directories(BEFORE ${SDK_SYSIF_INCLUDE})

include(HongmengCMakeNG/HMbuild)
