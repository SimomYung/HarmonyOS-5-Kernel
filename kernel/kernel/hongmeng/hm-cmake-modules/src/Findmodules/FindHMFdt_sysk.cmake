# Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
# Description: FindHMFdt_sysk.cmake
# Author: Huawei OS Kernel Lab
# Create: Sun Oct 08 08:16:06 2023

include(HMFindUtils)

FindHMLib(HMFdt_sysk HMFdt::HMFdt_sysk
          STATICLIB hmfdt_sysk
          QUIET "${HMFdt_FIND_QUIETLY}")

find_package_handle_standard_args(HMFdt_sysk
                                  REQUIRED_VARS
                                  HMFdt_sysk_static_FOUND
                                  NAME_MISMATCHED)
