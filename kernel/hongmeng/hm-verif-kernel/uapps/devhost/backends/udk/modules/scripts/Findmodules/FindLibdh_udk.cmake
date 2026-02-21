# Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: FindLibdh_udk.cmake
# Author: Huawei OS Kernel Lab
# Create: Tue Mar 01 16:12:06 2022

include(HMFindUtils)

FindHMLib(Libdh_udk Libdh_udk::Libdh_udk
          SHAREDLIB dh-udk
          LIB_PREFIXES "/"
          QUIET "${Libdh_udk_FIND_QUIETLY}")

FindHMLib(Libdh_udk Libdh_udk::Libdh_udk_srv
          STATICLIB udk
          LIB_PREFIXES "/"
          QUIET "${Libdh_udk_FIND_QUIETLY}")

find_package_handle_standard_args(Libdh_udk
                                  REQUIRED_VARS
                                  Libdh_udk_shared_FOUND
                                  NAME_MISMATCHED)

find_package_handle_standard_args(Libdh_udk_srv
                                  REQUIRED_VARS
                                  Libdh_udk_srv_static_FOUND
                                  NAME_MISMATCHED)
# vim:ts=4:sw=4:expandtab
