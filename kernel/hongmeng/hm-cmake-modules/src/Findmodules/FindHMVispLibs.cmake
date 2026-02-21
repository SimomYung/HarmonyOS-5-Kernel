# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: HMFindModule:find visp headers and libs
# Author: Huawei OS Kernel Lab
# Create: Fri Mar 11 18:27:40 2022

include(HMFindUtils)

FindHMLib(HMVispLib HMVispLibs::HMVispLib
    INCLUDE visp_stack.h
    INCLUDE_SUBDIRS hmnet/visp
    SHAREDLIB hm_visp
    QUIET "${HMVispLib_FIND_QUIETLY}")
find_package_handle_standard_args(HMVispLib
    REQUIRED_VARS
    HMVispLib_include_FOUND
    HMVispLib_static_FOUND
    NAME_MISMATCHED)

FindHMLib(HMVispDopraLib HMVispLibs::HMVispDopraLib
    SHAREDLIB hm_dopra_adapter
    QUIET "${HMVispDopraLib_FIND_QUIETLY}")
find_package_handle_standard_args(HMVispDopraLib
    REQUIRED_VARS
    HMVispDopraLib_static_FOUND
    NAME_MISMATCHED)

find_package_handle_standard_args(HMVispLibs
    REQUIRED_VARS
    HMVispLib_FOUND
    HMVispDopraLib_FOUND
    NAME_MISMATCHED)
