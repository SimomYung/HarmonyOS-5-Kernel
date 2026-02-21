# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: CMakeLists for builtin tests
# Author: Huawei OS Kernel Lab
# Create: Wed Sep 22 16:29:20 2021

ADD_TEST_MACRO(others.builtin.exec_use_whole
              COMMAND "${CMAKE_NM_COMMAND} -a test.elf")
set_property(TEST "others.builtin.exec_use_whole"
             APPEND PROPERTY
             PASS_REGULAR_EXPRESSION "T fooa")
set_property(TEST "others.builtin.exec_use_whole"
             APPEND PROPERTY
             PASS_REGULAR_EXPRESSION "T foob")

ADD_TEST_MACRO(others.builtin.dynamiclib_use_whole
              COMMAND "${CMAKE_NM_COMMAND} -a libtest.unstripped.so")
set_property(TEST "others.builtin.dynamiclib_use_whole"
             APPEND PROPERTY
             PASS_REGULAR_EXPRESSION "T fooa")
set_property(TEST "others.builtin.dynamiclib_use_whole"
             APPEND PROPERTY
             PASS_REGULAR_EXPRESSION "T foob")

ADD_TEST_MACRO(others.builtin.lib_same_src_name
             ALIAS others.builtin.lib_same_src_name.fooa
             COMMAND "${CMAKE_NM_COMMAND} -a libsame_src_name_test.a")
set_tests_properties("others.builtin.lib_same_src_name.fooa" PROPERTIES
             PASS_REGULAR_EXPRESSION "T fooa")

ADD_TEST_MACRO(others.builtin.lib_same_src_name
             ALIAS others.builtin.lib_same_src_name.foob
             COMMAND "${CMAKE_NM_COMMAND} -a libsame_src_name_test.a")
set_tests_properties("others.builtin.lib_same_src_name.foob" PROPERTIES
             PASS_REGULAR_EXPRESSION "T foob")

ADD_TEST_MACRO(others.builtin.builtin_depends
    COMMAND "${CMAKE_CAT_COMMAND} CMakeFiles/builtin.dev.dir/build.make")
set_property(TEST "others.builtin.builtin_depends"
             APPEND PROPERTY
             PASS_REGULAR_EXPRESSION "libbuiltin.dev.a: subdir/libbuiltin.dev.a")
