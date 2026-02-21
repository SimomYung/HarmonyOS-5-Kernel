# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild SetInclib cmd
# Author: Huawei OS Kernel Lab
# Create: Mon Jun 28 14:16:29 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Cache)

# inclib-isystem:Actually,Cmake supply INTERFACE_SYSTEM_INCLUDE_DIRECTORIES,
# currently only target have this property,SOURCE and DIRECTORY do not have it.
# so SOURCE and DIRECTORY put "-isystem dir" in COMPILE_OPTIONS
# In addition,if add include dir through target_include_directories with
# SYSTEM option. Dir will not set successfully if CMAKE check it included
# in CMAKE_{LANG}_IMPLICIT_INCLUDE_DIRECTORIES wheather cflags have -nostdinc.
# So target also use target_compile_options to set "-isystem dir"

function(hmbuild_set_source_inclib source inclib)
    cmake_parse_arguments (PARSE_ARGV 0 _srcinclib "ISYSTEM" "" "")
    hmcmakeng_debug("hmbuild_set_source_inclib: source=${source} INCLIB=${inclib}")

    separate_arguments(inclib)
    foreach(lib IN LISTS inclib)
        if (NOT TARGET ${lib})
            if (DEFINED HMBUILD_MAKE_BINARIES)
                hmcmakeng_error("Inclib ${lib} not found")
            endif()
            continue()
        endif()
        if (_srcinclib_ISYSTEM)
             # Not use "SHELL: -isystem dir" because cmake behavior set COMPILE_OPTIONS
             # between src and others is different,src not have de-duplicate
             # policy , and if directly set "-isystem dir"(cmake will add double quotation)
             # so there use "-isystem" "dir"
            set_property(SOURCE ${source} APPEND PROPERTY COMPILE_OPTIONS
                         -isystem $<TARGET_PROPERTY:${lib},INTERFACE_INCLUDE_DIRECTORIES>)
        else()
            set_property(SOURCE ${source} APPEND PROPERTY INCLUDE_DIRECTORIES
                         $<TARGET_PROPERTY:${lib},INTERFACE_INCLUDE_DIRECTORIES>)
        endif()
        set_property(SOURCE ${source} APPEND PROPERTY COMPILE_DEFINITIONS
                     $<TARGET_PROPERTY:${lib},INTERFACE_COMPILE_DEFINITIONS>)
    endforeach()
endfunction(hmbuild_set_source_inclib)

function(hmbuild_set_dir_inclib inclib)
    cmake_parse_arguments (PARSE_ARGV 0 _dirinclib "ISYSTEM" "" "")
    hmcmakeng_debug("hmbuild_set_dir_inclib: INCLIB=${inclib}, ISYSTEM=${_dirinclib_ISYSTEM}")
    separate_arguments(inclib)
    foreach(lib IN LISTS inclib)
        if (NOT TARGET ${lib})
            if (DEFINED HMBUILD_MAKE_BINARIES)
                hmcmakeng_error("Inclib ${lib} not found")
            endif()
            continue()
        endif()
        if (_dirinclib_ISYSTEM)
            set_property(DIRECTORY APPEND PROPERTY COMPILE_OPTIONS
                         "SHELL: -isystem $<TARGET_PROPERTY:${lib},INTERFACE_INCLUDE_DIRECTORIES>")
        else()
            set_property(DIRECTORY APPEND PROPERTY INCLUDE_DIRECTORIES
                         $<TARGET_PROPERTY:${lib},INTERFACE_INCLUDE_DIRECTORIES>)
        endif()
        set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
                     $<TARGET_PROPERTY:${lib},INTERFACE_COMPILE_DEFINITIONS>)
    endforeach()
endfunction(hmbuild_set_dir_inclib)

function(hmbuild_set_target_inclib target inclib_list)
    cmake_parse_arguments (PARSE_ARGV 0 _tgtsetinclib "ISYSTEM" "" "")
    hmcmakeng_debug("hmbuild_set_target_inclib:target=${target} INCLIBS=${inclib_list} ISYSTEM=${_tgtsetinclib_ISYSTEM}")

    hmbuild_target_cmakename_of(cmake_target ${target})
    foreach(lib IN LISTS inclib_list)
        if (NOT TARGET ${lib})
            if (DEFINED HMBUILD_MAKE_BINARIES)
                hmcmakeng_error("Inclib ${lib} not found")
            endif()
            continue()
        endif()
        if (_tgtsetinclib_ISYSTEM)
            target_compile_options("${cmake_target}" PRIVATE
                         "SHELL: -isystem $<TARGET_PROPERTY:${lib},INTERFACE_INCLUDE_DIRECTORIES>")
        else()
            target_include_directories("${cmake_target}"
                                       PRIVATE
                                       $<TARGET_PROPERTY:${lib},INTERFACE_INCLUDE_DIRECTORIES>)
        endif()
        get_target_property(target_includes "${cmake_target}" INCLUDE_DIRECTORIES)
        set_property(TARGET "${cmake_target}" APPEND PROPERTY COMPILE_DEFINITIONS
                     $<TARGET_PROPERTY:${lib},INTERFACE_COMPILE_DEFINITIONS>)
    endforeach()
endfunction(hmbuild_set_target_inclib)

function(hmbuild_append_target_inclib targets inclibs)
    cmake_parse_arguments (PARSE_ARGV 0 _tgtinclib "ISYSTEM" "" "")
     hmcmakeng_debug(
         "hmbuild_append_target_inclib:targets=${targets} INCLIBS=${inclibs}, ISYSTEM=${_tgtinclib_ISYSTEM}")
    separate_arguments(targets)
    separate_arguments(inclibs)
    foreach(t ${targets})
        foreach(lib IN LISTS inclibs)
            if (NOT ${_tgtinclib_ISYSTEM})
                hmbuild_cache_append_to(inclib_list_${t} "${lib}")
            else()
                hmbuild_cache_append_to(inclib_sys_list_${t} "${lib}")
            endif()
        endforeach()
    endforeach()
endfunction(hmbuild_append_target_inclib)

function(hmbuild_inclib_config_target target)
    hmbuild_cache_val_of(inclib_list inclib_list_${target})
    hmcmakeng_debug("hmbuild_inclib_config_target: inclib_list_${target}=${inclib_list}")
    if (inclib_list)
        hmbuild_set_target_inclib(${target} "${inclib_list}")
    endif(inclib_list)

    hmbuild_cache_val_of(inclib_list inclib_sys_list_${target})
    hmcmakeng_debug("hmbuild_inclib_config_target: inclib_sys_list_${target}=${inclib_list}")
    if (inclib_list)
        hmbuild_set_target_inclib(${target} "${inclib_list}" "ISYSTEM")
    endif(inclib_list)
endfunction(hmbuild_inclib_config_target)

# vim:ts=4:sw=4:expandtab
