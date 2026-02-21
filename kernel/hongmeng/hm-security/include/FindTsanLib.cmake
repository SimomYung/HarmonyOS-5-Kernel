# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: HMFindModule: find libtsan
# Author: Huawei OS Kernel Lab
# Create: Tue Nov 15 16:35:37 2022

include(HMFindUtils)

FindHMLib(TsanLib TsanLib::TsanLib
          INCLUDE libtsan/tsan.h
          STATICLIB tsan
          SHAREDLIB tsan
          QUIET "${TsanLib_FIND_QUIETLY}")

find_package_handle_standard_args(TsanLib
                                    REQUIRED_VARS
                                    TsanLib_static_FOUND
                                    TsanLib_shared_FOUND)

# vim:ts=4:sw=4:expandtab
