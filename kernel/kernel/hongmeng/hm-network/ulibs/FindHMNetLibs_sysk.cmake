# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: Find network libs with sysk suffix
# Author: Huawei OS Kernel Lab
# Create: Mon Feb 21 10:14:20 2022

include(HMFindUtils)

FindHMLib(HMNetSrvLib_sysk HMNetLibs::HMNetSrvLib_sysk
    STATICLIB hmnetd_sysk
    QUIET "${HMNetLibs_FIND_QUIETLY}"
    )

FindHMLib(HMNetLib_sysk HMNetLibs::HMNetLib_sysk
    INCLUDE hmnet_devices.h
    INCLUDE_SUBDIRS hmnet
    STATICLIB hmsrv_net_sysk
    SHAREDLIB hmsrv_net_sysk
    QUIET "${HMNetLibs_FIND_QUIETLY}"
    )
find_package_handle_standard_args(HMNetLib_sysk
    REQUIRED_VARS
    HMNetLib_sysk_static_FOUND
    HMNetLib_sysk_shared_FOUND
    NAME_MISMATCHED
    )

FindHMLib(HMUxnetLib_sysk HMNetLibs::HMUxnetLib_sysk
    STATICLIB hmsrv_uxnet_sysk
    QUIET "${HMNetLibs_FIND_QUIETLY}"
    )
find_package_handle_standard_args(HMUxnetLib_sysk
    REQUIRED_VARS
    HMUxnetLib_sysk_static_FOUND
    NAME_MISMATCHED
    )

find_package_handle_standard_args(HMNetLibs
    REQUIRED_VARS
    HMNetLib_sysk_FOUND
    HMUxnetLib_sysk_FOUND
    NAME_MISMATCHED
    )
