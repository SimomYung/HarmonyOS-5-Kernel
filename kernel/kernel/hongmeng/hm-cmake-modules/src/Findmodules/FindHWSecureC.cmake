# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Cmake module for finding hm ulibs
# Author: Huawei OS Kernel Lab
# Create: Wed Jun 02 10:10:05 2021

include(HMFindUtils)

FindHMLib(HWSecureC HWSecureC::HWSecureC
          INCLUDE       securec.h
          STATICLIB     libhwsecurec.a
          SHAREDLIB     hwsecurec
          QUIET         "${HWSecureC_FIND_QUIETLY}"
          )

FindHMLib(HWSecureC_pic HWSecureC::HWSecureC_pic
          INCLUDE       securec.h
          STATICLIB     libhwsecurec.pic.a
          QUIET         "${HWSecureC_FIND_QUIETLY}"
          )

find_package_handle_standard_args(HWSecureC
                                    REQUIRED_VARS
                                    HWSecureC_static_FOUND
                                    HWSecureC_shared_FOUND
                                    HWSecureC_pic_static_FOUND
                                    NAME_MISMATCHED)

# vim:ts=4:sw=4:expandtab
