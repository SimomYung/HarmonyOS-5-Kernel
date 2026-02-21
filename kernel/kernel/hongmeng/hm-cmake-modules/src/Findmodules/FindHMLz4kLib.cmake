# Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
# Description: Cmake module for finding liblz4k
# Author: Huawei OS Kernel Lab
# Create: Mon Mar 06 10:05:21 2023

include(HMFindUtils)

FindHMLib(HMLz4kLib HMLz4kLib::HMLz4kLib
          STATICLIB liblz4k.a
          QUIET "${HMLz4kLib_FIND_QUIETLY}"
          )

find_package_handle_standard_args(HMLz4kLib
                                    REQUIRED_VARS
                                    HMLz4kLib_static_FOUND
                                    NAME_MISMATCHED)
# vim:ts=4:sw=4:expandtab
