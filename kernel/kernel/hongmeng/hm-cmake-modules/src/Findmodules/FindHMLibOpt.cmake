# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: FindHMLibOpt.cmake
# Author: Huawei OS Kernel Lab
# Create: Fri Feb 18 07:43:06 2022

include(HMFindUtils)

FindHMLib(HMLibOpt HMLibOpt::HMLibOpt
                    STATICLIB libopt_arm.a )

find_package_handle_standard_args(HMLibOpt
                                    REQUIRED_VARS
                                    HMLibOpt_static_FOUND
                                    NAME_MISMATCHED)
