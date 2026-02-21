# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: libversion.cmake
# Author: Huawei OS Kernel Lab
# Create: Tue Oct 26 04:07:21 2021

string(APPEND suffix_test_command "${CMAKE_LS_COMMAND} libtest-dynamic_suffix.so.0.1")
string(APPEND suffix_test_command " libtest-dynamic_suffix.unstripped.so")
string(APPEND suffix_test_command " libtest-static_suffix.a")
string(APPEND suffix_test_command " test_output_suffix")
ADD_TEST_MACRO(kconfig.suffix_test COMMAND "${suffix_test_command}")
unset(suffix_test_command)

ADD_TEST_MACRO(kconfig.source_test)

