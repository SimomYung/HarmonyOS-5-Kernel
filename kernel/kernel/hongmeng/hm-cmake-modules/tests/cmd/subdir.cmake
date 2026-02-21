# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: Cmake for subdir cmd testing
# Author: Huawei OS Kernel Lab
# Create: Tue Oct 26 04:07:21 2021

ADD_TEST_MACRO(cmd.subdir.base 
               COMMAND "test.elf")
ADD_TEST_MACRO(cmd.subdir.multilayer
               COMMAND "test.elf")
