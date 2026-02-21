# Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: Cmake module for finding libhitls sysk libs
# Author: Huawei OS Kernel Lab
# Create: Thu Apr 14 20:00:05 2022

include(HMFindUtils)
FindHMLib(CryptoLib_sysk HMHitlsLib::CryptoLib_sysk
          INCLUDE crypto/crypt_eal_rand.h
          STATICLIB hitls_crypto_sysk
          QUIET "${HMHitlsLib_FIND_QUIETLY}")

set(CryptoLib_FIND_QUIETLY "${HMHitlsLib_FIND_QUIETLY}")
find_package_handle_standard_args(CryptoLib_sysk
                                  REQUIRED_VARS
                                  CryptoLib_sysk_static_FOUND
                                  NAME_MISMATCHED)

FindHMLib(BslLib_sysk HMHitlsLib::BslLib_sysk
          INCLUDE bsl/bsl_sal.h
          STATICLIB hitls_bsl_sysk
          QUIET "${HMHitlsLib_FIND_QUIETLY}")


set(BslLib_FIND_QUIETLY "${HMHitlsLib_FIND_QUIETLY}")
find_package_handle_standard_args(BslLib_sysk
                                  REQUIRED_VARS
                                  BslLib_sysk_static_FOUND
                                  NAME_MISMATCHED)

find_package_handle_standard_args(HMHitlsLib_sysk
                                  REQUIRED_VARS
                                  CryptoLib_sysk_FOUND
                                  BslLib_sysk_FOUND
                                  NAME_MISMATCHED)
# vim:ts=4:sw=4:expandtab
