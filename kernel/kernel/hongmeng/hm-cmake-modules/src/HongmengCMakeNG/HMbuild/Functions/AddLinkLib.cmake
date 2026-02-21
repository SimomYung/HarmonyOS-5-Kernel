# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Add external library to target
# Author: Huawei OS Kernel Lab
# Create: Tue Jun 22 20:27:46 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Cache)

function(hmbuild_set_target_linklib target linklib_list)
    hmcmakeng_debug("hmbuild_set_target_linklib:target=${target} FLAGS=${linklib_list}")
    # Headers NOT NEED
    if (NOT (DEFINED HMBUILD_MAKE_BINARIES))
        return()
    endif()
    hmbuild_target_cmakename_of(cmake_target ${target})
    if (NOT TARGET ${cmake_target})
        hmcmakeng_error("hmbuild_set_target_linklib failed: ${cmake_target} is not a target")
        return()
    endif()

    separate_arguments(linklib_list)
    foreach(lib IN LISTS linklib_list)
        target_link_libraries(${cmake_target} PRIVATE ${lib})
    endforeach()

endfunction(hmbuild_set_target_linklib)

function(hmbuild_append_target_linklib target libraries)
    cmake_parse_arguments (PARSE_ARGV 0 _tgtlinklib "" "CONDITION" "")
    hmcmakeng_debug(
            "hmbuild_append_target_linklib:target=${target} library=${libraries} CONDITION=${_tgtlinklib_CONDITION}")
    # Check condition
    hmbuild_check_condition(checkcond "${_tgtlinklib_CONDITION}")
    if (NOT ${checkcond})
        return()
    endif()

    separate_arguments(libraries)
    foreach(l ${libraries})
        hmbuild_cache_append_to(linklib_list_${target} "${l}")
    endforeach()
endfunction(hmbuild_append_target_linklib)

function(hmbuild_linklib_config_target target)
    # Headers NOT NEED
    if (NOT (DEFINED HMBUILD_MAKE_BINARIES))
        return()
    endif()

    hmbuild_cache_val_of(linklib_list linklib_list_${target})
    hmcmakeng_debug("hmbuild_linklib_config_target: linklib_list_${target}=${linklib_list}")
    if (linklib_list)
        hmbuild_set_target_linklib(${target} "${linklib_list}")
    endif(linklib_list)
endfunction(hmbuild_linklib_config_target)

# vim:ts=4:sw=4:expandtab
