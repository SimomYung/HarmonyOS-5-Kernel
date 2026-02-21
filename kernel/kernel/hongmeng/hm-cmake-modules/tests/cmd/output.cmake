# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: libversion.cmake
# Author: Huawei OS Kernel Lab
# Create: Tue Oct 26 04:07:21 2021

string(APPEND output_test_command "${CMAKE_LS_COMMAND} libtest-dynamic.so.0.1")
string(APPEND output_test_command " libtest-dynamic.unstripped.so")
string(APPEND output_test_command " libtest-static.a")
string(APPEND output_test_command " test_output")
ADD_TEST_MACRO(cmd.outputcmd.base COMMAND "${output_test_command}")

