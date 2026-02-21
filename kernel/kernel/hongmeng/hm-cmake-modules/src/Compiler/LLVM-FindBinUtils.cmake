# Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
# Description: LLVM-FindBinutils.cmake to inherit clang-findbinutils
# Author: Huawei OS Kernel Lab
# Create: Fri Mar 24 17:02:50 2023

# These define CMAKE_${_CMAKE_PROCESSING_LANGUAGE}_COMPILER_AR and CMAKE_${_CMAKE_PROCESSING_LANGUAGE}_COMPILER_RANLIB
# We can replace it by GNU make if toolchain mix uses different tools

include(Compiler/Clang-FindBinUtils)

# vim:ts=4:sw=4:expandtab
