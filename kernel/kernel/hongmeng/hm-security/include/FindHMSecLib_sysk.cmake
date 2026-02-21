# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: HMFindModule: find hmsrv_sec_sysk
# Author: Huawei OS Kernel Lab
# Create: Mon Feb 28 14:39:52 2022

include(HMFindUtils)

FindHMLib(HMSecLib_sysk HMSecLib::HMSecLib_sysk
          INCLUDE libpolicy_api.h
          STATICLIB hmsrv_sec_sysk
          SHAREDLIB hmsrv_sec_sysk
          QUIET "${HMSecLib_FIND_QUIETLY}")

find_package_handle_standard_args(HMSecLib
                                    REQUIRED_VARS
                                    HMSecLib_sysk_static_FOUND
                                    HMSecLib_sysk_shared_FOUND
                                    NAME_MISMATCHED)

# vim:ts=4:sw=4:expandtab
