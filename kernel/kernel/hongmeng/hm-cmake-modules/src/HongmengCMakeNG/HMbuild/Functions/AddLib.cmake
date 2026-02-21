# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild hmbuild_add_library function
# Author: Huawei OS Kernel Lab
# Create: Wed May 26 13:20:30 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Condition)
include(HongmengCMakeNG/HMbuild/Functions/AddDynamicLib)
include(HongmengCMakeNG/HMbuild/Functions/AddStaticLib)

function(hmbuild_append_lib_list)
    cmake_parse_arguments (PARSE_ARGV 0 _appendlib "PREPEND" "TARGET;TYPE;CONDITION" "")
    hmcmakeng_debug("hmbuild_append_lib_list: target=${_appendlib_TARGET} type=${_appendlib_TYPE} CONDITION=${_appendlib_CONDITION} PREPEND=${_appendlib_PREPEND}")

    # Check condition
    hmbuild_check_condition(checkcond "${_appendlib_CONDITION}")
    if (NOT ${checkcond})
        return()
    endif()

    if (_appendlib_PREPEND)
        set(prepend_op "PREPEND")
    else()
        set(prepend_op "")
    endif()

    # Append to hmcache list
    if ("${_appendlib_TYPE}" STREQUAL "static")
        hmbuild_cache_append_to(targets_lib_static   "${_appendlib_TARGET}" ${prepend_op})
    elseif ("${_appendlib_TYPE}" STREQUAL "dynamic")
        hmbuild_cache_append_to(targets_lib_dynamic  "${_appendlib_TARGET}" ${prepend_op})
    else()
        hmcmakeng_error("fatal error: AddLib ${_appendlib_TARGET} with type ${_appendlib_TYPE}")
    endif()
endfunction(hmbuild_append_lib_list)

function(hmbuild_lib_add_all_static_targets)
    # Headers only build?
    if (NOT (DEFINED HMBUILD_MAKE_BINARIES))
        return()
    endif()

    hmbuild_cache_val_of(targets targets_lib_static)
    foreach(t ${targets})
        hmcmakeng_status("Target: static lib ${t} in ${HMBUILD_DIR_PREFIX_VERBOSE}")
        hmbuild_add_static_library(${t})
    endforeach()
endfunction(hmbuild_lib_add_all_static_targets)

function(hmbuild_lib_add_all_dynamic_targets)
    # Headers only build?
    if (NOT (DEFINED HMBUILD_MAKE_BINARIES))
        return()
    endif()

    hmbuild_cache_val_of(targets targets_lib_dynamic)
    foreach(t ${targets})
        hmcmakeng_status("Target: dynamic lib ${t} in ${HMBUILD_DIR_PREFIX_VERBOSE}")
        hmbuild_add_dynamic_library(${t})
    endforeach()
endfunction(hmbuild_lib_add_all_dynamic_targets)

# vim:ts=4:sw=4:expandtab
