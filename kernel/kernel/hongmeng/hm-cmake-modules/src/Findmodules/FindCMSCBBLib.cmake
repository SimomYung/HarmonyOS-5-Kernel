# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: Cmake module for finding libcmscbb
# Author: Huawei OS Kernel Lab
# Create: Sun Jul 10 21:31:25 2022

include(HMFindUtils)

FindHMLib(CMSCBBLib CMSCBBLib::CMSCBBLib
          STATICLIB hmsrv_cmscbb_securec
          SHAREDLIB cmscbb
          QUIET "${CMSCBBLib_FIND_QUIETLY}"
          )

find_package_handle_standard_args(CMSCBBLib
                                    REQUIRED_VARS
                                    CMSCBBLib_static_FOUND
                                    CMSCBBLib_shared_FOUND
                                    NAME_MISMATCHED)
# vim:ts=4:sw=4:expandtab
