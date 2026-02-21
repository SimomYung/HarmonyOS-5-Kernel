# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild: excludecflags command
# Author: Huawei OS Kernel Lab
# Create: Sat May 29 22:22:16 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Builtin)
include(HongmengCMakeNG/HMbuild/Functions/ExcludeCflags)

# excludecflags-target: -a '-b x=x' 'regex'
# excludecflags-file.c: -a '-b x=x' 'regex'
# excludecflags-.: -a '-b x=x' 'regex'
# excludecflags: -a '-b x=x' 'regex'
function(hmbuild_cmd_excludecflags)
    if (NOT (DEFINED HMBUILD_MAKE_BINARIES))
        return()
    endif()
    cmake_parse_arguments (PARSE_ARGV 0 _excflags "" "" "CMDLIST;ARG;CMD")
    hmcmakeng_debug("hmbuild_cmd_excludecflags: CMD=${_excflags_CMD} ARG=${_excflags_ARG}")

    # to make sure SUBLIST 1 -1 always success
    if ("excludecflags" STREQUAL "${_excflags_CMD}")
        set(_excflags_CMDLIST "excludecflags;y")
    endif()
    list(SUBLIST _excflags_CMDLIST 1 -1  parts)
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
    hmbuild_check_condition(checkcond "${cond}")
    if (NOT ${checkcond})
        return()
    endif()

    # Add leading and trailing spaces for CMAKE_C_FLAGS regex matching,
    # and we only handle those excluded flags in CMAKE_C_FLAGS.
    set(global_c_flags " ${CMAKE_C_FLAGS} ")
    separate_arguments(_excflags_ARG UNIX_COMMAND "${_excflags_ARG}")
    foreach(c ${_excflags_ARG})
        # Add leading and trailing spaces to prevent "AAA" matching "BAAAC"
        string(REGEX MATCHALL " ${c} " result "${global_c_flags}")
        if (result)
            string(STRIP "${result}" stripped_result)
            list(APPEND real_exclude_cflags "${stripped_result}")
        else()
            continue()
        endif()
    endforeach()
    if (NOT real_exclude_cflags)
        return()
    endif()

    hmcmakeng_status("hmbuild_cmd_excludecflags: real_exclude_cflags=${real_exclude_cflags}")
    if (DEFINED target)
        hmbuild_append_target_exclude_cflags("${target}" "${real_exclude_cflags}")
    elseif(DEFINED target_dir)
        hmbuild_append_dir_exclude_cflags("${real_exclude_cflags}")
    elseif(DEFINED target_source)
        hmbuild_append_source_exclude_cflags("${target_source}" "${real_exclude_cflags}")
    else()
        hmbuild_builtin_all_target_name(target_list)
        hmbuild_append_target_exclude_cflags("${target_list}" "${real_exclude_cflags}")
    endif()

    hmbuild_cache_set(have_exclude_cmd "TRUE")
endfunction(hmbuild_cmd_excludecflags)

# vim:ts=4:sw=4:expandtab
