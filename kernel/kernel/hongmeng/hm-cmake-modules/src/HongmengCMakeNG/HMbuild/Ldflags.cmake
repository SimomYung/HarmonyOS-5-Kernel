# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Common CFLAGS settings for HMbuild
# Author: Huawei OS Kernel Lab
# Create: Thu Sep 23 20:14:13 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)

if (NOT ("${CMAKE_HONGMENG_LINKER_ID}" STREQUAL ""))
    # subdir inherit CMAKE_C_LINK_FLAGS so use a guard variable
    # to make sure it is inited only once.
    if (NOT (DEFINED __HMBUILD_LDFLAGS_INITED))
        include(HongmengCMakeNG/HMbuild/Ldflags/${CMAKE_HONGMENG_LINKER_ID})
        string(APPEND CMAKE_C_LINK_FLAGS
            " ${CONFIG_HMBUILD_EXTRA_LDFLAGS_BASE} ${CONFIG_HMBUILD_EXTRA_LDFLAGS}")
        string(APPEND CMAKE_SHARED_LINKER_FLAGS
            " ${CONFIG_HMBUILD_EXTRA_LDFLAGS_BASE} ${CONFIG_HMBUILD_EXTRA_LDFLAGS}")
        set(__HMBUILD_LDFLAGS_INITED "1")
    endif()
endif()

# vim:ts=4:sw=4:expandtab
