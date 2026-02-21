# Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
# Description: Cmake module for finding liblz4
# Author: Huawei OS Kernel Lab
# Create: Mon Mar 06 10:05:21 2023

include(HMFindUtils)

FindHMLib(HMLz4Lib HMLz4Lib::HMLz4Lib
          STATICLIB liblz4.a
          QUIET "${HMLz4Lib_FIND_QUIETLY}"
          )

find_package_handle_standard_args(HMLz4Lib
                                    REQUIRED_VARS
                                    HMLz4Lib_static_FOUND
                                    NAME_MISMATCHED)
# vim:ts=4:sw=4:expandtab
