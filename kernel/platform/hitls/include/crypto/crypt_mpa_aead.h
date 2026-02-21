/**
 * @defgroup    crypt_mpa_aead    crypt_mpa_aead.h
 * @ingroup     crypto
 * @copyright   Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * @brief       MPA Multi-stream parallel computing external interface file
 */
#ifndef CRYPT_MPA_AEAD_H
#define CRYPT_MPA_AEAD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup crypt_mpa_aead
 *
 * Encryption and authentication calculation sequence
 */
typedef enum {
    MPA_MTE, /**< hmac->aes */
    MPA_ETM, /**< aes->hmac */
} CRYPT_MPA_AeadOrder;

/**
 * @ingroup crypt_mpa_aead
 *
 * AES encryption and decryption mode
 */
typedef enum {
    MPA_ENC, /**< Encrypt */
    MPA_DEC, /**< Decrypt */
} CRYPT_MPA_EncMode;

/**
 * @ingroup crypt_mpa_aead
 *
 * hmac algorithm type
 */
typedef enum {
    MPA_HASH_NONE, /**< hash algorithm unused. */
    MPA_MD5,       /**< md5 algorithm (reserved) */
    MPA_SHA1,      /**< sha1 algorithm (reserved) */
    MPA_SHA224,    /**< sha224 algorithm (reserved) */
    MPA_SHA256,    /**< sha256 algorithm */
    MPA_SHA384,    /**< sha384 algorithm (reserved) */
    MPA_SHA512     /**< sha512 algorithm */
} CRYPT_MPA_HashAlg;

/**
 * @ingroup crypt_mpa_aead
 *
 * aes algorithm type
 */
typedef enum {
    MPA_AES_NONE,   /**< aes algorithm unused */
    MPA_AES128_CBC, /**< aes128cbc algorithm */
    MPA_AES192_CBC, /**< aes192cbc algorithm */
    MPA_AES256_CBC  /**< aes256cbc algorithm */
} CRYPT_MPA_AesMode;

/**
 * @ingroup crypt_mpa_aead
 *
 * Based stream, External data structure. Set the details about a single stream.
 */
typedef struct {
    /* common */
    uint32_t jobId;             /**< job flag */
    uint32_t aeadOrder;         /**< Authentication and encryption mode：MPA_MTE、MPA_ETM */

    /* aes alg resource */
    const uint8_t *msgInput;    /**< Message input for AES calculation, which is plain text or cipher text. */
    uint8_t *msgOutput;         /**< The output of the AES algorithm., ciphertext or plaintext */
    uint32_t aesMsgLen;         /**< Length of the data calculated by the AES, it's an integer multiple
                                     of 16 bytes, and unsupport aes padding function. */
    uint8_t aesMode;            /**< AES mode: MPA_AES_NONE, MPA_AES128_CBC, MPA_AES192_CBC, MPA_AES256_CBC */
    uint8_t encryptDecrypt;     /**< AES encryption or decryption: MPA_ENC, MPA_DEC */
    uint8_t res1[2];            /**< Reservation. */
    const uint8_t *iv;          /**< AES initialization vector input. */
    const uint8_t *aesKey;      /**< AES key input */
    uint32_t keyLen;            /**< AES key length.
                                     aes128:16 bytes; aes192:24 bytes; aes256:32 bytes. */
    uint32_t ivLen;             /**< AES initial vector length is 16 bytes. */
    uint8_t *expandKeyEnc;      /**< AES encryption extended key input，
                                     aes128: 11 * 16 bytes; aes192: 13 * 16 bytes; aes256: 15 * 16 bytes. */
    uint8_t *expandKeyDec;      /**< AES decryption extended key input.
                                     aes128: 11 * 16 bytes; aes192: 13 * 16 bytes; aes256: 15 * 16 bytes. */

    /* hmac-sha resource */
    const uint8_t *hmacKey;     /**< HMAC key input */
    const uint8_t *msg;         /**< HMAC calculation, data input */
    uint32_t hmacKeyLen;        /**< HMAC key length */
    uint32_t msgLen;            /**< HMAC calculated data length */
    uint8_t *hashResult;        /**< MAC output of the HMAC calculation result */
    uint8_t hashAlg;            /**< HASH algorithm selection：MPA_HASH_NONE, MPA_SHA256，MPA_SHA512. */
    uint8_t res2[3];            /**< Reservation. */
    uint32_t hashResultLen;     /**< Input parameter, which determines the length of the returned MAC data.
                                     sha256: <= 32 bytes; sha512: <= 64 bytes. */
    const uint8_t *ipadKeyHash; /**< HMAC ipadKey hash value.
                                     If the ipadkey hash is not calculated in advance, NULL is assigned. */
    const uint8_t *opadKeyHash; /**< HMAC opadKey hash value.
                                     If the opadkey hash is not calculated in advance, NULL is assigned. */

    void *userData;             /**< user-defined usage. */
} CRYPT_MPA_Job;

typedef struct MGR_ CRYPT_MPA_Mgr;

/**
 * @ingroup crypt_mpa_aead
 * @brief Obtain a job from the mgr pool.
 * @attention Ensure that the input parameter mgr is not null.
 * @param mgr [IN] Multi-stream parallel computing management structure, including the job pool.
 * @return Return job, if successful.
 *         Return NULL, if failed.
 */
void *CRYPT_MPA_MallocJob(CRYPT_MPA_Mgr *mgr);

/**
 * @ingroup crypt_mpa_aead
 * @brief Submit the job to the scheduling layer
 * @attention The caller need ensure that the input parameters mgr and job are not null
 * @param mgr [IN] Multi-stream parallel computing management structure, including the job pool.
 * @param job [IN] Multi-stream parallel scheduling unit (The following algorithms are supported at present:
 *                 hmac, aes, aes-hmac)
 * @return The calculated job is returned, if the job calculation is complete, the user need to release the job.
 *         NULL is returned, if calculation completed without job is complete.
 */
void *CRYPT_MPA_CommitJob(CRYPT_MPA_Mgr *mgr, CRYPT_MPA_Job *job);

/**
 * @ingroup crypt_mpa_aead
 * @brief Initialize the multi-stream parallel computing module.
 * @return 0 is returned, if initialization succeeds.
 *         Other values are returned, if initialization fails.
 */
uint32_t CRYPT_MPA_Init(void);

/**
 * @ingroup crypt_mpa_aead
 * @brief init mgr resource
 * @param mgr [IN] Multi-stream parallel computing management structure, including the job pool.
 * @param order [IN] Specify the sequence of encryption, decryption, and authentication.
 * @return 0 is returned, if initialization succeeds.
 *         Other values are returned, if initialization fails.
 */
uint32_t CRYPT_MPA_InitMgr(CRYPT_MPA_Mgr *mgr, CRYPT_MPA_AeadOrder order);

/**
 * @ingroup crypt_mpa_aead
 * @brief Apply for the mgr resource
 * @return mgr is returned, if the operation is successful.
 *         NULL is returned, if the operation fails.
 */
CRYPT_MPA_Mgr *CRYPT_MPA_MallocMgr(void);

/**
 * @ingroup crypt_mpa_aead
 * @brief Release the mgr resource, this interface is used together with the CRYPT_MPA_MallocMgr interface.
 * @param mgr [IN] Multi-stream parallel computing management structure, including the job pool.
 * @return NONE
 */
void CRYPT_MPA_FreeMgr(CRYPT_MPA_Mgr *mgr);

/**
 * @ingroup crypt_mpa_aead
 * @brief Release job resources.
 *        If the CRYPT_MPA_CommitJob and CRYPT_MPA_FetchJob interfaces return job resources, you need to
 * call this API to release job resources.
 * @attention The caller need ensure that the input parameters mgr and job are not NULL.
 * @param mgr [IN] Multi-stream parallel computing management structure, including the job pool.
 * @param job [IN]  Multi-stream parallel scheduling unit.
 * (the following algorithms are supported at present: hmac，aes，aes-hmac)
 * @return NONE
 */
void CRYPT_MPA_FreeJob(CRYPT_MPA_Mgr *mgr, CRYPT_MPA_Job *job);

/**
 * @ingroup crypt_mpa_aead
 * @brief Forcibly process the job and return the processed job, one job is returned each time until all
 * the jobs are processed.
 * @attention The caller need ensure that the input parameter mgr is not NULL.
 * @param mgr [IN] Multi-stream parallel computing management structure, including the job pool.
 * @return  The processed jobs are returned, if there are unprocessed jobs submitted by the user.
 *          NULL is returned, if all jobs are processed.
 *          Note that the user needs to release the returned job.
 */
void *CRYPT_MPA_FetchJob(CRYPT_MPA_Mgr *mgr);

/**
 * @ingroup crypt_mpa_aead
 * @brief AES extended key algorithm
 * @param aesMode [IN] Specify the AES mode.
 * @param key [IN] AES original key.
 *                 aes128: 16 bytes; aes192: 24 bytes; aes256: 32 bytes.
 * @param encExpKey [OUT] Encryption extended key Start address, users must ensure 16-byte alignment.
 * @param decExpKey [OUT] Decryption extended key start address, users must ensure 16-byte alignment.
 * @return The value 0 is returned, if the operation is successful.
 *         Other values are returned, if the operation fails.
 */
uint32_t CRYPT_MPA_AesExpsionKey(CRYPT_MPA_AesMode aesMode, const uint8_t *key, uint8_t *encExpKey, uint8_t *decExpKey);

/**
 * @ingroup crypt_mpa_aead
 * @brief hmac-sha2 algorithm opadkey ipadkey hash calculation.
 * Note this API does not padding, but before using this API, you should call CRYPT_MPA_Init for initialization.
 * @param hashAlg [IN] Specifies the hash mode.
 * @param key [IN] HMAC key
 * @param keyLen [IN] HMAC key length
 * @param opadKeyHash [OUT] opay key hash calculation result. SHA256 requires 32 bytes, SHA512 algorithm requires
 * 64 bytes of space.
 * @param opadKeyHashLen [IN] Specifies the length of the opadKeyHash.
 * @param ipadKeyHash [OUT] ipay key hash calculation result. SHA256 requires 32 bytes. SHA512 algorithm requires
 * 64-byte space.
 * @param ipadKeyHashLen [IN] Specifies the ipadKeyHash length.
 * @return The value 0 is returned, if the operation is successful.
 *         Other values are returned, if the operation fails.
 */
uint32_t CRYPT_MPA_PadKeyHashNoPadding(CRYPT_MPA_HashAlg hashAlg, const uint8_t *key, uint32_t keyLen,
    uint8_t *opadKeyHash, uint32_t opadKeyHashLen, uint8_t *ipadKeyHash, uint32_t ipadKeyHashLen);

#ifdef __cplusplus
}
#endif

#endif