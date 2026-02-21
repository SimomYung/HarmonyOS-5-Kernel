# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: libversion.cmake
# Author: Huawei OS Kernel Lab
# Create: Tue Oct 26 04:07:21 2021

ADD_TEST_MACRO(cmd.installinc.subdirContainHypen)
set_property(TEST "cmd.installinc.subdirContainHypen" APPEND PROPERTY
             PASS_REGULAR_EXPRESSION "installinc:.*install\.h.*subdir=\(test-a\)")
