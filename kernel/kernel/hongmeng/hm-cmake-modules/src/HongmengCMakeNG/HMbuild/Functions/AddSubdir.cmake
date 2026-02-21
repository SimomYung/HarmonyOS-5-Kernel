# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild hmbuild_add_subdir function
# Author: Huawei OS Kernel Lab
# Create: Sat Jun 05 15:00:58 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Functions/AddBuiltinLib)

function(hmbuild_add_subdir subdir)
    hmcmakeng_debug("add subdir: ${subdir}")
    add_subdirectory(${subdir})

    # Headers only building, no need to add dependencies to builtin.a (and it is not exist...)
    if (NOT (DEFINED HMBUILD_MAKE_BINARIES))
        return()
    endif()

    hmbuild_add_all_builtin_subdir(${subdir})
endfunction(hmbuild_add_subdir)

function(hmbuild_subdirs_add_all)
    hmbuild_cache_val_of(subdirs subdirs)
    foreach(d ${subdirs})
        hmbuild_add_subdir(${d})
    endforeach()
endfunction(hmbuild_subdirs_add_all)

function(hmbuild_append_subdir_list)
    cmake_parse_arguments (PARSE_ARGV 0 _appendsubdir "" "CONDITION;SUBDIR" "")
    hmcmakeng_debug("hmbuild_append_subdir_list: cond=${_appendsubdir_CONDITION}, subdir=${_appendsubdir_SUBDIR}")

    hmbuild_check_condition(checkcond "${_appendsubdir_CONDITION}")
    if (NOT ${checkcond})
        return()
    endif()

    # remove ending '/'
    string(REGEX REPLACE "/+$" "" appendsubdir "${_appendsubdir_SUBDIR}")
    hmbuild_cache_append_to(subdirs "${appendsubdir}")
endfunction(hmbuild_append_subdir_list)

# vim:ts=4:sw=4:expandtab
