# Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Cmake module for finding liblzma
# Author: Huawei OS Kernel Lab
# Create: Thu Jan 13 10:05:21 2022

include(HMFindUtils)

FindHMLib(HMLzmaLib HMLzmaLib::HMLzmaLib
          STATICLIB liblzma.a
          QUIET "${HMLzmaLib_FIND_QUIETLY}"
          )

find_package_handle_standard_args(HMLzmaLib
                                    REQUIRED_VARS
                                    HMLzmaLib_static_FOUND
                                    NAME_MISMATCHED)
# vim:ts=4:sw=4:expandtab
