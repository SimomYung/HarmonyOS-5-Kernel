# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild set dynamic lib version
# Author: Huawei OS Kernel Lab
# Create: Tue Sep 14 10:07:07 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Cache)

function(hmbuild_append_lib_version target)
    cmake_parse_arguments (PARSE_ARGV 0 _version "" "" "MAJOR;MINOR")
    hmcmakeng_debug(
        "hmbuild_append_lib_version:target=${target}, MAJOR=${_version_MAJOR}.${_version_MINOR}")
    hmbuild_cache_append_to(lib_version_major_${target} "${_version_MAJOR}")
    hmbuild_cache_append_to(lib_version_minor_${target} "${_version_MINOR}")
endfunction(hmbuild_append_lib_version)

function(hmbuild_libversion_config_of outvar  target)
    hmbuild_cache_val_of(major_num lib_version_major_${target})
    hmbuild_cache_val_of(minor_num lib_version_minor_${target})

    # If version not set, give default 0.1
    if ("" STREQUAL "${major_num}")
        set(${outvar}_major "0" PARENT_SCOPE)
        set(${outvar}_minor "1" PARENT_SCOPE)
    else()
        set(${outvar}_major ${major_num} PARENT_SCOPE)
        set(${outvar}_minor ${minor_num} PARENT_SCOPE)
    endif()
endfunction(hmbuild_libversion_config_of)

# vim:ts=4:sw=4:expandtab
