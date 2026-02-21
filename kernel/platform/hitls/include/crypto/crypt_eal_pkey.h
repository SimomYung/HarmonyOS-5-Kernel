/**
 * @defgroup    crypt_eal_pkey  crypt_eal_pkey.h
 * @ingroup     crypto
 * @copyright   Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * @brief       Asymmetric algorithm external interface file
 */

#ifndef CRYPT_EAL_PKEY_H
#define CRYPT_EAL_PKEY_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "crypt_algid.h"
#include "crypt_types.h"
#include "crypt_eal_pkey.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @ingroup crypt_eal_pkey
 *
 * EAL public key structure
 */
typedef struct {
    CRYPT_PKEY_AlgId id; /**< Public Key Algorithm ID */
    union {
        CRYPT_RsaPub rsaPub; /**< RSA public key structure */
        CRYPT_DsaPub dsaPub; /**< DSA public key structure */
        CRYPT_DhPub dhPub;   /**< DH public key structure */
        CRYPT_EccPub eccPub; /**< ECC public key structure */
        CRYPT_Curve25519Pub curve25519Pub; /**< ed25519/x25519 public key structure */
        CRYPT_Curve448Pub curve448Pub; /**< ed448/x448 public key structure */
        CRYPT_Sm9Pub sm9Pub; /**< sm9 public key structure */
    }key;                           /**< Public key union of all algorithms */
} CRYPT_EAL_PkeyPub;

/**
 * @ingroup crypt_eal_pkey
 *
 * EAL private key structure
 */
typedef struct {
    CRYPT_PKEY_AlgId id; /**< private key algorithm ID */
    union {
        CRYPT_RsaPrv rsaPrv; /**< RSA private key structure */
        CRYPT_DsaPrv dsaPrv; /**< DSA private key structure */
        CRYPT_DhPrv  dhPrv;  /**< DH private key structure */
        CRYPT_EccPrv eccPrv; /**< ECC private key structure */
        CRYPT_Curve25519Prv curve25519Prv; /**< ed25519/x25519 private key structure */
        CRYPT_Curve448Prv curve448Prv; /**< ed448/x448 private key structure */
        CRYPT_Sm9Prv sm9Prv; /**< sm9 private key structure */
    } key;                           /**<Private key union of all algorithms */
} CRYPT_EAL_PkeyPrv;

/**
 * @ingroup crypt_eal_pkey
 *
 * Structure used by the Para parameter of the asymmetric algorithm, including the algorithm ID and the
 * para combination of the corresponding algorithm.
 */
typedef struct {
    CRYPT_PKEY_AlgId id; /**< asymmetric algorithm ID */
    union {
        CRYPT_RsaPara rsaPara; /**< RSA Para structure */
        CRYPT_DsaPara dsaPara; /**< DSA Para structure */
        CRYPT_DhPara  dhPara;  /**< DH Para structure */
        CRYPT_EccPara eccPara; /**< ECC Para structure */
    } para;                            /**<Para union of all algorithms */
} CRYPT_EAL_PkeyPara;

/**
 * @ingroup  crypt_eal_pkey
 *
 * Pkey session structure.
 */
typedef struct EAL_PkeyCtx CRYPT_EAL_PkeyCtx;

/**
 * @ingroup crypt_eal_pkey
 * @brief   Check whether the id is valid asymmetric algorithm ID.
 *
 * @param   id [IN] Asymmetric algorithm ID
 *
 * @return   true, if the value is valid.
 *           false, if the value is invalid.
 */
bool CRYPT_EAL_PkeyIsValidAlgId(CRYPT_PKEY_AlgId id);

/* Pkey external interface */
/**
 * @ingroup crypt_eal_pkey
 * @brief   Create an asymmetric key pair structure.
 *
 * @param   id [IN] Algorithm ID
 *
 * @return  CRYPT_EAL_PkeyCtx pointer.
 *          NULL, if the operation fails.
 */
CRYPT_EAL_PkeyCtx *CRYPT_EAL_PkeyNewCtx(CRYPT_PKEY_AlgId id);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Copy the pkey context.
 *
 * @param   to [IN/OUT] Target pkey context
 * @param   from [IN] Source pkey context
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyCopyCtx(CRYPT_EAL_PkeyCtx *to, const CRYPT_EAL_PkeyCtx *from);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Copy the Pkey context.
 *          After the duplication is complete, call the CRYPT_EAL_PkeyFreeCtx interface to release the memory.
 *
 * @param   ctx [IN] Source Pkey context
 *
 * @return  CRYPT_EAL_PkeyCtx, Pkey context pointer.
 *          NULL, if the operation fails.
 */
CRYPT_EAL_PkeyCtx *CRYPT_EAL_PkeyDupCtx(const CRYPT_EAL_PkeyCtx *pkey);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Release the asymmetric key pair structure.
 *
 * @param   pkey [IN] Pkey context, which need to be set NULL by the caller.
 *
 * @return  None
 */
void CRYPT_EAL_PkeyFreeCtx(CRYPT_EAL_PkeyCtx *pkey);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Set the key parameters, the key parameter marked as "para" is applied for and released by the caller.
 *
 * @param   pkey [IN/OUT] Structure of the key pair to be set
 * @param   para [IN] Parameter
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeySetPara(CRYPT_EAL_PkeyCtx *pkey, const CRYPT_EAL_PkeyPara *para);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Obtain the key parameter, the key parameter marked as "para" is applied for and released by the caller.
 *
 * @param   pkey [IN] Key pair structure
 * @param   para [OUT] Parameter to be received
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyGetPara(const CRYPT_EAL_PkeyCtx *pkey, CRYPT_EAL_PkeyPara *para);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Set key parameters.
 *
 * @param   pkey [IN/OUT] Structure of the key pair to be set.
 * @param   id [IN] Parameter ID.
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeySetParaById(CRYPT_EAL_PkeyCtx *pkey, CRYPT_PKEY_ParaId id);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Generate the key data.
 *
 * @param   pkey [IN/OUT] Key pair structure for receiving key data.
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyGen(CRYPT_EAL_PkeyCtx *pkey);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Set the public key. The caller applies for and releases memory for the public key marked as "key".
 *
 * @param   pkey [OUT] Key pair structure for receiving key data
 * @param   key  [IN] Public key data
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeySetPub(CRYPT_EAL_PkeyCtx *pkey, const CRYPT_EAL_PkeyPub *key);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Set the private key. The caller applies for and releases memory for the private key marked as "key".
 *
 * @param   pkey [OUT] Key pair structure for receiving key data
 * @param   key  [IN] Private key data
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeySetPrv(CRYPT_EAL_PkeyCtx *pkey, const CRYPT_EAL_PkeyPrv *key);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Obtain the public key. The caller applies for and releases memory for the public key marked as "key".
 *
 * @param   pkey [IN] Key session
 * @param   key  [OUT] Public key data
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyGetPub(const CRYPT_EAL_PkeyCtx *pkey, CRYPT_EAL_PkeyPub *key);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Obtain the private key. The caller applies for and releases memory for the private key marked as "key".
 *
 * @param   pkey [IN] Key session
 * @param   key  [OUT] Private key data
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyGetPrv(const CRYPT_EAL_PkeyCtx *pkey, CRYPT_EAL_PkeyPrv *key);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Signature interface
 *
 * @param   pkey     [IN] Key session
 * @param   id       [IN] Hash algorithm ID.
 * @param   data     [IN] Plaintext data
 * @param   dataLen  [IN] Plaintext length. The maximum length is [0, 0xffffffff].
 * @param   sign     [OUT] Signature data. The length of the memory buff used to save the signature must be
 * greater than or equal to the key modulo length.
 * @param   signLen  [OUT/IN] Length of the signature data, You can obtain the value by calling
 * CRYPT_EAL_PkeyGetSignLen.
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeySign(const CRYPT_EAL_PkeyCtx *pkey, CRYPT_MD_AlgId id, const uint8_t *data,
    uint32_t dataLen, uint8_t *sign, uint32_t *signLen);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Signature verification interface
 *
 * @param   pkey      [IN] Key session
 * @param   id        [IN] Hash algorithm ID.
 * @param   data      [IN] Plaintext data
 * @param   dataLen   [IN] Plaintext length. The maximum length is [0,0xffffffff].
 * @param   sign      [IN] Signature data
 * @param   signLen   [IN] Length of the signature data
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyVerify(const CRYPT_EAL_PkeyCtx *pkey, CRYPT_MD_AlgId id, const uint8_t *data,
    uint32_t dataLen, const uint8_t *sign, uint32_t signLen);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Signature recover interface
 *
 * @param   pkey      [IN] Key session
 * @param   sign      [IN] Signature data
 * @param   signLen   [IN] Length of the signature data
 * @param   data      [OUT] Signature recovery data.
 * @param   dataLen   [OUT] Length of the signature recovery data, the maximum length is [0,0xffffffff].
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyVerifyRecover(const CRYPT_EAL_PkeyCtx *pkey, const uint8_t *sign,
    uint32_t signLen, uint8_t *data, uint32_t *dataLen);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Sign hash data
 *
 * @param   pkey      [IN] Key session
 * @param   hash      [IN] Hash data
 * @param   hashLen   [IN] Hash length.
 *                         When RSA is used for signature, the hash length should correspond to the
 *                         digest length of the hash algorithm on which the padding method depends.
 * @param   sign      [OUT] Signature data. The length of the memory buff used to save the signature
 *                          must be greater than or equal to the key module length.
 * @param   signLen   [OUT/IN]  Length of the signature data.
 *                              The value can be obtained by calling CRYPT_EAL_PkeyGetSignLen.
 *
 * @return if successful, return CRYPT_SUCCESS
 *         For other error codes see crypt_errno.h
 */
int32_t CRYPT_EAL_PkeySignData(const CRYPT_EAL_PkeyCtx *pkey, const uint8_t *hash, uint32_t hashLen,
    uint8_t *sign, uint32_t *signLen);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Verify the signature of the hash data
 *
 * @param   pkey     [IN] Key session
 * @param   hash     [IN] Hash data
 * @param   hashLen  [IN] Hash length.
 *                   When RSA is used for signature, the hash length should correspond to the digest
 *                   length of the hash algorithm on which the padding method depends.
 * @param   sign     [IN] Signature data
 * @param   signLen  [IN] Length of the signature data
 *
 * @return  CRYPT_SUCCESS, if successful.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyVerifyData(const CRYPT_EAL_PkeyCtx *pkey, const uint8_t *hash, uint32_t hashLen,
    const uint8_t *sign, uint32_t signLen);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Encrypt data.
 *
 * @param   pkey      [IN] Key session
 * @param   data      [IN] Input plaintext data.
 * @param   dataLen   [IN] Input plaintext data length.
 *
 * @param   out      [OUT] Encrypted data. The buff length of the memory used to store the encrypted data
 *                         must be greater than or equal to the key modulus length.
 * @param   outLen   [OUT/IN] Encrypted data length.
 *
 * @return  CRYPT_SUCCESS, if successful.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyEncrypt(const CRYPT_EAL_PkeyCtx *pkey, const uint8_t *data, uint32_t dataLen,
    uint8_t *out, uint32_t *outLen);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Decrypt the data.
 *
 * @param   pkey      [IN] Key session
 * @param   data      [IN] Input ciphertext data.
 * @param   dataLen   [IN] Input ciphertext data length.
 *
 * @param   out      [OUT] Decrypted data
 * @param   outLen   [OUT/IN] Length of the decrypted data.
 *
 * @return  CRYPT_SUCCESS, if successful.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyDecrypt(const CRYPT_EAL_PkeyCtx *pkey, const uint8_t *data, uint32_t dataLen,
    uint8_t *out, uint32_t *outLen);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Check whether the public and private keys match.
 *
 * @param   pubKey      [IN] Public key
 * @param   prvKey      [IN] private key
 *
 * @return  CRYPT_SUCCESS, if successful.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyPairCheck(CRYPT_EAL_PkeyCtx *pubKey, CRYPT_EAL_PkeyCtx *prvKey);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Compute the shared key.
 *
 * @param   pkey         [IN] Key session
 * @param   pubKey       [IN] Public key session
 * @param   share        [OUT] Shared key
 * @param   shareLen     [IN/OUT] The input parameter is the share space length, and the output parameter is the
 * valid share space length, the required space can be obtained by calling the CRYPT_EAL_PkeyGetKeyLen interface.
 *
 * @return  CRYPT_SUCCESS, if successful.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyComputeShareKey(const CRYPT_EAL_PkeyCtx *pkey, const CRYPT_EAL_PkeyCtx *pubKey,
    uint8_t *share, uint32_t *shareLen);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Obtain the number of bytes in the key length. Only supports CRYPT_PKEY_RSA and CRYPT_PKEY_ECDSA.
 *
 * @param   pkey [IN] Key session
 *
 * @return  Key length, if successful.
 *          0, if failed.
 */
uint32_t CRYPT_EAL_PkeyGetKeyLen(const CRYPT_EAL_PkeyCtx *pkey);

/**
 * @ingroup  crypt_eal_pkey
 * @brief    Obtain the key security strength.
 *
 * @param   pkey [IN] Key session
 *
 * @return  Key security strength, if successful.
 *          0, if failed.
 */
uint32_t CRYPT_EAL_PkeyGetSecurityBits(const CRYPT_EAL_PkeyCtx *pkey);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Obtain the number of bits in the key length.
 *
 * @param   pkey [IN] Key session
 *
 * @return  Number of key bits, if successful.
 *          0, if failed.
 */
uint32_t CRYPT_EAL_PkeyGetKeyBits(const CRYPT_EAL_PkeyCtx *pkey);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Obtains the signature length of the key for signature, only support algorithm that can be signed.
 *
 * @param   pkey [IN] Key session
 *
 * @return  Signature length, if successful.
 *          0, if failed.
 */
uint32_t CRYPT_EAL_PkeyGetSignLen(const CRYPT_EAL_PkeyCtx *pkey);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Make specific option for setting/obtain, supported option can see the structure of CRYPT_PkeyCtrl.
 *
 * @param   pkey [IN] Key session
 * @param   opt [IN] Option information
 * @param   val [IN/OUT] Data to be set/obtained
 * @param   len [IN] Length of the data marked as "val"
 *
 * @return  CRYPT_SUCCESS, if successful.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyCtrl(CRYPT_EAL_PkeyCtx *pkey, CRYPT_PkeyCtrl opt, void *val, uint32_t len);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Obtain the key algorithm type.
 *
 * @param   pkey [IN] Key session
 *
 * @return  Key algorithm type
 */
CRYPT_PKEY_AlgId CRYPT_EAL_PkeyGetId(const CRYPT_EAL_PkeyCtx *pkey);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Obtain the key algorithm parameter ID.
 *
 * @param   pkey [IN] Key session
 *
 * @return  Algorithm parameter ID
 */
CRYPT_PKEY_ParaId CRYPT_EAL_PkeyGetParaId(const CRYPT_EAL_PkeyCtx *pkey);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Check the key pair consistency. Only supports CRYPT_PKEY_DH.
 *
 * @param   pkey [IN] Key session
 *
 * @return  CRYPT_SUCCESS, if successful.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyCheck(const CRYPT_EAL_PkeyCtx *pkey);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Compare keys or parameters
 *
 * @param   a [IN] Key session
 * @param   b [IN] Key session
 *
 * @return  CRYPT_SUCCESS, a and b are the same(include both a and b are null)
 *          CRYPT_NULL_INPUT, incorrect null pointer input.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyCmp(const CRYPT_EAL_PkeyCtx *a, const CRYPT_EAL_PkeyCtx *b);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Set the user's personal data.
 *
 * @param   pkey [IN] Key session
 * @param   data [IN] Pointer to the user's personal data
 *
 * @return  CRYPT_SUCCESS, if successful.
 *          CRYPT_NULL_INPUT, if pkey is NULL.
 */
int32_t CRYPT_EAL_PkeySetExtData(CRYPT_EAL_PkeyCtx *pkey, void *data);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Obtain the user's personal data.
 *
 * @param   pkey [IN] Key session
 *
 * @return  void*(user personal data pointer), which indicates successful.
 *          NULL, which indicates failed.
 */
void *CRYPT_EAL_PkeyGetExtData(const CRYPT_EAL_PkeyCtx *pkey);


/**
 * @ingroup crypt_eal_pkey
 * @brief   Key pairing consistency test
 *
 * @param   pkey     [IN] Key pair structure
 *
 * @return   true, consistent.
 *           false, non-consistent.
 */
typedef bool (*CRYPT_EAL_Pct)(CRYPT_EAL_PkeyCtx *pkey);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Key pair consistency test callback registration interface.
 * After the CMVP feature is enabled, the CMVP provides the default implementation, this callback is called
 * after the EAL generates an asymmetric key pair, and if the callback fails, the EAL interface fails.
 *
 * @param   func     [IN] Key pair consistency test callback
 *
 * @return  None
 */
void CRYPT_EAL_RegPct(CRYPT_EAL_Pct func);
/**
 * @ingroup crypt_eal_pkey
 *
 * Asymmetric compliance checking data
 */
typedef struct {
    const CRYPT_EAL_PkeyPara *para; /**< CRYPT_EAL_PkeyPara structure */
    const CRYPT_EAL_PkeyPub *pub; /**< CRYPT_EAL_PkeyPub structure */
    const CRYPT_EAL_PkeyPrv *prv; /**< CRYPT_EAL_PkeyPrv structure */
    CRYPT_MD_AlgId mdId; /**< Hash algorithm ID */
    CRYPT_PKEY_ParaId paraId; /**< Asymmetric algorithm parameter ID */
    CRYPT_EVENT_TYPE oper; /**< Operation that is being performed. */
    const CRYPT_RSA_PkcsV15Para *pkcsv15; /**< CRYPT_RSA_PkcsV15Para structure */
    const CRYPT_RSA_PssPara *pss; /**< CRYPT_RSA_PssPara structure */
    const CRYPT_RSA_OaepPara *oaep; /**< CRYPT_RSA_OaepPara structure */
} CRYPT_EAL_PkeyC2Data;

/**
  * @ingroup crypt_eal_pkey
  * @brief  Asymmetric compliance checking
  *
  * @param id [IN] Asymmetric algorithm ID.
  * @param data [IN] Data
  *
  * @return  true, test successful.
  *          false, test failed.
  */
typedef bool (*CRYPT_EAL_PkeyC2)(CRYPT_PKEY_AlgId id, const CRYPT_EAL_PkeyC2Data *data);

/**
 * @ingroup crypt_eal_pkey
 * @brief   Callback registration interface for asymmetric compliance checking, which is used to check
 * whether algorithm parameters are compliant and whether the module status is available, and the CMVP
 * provides the default implementation after the CMVP feature is enabled.
 *     The asymmetric EAL interface calls this callback before excute an operation, and if the callback
 * fails, the EAL interface fails.
 *     After the CMVP feature is enabled, the CMVP provides the default implementation.
 *     Any algorithm can be used in non-approved mode.
 *     In ISO19790 mode, only support CRYPT_PKEY_DSA/CRYPT_PKEY_X25519/CRYPT_PKEY_RSA/CRYPT_PKEY_DH/
 * CRYPT_PKEY_ECDSA/CRYPT_PKEY_ECDH algorithm.
 *     RSA key length must be at least 2048 bits.
 *     In the DSA algorithm, p must be at least 2048 bits, and q must be at least 224 bits.
 *     The value of p in the ECDSA algorithm is at least 224 bits, and the curve supports only P-224/P-256/P-384/P-521.
 *     In the DH algorithm, p must be at least 2048 bits, and q must be at least 256 bits.
 *     The ECDH algorithm curve supports only P-224/P-256/P-384/P-521.
 *     Signature support: CRYPT_MD_SHA224/CRYPT_MD_SHA256/CRYPT_MD_SHA384/CRYPT_MD_SHA512.
 *     verification support: CRYPT_MD_SHA1/CRYPT_MD_SHA224/CRYPT_MD_SHA256/CRYPT_MD_SHA384/CRYPT_MD_SHA512.
 *
 * @param   func     [IN] Asymmetric compliance checking callback
 *
 * @return  None
 */
void CRYPT_EAL_RegPkeyC2(CRYPT_EAL_PkeyC2 func);

/**
 * @ingroup crypt_eal_pkey
 * @brief   EAL layer reference counting auto-increment
 *
 * @param   pkey [IN] Key session
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyUpRef(CRYPT_EAL_PkeyCtx *pkey);

/**
 * @ingroup crypt_eal_pkey
 * @brief   copy param from src to dest.
 *
 * @param   src [IN] Key session
 * @param   dest [IN] Key session
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes see crypt_errno.h.
 */
int32_t CRYPT_EAL_PkeyCopyParam(const CRYPT_EAL_PkeyCtx *src, const CRYPT_EAL_PkeyCtx *dest);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // CRYPT_EAL_PKEY_H
