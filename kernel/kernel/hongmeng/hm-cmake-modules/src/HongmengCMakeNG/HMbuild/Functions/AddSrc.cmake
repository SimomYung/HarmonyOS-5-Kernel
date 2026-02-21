# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild function hmbuild_add_src
# Author: Huawei OS Kernel Lab
# Create: Wed May 26 10:15:39 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/HMbuild/Cache)
include(HongmengCMakeNG/HMbuild/Builtin)

function(hmbuild_add_src)
    cmake_parse_arguments (PARSE_ARGV 0 _addsrc "" "TARGET;CONDITION" "SOURCES")
    hmcmakeng_debug("hmbuild_add_src: target=${_addsrc_TARGET}, CONDITION=${_addsrc_CONDITION}, SOURCES=${_addsrc_SOURCES}")

    if ("${_addsrc_CONDITION}" MATCHES "^CONFIG_.*")
        if (NOT DEFINED ${_addsrc_CONDITION})
            return()
        endif()
    endif()

    # If TARGET not set, add it to builtin target
    if ("" STREQUAL "${_addsrc_TARGET}")
        hmbuild_builtin_all_target_name(target_list)
    else()
        set(target_list "${_addsrc_TARGET}")
    endif()

    separate_arguments(target_list)
    separate_arguments(_addsrc_SOURCES)
    foreach(t ${target_list})
        foreach(s ${_addsrc_SOURCES})
            hmbuild_cache_append_to(src_${t} ${s})
        endforeach()
    endforeach()
endfunction()

function(get_all_targets_contain_src outvar src)
    hmbuild_all_targets(targets)
    set(result "")
    foreach (t IN LISTS targets)
        hmbuild_cache_val_of(srcs "src_${t}")
        separate_arguments(srcs)
        if ("${src}" IN_LIST srcs)
            list(APPEND result "${t}")
        endif()
    endforeach()
    set(${outvar} ${result} PARENT_SCOPE)
endfunction(get_all_targets_contain_src)

# vim:ts=4:sw=4:expandtab
