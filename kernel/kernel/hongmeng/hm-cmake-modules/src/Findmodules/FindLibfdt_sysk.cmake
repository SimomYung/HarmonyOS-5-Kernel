# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: FindHMKlibs.cmake
# Author: Huawei OS Kernel Lab
# Create: Fri Feb 18 07:43:06 2022

include(HMFindUtils)

FindHMLib(Libfdt_sysk Libfdt::Libfdt_sysk
                    STATICLIB fdt_sysk
                    QUIET "${HMLibfdt_FIND_QUIETLY}")

find_package_handle_standard_args(Libfdt_sysk
                                    REQUIRED_VARS
                                    Libfdt_sysk_static_FOUND
                                    NAME_MISMATCHED)
