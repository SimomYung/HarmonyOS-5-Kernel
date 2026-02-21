# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Cmake module for finding hm ulibs
# Author: Huawei OS Kernel Lab
# Create: Wed Jun 02 10:10:05 2021

include(HMFindUtils)

FindHMLib(HWSecureC_sysk HWSecureC::HWSecureC_sysk
          INCLUDE       securec.h
          STATICLIB     hwsecurec_sysk
          SHAREDLIB     hwsecurec_sysk
          QUIET         "${HWSecureC_FIND_QUIETLY}"
          )

FindHMLib(HWSecureC_sysk_pic HWSecureC::HWSecureC_sysk_pic
          INCLUDE       securec.h
          STATICLIB     hwsecurec_sysk.pic
          QUIET         "${HWSecureC_FIND_QUIETLY}"
          )

find_package_handle_standard_args(HWSecureC
                                    REQUIRED_VARS
                                    HWSecureC_sysk_static_FOUND
                                    HWSecureC_sysk_shared_FOUND
                                    HWSecureC_sysk_pic_static_FOUND
                                    NAME_MISMATCHED)

# vim:ts=4:sw=4:expandtab
