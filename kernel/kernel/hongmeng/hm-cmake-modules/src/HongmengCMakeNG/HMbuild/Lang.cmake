# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: HMbuild: set lang
# Author: Huawei OS Kernel Lab
# Create: Fri Jun 11 06:06:35 2021
# Header of HMbuild
# This is the first setup phase, should depends on nothing.

include_guard(GLOBAL)

macro(hmbuild_setup_lang)
    if (DEFINED CONFIG_HMBUILD_MAKE_HEADERS)
        set(HMBUILD_MAKE_HEADERS "Y")
    endif()

    if (DEFINED CONFIG_HMBUILD_MAKE_BINARIES)
        # HM customised compiler not link with a proper C standard library,
        # so we set CMAKE_TRY_COMPILE_TARGET_TYPE to avoid compiler check error
        set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
        enable_language(C)
        enable_language(CXX)
        enable_language(ASM)
    endif()

    get_property(HMBUILD_ENABLED_LANGUAGES_LIST GLOBAL PROPERTY ENABLED_LANGUAGES)
    if ("C" IN_LIST HMBUILD_ENABLED_LANGUAGES_LIST)
        set(HMBUILD_LANG_ENABLED_C      "TRUE")
    endif()
    if ("CXX" IN_LIST HMBUILD_ENABLED_LANGUAGES_LIST)
        set(HMBUILD_LANG_ENABLED_CXX    "TRUE")
    endif()
    if ("ASM" IN_LIST HMBUILD_ENABLED_LANGUAGES_LIST)
        set(HMBUILD_LANG_ENABLED_ASM    "TRUE")
    endif()
    if ((DEFINED CONFIG_HMBUILD_MAKE_BINARIES) AND
        ((DEFINED HMBUILD_LANG_ENABLED_C) OR
         (DEFINED HMBUILD_LANG_ENABLED_CXX) OR
         (DEFINED HMBUILD_LANG_ENABLED_ASM)))
        set(HMBUILD_MAKE_BINARIES "Y")
    endif()
endmacro(hmbuild_setup_lang)

hmbuild_setup_lang()
# vim:ts=4:sw=4:expandtab
