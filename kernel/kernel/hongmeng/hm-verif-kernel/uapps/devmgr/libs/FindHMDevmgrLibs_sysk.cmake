# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Find devmgr include and libs
# Author: Huawei OS Kernel Lab
# Create: Fri May 06 15:17:56 2022

include(HMFindUtils)

FindHMLib(Libdh_sysk HMDevmgrLibs::Libdh_sysk
    INCLUDE devhost.h
    INCLUDE_SUBDIRS libdevhost
    STATICLIB dh_sysk
    SHAREDLIB dh_sysk
    QUIET "${Libdh_FIND_QUIETLY}")

find_package_handle_standard_args(Libdh_sysk
    REQUIRED_VARS
    Libdh_include_FOUND
    Libdh_sysk_static_FOUND
    Libdh_sysk_shared_FOUND
    NAME_MISMATCHED)

FindHMLib(Libhmsrv_io_sysk HMDevmgrLibs::Libhmsrv_io_sysk
    INCLUDE io.h
    INCLUDE_SUBDIRS libhmsrv_io
    STATICLIB hmsrv_io_sysk
    SHAREDLIB hmsrv_io_sysk
    QUIET "${Libhmsrv_io_FIND_QUIETLY}")

find_package_handle_standard_args(Libhmsrv_io_sysk
    REQUIRED_VARS
    Libhmsrv_io_include_FOUND
    Libhmsrv_io_sysk_static_FOUND
    Libhmsrv_io_sysk_shared_FOUND
    NAME_MISMATCHED)

find_package_handle_standard_args(HMDevmgrLibs
    REQUIRED_VARS
    Libdh_sysk_FOUND
    Libhmsrv_io_sysk_FOUND
    NAME_MISMATCHED)
