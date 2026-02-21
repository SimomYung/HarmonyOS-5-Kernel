# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Generation of builtin static library
# Author: Huawei OS Kernel Lab
# Create: Tue Jun 08 11:03:53 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/HMbuild/Cache)

function(hmbuild_builtin_target_name outvar)
    cmake_parse_arguments(PARSE_ARGV 0 _builtin "" "SUBDIR" "")
    hmbuild_cache_namespace(namespace SUBDIR "${_builtin_SUBDIR}")

    # See HMbuild to check how HMBUILD_DIR_PREFIX is set.
    # First level builtin name would be builtin, builtin in
    # subdirectory would be subdirX_builtin.
    set(${outvar} "${namespace}builtin" PARENT_SCOPE)
endfunction(hmbuild_builtin_target_name)

function(hmbuild_fpic_builtin_target_name outvar)
    cmake_parse_arguments(PARSE_ARGV 0 _builtinfpic "" "SUBDIR" "")
    hmbuild_builtin_target_name(builtinlib SUBDIR ${_builtinfpic_SUBDIR})
    set(${outvar} "${builtinlib}_fpic" PARENT_SCOPE)
endfunction(hmbuild_fpic_builtin_target_name)

function(hmbuild_builtin_all_target_name outvar)
    hmbuild_builtin_target_name(builtinlib)
    list(APPEND target_list "${builtinlib}")

    hmbuild_fpic_builtin_target_name(builtinfpiclib)
    list(APPEND target_list "${builtinfpiclib}")

    set(${outvar} "${target_list}" PARENT_SCOPE)
endfunction(hmbuild_builtin_all_target_name)

# vim:ts=4:sw=4:expandtab
