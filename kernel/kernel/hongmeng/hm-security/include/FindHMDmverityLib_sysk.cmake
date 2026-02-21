# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: Cmake module for finding libhmdmverity_sysk
# Author: Huawei OS Kernel Lab
# Create: Wed Oct 12 11:06:04 2022

include(HMFindUtils)

FindHMLib(HMDmverityLib_sysk HMDmverityLib::HMDmverityLib_sysk
          INCLUDE libhmdmverity/hmdmverity.h
          STATICLIB hmdmverity_sysk
          SHAREDLIB hmdmverity_sysk
          QUIET "${HMDmverityLib_FIND_QUIETLY}")

# vim:ts=4:sw=4:expandtab
