# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Cmake module for finding hongmeng libc and libc-sys
# Author: Huawei OS Kernel Lab
# Create: Wed Dec 29 19:57:44 2021

include(HMFindUtils)

FindHMLib(HMUserLibC HMLibC::HMUserLibC
          INCLUDE stdio.h
          LIB_PREFIXES "/"
          STATICLIB libc.a
          SHAREDLIB c
          QUIET "${HMLibC_FIND_QUIETLY}")

FindHMLib(HMUserLibC_fpic HMLibC::HMUserLibC_fpic
          INCLUDE stdio.h
          LIB_PREFIXES "/"
          STATICLIB libc-fpic.a
          QUIET "${HMLibC_FIND_QUIETLY}")

FindHMLib(HMSysLibC HMLibC::HMSysLibC
          INCLUDE sys/klog.h
          LIB_PREFIXES "/"
          STATICLIB libc-sys.a
          SHAREDLIB c-sys
          QUIET "${HMLibC_FIND_QUIETLY}")

FindHMLib(HMLibHMC HMLibC::HMLibHMC
          LIB_PREFIXES "/"
          STATICLIB libhmc.a
          SHAREDLIB hmc
          QUIET "${HMLibC_FIND_QUIETLY}")

FindHMLib(HMMemLeak HMLibC::HMMemLeak
          LIB_PREFIXES "/"
          STATICLIB libmemleak.a
          SHAREDLIB memleak
          QUIET "${HMLibC_FIND_QUIETLY}")

set(HMUserLibC_FIND_QUIETLY "${HMLibC_FIND_QUIETLY}")
find_package_handle_standard_args(HMUserLibC
                                  REQUIRED_VARS
                                  HMUserLibC_static_FOUND
                                  HMUserLibC_shared_FOUND
                                  NAME_MISMATCHED)

set(HMUserLibC_fpic_FIND_QUIETLY "${HMLibC_FIND_QUIETLY}")
find_package_handle_standard_args(HMUserLibC_fpic
                                  REQUIRED_VARS
                                  HMUserLibC_static_FOUND
                                  HMUserLibC_fpic_static_FOUND
                                  HMUserLibC_shared_FOUND
                                  NAME_MISMATCHED)

set(HMSysLibC_FIND_QUIETLY "${HMLibC_FIND_QUIETLY}")
find_package_handle_standard_args(HMSysLibC
                                  REQUIRED_VARS
                                  HMSysLibC_static_FOUND
                                  HMSysLibC_shared_FOUND
                                  NAME_MISMATCHED)

set(HMLibHMC_FIND_QUIETLY "${HMLibC_FIND_QUIETLY}")
find_package_handle_standard_args(HMLibHMC
                                  REQUIRED_VARS
                                  HMLibHMC_static_FOUND
                                  HMLibHMC_shared_FOUND
                                  NAME_MISMATCHED)

set(HMMemLeak_FIND_QUIETLY "${HMLibC_FIND_QUIETLY}")
find_package_handle_standard_args(HMMemLeak
                                  REQUIRED_VARS
                                  HMMemLeak_static_FOUND
                                  HMMemLeak_shared_FOUND
                                  NAME_MISMATCHED)

find_package_handle_standard_args(HMLibC
                                  REQUIRED_VARS
                                  HMUserLibC_FOUND
                                  HMSysLibC_FOUND
                                  HMLibHMC_FOUND
                                  HMMemLeak_FOUND
                                  HMUserLibC_fpic_FOUND
                                  NAME_MISMATCHED)

# vim:ts=4:sw=4:expandtab
