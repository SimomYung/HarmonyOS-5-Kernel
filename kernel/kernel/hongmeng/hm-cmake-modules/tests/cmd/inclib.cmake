# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: cmake for adding testcases of cmd.inclib
# Author: Huawei OS Kernel Lab
# Create: Tue Oct 26 04:07:21 2021

ADD_TEST_MACRO(cmd.inclib.isystem.isystem_src COMMAND "test.elf")
set_tests_properties("cmd.inclib.isystem.isystem_src" PROPERTIES
                     PASS_REGULAR_EXPRESSION "\ntest isystem_src success")

ADD_TEST_MACRO(cmd.inclib.isystem.isystem_src_regex COMMAND "test.elf")
set_tests_properties("cmd.inclib.isystem.isystem_src_regex" PROPERTIES
                     PASS_REGULAR_EXPRESSION "\ntest isystem_src_regex success")

ADD_TEST_MACRO(cmd.inclib.isystem.isystem_target COMMAND "test.elf")
set_tests_properties("cmd.inclib.isystem.isystem_target" PROPERTIES
                     PASS_REGULAR_EXPRESSION "\ntest isystem_target success")

ADD_TEST_MACRO(cmd.inclib.isystem.isystem_dir COMMAND "test.elf")
set_tests_properties("cmd.inclib.isystem.isystem_dir" PROPERTIES
                     PASS_REGULAR_EXPRESSION "\ntest isystem_dir success\ntest func success")
