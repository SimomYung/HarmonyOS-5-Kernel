# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: CMakeLists for features tests
# Author: Huawei OS Kernel Lab
# Create: Thu Sep 22 16:02:16 2022

ADD_TEST_MACRO(others.features
               COMMAND "${CMAKE_CAT_COMMAND} ${CMAKE_BINARY_DIR}/tests/others/features/CMakeFiles/features.dir/flags.make")
set_property(TEST "others.features"
             APPEND PROPERTY
             PASS_REGULAR_EXPRESSION "-D__hmfeature_stackprotector__")
