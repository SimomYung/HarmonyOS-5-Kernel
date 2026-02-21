# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Hongmeng Platform settings
# Author: Huawei OS Kernel Lab
# Create: Tue May 18 14:24:37 2021

# Commands in this file would be evaluated twice when cross compiling, so do
# not use APPEND.

include(Platform/GNU)
if(__PLATFORM_HONGMENG)
    return()
endif()
set(__PLATFORM_HONGMENG 1)

# This is usually be set in UnixPaths, but we don't need full content
# of UnixPaths.
set(UNIX 1)

set(CMAKE_C_COMPILE_OPTIONS_PIC "-fPIC")

# vim:ts=4:sw=4:expandtab
