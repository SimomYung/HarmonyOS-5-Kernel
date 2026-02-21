# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: Cmake module for finding libhmsig
# Author: Huawei OS Kernel Lab
# Create: Tue Jun 07 16:40:59 2022

include(HMFindUtils)

FindHMLib(HMSigLib HMSigLib::HMSigLib
          INCLUDE libhmsig/hmsig_crl.h
          INCLUDE libhmsig/hmsig.h
          STATICLIB hmsig
          SHAREDLIB hmsig
          QUIET "${HMSigLib_FIND_QUIETLY}")

# vim:ts=4:sw=4:expandtab
