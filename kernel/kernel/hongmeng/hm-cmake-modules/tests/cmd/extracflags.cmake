# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: extracflags.cmake
# Author: Huawei OS Kernel Lab
# Create: Tue Oct 26 04:07:21 2021

ADD_TEST_MACRO(cmd.extracflags COMMAND "test.elf" BUILD_ONE)
set_tests_properties("cmd.extracflags" PROPERTIES
                    PASS_REGULAR_EXPRESSION "SUCCESS HAVE_FLAGS_FOO")

ADD_TEST_MACRO(cmd.extraldflags.dynamiclib
               COMMAND "${CMAKE_NM_COMMAND} -a libtest.unstripped.so"
               BUILD_ONE)
set_tests_properties("cmd.extraldflags.dynamiclib" PROPERTIES
                     FAIL_REGULAR_EXPRESSION "GLIBC")

ADD_TEST_MACRO(cmd.extraldflags.exec
               ALIAS cmd.extraldflags.exec.nostidlib
               BUILD_ONE)
set_tests_properties("cmd.extraldflags.exec.nostidlib" PROPERTIES WILL_FAIL true)

