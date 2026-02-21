# Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Cmake module for finding libz
# Author: Huawei OS Kernel Lab
# Create: Thu Jan 13 10:05:21 2022

include(HMFindUtils)

FindHMLib(HMZlib_sysk HMZlib::HMZlib_sysk
          STATICLIB libz_sysk.a
          QUIET "${HMZlib_FIND_QUIETLY}")

find_package_handle_standard_args(HMZlib_sysk
                                    REQUIRED_VARS
                                    HMZlib_sysk_static_FOUND
                                    NAME_MISMATCHED)
# vim:ts=4:sw=4:expandtab
