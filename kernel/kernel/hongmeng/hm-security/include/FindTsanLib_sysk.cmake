# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: HMFindModule: find libtsan_sysk
# Author: Huawei OS Kernel Lab
# Create: Tue Nov 15 16:39:03 2022

include(HMFindUtils)

FindHMLib(TsanLib_sysk TsanLib::TsanLib_sysk
          INCLUDE libtsan/tsan.h
          STATICLIB tsan_sysk
          SHAREDLIB tsan_sysk
          QUIET "${TsanLib_sysk_FIND_QUIETLY}")

find_package_handle_standard_args(TsanLib_sysk
                                    REQUIRED_VARS
                                    TsanLib_sysk_static_FOUND
                                    TsanLib_sysk_shared_FOUND
                                    NAME_MISMATCHED)

# vim:ts=4:sw=4:expandtab
