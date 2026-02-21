# Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
# Description: Cmake module for finding hm metalibs sysk
# Author: Huawei OS Kernel Lab
# Create: Tue Jan 17 10:26:52 2023

include(HMFindUtils)

FindHMLib(HMMetaLibs_sysk HMMetaLibs::HMMetaLibs_sysk
          STATICLIB libmetalibs_sysk.a
          QUIET "${HMMetaLibs_FIND_QUIETLY}")

FindHMLib(HMMetaLibs_fpic_sysk HMMetaLibs::HMMetaLibs_fpic_sysk
          STATICLIB libmetalibs_fpic_sysk.a
          QUIET "${HMMetaLibs_FIND_QUIETLY}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HMMetaLibs_sysk
                                  REQUIRED_VARS
                                  HMMetaLibs_sysk_static_FOUND
                                  HMMetaLibs_fpic_sysk_static_FOUND)
