# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Cmake module for finding libhmcrypt
# Author: Huawei OS Kernel Lab
# Create: Tue Mar 01 18:04:40 2022

include(HMFindUtils)

FindHMLib(HMCryptLib HMCryptLib::HMCryptLib
          INCLUDE libhmcrypt/hmbase64.h
          INCLUDE libhmcrypt/hmasn1.h
          INCLUDE libhmcrypt/hmpem.h
          INCLUDE libhmcrypt/hmcert.h
          INCLUDE libhmcrypt/hmpkcs7.h
          INCLUDE libhmcrypt/hmcrypt.h
          INCLUDE libhmcrypt/hmcrypto_common.h
          INCLUDE libhmcrypt/hmcrypto_skcipher.h
          INCLUDE libhmcrypt/hmcrypto_hash.h
          INCLUDE libhmcrypt/hmcrypto_mac.h
          INCLUDE libhmcrypt/hmcrypto_pkverify.h
          INCLUDE libhmcrypt/hmcrypto_drbg.h
          INCLUDE libhmcrypt/hmcrypto_algid.h
          INCLUDE libentropy/entropy.h
          INCLUDE libentropy/health_test.h
          INCLUDE libprng/non_crypto_prng.h
          STATICLIB hmcrypt
          SHAREDLIB hmcrypt
	  QUIET "${HMCryptLib_FIND_QUIETLY}")

# vim:ts=4:sw=4:expandtab
