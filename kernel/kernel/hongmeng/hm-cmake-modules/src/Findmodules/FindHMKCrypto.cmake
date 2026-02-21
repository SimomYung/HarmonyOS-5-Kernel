# Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
# Description: FindHMKcrypto.cmake
# Author: Huawei OS Kernel Lab
# Create: Mon June 17 17:43:06 2024

include(HMFindUtils)

FindHMLib(HMKCrypto HMKCrypto::HMKCrypto
                    INCLUDE_SUBDIRS hmkcrypto
                    INCLUDE hmkcrypto/hm_kcrypto_sha2.h
                    STATICLIB libhmkcrypto.a
                    QUIET "${HMKCrypto_FIND_QUIETLY}")

find_package_handle_standard_args(HMKCrypto
                                    REQUIRED_VARS
                                    HMKCrypto_static_FOUND
                                    NAME_MISMATCHED)

