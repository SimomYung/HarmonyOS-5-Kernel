# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: CMakeLists.txt for test features
# Author: Huawei OS Kernel Lab
# Create: Thu Sep 22 15:58:35 2022

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(test_features)
include(HongmengCMakeNG/Kconfig)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstack-protector")
include(HongmengCMakeNG/HMbuild)
