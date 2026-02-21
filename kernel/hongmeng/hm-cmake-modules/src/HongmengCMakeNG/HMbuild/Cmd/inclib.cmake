# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild inclib command
# Author: Huawei OS Kernel Lab
# Create: Mon Jun 28 13:59:21 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Functions/SetInclib)

# inclib-(isystem-)xxx: xxx
# Similar to cflags, add inclib include dir to dir, target or c file
# No condition is required
function(hmbuild_cmd_inclib)
    cmake_parse_arguments(PARSE_ARGV 0 _inclib "" "" "CMDLIST;ARG;CMD")
    hmcmakeng_debug("hmbuild_cmd_inclib: CMD=${_inclib_CMD}")

    # set default inclib to directory configuration
    # inclib: xxx == inclib-.: xxx
    if ("${_inclib_CMDLIST}" STREQUAL "inclib")
        set(parts ".")
    else()
        list(SUBLIST _inclib_CMDLIST 1 -1  parts)
    endif()

    foreach(p IN LISTS parts)
        if ((NOT DEFINED target_source) AND ("${p}" MATCHES "\\.c$|\\.S$"))
            set(target_source "${p}")
        elseif ((NOT DEFINED target_dir) AND ("${p}" STREQUAL "."))
            set(target_dir "${p}")
        elseif ((NOT DEFINED isystem) AND ("${p}" STREQUAL "isystem"))
            set(isystem "ISYSTEM")
        elseif(NOT DEFINED target)
            set(target "${p}")
        endif()
    endforeach()

    if ((NOT DEFINED target_source) AND (NOT DEFINED target_dir) AND (NOT DEFINED target))
        set(target_dir ".")
    endif()

    if (DEFINED target)
        hmbuild_append_target_inclib("${target}" "${_inclib_ARG}" "${isystem}")
    elseif(DEFINED target_dir)
        hmbuild_set_dir_inclib("${_inclib_ARG}" "${isystem}")
    elseif(DEFINED target_source)
        # for inclib-xxx.c: -a -b
        hmbuild_set_source_inclib("${target_source}" "${_inclib_ARG}" "${isystem}")
    else()
        # Not really useful because we always add a '.' to command
        hmbuild_builtin_all_target_name(target_list)
        hmbuild_append_target_inclib("${target_list}" "${_inclib_ARG}")
    endif()

endfunction(hmbuild_cmd_inclib)

# vim:ts=4:sw=4:expandtab
