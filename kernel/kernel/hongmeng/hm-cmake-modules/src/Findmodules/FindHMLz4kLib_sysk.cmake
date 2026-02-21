# Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
# Description: Cmake module for finding liblz4k_sysk
# Author: Huawei OS Kernel Lab
# Create: Mon Mar 06 13:17:58 2023

include(HMFindUtils)

FindHMLib(HMLz4kLib_sysk HMLz4kLib::HMLz4kLib_sysk
          STATICLIB liblz4k_sysk.a
          QUIET "${HMLz4kLib_FIND_QUIETLY}"
          )

find_package_handle_standard_args(HMLz4kLib
                                    REQUIRED_VARS
                                    HMLz4kLib_sysk_static_FOUND
                                    NAME_MISMATCHED)
# vim:ts=4:sw=4:expandtab
