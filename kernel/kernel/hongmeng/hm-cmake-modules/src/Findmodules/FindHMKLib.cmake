# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: FindHMKlibs.cmake
# Author: Huawei OS Kernel Lab
# Create: Fri Feb 18 07:43:06 2022

include(HMFindUtils)

FindHMLib(HMKLib HMKLib::HMKLib
                    STATICLIB libklibs.a
                    QUIET "${HMKLib_FIND_QUIETLY}")

find_package_handle_standard_args(HMKLib
                                    REQUIRED_VARS
                                    HMKLib_static_FOUND
                                    NAME_MISMATCHED)
