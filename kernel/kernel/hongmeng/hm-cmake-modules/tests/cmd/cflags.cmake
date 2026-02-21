# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: cmake for adding testcases of cmd.cflags
# Author: Huawei OS Kernel Lab
# Create: Tue Oct 26 04:07:21 2021

ADD_TEST_MACRO(cmd.cflags.target_elf COMMAND "test.elf")
set_tests_properties("cmd.cflags.target_elf" PROPERTIES
                    PASS_REGULAR_EXPRESSION "SUCCESS HAVE_FOO")

ADD_TEST_MACRO(cmd.cflags.target_dir COMMAND "test.elf")
set_tests_properties("cmd.cflags.target_dir" PROPERTIES
                    PASS_REGULAR_EXPRESSION "SUCCESS HAVE_FOO")

ADD_TEST_MACRO(cmd.cflags.target_src COMMAND "test.elf")
set_tests_properties("cmd.cflags.target_src" PROPERTIES
                    PASS_REGULAR_EXPRESSION "SUCCESS HAVE_SRC")

ADD_TEST_MACRO(cmd.cflags.target_src_regex COMMAND "test.elf")
set_tests_properties("cmd.cflags.target_src_regex" PROPERTIES
                    PASS_REGULAR_EXPRESSION "SUCCESS HAVE_FOO")

ADD_TEST_MACRO(cmd.cflags.group_cflags COMMAND "test.elf")
set_tests_properties("cmd.cflags.group_cflags" PROPERTIES
                    PASS_REGULAR_EXPRESSION "SUCCESS HAVE_DIR")
set_tests_properties("cmd.cflags.group_cflags" PROPERTIES
                    PASS_REGULAR_EXPRESSION "SUCCESS HAVE_TARGET")
set_tests_properties("cmd.cflags.group_cflags" PROPERTIES
                    PASS_REGULAR_EXPRESSION "SUCCESS HAVE_SRC")

ADD_TEST_MACRO(cmd.cflags.multi_cflags COMMAND "test.elf")
set_tests_properties("cmd.cflags.multi_cflags" PROPERTIES
                    PASS_REGULAR_EXPRESSION "SUCCESS HAVE_FOO")
set_tests_properties("cmd.cflags.multi_cflags" PROPERTIES
                    PASS_REGULAR_EXPRESSION "SUCCESS HAVE_MUTI")
