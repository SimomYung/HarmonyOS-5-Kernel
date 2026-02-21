# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Find network include and libs
# Author: Huawei OS Kernel Lab
# Create: Thu Nov 11 20:08:57 2021

include(HMFindUtils)

FindHMLib(HMNetSrvLib HMNetLibs::HMNetSrvLib
    STATICLIB libhmnetd.a
    QUIET "${HMNetLibs_FIND_QUIETLY}"
    )

FindHMLib(HMNetLib HMNetLibs::HMNetLib
    INCLUDE hmnet_devices.h
    INCLUDE_SUBDIRS hmnet
    STATICLIB libhmsrv_net.a
    SHAREDLIB hmsrv_net
    QUIET "${HMNetLibs_FIND_QUIETLY}"
    )
find_package_handle_standard_args(HMNetLib
    REQUIRED_VARS
    HMNetLib_static_FOUND
    HMNetLib_shared_FOUND
    NAME_MISMATCHED
    )

FindHMLib(HMUxnetLib HMNetLibs::HMUxnetLib
    STATICLIB libhmsrv_uxnet.a
    QUIET "${HMNetLibs_FIND_QUIETLY}"
    )
find_package_handle_standard_args(HMUxnetLib
    REQUIRED_VARS
    HMUxnetLib_static_FOUND
    NAME_MISMATCHED
    )

find_package_handle_standard_args(HMNetLibs
    REQUIRED_VARS
    HMNetLib_FOUND
    HMUxnetLib_FOUND
    NAME_MISMATCHED
    )
