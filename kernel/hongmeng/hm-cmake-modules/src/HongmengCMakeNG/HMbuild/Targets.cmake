# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild - config of targets
# Author: Huawei OS Kernel Lab
# Create: Sat Jun 05 12:46:07 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Cache)
include(HongmengCMakeNG/HMbuild/Functions/SetCFlags)
include(HongmengCMakeNG/HMbuild/Functions/SetInclib)
include(HongmengCMakeNG/HMbuild/Functions/SetInclude)
include(HongmengCMakeNG/HMbuild/Functions/AddLinkLib)
include(HongmengCMakeNG/HMbuild/Functions/ExcludeCflags)

function(hmbuild_all_targets outvar)
    cmake_parse_arguments(PARSE_ARGV 0 _target "" "SUBDIR" "")

    foreach (v IN ITEMS lib_static lib_dynamic exec_static exec_dynamic builtin)
        hmbuild_cache_val_of(targets targets_${v} SUBDIR "${_target_SUBDIR}")
        list(APPEND alltargets ${targets})
        hmcmakeng_trace("targets_${v}: ${targets}")
    endforeach()
    set(${outvar} ${alltargets} PARENT_SCOPE)
    hmcmakeng_trace("alltargets: ${alltargets}")
endfunction(hmbuild_all_targets)

function(hmbuild_targets_config_all)
    cmake_parse_arguments(PARSE_ARGV 0 _target "" "SUBDIR" "")

    # Headers only build?
    if (NOT (DEFINED HMBUILD_MAKE_BINARIES))
        return()
    endif()

    hmbuild_all_targets(targets SUBDIR "${_target_SUBDIR}")
    foreach (t IN LISTS targets)
        hmcmakeng_debug("config target ${t}")
        hmbuild_cflags_config_target(${t})
        hmbuild_include_config_target(${t})
        hmbuild_inclib_config_target(${t})
        hmbuild_linklib_config_target(${t})
        hmbuild_exclude_cflags_config_target(${t})
    endforeach()
endfunction(hmbuild_targets_config_all)

function(hmbuild_target_set_cmakename target cmakename)
    cmake_parse_arguments(PARSE_ARGV 0 _target "" "SUBDIR" "")

    hmbuild_cache_set("cmakename_${target}" "${cmakename}" SUBDIR "${_target_SUBDIR}")
endfunction(hmbuild_target_set_cmakename)

function(hmbuild_target_cmakename_of outvar target)
    cmake_parse_arguments(PARSE_ARGV 0 _target "" "SUBDIR" "")

    hmbuild_cache_val_of(out "cmakename_${target}" SUBDIR "${_target_SUBDIR}")
    set(${outvar} "${out}" PARENT_SCOPE)
endfunction(hmbuild_target_cmakename_of)

function(hmbuild_target_set_output_name target outputname)
    cmake_parse_arguments(PARSE_ARGV 0 _target "" "SUBDIR" "")

    hmbuild_cache_set("outputname_${target}" "${outputname}" SUBDIR "${_target_SUBDIR}")
endfunction(hmbuild_target_set_output_name)

function(hmbuild_target_output_name_of outvar target)
    cmake_parse_arguments(PARSE_ARGV 0 _target "" "SUBDIR" "")

    hmbuild_cache_val_of(out "outputname_${target}" SUBDIR "${_target_SUBDIR}")
    set(${outvar} "${out}" PARENT_SCOPE)
endfunction(hmbuild_target_output_name_of)

# vim:ts=4:sw=4:expandtab
