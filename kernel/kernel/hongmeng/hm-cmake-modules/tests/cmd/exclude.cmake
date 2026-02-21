# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: cmake for adding testcases of cmd.excludecflags
# Author: Huawei OS Kernel Lab
# Create: Wed Sep 22 16:29:20 2021

# excludecflags tests
ADD_TEST_MACRO(cmd.excludecflags.exclude_builtin
               ALIAS cmd.excludecflags.exclude_builtin.noexclude
               COMMAND "test.elf")
set_tests_properties("cmd.excludecflags.exclude_builtin.noexclude" PROPERTIES
                     PASS_REGULAR_EXPRESSION "SUCCESS HAVE_FOO")

set(cmd.excludecflags.exclude_builtin.exclude_BUILD_OPTIONS "-Dexclude_cmd=excludecflags-y: -DHAVE_FOO")
ADD_TEST_MACRO(cmd.excludecflags.exclude_builtin 
               ALIAS cmd.excludecflags.exclude_builtin.exclude
               COMMAND test.elf)
set_tests_properties("cmd.excludecflags.exclude_builtin.exclude" PROPERTIES
                      PASS_REGULAR_EXPRESSION "SUCCESS NO_FOO")

set_tests_properties("cmd.excludecflags.exclude_builtin.exclude" PROPERTIES
                      PASS_REGULAR_EXPRESSION "SUCCESS HAVE_BUILTIN")

ADD_TEST_MACRO(cmd.excludecflags.exclude_static_lib
               COMMAND "${CMAKE_NM_COMMAND} -a libtest.dev.a")
set_tests_properties("cmd.excludecflags.exclude_static_lib" PROPERTIES
                     FAIL_REGULAR_EXPRESSION "T foo")

ADD_TEST_MACRO(cmd.excludecflags.exclude_dynamic_lib
               COMMAND "${CMAKE_NM_COMMAND} -a libtest.unstripped.so")
set_tests_properties("cmd.excludecflags.exclude_dynamic_lib" PROPERTIES
                     FAIL_REGULAR_EXPRESSION "T foo")

ADD_TEST_MACRO(cmd.excludecflags.exclude_elf
               ALIAS cmd.excludecflags.exclude_elf.noexclude
               COMMAND "test_have_foo.elf")
set_tests_properties("cmd.excludecflags.exclude_elf.noexclude" PROPERTIES 
                     PASS_REGULAR_EXPRESSION "SUCCESS HAVE_FOO")

ADD_TEST_MACRO(cmd.excludecflags.exclude_elf
               ALIAS cmd.excludecflags.exclude_elf.exclude
               COMMAND "test_no_foo.elf")
set_tests_properties("cmd.excludecflags.exclude_elf.exclude" PROPERTIES
                     PASS_REGULAR_EXPRESSION "SUCCESS NO_FOO")

ADD_TEST_MACRO(cmd.excludecflags.exclude_dir
               ALIAS cmd.excludecflags.exclude_dir.exclude
               COMMAND "test.elf")
set_tests_properties("cmd.excludecflags.exclude_dir.exclude" PROPERTIES
                     PASS_REGULAR_EXPRESSION "SUCCESS NO_FOO")

ADD_TEST_MACRO(cmd.excludecflags.exclude_src
               COMMAND "test.elf")
set_tests_properties("cmd.excludecflags.exclude_src" PROPERTIES
                     PASS_REGULAR_EXPRESSION "SUCCESS NO_FOO")

ADD_TEST_MACRO(cmd.excludecflags.exclude_src_regex
               COMMAND "test.elf")
set_tests_properties("cmd.excludecflags.exclude_src_regex" PROPERTIES
                     PASS_REGULAR_EXPRESSION "SUCCESS NO_FOO")

ADD_TEST_MACRO(cmd.excludecflags.exclude_multi_src
               COMMAND "test.elf")
set_tests_properties("cmd.excludecflags.exclude_multi_src" PROPERTIES
                     PASS_REGULAR_EXPRESSION "\nfooa NO_FOO\nfoob HAVE_FOO\nfooc NO_FOO\n")

ADD_TEST_MACRO(cmd.excludecflags.only_support_global
               COMMAND "test.elf")
set_tests_properties("cmd.excludecflags.only_support_global" PROPERTIES
                     PASS_REGULAR_EXPRESSION "SUCCESS HAVE_FOO")

ADD_TEST_MACRO(cmd.excludecflags.exclude_regex_target
               ALIAS cmd.excludecflags.exclude_regex_target.noexclude
               COMMAND "test_have_macro.elf")
set_tests_properties("cmd.excludecflags.exclude_regex_target.noexclude" PROPERTIES 
                     PASS_REGULAR_EXPRESSION "SUCCESS MACRO_VALUE=100")

ADD_TEST_MACRO(cmd.excludecflags.exclude_regex_target
               ALIAS cmd.excludecflags.exclude_regex_target.exclude
               COMMAND "test_no_macro.elf")
set_tests_properties("cmd.excludecflags.exclude_regex_target.exclude" PROPERTIES
                     PASS_REGULAR_EXPRESSION "SUCCESS NO_MACRO_VALUE")


ADD_TEST_MACRO(cmd.excludecflags.exclude_kasan_target)
ADD_TEST_MACRO(cmd.excludecflags.exclude_kasan_src)

ADD_TEST_MACRO(cmd.excludecflags.exclude_project
               COMMAND "test.elf")
set_tests_properties("cmd.excludecflags.exclude_project" PROPERTIES
                     PASS_REGULAR_EXPRESSION "NOT DEFINE HAVE_FOO.*NOT DEFINE HAVE_FOOB")
