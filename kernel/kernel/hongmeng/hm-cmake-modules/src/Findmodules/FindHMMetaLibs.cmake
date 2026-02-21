# Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
# Description: Cmake module for finding hm metalibs
# Author: Huawei OS Kernel Lab
# Create: Tue Jan 17 10:25:34 2023

include(HMFindUtils)

FindHMLib(HMMetaLibs HMMetaLibs::HMMetaLibs
          STATICLIB libmetalibs.a
          QUIET "${HMMetaLibs_FIND_QUIETLY}")

FindHMLib(HMMetaLibs_fpic HMMetaLibs::HMMetaLibs_fpic
          STATICLIB libmetalibs_fpic.a
          QUIET "${HMMetaLibs_FIND_QUIETLY}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HMMetaLibs
                                  REQUIRED_VARS
                                  HMMetaLibs_static_FOUND
                                  HMMetaLibs_fpic_static_FOUND)
