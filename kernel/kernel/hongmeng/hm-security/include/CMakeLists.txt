# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: master cmake for hm-security-headers
# Author: Huawei OS Kernel Lab
# Create: Fri Jan 07 11:07:13 2022

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(hm-security-headers LANGUAGES NONE)

include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/HMbuild)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/../manifest
        DESTINATION ${CONFIG_HMBUILD_INSTALL_DIR_CONFDIR})
