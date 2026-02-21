# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMFindModule: find LwIP include and libs
# Author: Huawei OS Kernel Lab
# Create: Sat Dec 25 16:59:41 2021

include(HMFindUtils)

FindHMLib(HMLwIPLib HMLwIPLib::HMLwIPLib
    INCLUDE lwip/sys.h
    INCLUDE_SUBDIRS hmnet/lwip/include
    STATICLIB libhmlwip.a
    QUIET "${HMLwIPLib_FIND_QUIETLY}")
FindHMLib(HMLwIPConfigLib HMLwIPLib::HMLwIPConfigLib
    INCLUDE lwipopts.h
    INCLUDE_SUBDIRS hmnet/lwip/config
    STATICLIB libhmlwip.a
    QUIET "${HMLwIPConfigLib_FIND_QUIETLY}")
find_package_handle_standard_args(HMLwIPLib
    REQUIRED_VARS
    HMLwIPLib_static_FOUND
    NAME_MISMATCHED)
