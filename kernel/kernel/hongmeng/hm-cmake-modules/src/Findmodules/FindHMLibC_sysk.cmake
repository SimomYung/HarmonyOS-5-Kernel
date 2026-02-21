# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Cmake module for finding hongmeng libc and libc-sys
# Author: Huawei OS Kernel Lab
# Create: Wed Dec 29 19:57:44 2021

include(HMFindUtils)

FindHMLib(HMUserLibC_sysk HMLibC::HMUserLibC_sysk
          INCLUDE stdio.h
          LIB_PREFIXES "/"
          STATICLIB c_sysk
          SHAREDLIB c_sysk
          QUIET "${HMLibC_FIND_QUIETLY}")

FindHMLib(HMSysLibC_sysk HMLibC::HMSysLibC_sysk
          INCLUDE sys/klog.h
          LIB_PREFIXES "/"
          STATICLIB c-sys_sysk
          SHAREDLIB c-sys_sysk
          QUIET "${HMLibC_FIND_QUIETLY}")

set(HMUserLibC_FIND_QUIETLY "${HMLibC_FIND_QUIETLY}")
find_package_handle_standard_args(HMUserLibC_sysk
                                  REQUIRED_VARS
                                  HMUserLibC_sysk_static_FOUND
                                  HMUserLibC_sysk_shared_FOUND
                                  NAME_MISMATCHED)

set(HMSysLibC_FIND_QUIETLY "${HMLibC_FIND_QUIETLY}")
find_package_handle_standard_args(HMSysLibC_sysk
                                  REQUIRED_VARS
                                  HMSysLibC_sysk_static_FOUND
                                  HMSysLibC_sysk_shared_FOUND
                                  NAME_MISMATCHED)

find_package_handle_standard_args(HMLibC
                                  REQUIRED_VARS
                                  HMUserLibC_sysk_FOUND
                                  HMSysLibC_sysk_FOUND
                                  NAME_MISMATCHED)

# vim:ts=4:sw=4:expandtab
