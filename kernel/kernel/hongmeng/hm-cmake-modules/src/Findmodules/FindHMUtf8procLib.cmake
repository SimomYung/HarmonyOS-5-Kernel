# Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Cmake module for finding libz
# Author: Huawei OS Kernel Lab
# Create: Thu Oco 13 10:05:21 2023

include(HMFindUtils)

FindHMLib(HMUtf8procLib HMUtf8procLib::HMUtf8procLib
		STATICLIB libutf8proc.a
		QUIET "${HMUtf8procLib_FIND_QUIETLY}")

find_package_handle_standard_args(HMUtf8procLib
                                    REQUIRED_VARS
                                    HMUtf8procLib_static_FOUND
                                    NAME_MISMATCHED)
# vim:ts=4:sw=4:expandtab
