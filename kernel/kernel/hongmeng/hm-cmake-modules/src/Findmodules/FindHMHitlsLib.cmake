# Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: Cmake module for finding libhitls
# Author: Huawei OS Kernel Lab
# Create: Thu Apr 14 20:00:05 2022

include(HMFindUtils)
FindHMLib(CryptoLib HMHitlsLib::CryptoLib
          INCLUDE crypto/crypt_eal_rand.h
          STATICLIB hitls_crypto
          SHAREDLIB hitls_crypto
          QUIET "${HMHitlsLib_FIND_QUIETLY}")

set(CryptoLib_FIND_QUIETLY "${HMHitlsLib_FIND_QUIETLY}")
find_package_handle_standard_args(CryptoLib
                                  REQUIRED_VARS
                                  CryptoLib_static_FOUND
                                  CryptoLib_shared_FOUND
                                  NAME_MISMATCHED)

FindHMLib(BslLib HMHitlsLib::BslLib
          INCLUDE bsl_obj.h
          INCLUDE_SUBDIRS bsl
          STATICLIB hitls_bsl
          SHAREDLIB hitls_bsl
          QUIET "${HMHitlsLib_FIND_QUIETLY}")


set(BslLib_FIND_QUIETLY "${HMHitlsLib_FIND_QUIETLY}")
find_package_handle_standard_args(BslLib
                                  REQUIRED_VARS
                                  BslLib_include_FOUND
                                  BslLib_static_FOUND
                                  BslLib_shared_FOUND
                                  NAME_MISMATCHED)

find_package_handle_standard_args(HMHitlsLib
                                  REQUIRED_VARS
                                  CryptoLib_FOUND
                                  BslLib_FOUND
                                  NAME_MISMATCHED)
# vim:ts=4:sw=4:expandtab
