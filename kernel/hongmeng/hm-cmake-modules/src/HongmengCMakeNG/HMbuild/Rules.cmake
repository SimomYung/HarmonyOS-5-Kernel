# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild: rules for building things
# Author: Huawei OS Kernel Lab
# Create: Fri Jun 11 06:06:35 2021

include_guard(GLOBAL)

set(CMAKE_C_LINK_EXECUTABLE
        "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> \
        <LINK_LIBRARIES>")

# vim:ts=4:sw=4:expandtab
