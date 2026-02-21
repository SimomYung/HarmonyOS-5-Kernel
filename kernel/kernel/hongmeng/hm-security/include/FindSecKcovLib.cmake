# Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
# Description: Cmake module for finding libseckcov
# Author: Huawei OS Kernel Lab
# Create: Fri Aug 09 10:48:33 2024

include(HMFindUtils)

FindHMLib(SecKcovLib SecKcovLib::SecKcovLib
          INCLUDE libseckcov/seckcov.h
          STATICLIB seckcov
          SHAREDLIB seckcov
          QUIET "${SecKcovLib_FIND_QUIETLY}")

# vim:ts=4:sw=4:expandtab
