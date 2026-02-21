# Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: Cmake module for finding libsepol
# Author: Huawei OS Kernel Lab
# Create: Thu Dec 10 20:00:05 2022

include(HMFindUtils)
FindHMLib(HMSepolLib_sysk HMSepolLib::HMSepolLib_sysk
          SHAREDLIB sepol_sysk
          QUIET "${HMSepolLib_FIND_QUIETLY}")

find_package_handle_standard_args(HMSepolLib_sysk
                                  REQUIRED_VARS
                                  HMSepolLib_sysk_shared_FOUND
                                  NAME_MISMATCHED)
# vim:ts=4:sw=4:expandtab
