# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: libversion.cmake
# Author: Huawei OS Kernel Lab
# Create: Tue Oct 26 04:07:21 2021

ADD_TEST_MACRO(cmd.lib.dynamic.soname
               COMMAND "${CMAKE_READELF_COMMAND} -d libtest_output.unstripped.so")
set_property(TEST "cmd.lib.dynamic.soname" APPEND PROPERTY
             PASS_REGULAR_EXPRESSION "Library soname:.*\[libtest_output\.so\.2\]")
