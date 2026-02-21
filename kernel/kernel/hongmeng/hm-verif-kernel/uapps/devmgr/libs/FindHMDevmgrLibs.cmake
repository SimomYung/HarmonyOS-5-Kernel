# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Find devmgr include and libs
# Author: Huawei OS Kernel Lab
# Create: Fri May 06 15:17:56 2022

include(HMFindUtils)

FindHMLib(Libdh HMDevmgrLibs::Libdh
    INCLUDE devhost.h
    INCLUDE_SUBDIRS libdevhost
    STATICLIB dh
    SHAREDLIB dh
    QUIET "${Libdh_FIND_QUIETLY}")

find_package_handle_standard_args(Libdh
    REQUIRED_VARS
    Libdh_include_FOUND
    Libdh_static_FOUND
    Libdh_shared_FOUND
    NAME_MISMATCHED)

FindHMLib(Libhmsrv_io HMDevmgrLibs::Libhmsrv_io
    INCLUDE io.h
    INCLUDE_SUBDIRS libhmsrv_io
    STATICLIB hmsrv_io
    SHAREDLIB hmsrv_io
    QUIET "${Libhmsrv_io_FIND_QUIETLY}")

find_package_handle_standard_args(Libhmsrv_io
    REQUIRED_VARS
    Libhmsrv_io_include_FOUND
    Libhmsrv_io_static_FOUND
    Libhmsrv_io_shared_FOUND
    NAME_MISMATCHED)

find_package_handle_standard_args(HMDevmgrLibs
    REQUIRED_VARS
    Libdh_FOUND
    Libhmsrv_io_FOUND
    NAME_MISMATCHED)
