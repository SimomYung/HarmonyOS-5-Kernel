# Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
# Description: Cmake module for finding liblz4_sysk
# Author: Huawei OS Kernel Lab
# Create: Mon Mar 06 13:17:58 2023

include(HMFindUtils)

FindHMLib(HMLz4Lib_sysk HMLz4Lib::HMLz4Lib_sysk
          STATICLIB liblz4_sysk.a
          QUIET "${HMLz4Lib_FIND_QUIETLY}"
          )

find_package_handle_standard_args(HMLz4Lib
                                    REQUIRED_VARS
                                    HMLz4Lib_sysk_static_FOUND
                                    NAME_MISMATCHED)
# vim:ts=4:sw=4:expandtab
