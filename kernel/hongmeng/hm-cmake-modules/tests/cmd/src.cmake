# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: libversion.cmake
# Author: Huawei OS Kernel Lab
# Create: Tue Oct 26 04:07:21 2021

ADD_TEST_MACRO(cmd.src.exec COMMAND "test.elf")
set_tests_properties("cmd.src.exec" PROPERTIES
                    PASS_REGULAR_EXPRESSION "this is src main")
ADD_TEST_MACRO(cmd.src.lib)
