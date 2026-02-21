# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: HMFindModule: find libkasan_sysk
# Author: Huawei OS Kernel Lab
# Create: Tue Jun 07 16:39:03 2022

include(HMFindUtils)

FindHMLib(KasanLib_sysk KasanLib::KasanLib_sysk
          INCLUDE libkasan/kasan.h
          STATICLIB kasan_sysk
          SHAREDLIB kasan_sysk
          QUIET "${KasanLib_sysk_FIND_QUIETLY}")

find_package_handle_standard_args(KasanLib_sysk
                                    REQUIRED_VARS
                                    KasanLib_sysk_static_FOUND
                                    KasanLib_sysk_shared_FOUND
                                    NAME_MISMATCHED)

# vim:ts=4:sw=4:expandtab
