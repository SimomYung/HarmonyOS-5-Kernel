# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild: cflags command
# Author: Huawei OS Kernel Lab
# Create: Sat May 29 22:22:16 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Builtin)
include(HongmengCMakeNG/HMbuild/Functions/SetCFlags)

# cflags-target: -a -b -c
# cflags-file.c: -a -b -c
# cflags-.: -a -b -c
# cflags: -a -b -c
include(HongmengCMakeNG/Log)
function(hmbuild_cmd_cflags)
    cmake_parse_arguments (PARSE_ARGV 0 _cflags "" "" "CMDLIST;ARG;CMD")
    hmcmakeng_debug("hmbuild_cmd_cflags: CMD=${_cflags_CMD}")

    # to make sure SUBLIST 1 -1 always success
    if ("cflags" STREQUAL "${_cflags_CMD}")
        set(_cflags_CMDLIST "cflags;y")
    endif()

    list(SUBLIST _cflags_CMDLIST 1 -1  parts)
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
        hmbuild_append_target_cflags("${target}" "${_cflags_ARG}"
                                     CONDITION "${cond}")
    elseif(DEFINED target_dir)
        hmbuild_set_dir_cflags("${_cflags_ARG}"
                               CONDITION "${cond}")
    elseif(DEFINED target_source)
        # for cflags-xxx.c: -a -b
        hmbuild_set_source_cflags("${target_source}" "${_cflags_ARG}"
                                  CONDITION "${cond}")
    else()
        hmbuild_builtin_all_target_name(target_list)
        hmbuild_append_target_cflags("${target_list}" "${_cflags_ARG}"
                                     CONDITION "${cond}")
    endif()

endfunction(hmbuild_cmd_cflags)

# vim:ts=4:sw=4:expandtab
