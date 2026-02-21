# Copyright (c) Huawei Technologies Co., Ltd. 2022-2024. All rights reserved.
# Description: Cmake module for finding libhmelog_sysk
# Author: Huawei OS Kernel Lab
# Create: Thu Apr 25 14:45:15 2024

include(HMFindUtils)

FindHMLib(HMELogLib_sysk HMELogLib::HMELogLib_sysk
          INCLUDE libhmelog/libhmelog.h
          STATICLIB hmelog_sysk
          SHAREDLIB hmelog_sysk
          QUIET "${HMELogLib_FIND_QUIETLY}")

# vim:ts=4:sw=4:expandtab
