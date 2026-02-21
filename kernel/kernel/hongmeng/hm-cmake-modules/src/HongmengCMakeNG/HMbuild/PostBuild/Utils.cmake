# Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: HMbuild post-build utils
# Author: Huawei OS Kernel Lab
# Create: Tue Feb 8 10:47:33 2022

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)

# interface for setting the post-proc argumment
function(hmbuild_postbuild_set_arg module_name arg_name arg_value)
    set(HMBUILD_POSTBUILD_${module_name}_${arg_name} ${arg_value} CACHE INTERNAL
        "HMbuild post process argument ${arg_name} of module:${module_name}" FORCE)
endfunction(hmbuild_postbuild_set_arg)

# interface for getting the post-proc argumment
function(hmbuild_postbuild_get_arg module_name arg_name out_var)
    set(${out_var} ${HMBUILD_POSTBUILD_${module_name}_${arg_name}} PARENT_SCOPE)
endfunction(hmbuild_postbuild_get_arg)

# call this function to invoke post-build modules
function(hmbuild_postbuild target postbuild_tail_target)
    hmbuild_target_cmakename_of(cmake_target ${target})
    # Post-build modules update tail target for next module to depends
    set(tail_target ${cmake_target})
    include(HongmengCMakeNG/HMbuild/PostBuild/Xom)
    include(HongmengCMakeNG/HMbuild/PostBuild/Strip)
    set(${postbuild_tail_target} ${tail_target} PARENT_SCOPE)
endfunction(hmbuild_postbuild)

# vim:ts=4:sw=4:expandtab
