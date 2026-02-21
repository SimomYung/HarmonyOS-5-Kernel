# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: CMakeLists.txt for ulibs
# Author: Huawei OS Kernel Lab
# Create: Tue Nov 02 16:44:27 2021

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(hm-network-libs LANGUAGES NONE)
include(HongmengCMakeNG/Kconfig)

if (DEFINED CONFIG_HMBUILD_MAKE_HEADERS)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/../manifest
        DESTINATION ${CONFIG_HMBUILD_INSTALL_DIR_CONFDIR}
        )
endif()

include(HongmengCMakeNG/HMbuild)
