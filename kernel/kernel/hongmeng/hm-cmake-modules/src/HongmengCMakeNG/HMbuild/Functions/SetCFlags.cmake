# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild set cflags
# Author: Huawei OS Kernel Lab
# Create: Sun May 30 01:46:10 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Cache)

function(_proccess_cflag_with_shell outvar cflag)
    if(cflag MATCHES ".+ .+")
        set(${outvar} "SHELL:${cflag}" PARENT_SCOPE)
    else()
        set(${outvar} "${cflag}" PARENT_SCOPE)
    endif()
endfunction(_proccess_cflag_with_shell)

function(hmbuild_set_source_cflags source flags)
    cmake_parse_arguments (PARSE_ARGV 0 _srccflags "" "CONDITION" "")
    hmcmakeng_debug("hmbuild_set_source_cflags: source=${source} FLAGS=${flags} CONDITION=${_srccflags_CONDITION}")
    # Check condition
    hmbuild_check_condition(checkcond "${_srccflags_CONDITION}")
    if (NOT ${checkcond})
        return()
    endif()
    # Ugly: cmake behavior which about set COMPILE_OPTIONS
    # between src and others is different,src not have de-duplicate
    # policy and if we want set cflag=A B;C D (cmd usually like
    # cflags-src: 'A B','C D'),if directoly use 'A B;C D' set src flags,
    # it will be set as this cflag="A B" "C D"(cmake add double quotation)
    # so need chagnge cmd 'A B';'C D' to A;B;C;D
    string(REGEX REPLACE "\'" " " flags "${flags}")
    separate_arguments(flags)
    foreach(f ${flags})
        set_property(SOURCE ${source} APPEND PROPERTY COMPILE_OPTIONS ${f})
    endforeach()
endfunction(hmbuild_set_source_cflags)

function(hmbuild_set_dir_cflags flags)
    cmake_parse_arguments (PARSE_ARGV 0 _dircflags "" "CONDITION" "")
    hmcmakeng_debug("hmbuild_set_dir_cflags:FLAGS=${flags} CONDITION=${_dircflags_CONDITION}")
    # Check condition
    hmbuild_check_condition(checkcond "${_dircflags_CONDITION}")
    if (NOT ${checkcond})
        return()
    endif()

    separate_arguments(flags UNIX_COMMAND "${flags}")
    foreach(f ${flags})
        _proccess_cflag_with_shell(out_flag "${f}")
        set_property(DIRECTORY APPEND PROPERTY COMPILE_OPTIONS "${out_flag}")
    endforeach()
endfunction(hmbuild_set_dir_cflags)

function(hmbuild_set_target_cflags target cflags_list)
    hmcmakeng_debug("hmbuild_set_target_cflags:target=${target} FLAGS=${cflags_list}")

    hmbuild_target_cmakename_of(cmake_target ${target})
    target_compile_options("${cmake_target}" PRIVATE "${cflags_list}")
endfunction(hmbuild_set_target_cflags)

function(hmbuild_append_target_cflags targets flags)
    cmake_parse_arguments (PARSE_ARGV 0 _tgtcflags "" "CONDITION" "")
    hmcmakeng_debug(
            "hmbuild_append_target_cflags:targets=${targets} FLAGS=${flags} CONDITION=${_tgtcflags_CONDITION}")
    # Check condition
    hmbuild_check_condition(checkcond "${_tgtcflags_CONDITION}")
    if (NOT ${checkcond})
        return()
    endif()

    separate_arguments(targets)
    separate_arguments(flags UNIX_COMMAND "${flags}")
    foreach(t ${targets})
        foreach(f ${flags})
            _proccess_cflag_with_shell(out_flag "${f}")
            hmbuild_cache_append_to(cflags_list_${t} "${out_flag}")
        endforeach()
    endforeach()
endfunction(hmbuild_append_target_cflags)

function(hmbuild_cflags_config_target target)
    hmbuild_cache_val_of(cflags_list cflags_list_${target})
    hmcmakeng_debug("hmbuild_cflags_config_target: cflags_list_${target}=${cflags_list}")
    if (cflags_list)
        hmbuild_set_target_cflags(${target} "${cflags_list}")
    endif(cflags_list)
endfunction(hmbuild_cflags_config_target)

function(hmbuild_cflags_config_is_include_fpic target)
    hmbuild_cache_val_of(cflags_list cflags_list_${target})
    hmcmakeng_debug("hmbuild_cflags_config_target: cflags_list_${target}=${cflags_list}")
    if (cflags_list)
        hmbuild_set_target_cflags(${target} "${cflags_list}")
    endif(cflags_list)
endfunction(hmbuild_cflags_config_target)

# vim:ts=4:sw=4:expandtab
