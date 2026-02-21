# Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
# Description: HMFindModule: find hmsrv_sec
# Author: Huawei OS Kernel Lab
# Create: Tue Jun 22 17:06:52 2021

include(HMFindUtils)

FindHMLib(HMSecLib HMSecLib::HMSecLib
          INCLUDE libpolicy_api.h
          STATICLIB libhmsrv_sec.a
          SHAREDLIB hmsrv_sec
          QUIET "${HMSecLib_FIND_QUIETLY}")

find_package_handle_standard_args(HMSecLib
                                    REQUIRED_VARS
                                    HMSecLib_static_FOUND
                                    HMSecLib_shared_FOUND
                                    NAME_MISMATCHED)

# vim:ts=4:sw=4:expandtab
