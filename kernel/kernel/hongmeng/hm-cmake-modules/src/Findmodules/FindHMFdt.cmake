# Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
# Description: FindHMFdt.cmake
# Author: Huawei OS Kernel Lab
# Create: Sun Oct 08 08:16:06 2023

include(HMFindUtils)

FindHMLib(HMFdt HMFdt::HMFdt
                STATICLIB hmfdt
                QUIET "${HMFdt_FIND_QUIETLY}")

find_package_handle_standard_args(HMFdt
                                  REQUIRED_VARS
                                  HMFdt_static_FOUND
                                  NAME_MISMATCHED)
