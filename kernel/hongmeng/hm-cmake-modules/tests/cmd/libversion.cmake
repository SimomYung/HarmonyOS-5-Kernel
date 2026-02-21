# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: libversion.cmake
# Author: Huawei OS Kernel Lab
# Create: Tue Oct 26 04:07:21 2021

ADD_TEST_MACRO(cmd.libversion.base 
               COMMAND "${CMAKE_LS_COMMAND} libtest.so.2.2")
