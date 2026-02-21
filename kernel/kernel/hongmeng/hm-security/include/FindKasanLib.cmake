# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: HMFindModule: find libkasan
# Author: Huawei OS Kernel Lab
# Create: Tue Jun 07 16:35:37 2022

include(HMFindUtils)

FindHMLib(KasanLib KasanLib::KasanLib
          INCLUDE libkasan/kasan.h
          STATICLIB kasan
          SHAREDLIB kasan
          QUIET "${KasanLib_FIND_QUIETLY}")

find_package_handle_standard_args(KasanLib
                                    REQUIRED_VARS
                                    KasanLib_static_FOUND
                                    KasanLib_shared_FOUND
                                    NAME_MISMATCHED)

# vim:ts=4:sw=4:expandtab
