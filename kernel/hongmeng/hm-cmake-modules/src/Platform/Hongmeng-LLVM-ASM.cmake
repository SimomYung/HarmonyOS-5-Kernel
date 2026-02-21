# Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
# Description: Hongmeng special settings for LLVM ASM
# Author: Huawei OS Kernel Lab
# Create: Fri Mar 24 17:02:50 2023

# LLVM toolchain use clang as as frontend.
set(CMAKE_HONGMENG_ASSEMBLER_ID             "Clang")
set(CMAKE_HONGMENG_ASSEMBLER_IS_CLANG       "TRUE")

# This file is loaded by CMakeASMInformation before it setting CMAKE_ASM_COMPILE_OBJECT.
# It seems to be an API allows toolchain define its own rules.
set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM${ASM_DIALECT}_COMPILER> -D__ASSEMBLY__ <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")

# Override these settings set by Compiler/GNU.cmake: this is Platform setting of CMAKE_X_FLAGS_INIT
# Do not APPEND. Directly set them for Hongmeng.
# But keep CMAKE_ASM_FLAGS_INIT untouched because it is part of API.
set(CMAKE_ASM_FLAGS_DEBUG_INIT            " -g ")
set(CMAKE_ASM_FLAGS_MINSIZEREL_INIT       " -g ")
set(CMAKE_ASM_FLAGS_RELEASE_INIT          " -g ")
set(CMAKE_ASM_FLAGS_RELWITHDEBINFO_INIT   " -g ")

# vim:ts=4:sw=4:expandtab
