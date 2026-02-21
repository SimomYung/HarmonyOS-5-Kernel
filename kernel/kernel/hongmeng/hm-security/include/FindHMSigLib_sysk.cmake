# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: Cmake module for finding libhmsig_sysk
# Author: Huawei OS Kernel Lab
# Create: Sun Jul 10 20:52:04 2022

include(HMFindUtils)

FindHMLib(HMSigLib_sysk HMSigLib::HMSigLib_sysk
          INCLUDE libhmsig/hmsig_crl.h
          STATICLIB hmsig_sysk
          SHAREDLIB hmsig_sysk
          QUIET "${HMSigLib_FIND_QUIETLY}")

# vim:ts=4:sw=4:expandtab
