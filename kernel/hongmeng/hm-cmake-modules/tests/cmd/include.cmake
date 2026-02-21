# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: cmake for adding testcases of cmd.include
# Author: Huawei OS Kernel Lab
# Create: Tue Oct 26 04:07:21 2021

ADD_TEST_MACRO(cmd.include.base COMMAND "${CMAKE_LS_COMMAND} libtest.so.0.1")

ADD_TEST_MACRO(cmd.include.include_project COMMAND "test.elf")
set_tests_properties("cmd.include.include_project" PROPERTIES
                     PASS_REGULAR_EXPRESSION "fooa success")

ADD_TEST_MACRO(cmd.include.include_src COMMAND "test.elf")
set_tests_properties("cmd.include.include_src" PROPERTIES
                     PASS_REGULAR_EXPRESSION "fooa success")

ADD_TEST_MACRO(cmd.include.include_src_regex COMMAND "test.elf")
set_tests_properties("cmd.include.include_src_regex" PROPERTIES
                     PASS_REGULAR_EXPRESSION "fooa success")
