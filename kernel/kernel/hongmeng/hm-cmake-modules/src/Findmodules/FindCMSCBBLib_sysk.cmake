# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: Cmake module for finding libcmscbb_sysk
# Author: Huawei OS Kernel Lab
# Create: Sun Jul 10 21:35:17 2022

include(HMFindUtils)

FindHMLib(CMSCBBLib_sysk CMSCBBLib::CMSCBBLib_sysk
          STATICLIB hmsrv_cmscbb_securec_sysk
          QUIET "${CMSCBBLib_FIND_QUIETLY}"
          )

find_package_handle_standard_args(CMSCBBLib_sysk
                                    REQUIRED_VARS
                                    CMSCBBLib_sysk_static_FOUND
                                    NAME_MISMATCHED)
# vim:ts=4:sw=4:expandtab
