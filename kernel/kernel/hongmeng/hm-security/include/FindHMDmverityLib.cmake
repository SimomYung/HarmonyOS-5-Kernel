# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: Cmake module for finding libhmdmverity
# Author: Huawei OS Kernel Lab
# Create: Wed Oct 12 11:05:59 2022

include(HMFindUtils)

FindHMLib(HMDmverityLib HMDmverityLib::HMDmverityLib
          INCLUDE libhmdmverity/hmdmverity.h
          STATICLIB hmdmverity
          SHAREDLIB hmdmverity
          QUIET "${HMDmverityLib_FIND_QUIETLY}")

# vim:ts=4:sw=4:expandtab
