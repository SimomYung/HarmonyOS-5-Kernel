# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Check if this build is debug building
# Author: Huawei OS Kernel Lab
# Create: Wed Aug 18 15:37:28 2021

if (DEFINED CONFIG_HMBUILD_PROFILE_RELEASE)
  set(__HMBUILD_FEATURE_DEBUG_VALUE "FALSE")
else()
  set(__HMBUILD_FEATURE_DEBUG_VALUE "TRUE")
endif()

# vim:ts=4:sw=4:expandtab
