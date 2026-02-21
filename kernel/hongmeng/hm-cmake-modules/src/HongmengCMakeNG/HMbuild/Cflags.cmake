# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Common CFLAGS settings for HMbuild
# Author: Huawei OS Kernel Lab
# Create: Sat May 29 02:50:08 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)

if (NOT ("${CMAKE_HONGMENG_COMPILER_ID}" STREQUAL ""))
    # subdir inherit CMAKE_C_FLAGS so use a guard variable
    # to make sure it is inited only once.
    if (NOT (DEFINED __HMBUILD_CFLAGS_INITED))
        include(HongmengCMakeNG/HMbuild/Cflags/${CMAKE_HONGMENG_COMPILER_ID})
        string(APPEND CMAKE_C_FLAGS
            " ${HMCMAKENG_KBUILD_CFLAGS_INCAUTOCONF} ${CONFIG_HMBUILD_EXTRA_CFLAGS_BASE} ${CONFIG_HMBUILD_EXTRA_CFLAGS}"
            )
        if (CONFIG_HMBUILD_LTO)
            string(APPEND CMAKE_C_FLAGS " -flto")
        endif()
        string(APPEND CMAKE_ASM_FLAGS 
            " ${HMCMAKENG_KBUILD_CFLAGS_INCAUTOCONF} ${CONFIG_HMBUILD_EXTRA_CFLAGS_BASE} ${CONFIG_HMBUILD_EXTRA_CFLAGS}"
            )
        if (CONFIG_HMBUILD_LTO)
            string(APPEND CMAKE_ASM_FLAGS " -flto")
        endif()
        set(__HMBUILD_CFLAGS_INITED "1")
    endif()
endif()

# vim:ts=4:sw=4:expandtab
