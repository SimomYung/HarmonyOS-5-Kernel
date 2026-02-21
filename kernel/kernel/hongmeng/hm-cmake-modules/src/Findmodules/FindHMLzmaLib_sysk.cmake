# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: Cmake module for finding liblzma_sysk
# Author: Huawei OS Kernel Lab
# Create: Sun Jul 10 13:17:58 2022

include(HMFindUtils)

FindHMLib(HMLzmaLib_sysk HMLzmaLib::HMLzmaLib_sysk
          STATICLIB liblzma_sysk.a
          QUIET "${HMLzmaLib_FIND_QUIETLY}"
          )

find_package_handle_standard_args(HMLzmaLib
                                    REQUIRED_VARS
                                    HMLzmaLib_sysk_static_FOUND
                                    NAME_MISMATCHED)
# vim:ts=4:sw=4:expandtab
