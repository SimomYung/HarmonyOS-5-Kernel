# Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
# Description: Hongmeng toolchain interface for LLVM
# Author: Huawei OS Kernel Lab
# Create: Fri Mar 24 17:02:50 2023

set(CMAKE_HONGMENG_COMPILER_ID  "Clang")
set(CMAKE_HONGMENG_ARCHIVER_ID  "LLVM")
set(CMAKE_HONGMENG_LINKER_ID    "LLVM")

set(CMAKE_HONGMENG_COMPILER_IS_CLANG    "TRUE")
set(CMAKE_HONGMENG_ARCHIVER_IS_LLVM     "TRUE")
set(CMAKE_HONGMENG_LINKER_IS_LLVM       "TRUE")

# NOTE: -Wno will be cleaned up
set(CMAKE_HONGMENG_CLANG_C_FLAGS_WARNINGS
    " \
    -Wall -Werror -Wextra \
    -Wfloat-equal \
    -Wmissing-declarations \
    -Wmissing-format-attribute \
    -Wpointer-arith \
    -Wsign-compare \
    -Wuninitialized \
    -Wunused \
    -Wno-absolute-value \
    -Wno-address-of-packed-member \
    -Wno-array-bounds \
    -Wno-asm-operand-widths \
    -Wno-constant-conversion \
    -Wno-duplicate-decl-specifier \
    -Wno-enum-conversion \
    -Wno-format \
    -Wno-format-security \
    -Wno-ignored-attributes \
    -Wno-ignored-optimization-argument \
    -Wno-incompatible-library-redeclaration \
    -Wno-incompatible-pointer-types \
    -Wno-int-conversion \
    -Wno-logical-not-parentheses \
    -Wno-missing-field-initializers \
    -Wno-non-literal-null-conversion \
    -Wno-parentheses-equality \
    -Wno-pointer-bool-conversion \
    -Wno-self-assign \
    -Wno-sign-compare \
    -Wno-sometimes-uninitialized \
    -Wno-strncat-size \
    -Wno-switch \
    -Wno-tautological-overlap-compare \
    -Wno-tautological-pointer-compare \
    -Wno-typedef-redefinition \
    -Wno-unaligned-access \
    -Wno-uninitialized \
    -Wno-unknown-attributes \
    -Wno-unknown-pragmas \
    -Wno-unknown-warning-option \
    -Wno-unused-but-set-variable \
    -Wno-unused-command-line-argument \
    -Wno-unused-function \
    -Wno-unused-parameter \
    -Wno-unused-variable \
    -Wvla \
    -Wcast-function-type \
    -fstrict-flex-arrays=3 \
    -Wzero-length-array \
    -Wincompatible-function-pointer-types \
    ")

set(CMAKE_HONGMENG_CLANG_C_FLAGS_COMMON
    " \
    -pipe -std=gnu99 -g -O2 \
    -fno-common \
    -fsigned-char \
    -fno-emulated-tls \
    -fno-exceptions \
    ")
if ((CMAKE_C_COMPILER_TARGET MATCHES "aarch64") OR (CMAKE_C_COMPILER_TARGET MATCHES "aarch64_be"))
string(APPEND CMAKE_HONGMENG_CLANG_C_FLAGS_COMMON " -mno-outline-atomics ")
endif()

set(CMAKE_SKIP_RPATH TRUE)

# Override these settings set by Compiler/GNU.cmake: this is Platform setting of CMAKE_X_FLAGS_INIT
# Do not APPEND. Directly set them for Hongmeng.
# But keep CMAKE_C_FLAGS_INIT untouched because it is part of API.
set(CMAKE_C_FLAGS_DEBUG_INIT            "${CMAKE_HONGMENG_CLANG_C_FLAGS_COMMON} ${CMAKE_HONGMENG_CLANG_C_FLAGS_WARNINGS}")
set(CMAKE_C_FLAGS_MINSIZEREL_INIT       "${CMAKE_HONGMENG_CLANG_C_FLAGS_COMMON} ${CMAKE_HONGMENG_CLANG_C_FLAGS_WARNINGS}")
set(CMAKE_C_FLAGS_RELEASE_INIT          "${CMAKE_HONGMENG_CLANG_C_FLAGS_COMMON} ${CMAKE_HONGMENG_CLANG_C_FLAGS_WARNINGS}")
set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT   "${CMAKE_HONGMENG_CLANG_C_FLAGS_COMMON} ${CMAKE_HONGMENG_CLANG_C_FLAGS_WARNINGS}")

set(CMAKE_HONGMENG_LINKER_COMMON " --ld-path=${CMAKE_LINKER} -Wl,-plugin-opt=-emulated-tls=0 -fno-exceptions ")
set(CMAKE_HONGMENG_LINKER_SECURITY " -Wl,-z,relro,-z,now  -Wl,-z,noexecstack -Wl,--hash-style=gnu")

# executable linker flags
string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT ${CMAKE_HONGMENG_LINKER_COMMON} ${CMAKE_HONGMENG_LINKER_SECURITY})
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG_INIT ${CMAKE_HONGMENG_LINKER_COMMON} ${CMAKE_HONGMENG_LINKER_SECURITY})
string(APPEND CMAKE_EXE_LINKER_FLAGS_RELEASE_INIT ${CMAKE_HONGMENG_LINKER_COMMON} ${CMAKE_HONGMENG_LINKER_SECURITY})
string(APPEND CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO_INIT ${CMAKE_HONGMENG_LINKER_COMMON} ${CMAKE_HONGMENG_LINKER_SECURITY})

# shared lib linker flags
string(APPEND CMAKE_SHARED_LINKER_FLAGS_INIT ${CMAKE_HONGMENG_LINKER_COMMON} ${CMAKE_HONGMENG_LINKER_SECURITY})
string(APPEND CMAKE_SHARED_LINKER_FLAGS_DEBUG_INIT ${CMAKE_HONGMENG_LINKER_COMMON} ${CMAKE_HONGMENG_LINKER_SECURITY})
string(APPEND CMAKE_SHARED_LINKER_FLAGS_RELEASE_INIT ${CMAKE_HONGMENG_LINKER_COMMON} ${CMAKE_HONGMENG_LINKER_SECURITY})
string(APPEND CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO_INIT ${CMAKE_HONGMENG_LINKER_COMMON} ${CMAKE_HONGMENG_LINKER_SECURITY})

# vim:ts=4:sw=4:expandtab
