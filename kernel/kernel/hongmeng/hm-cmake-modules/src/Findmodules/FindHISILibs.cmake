# Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
# Description: FindHISILibs.cmake
# Author: Huawei OS Kernel Lab
# Create: Mon Now 25 17:43:06 2024

include(HMFindUtils)

FindHMLib(HISILibs HISILibs::HISILibs
                    INCLUDE_SUBDIRS exts/hisi/
                    INCLUDE hisilib_headers.txt
                    QUIET "${HISILibs_FIND_QUIETLY}")

find_package_handle_standard_args(HISILibs
                                    REQUIRED_VARS
                                    HISILibs_include_FOUND
                                    NAME_MISMATCHED)
