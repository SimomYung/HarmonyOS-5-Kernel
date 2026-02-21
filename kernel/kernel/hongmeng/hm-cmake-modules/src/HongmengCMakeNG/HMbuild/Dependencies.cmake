# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild: Find basic dependencies
# Author: Huawei OS Kernel Lab
# Create: Tue Jun 01 11:37:19 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)

# This is the 3rd setup. It knows language enabled, features enabled/disabled.
macro(hmbuild_setup_find_package)
    if (DEFINED CONFIG_HMBUILD_FINDPATH_INCLUDE)
        set(HMFIND_PATH_INCLUDE "${CONFIG_HMBUILD_FINDPATH_INCLUDE}")
    else()
        set(HMFIND_PATH_INCLUDE "include")
        hmcmakeng_warn("CONFIG_HMBUILD_FINDPATH_INCLUDE not provided, use default 'include'")
    endif()

    if (DEFINED CONFIG_HMBUILD_FINDPATH_LIB)
        set(HMFIND_PATH_LIB "${CONFIG_HMBUILD_FINDPATH_LIB}")
    else()
        set(HMFIND_PATH_LIB "lib")
        hmcmakeng_warn("CONFIG_HMBUILD_FINDPATH_LIB not provided, use default 'lib'")
    endif()
endmacro(hmbuild_setup_find_package)

macro(hmbuild_setup_deps_base)
    if (NOT DEFINED HMCMAKENG_HMBUILD_DEPS_INITED)
        set(HMCMAKENG_HMBUILD_DEPS_INITED "1")

        hmcmakeng_debug("hmbuild_deps_base")

        find_package(HMBase REQUIRED)

        # hmbuild_find_package with REQUIRED only generate not-found message.
        # Check XXX_FOUND instead to stop processing.
        find_package(CompilerLibs QUIET)
        find_package(HMUlibs      QUIET)
        find_package(HWSecureC    QUIET)
        find_package(HMFSLibs     QUIET)
        find_package(HMSecLib     QUIET)
        find_package(HMLibOpt     QUIET)

        if (NOT CompilerLibs_FOUND)
            if (HMBUILD_MAKE_BINARIES)
                message(FATAL_ERROR "CompilerLibs not found")
            endif()
        endif()
    endif()
endmacro(hmbuild_setup_deps_base)

# vim:ts=4:sw=4:expandtab
