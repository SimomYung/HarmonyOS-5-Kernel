# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: Cmake module for finding libhmcrypt_sysk
# Author: Huawei OS Kernel Lab
# Create: Sun Jul 10 13:10:02 2022

include(HMFindUtils)

FindHMLib(HMCryptLib_sysk HMCryptLib::HMCryptLib_sysk
          INCLUDE libentropy/entropy.h
          INCLUDE libentropy/health_test.h
          INCLUDE libprng/non_crypto_prng.h
          STATICLIB hmcrypt_sysk
          SHAREDLIB hmcrypt_sysk
          QUIET "${HMCryptLib_FIND_QUIETLY}")

# vim:ts=4:sw=4:expandtab
