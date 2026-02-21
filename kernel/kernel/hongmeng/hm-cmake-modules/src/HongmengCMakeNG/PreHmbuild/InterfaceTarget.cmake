# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: Interface target module for user and cmakeng
# Author: Huawei OS Kernel Lab
# Create: Sat Nov 27 09:05:17 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)

function(_hmbuild_generate_interface_target_name outvar target)
    set(${outvar} "HMBUILD_TARGET_INTERFACE_${target}" PARENT_SCOPE)
endfunction(_hmbuild_generate_interface_target_name)

function(_hmbuild_create_interface_target target)
    _hmbuild_generate_interface_target_name(interface_target ${target})
    if (NOT TARGET ${interface_target})
        add_custom_target(${interface_target}
                COMMENT "Interface Target for ${target}.")
    endif()
endfunction(_hmbuild_create_interface_target)

function(_hmbuild_set_interface_target_property target property value)
    hmcmakeng_debug("_hmbuild_set_interface_target_property: target:${target} property:${property} value:${value}")
    _hmbuild_generate_interface_target_name(interface_target ${target})
    set_target_properties(${interface_target}
                          PROPERTIES ${property} "${value}")
endfunction(_hmbuild_set_interface_target_property)

function(hmbuild_get_interface_target_property outvar target property)
    _hmbuild_generate_interface_target_name(interface_target ${target})
    if(NOT TARGET ${interface_target})
        set(${outvar} "NOTFOUND" PARENT_SCOPE)
        return()
    endif()
    get_target_property(value ${interface_target} ${property})
    set(${outvar} ${value} PARENT_SCOPE)
endfunction(hmbuild_get_interface_target_property)

function(hmbuild_config_target target)
    cmake_parse_arguments (PARSE_ARGV 0 _settgt "" "" "PROPERTY")
    _hmbuild_create_interface_target(${target})
    if(_settgt_PROPERTY)
        list(GET _settgt_PROPERTY 0 property)
        list(SUBLIST _settgt_PROPERTY 1 -1 value)
        _hmbuild_set_interface_target_property(${target} ${property} "${value}")
    endif()
endfunction(hmbuild_config_target)
