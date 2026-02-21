# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: Cmake module for finding libhmulibs_sysk
# Author: Huawei OS Kernel Lab
# Create: Thu Jul 14 15:52:54 2022

include(HMFindUtils)

FindHMLib(HMUlibs_sysk HMUlibs::HMUlibs_sysk
          INCLUDE libhmlog/hmlog.h
          STATICLIB hmulibs_sysk
          QUIET "${HMUlibs_FIND_QUIETLY}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HMUlibs_sysk
                                    REQUIRED_VARS
                                    HMUlibs_sysk_include_FOUND
                                    HMUlibs_sysk_static_FOUND
                                    NAME_MISMATCHED)
