# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Condition check for HMbuild
# Author: Huawei OS Kernel Lab
# Create: Fri May 28 11:04:21 2021

include_guard(GLOBAL)

function(hmbuild_check_condition outvar cond)
    if ("${cond}" STREQUAL "y")
        set(${outvar} "TRUE" PARENT_SCOPE)
        return()
    endif()
    if ("${cond}" MATCHES "^CONFIG_.*" AND DEFINED ${cond})
        set(${outvar} "TRUE" PARENT_SCOPE)
        return()
    endif()
    set(${outvar} "FALSE" PARENT_SCOPE)
endfunction()

# vim:ts=4:sw=4:expandtab
