# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Parser of HMbuild 'include-' command
# Author: Huawei OS Kernel Lab
# Create: Fri Jun 18 16:36:33 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Functions/SetInclude)

function(hmbuild_cmd_include)
    cmake_parse_arguments (PARSE_ARGV 0 _include "" "" "CMDLIST;ARG;CMD")
    hmcmakeng_debug("hmbuild_cmd_include, CMDLIST=${_include_CMDLIST}, ARG=${_include_ARG}")

    # to make sure SUBLIST 1 -1 always success
    if ("include" STREQUAL "${_include_CMD}")
        set(_include_CMDLIST "include;y")
    endif()

    list(SUBLIST _include_CMDLIST 1 -1  parts)
    foreach(p IN LISTS parts)
        if ("${p}" STREQUAL "y")
            set(cond "${p}")
        elseif ("${p}" MATCHES "^CONFIG_.*")
            set(cond "${p}")
        elseif ((NOT DEFINED target_source) AND ("${p}" MATCHES "\\.c$|\\.S$"))
            set(target_source "${p}")
        elseif ((NOT DEFINED target_dir) AND ("${p}" STREQUAL "."))
            set(target_dir "${p}")
        elseif(NOT DEFINED target)
            set(target "${p}")
        endif()
    endforeach()

    if (NOT DEFINED cond)
        set(cond "y")
    endif()

    if (DEFINED target)
        hmbuild_append_target_include("${target}" "${_include_ARG}"
                                     CONDITION "${cond}")
    elseif(DEFINED target_dir)
        hmbuild_set_dir_include("${_include_ARG}"
                               CONDITION "${cond}")
    elseif(DEFINED target_source)
        # for include-xxx.c: -a -b
        hmbuild_set_source_include("${target_source}" "${_include_ARG}"
                                  CONDITION "${cond}")
    else()
        hmbuild_builtin_all_target_name(target_list)
        hmbuild_append_target_include("${target_list}" "${_include_ARG}"
                                     CONDITION "${cond}")
    endif()
endfunction(hmbuild_cmd_include)

# vim:ts=4:sw=4:expandtab
