# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: LLVM Clang CFLAGS settings for HMbuild
# Author: Huawei OS Kernel Lab
# Create: Sat May 29 02:50:08 2021

# See Platform/Hongmeng-xxx-C. Most of required CFLAGS is consered as
# Hongmeng Platform level requirement, not only HMbuild level configuration.

string(APPEND CMAKE_C_FLAGS " ${CMAKE_C_FLAGS_WARNINGS}")

if(CONFIG_HMBUILD_LLVM_RETURN_GUARD)
    string(APPEND CMAKE_C_FLAGS " -fstack-protector-ret-strong ")
elseif(CONFIG_HMBUILD_LLVM_RETURN_GUARD_ALL)
    string(APPEND CMAKE_C_FLAGS " -fstack-protector-ret-all ")
else() # No option or CONFIG_HMBUILD_LLVM_RETURN_GUARD_DISABLE
    string(APPEND CMAKE_C_FLAGS " -fstack-protector-strong ")
endif()

# vim:ts=4:sw=4:expandtab
