# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Mon Oct 11 20:32:13 2021

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(test_exec_use_builtin_whole)

include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/PreHmbuild)

hmbuild_config_target(test.elf
  PROPERTY HMBUILD_LINK_WHOLE_BUILTIN TRUE)

include(HongmengCMakeNG/HMbuild)
