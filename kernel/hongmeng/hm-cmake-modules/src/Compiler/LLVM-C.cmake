# Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
# Description: LLVM-C.cmake to inherit clang-c
# Author: Huawei OS Kernel Lab
# Create: Fri Mar 24 17:02:50 2023

include(Compiler/Clang-C)

set(CMAKE_C_DEPFILE_FORMAT gcc)
set(CMAKE_C_DEPENDS_USE_COMPILER TRUE)
set(CMAKE_DEPFILE_FLAGS_C "-MD -MT <DEP_TARGET> -MF <DEP_FILE>")

# vim:ts=4:sw=4:expandtab
