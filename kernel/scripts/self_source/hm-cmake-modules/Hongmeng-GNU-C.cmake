# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Hongmeng toolchain interface for GNU
# Author: Huawei OS Kernel Lab
# Create: Mon Aug 02 17:43:12 2021

set(CMAKE_HONGMENG_COMPILER_ID  "GNU")
set(CMAKE_HONGMENG_ARCHIVER_ID  "GNU")
set(CMAKE_HONGMENG_LINKER_ID    "GNU")

set(CMAKE_HONGMENG_COMPILER_IS_GNU     "TRUE")
set(CMAKE_HONGMENG_ARCHIVER_IS_GNU     "TRUE")
set(CMAKE_HONGMENG_LINKER_IS_GNU       "TRUE")

set(CMAKE_HONGMENG_GCC_C_FLAGS_WARNINGS
    "-Wall -Werror -Wextra -Wunused -Wpointer-arith -Wmissing-declarations \
     -Wmissing-format-attribute -Wsign-compare -Wunused-but-set-variable \
     -Wuninitialized \
     -Wfloat-equal \
     -Wdate-time \
     -Werror-implicit-function-declaration \
     -Wframe-larger-than=8192 \
     -Wmissing-prototypes \
     -Wold-style-definition \
     -Wstrict-prototypes \
     -Wswitch-default \
     -Wtrampolines \
     -Wundef \
     -Wvla \
     -Wno-unused-function \
     -Wno-int-conversion \
     -Wno-incompatible-pointer-types \
     -Wno-unused-parameter \
     -Wno-unused-variable \
     -Wno-missing-prototypes \
     -Wno-missing-declarations \
     -Wno-discarded-qualifiers \
     -Wno-format \
     ")

set(CMAKE_HONGMENG_CLANG_C_FLAGS_COMMON
    " \
    -pipe -std=gnu99 -g -O2 \
    -fno-common \
    -fstack-protector-strong \
    ")

set(CMAKE_SKIP_RPATH TRUE)

# Override these settings set by Compiler/GNU.cmake: this is Platform setting of CMAKE_X_FLAGS_INIT
# Do not APPEND. Directly set them for Hongmeng.
# But keep CMAKE_C_FLAGS_INIT untouched because it is part of API.
set(CMAKE_C_FLAGS_DEBUG_INIT            " ${CMAKE_HONGMENG_CLANG_C_FLAGS_COMMON} ${CMAKE_HONGMENG_GCC_C_FLAGS_WARNINGS}")
set(CMAKE_C_FLAGS_MINSIZEREL_INIT       " ${CMAKE_HONGMENG_CLANG_C_FLAGS_COMMON} ${CMAKE_HONGMENG_GCC_C_FLAGS_WARNINGS}")
set(CMAKE_C_FLAGS_RELEASE_INIT          " ${CMAKE_HONGMENG_CLANG_C_FLAGS_COMMON} ${CMAKE_HONGMENG_GCC_C_FLAGS_WARNINGS}")
set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT   " ${CMAKE_HONGMENG_CLANG_C_FLAGS_COMMON} ${CMAKE_HONGMENG_GCC_C_FLAGS_WARNINGS}")

set(CMAKE_HONGMENG_LINKER_COMMON " ")
set(CMAKE_HONGMENG_GCC_LINKER_SECURITY " -Wl,-z,relro,-z,now  -Wl,-z,noexecstack -Wl,--hash-style=gnu")

# executable linker flags
string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT ${CMAKE_HONGMENG_LINKER_COMMON} ${CMAKE_HONGMENG_GCC_LINKER_SECURITY})
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG_INIT ${CMAKE_HONGMENG_LINKER_COMMON} ${CMAKE_HONGMENG_GCC_LINKER_SECURITY})
string(APPEND CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO_INIT ${CMAKE_HONGMENG_LINKER_COMMON} ${CMAKE_HONGMENG_GCC_LINKER_SECURITY})

# shared lib linker flags
string(APPEND CMAKE_SHARED_LINKER_FLAGS_INIT ${CMAKE_HONGMENG_LINKER_COMMON} ${CMAKE_HONGMENG_GCC_LINKER_SECURITY})
string(APPEND CMAKE_SHARED_LINKER_FLAGS_DEBUG_INIT ${CMAKE_HONGMENG_LINKER_COMMON} ${CMAKE_HONGMENG_GCC_LINKER_SECURITY})
string(APPEND CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO_INIT ${CMAKE_HONGMENG_LINKER_COMMON} ${CMAKE_HONGMENG_GCC_LINKER_SECURITY})

# vim:ts=4:sw=4:expandtab
