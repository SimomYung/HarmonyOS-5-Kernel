/**
 * @defgroup  hitls_dto hitls_dto.h
 * @ingroup   tls
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * @brief     DTLS traffic offload
 */

#ifndef HITLS_DTO_H
#define HITLS_DTO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DTLS_EPOCH_SIZE                           2u
#define DTLS_SEQ_NUM_SIZE                         6u

#define HITLS_CIPHER_AES_GCM_128_IV_SIZE          8u
#define HITLS_CIPHER_AES_GCM_128_KEY_SIZE         16u
#define HITLS_CIPHER_AES_GCM_128_SALT_SIZE        4u

#define HITLS_CIPHER_AES_GCM_256_IV_SIZE          8u
#define HITLS_CIPHER_AES_GCM_256_KEY_SIZE         32u
#define HITLS_CIPHER_AES_GCM_256_SALT_SIZE        4u

#define HITLS_CIPHER_AES_CBC_128_IV_SIZE          16u
#define HITLS_CIPHER_AES_CBC_128_KEY_SIZE         16u
#define HITLS_MAC_AES_CBC_128_SIZE                64u

#define HITLS_CIPHER_AES_CBC_256_IV_SIZE          16u
#define HITLS_CIPHER_AES_CBC_256_KEY_SIZE         32u
#define HITLS_MAC_AES_CBC_256_SIZE                64u

 typedef struct {
    int32_t version;
    int32_t cipherType;
} CryptoInfo;

typedef struct {
    CryptoInfo info;
    uint8_t iv[HITLS_CIPHER_AES_GCM_128_IV_SIZE];
    uint8_t key[HITLS_CIPHER_AES_GCM_128_KEY_SIZE];
    uint8_t salt[HITLS_CIPHER_AES_GCM_128_SALT_SIZE];
} DTLS12_AesGcm128CryptoInfo;

typedef struct {
    CryptoInfo info;
    uint8_t iv[HITLS_CIPHER_AES_GCM_256_IV_SIZE];
    uint8_t key[HITLS_CIPHER_AES_GCM_256_KEY_SIZE];
    uint8_t salt[HITLS_CIPHER_AES_GCM_256_SALT_SIZE];
} DTLS12_AesGcm256CryptoInfo;

typedef struct {
    CryptoInfo info;
    uint8_t iv[HITLS_CIPHER_AES_CBC_128_IV_SIZE];
    uint8_t key[HITLS_CIPHER_AES_CBC_128_KEY_SIZE];
    uint8_t macKey[HITLS_MAC_AES_CBC_128_SIZE];
    uint32_t hashAlg;
} DTLS12_AesCbc128CryptoInfo;

typedef struct {
    CryptoInfo info;
    uint8_t iv[HITLS_CIPHER_AES_CBC_256_IV_SIZE];
    uint8_t key[HITLS_CIPHER_AES_CBC_256_KEY_SIZE];
    uint8_t macKey[HITLS_MAC_AES_CBC_256_SIZE];
    uint32_t hashAlg;
} DTLS12_AesCbc256CryptoInfo;

/* DTLS key information record */
typedef struct {
    union {
        DTLS12_AesGcm128CryptoInfo gcm128;
        DTLS12_AesGcm256CryptoInfo gcm256;
        DTLS12_AesCbc128CryptoInfo cbc128;
        DTLS12_AesCbc256CryptoInfo cbc256;
    } cryptoInfo;
    uint32_t cryptoInfoLen;
} HITLS_DtoCryptoInfo;

/* DTLS epoch and sequence records */
typedef struct {
    uint16_t writeEpoch;
    uint16_t readEpoch;
    uint8_t writeSeq[DTLS_SEQ_NUM_SIZE];
    uint8_t readSeq[DTLS_SEQ_NUM_SIZE];
} HITLS_DtoEpochParam;

#ifdef __cplusplus
}
#endif

#endif /* HITLS_DTO_H */