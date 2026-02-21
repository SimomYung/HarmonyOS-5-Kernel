# Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
# Description: Master cmake for vsync
# Author: Huawei OS Kernel Lab
# Create: Mon Aug 09 14:56:51 2021

cmake_minimum_required(VERSION 3.16)
project(vsync LANGUAGES NONE)

include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/HMbuild)

install(
    CODE "file(REMOVE_RECURSE ${PROJECT_BINARY_DIR}/install)
          file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/install/include)
          file(CREATE_LINK libvsync/vsync ${PROJECT_BINARY_DIR}/install/include/vsync SYMBOLIC)
          file(INSTALL DESTINATION ${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}
               TYPE FILE FILES ${PROJECT_BINARY_DIR}/install/include/vsync)"
    )

# vim:ts=4:sw=4:expandtab
