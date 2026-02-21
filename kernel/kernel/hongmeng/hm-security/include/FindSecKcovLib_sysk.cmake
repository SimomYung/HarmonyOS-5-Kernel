# Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
# Description: Cmake module for finding libseckcov_sysk
# Author: Huawei OS Kernel Lab
# Create: Fri Aug 09 10:48:33 2024

include(HMFindUtils)

FindHMLib(SecKcovLib_sysk SecKcovLib::SecKcovLib_sysk
          INCLUDE libseckcov/seckcov.h
          STATICLIB seckcov_sysk
          SHAREDLIB seckcov_sysk
          QUIET "${SecKcovLib_sysk_FIND_QUIETLY}")

# vim:ts=4:sw=4:expandtab
