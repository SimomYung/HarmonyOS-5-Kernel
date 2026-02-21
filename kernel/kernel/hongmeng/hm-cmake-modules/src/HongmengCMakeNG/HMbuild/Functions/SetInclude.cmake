# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild set include
# Author: Huawei OS Kernel Lab
# Create: Sun May 30 01:46:10 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Cache)

function(hmbuild_set_source_include source include)
    cmake_parse_arguments (PARSE_ARGV 0 _srcinclude "" "CONDITION" "")
    hmcmakeng_debug("hmbuild_set_source_include: source=${source} FLAGS=${include} CONDITION=${_srcinclude_CONDITION}")
    # Check condition
    hmbuild_check_condition(checkcond "${_srcinclude_CONDITION}")
    if (NOT ${checkcond})
        return()
    endif()

    separate_arguments(include)
    foreach(i ${include})
        if(NOT IS_ABSOLUTE ${i})
            get_filename_component(i ${i} ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR})
        endif()
        set_property(SOURCE ${source} APPEND PROPERTY INCLUDE_DIRECTORIES ${i})
    endforeach()
endfunction(hmbuild_set_source_include)

function(hmbuild_set_dir_include include)
    cmake_parse_arguments (PARSE_ARGV 0 _dirinclude "" "CONDITION" "")
    hmcmakeng_debug("hmbuild_set_dir_include:FLAGS=${include} CONDITION=${_dirinclude_CONDITION}")
    # Check condition
    hmbuild_check_condition(checkcond "${_dirinclude_CONDITION}")
    if (NOT ${checkcond})
        return()
    endif()

    separate_arguments(include)
    foreach(i ${include})
        if(NOT IS_ABSOLUTE ${i})
            get_filename_component(i ${i} ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR})
        endif()
        set_property(DIRECTORY APPEND PROPERTY INCLUDE_DIRECTORIES ${i})
    endforeach()
endfunction(hmbuild_set_dir_include)

function(hmbuild_set_target_include target include_list)
    hmcmakeng_debug("hmbuild_set_target_include:target=${target} FLAGS=${include_list}")

    hmbuild_target_cmakename_of(cmake_target ${target})
    # FIXME CMP0021
    separate_arguments(include_list)
    foreach(i ${include_list})
        target_include_directories("${cmake_target}" PRIVATE "${i}")
    endforeach()
endfunction(hmbuild_set_target_include)

function(hmbuild_append_target_include targets include)
    cmake_parse_arguments (PARSE_ARGV 0 _tgtinclude "" "CONDITION" "")
    hmcmakeng_debug(
            "hmbuild_append_target_include:targets=${targets} FLAGS=${include} CONDITION=${_tgtinclude_CONDITION}")
    # Check condition
    hmbuild_check_condition(checkcond "${_tgtinclude_CONDITION}")
    if (NOT ${checkcond})
        return()
    endif()

    separate_arguments(targets)
    separate_arguments(include)
    foreach(t ${targets})
        foreach(i ${include})
            hmbuild_cache_append_to(include_list_${t} "${i}")
        endforeach()
    endforeach()

endfunction(hmbuild_append_target_include)

function(hmbuild_include_config_target target)
    hmbuild_cache_val_of(include_list include_list_${target})
    hmcmakeng_debug("hmbuild_include_config_target: include_list_${target}=${include_list}")
    if (include_list)
        hmbuild_set_target_include(${target} "${include_list}")
    endif(include_list)
endfunction(hmbuild_include_config_target)

# vim:ts=4:sw=4:expandtab
