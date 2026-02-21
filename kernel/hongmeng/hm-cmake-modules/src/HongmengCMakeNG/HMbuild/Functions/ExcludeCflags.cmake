# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: HMbuild hmbuild_exclude_cflags function
# Author: Huawei OS Kernel Lab
# Create: Fri Oct 15 11:54:16 2021

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Cache)
include(HongmengCMakeNG/HMbuild/Functions/AddSrc)

if(NOT __DEFINED_HMBUILD_EXCLUDE_CFLAGS)
    define_property(DIRECTORY PROPERTY "HMBUILD_EXCLUDE_CFLAGS" INHERITED
      BRIEF_DOCS "Exclude flags set on directory"
      FULL_DOCS
      "Exclude flags set on directory shoud inherited "
      "by all subdirectories"
      )
    set(__DEFINED_HMBUILD_EXCLUDE_CFLAGS 1)
endif()

include_guard(GLOBAL)

#
# Applies CMAKE_C_FLAGS to all targets in the current CMake directory.
#
function(apply_global_c_flags_to_all_targets)
    # Add leading and trailing spaces for the subsequent regex matching
    set(global_c_flags "SHELL: ${CMAKE_C_FLAGS} ")
    hmbuild_all_targets(targets)
    foreach(t IN LISTS targets)
        hmbuild_target_cmakename_of(cmake_target ${t})
        # not use PUBILIC, otherwise will set INTERFACE_COMPILE_OPTIONS
        target_compile_options(${cmake_target} PRIVATE ${global_c_flags})
        hmcmakeng_debug("apply_global_c_flags_to_all_targets: apply ${cmake_target} ${global_c_flags}")
        hmbuild_cache_set(exclude_global_cflags_list_"${t}" "${global_c_flags}")
    endforeach()
endfunction(apply_global_c_flags_to_all_targets)

function(__append_cflag_to_src src flag)
    hmbuild_cache_val_of(already_append_flags "appended_c_flags_${src}")
    if ("${flag}" IN_LIST already_append_flags)
        return()
    endif()
    hmbuild_cache_append_to(appended_c_flags_${src} "${flag}")
    # Note: The property COMPILE_OPTIONS of SOURCE is a
    # semicolon-separated list of options.
    separate_arguments(flag)
    get_property(source_compile_options_list SOURCE ${src} PROPERTY COMPILE_OPTIONS)
    if(source_compile_options_list)
        list(APPEND source_compile_options_list "${flag}")
    else()
        set(source_compile_options_list "${flag}")
    endif()
    set_property(SOURCE ${src} PROPERTY COMPILE_OPTIONS "${source_compile_options_list}")
    hmcmakeng_debug("__append_cflag_to_src: set ${src}=${source_compile_options_list}")
endfunction(__append_cflag_to_src)

function(hmbuild_exclude_target_cflags target cflags_list)
    hmbuild_target_cmakename_of(cmake_target ${target})
    hmcmakeng_debug("hmbuild_exclude_target_cflags:target=${target} FLAGS=${cflags_list} cmake_target=${cmake_target}")

    # Taget cflags= global_flags;others
    # we only remove flags in global_flags and keep the flags' order
    get_target_property(target_flags ${cmake_target} COMPILE_OPTIONS)
    hmbuild_cache_val_of(global_flags exclude_global_cflags_list_"${target}")
    hmbuild_cache_val_of(already_removed_flags "removed_c_flags_tgt_${cmake_target}")
    set(after_removed_global_flags ${global_flags})
    foreach(flag IN LISTS cflags_list)
        if (flag IN_LIST already_removed_flags)
            continue()
        endif()
        # Add leading and trailing spaces to prevent "AAA" matching "BAAAC"
        string(REGEX REPLACE " ${flag} " " " after_removed_global_flags "${after_removed_global_flags}")
        hmbuild_cache_append_to("removed_c_flags_tgt_${target}" "${flag}")
    endforeach()
    hmbuild_cache_set(exclude_global_cflags_list_"${target}" "${after_removed_global_flags}")
    list(APPEND new_target_flags "${after_removed_global_flags}")
    foreach(f IN LISTS target_flags)
        if(NOT ("${f}" STREQUAL "${global_flags}"))
            list(APPEND new_target_flags "${f}")
        endif()
    endforeach()
    set_target_properties(${cmake_target} PROPERTIES COMPILE_OPTIONS "${new_target_flags}")
    hmcmakeng_debug("hmbuild_exclude_target_cflags: set ${cmake_target}=${new_target_flags}")
endfunction(hmbuild_exclude_target_cflags)

function(hmbuild_append_src_cflags src cflags_list)
    hmbuild_cache_val_of(src_list exclude_src_list)
    if (src IN_LIST src_list)
        hmbuild_cache_val_of(exclude_cflags_list exclude_cflags_list_${src})
        foreach(flag IN LISTS exclude_cflags_list)
            list(REMOVE_ITEM cflags_list "${flag}")
        endforeach()
    endif()
    hmcmakeng_debug("hmbuild_append_src_cflags:src=${src} FLAGS=${cflags_list}")
    hmbuild_cache_val_of(append_flags "append_c_flags_${src}")
    foreach(flag IN LISTS cflags_list)
        if (flag IN_LIST append_flags)
            continue()
        endif()
        hmbuild_cache_append_to("append_c_flags_${src}" "${flag}")
        __append_cflag_to_src("${src}" "${flag}")
    endforeach()
endfunction(hmbuild_append_src_cflags)

function(hmbuild_append_source_exclude_cflags src flags)
    hmcmakeng_debug(
            "hmbuild_append_source_exclude_cflags:SRC=${src} FLAGS=${flags}")
    hmbuild_cache_append_to(exclude_cflags_list_${src} "${flags}")
    hmbuild_cache_val_of(src_list exclude_src_list)
    if (NOT ("${src}" IN_LIST src_list))
        hmbuild_cache_append_to(exclude_src_list "${src}")
    endif()
endfunction(hmbuild_append_source_exclude_cflags)

function(hmbuild_append_target_exclude_cflags targets flags)
    hmcmakeng_debug(
            "hmbuild_append_target_exclude_cflags:targets=${targets} FLAGS=${flags}")
    separate_arguments(targets)
    foreach(t ${targets})
        hmbuild_cache_append_to(exclude_cflags_list_${t} "${flags}")
    endforeach()
endfunction(hmbuild_append_target_exclude_cflags)

function(hmbuild_append_dir_exclude_cflags flags)
    hmcmakeng_debug(
            "hmbuild_append_dir_exclude_cflags:FLAGS=${flags}")
    # When using APPEND or APPEND_STRING with a property
    # defined to support INHERITED behavior  no inheriting occurs
    # when finding the initial value to append to. If the property
    # is not already directly set in the nominated scope, the command
    # will behave as though APPEND or APPEND_STRING had not been given.
    get_property(exclude_flags DIRECTORY PROPERTY HMBUILD_EXCLUDE_CFLAGS)
    list(APPEND exclude_flags ${flags})
    set_property(DIRECTORY APPEND PROPERTY HMBUILD_EXCLUDE_CFLAGS "${exclude_flags}")
endfunction(hmbuild_append_dir_exclude_cflags)

function(hmbuild_exclude_source_cflags)
    hmbuild_cache_val_of(src_list exclude_src_list)
    if (NOT src_list)
        return()
    endif()
    hmcmakeng_debug("hmbuild_exclude_source_cflags: src_list=${src_list}")

    # Add excluded cflags for source except src
    foreach(src IN LISTS src_list)
        hmbuild_cache_val_of(cflags_list exclude_cflags_list_${src})
        get_all_targets_contain_src(target_list "${src}")
        if (NOT target_list)
            hmcmakeng_error("No target contains source: ${src}")
        endif()
        separate_arguments(target_list)
        foreach(target IN LISTS target_list)
            # remove excluded cflags for target
            hmbuild_exclude_target_cflags("${target}" "${cflags_list}")
            hmbuild_target_cmakename_of(cmake_target ${target})
            get_target_property(target_sources ${cmake_target} SOURCES)
            # add excluded cflags for sources except src
            list(REMOVE_ITEM target_sources "${src}")
            foreach(s ${target_sources})
                hmbuild_append_src_cflags(${s} "${cflags_list}")
            endforeach()
        endforeach()
    endforeach()
endfunction(hmbuild_exclude_source_cflags)

function(hmbuild_exclude_dir_cflags)
    get_property(cflags_list DIRECTORY PROPERTY HMBUILD_EXCLUDE_CFLAGS)
    hmcmakeng_debug("hmbuild_exclude_cflags_dir: exclude_dir_cflags_list=${cflags_list}")
    if (NOT cflags_list)
        return()
    endif()
    # remove all targets
    hmbuild_all_targets(targets)
    foreach (t IN LISTS targets)
        hmbuild_exclude_target_cflags(${t} "${cflags_list}")
    endforeach()
endfunction(hmbuild_exclude_dir_cflags)

function(hmbuild_exclude_cflags_config_target target)
    hmbuild_cache_val_of(cflags_list exclude_cflags_list_${target})
    hmcmakeng_debug("hmbuild_exclude_cflags_config_target: exclude_cflags_list_${target}=${cflags_list}")
    if (cflags_list)
        hmbuild_exclude_target_cflags(${target} "${cflags_list}")
    endif(cflags_list)
endfunction(hmbuild_exclude_cflags_config_target)

macro(hmbuild_exclude_all)
   hmbuild_cache_val_of(_excludecflags_cmd "have_exclude_cmd")
   get_property(_excludecflags_dir DIRECTORY PROPERTY HMBUILD_EXCLUDE_CFLAGS)
   if (_excludecflags_cmd OR  _excludecflags_dir)
        apply_global_c_flags_to_all_targets()
        unset(CMAKE_C_FLAGS)
   endif()
   unset(_excludecflags_cmd)
   unset(_excludecflags_dir)
   # Exclude cflags in the order of dir,targets,src
   hmbuild_exclude_dir_cflags()
   hmbuild_targets_config_all()
   hmbuild_exclude_source_cflags()
endmacro(hmbuild_exclude_all)
# vim:ts=4:sw=4:expandtab
