/**
 * @defgroup    crypt_eal_rand  crypt_eal_rand.h
 * @ingroup     crypto
 * @copyright   Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * @brief       RAND algorithm external interface file
 */

#ifndef CRYPT_EAL_RAND_H
#define CRYPT_EAL_RAND_H

#include <stdbool.h>
#include <stdint.h>
#include "crypt_algid.h"
#include "crypt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup crypt_eal_rand
 * @brief Random compliance checking
 *
 * id: random algorithm ID.
 *
 * @return true, if successful.
 *         false, if failed.
 */
typedef bool (*CRYPT_EAL_RandC2)(CRYPT_RAND_AlgId id);

/**
  * @ingroup crypt_eal_rand
  * @brief rand generate callback
  *
  * rand：string
  * randLen： len
  *
  * @return  int32_t, defined by users.
  */
typedef int32_t (*CRYPT_EAL_RandFunc)(uint8_t *rand, uint32_t randLen);

/**
 * @ingroup crypt_eal_rand
 * @brief   Registration interface for checking the compliance of the random number algorithm, which is used to check
 * whether the module status is available.
 *     After the CMVP feature is enabled, the CMVP provides the default implementation.
 *     The random number interface of the EAL calls CRYPT_EAL_RegRandC2 before excute an operation, if the callback
 * fails will cause CRYPT_EAL_RandInit/CRYPT_EAL_RandbytesWithAdin/CRYPT_EAL_Randbytes/CRYPT_EAL_RandSeedWithAdin/
 * CRYPT_EAL_RandSeed interface fails.
 *     In non-approved mode, Any algorithm can be used.
 *     In ISO19790 mode, only support CRYPT_MD_SHA224/CRYPT_MD_SHA256/CRYPT_MD_SHA384/CRYPT_MD_SHA512 algorithm.
 *
 * @param   func     [IN] Random number compliance checking
 *
 * @return None
 */
void CRYPT_EAL_RegRandC2(CRYPT_EAL_RandC2 func);

/**
 * @ingroup crypt_eal_rand
 * @brief Random number initialization interface. This interface does not support multiple threads.
 * @par
 *      Initialize global random number to RAND, Entropy sources and addtional random numbers in the seed material
 * which implemented by HiTLS. and this value is provided by the user. if user not provid the entropy source
 * (seedMeth and seedCtx are both NULL), the default software entropy source is used.
 *      In addition, this interface does not support multiple threads.
 *      The global random number is initialized to the random generation algorithm described in Nist 800-90a.
 *      Application scenarios are as follows:
 *      1. seedMeth == NULL && seedCtx == NULL ====> Use the default system entropy source in AES_CTR mode
 * (that is, non-DF cannot use the default entropy source).
 *      2. seedMeth == NULL && seedCtx != NULL ===> Error report..
 *      3. seedMeth != NULL ====> This function can be used normally, seedCtx is not restricted, but make sure
 * seedMeth can handle all kinds of situations.
 *
 * @attention:  Support obtain or generate random numbers with multithreading, but not support initialization
 * and deinitialization with multithreading.
 * @param id [IN] RAND id
 * @param seedMeth [IN] Seed method, which can be set NULL with seedCtx, The default entropy source is used
 * or provided by the user.
 * @param seedCtx [IN] Seed context information, which can be set NULL, But the seedMeth provided by the user can
 * handle the situation where seedCtx is NULL.
 * Generally, seedCtx needs to contain data such as entropy and nonce.
 * @param pers [IN] Personal data, which can be NULL.
 * @param persLen [IN] Length of the personal data, the length ranges from [0,0x7FFFFFF0].
 * @return CRYPT_SUCCESS, if successful.
 *         For other error codes, see the crypt_errno.h file.
 */
int32_t CRYPT_EAL_RandInit(CRYPT_RAND_AlgId id, CRYPT_RandSeedMethod *seedMeth, void *seedCtx,
    const uint8_t *pers, uint32_t persLen);

/**
 * @ingroup crypt_eal_rand
 * @brief   Deinitializing the global RAND interface, this interface does not support multiple threads.
 * @return  void, no return value.
 */
void CRYPT_EAL_RandDeinit(void);

/**
 * @ingroup crypt_eal_rand
 * @brief   Generate a random number.
 * The addtional data marked as "addin" can be NULL, and additional data specified by the user.
 * This interface does not support multiple threads.
 *
 * @param byte  [OUT] Output random numbers, the memory is provided by the user.
 * @param len   [IN] Required random number length, the maximum length is (0, 65536].
 * @param addin [IN] Addtional data, which can set be NULL.
 * @param addinLen [IN] Addtional data length, the maximum length is[0,0x7FFFFFF0].
 * @return CRYPT_SUCCESS, if successful.
 *         For other error codes, see the crypt_errno.h file.
 */
int32_t CRYPT_EAL_RandbytesWithAdin(uint8_t *byte, uint32_t len, uint8_t *addin, uint32_t addinLen);

/**
 * @ingroup crypt_eal_rand
 * @brief Generate a random number, which is equivalent to CRYPT_EAL_RandbytesWithAdin(bytes, len, NULL, 0).
 * This interface supports multi-thread access.
 *
 * @param byte [OUT] Used to store output random numbers, the memory is provided by the user.
 * @param len  [IN] Required random number length, the length range is(0, 65536].
 * @return CRYPT_SUCCESS, if successful.
 *         For other error codes, see the crypt_errno.h file.
 */
int32_t CRYPT_EAL_Randbytes(uint8_t *byte, uint32_t len);

/**
 * @ingroup crypt_eal_rand
 * @brief Regenerate the seed.
 * The addtional data can set be NULL, and this interface supports multi-thread access.
 *
 * @param addin [IN] Additional data, which can set be NULL.
 * @param addinLen [IN] Addtional data length, the range is [0,0x7FFFFFF0].
 * @return CRYPT_SUCCESS, if successful.
 *         For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_RandSeedWithAdin(uint8_t *addin, uint32_t addinLen);

/**
 * @ingroup crypt_eal_rand
 * @brief Regenerate the seed, which is equivalent to CRYPT_EAL_RandSeedWithAdin(NULL, 0), and the interface
 * supports multi-thread access.
 *
 * @return  CRYPT_SUCCESS
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_RandSeed(void);

typedef struct EAL_RndCtx CRYPT_EAL_RndCtx;

/**
 * @ingroup CRYPT_EAL_DrbgDeinit
 * @brief   CRYPT_EAL_DrbgDeinit Deinitialization interface, this interface does not support multiple threads.
 * @return Void, no value is returned.
 */
void CRYPT_EAL_DrbgDeinit(CRYPT_EAL_RndCtx *ctx);

/**
 * @ingroup crypt_eal_rand
 * @brief Generate a random number.
 * The addtional data can be NULL, user specifies the addtional data, and the interface supports multi-thread access.
 *
 * @param drbgCtx  [IN] DRBG handle
 * @param byte     [OUT] Outputs random numbers. the memory is provided by the user.
 * @param len      [IN] Required random number length. the range is (0, 65536].
 * @param addin    [IN] Addtional data, which can be NULL.
 * @param addinLen [IN] Addtional data length. the range is [0,0x7FFFFFF0].
 * @return  CRYPT_SUCCESS, if successful.
 *          For other error codes, see the crypt_errno.h file.
 */
int32_t CRYPT_EAL_DrbgbytesWithAdin(CRYPT_EAL_RndCtx *ctx, uint8_t *byte, uint32_t len, uint8_t *addin, uint32_t addinLen);

/**
 * @ingroup crypt_eal_rand
 * @brief set rand fun callback.
 *
 * @param func  [IN] random func, defined by users
 * @return  void.
 */
void CRYPT_EAL_SetRandCallBack(CRYPT_EAL_RandFunc func);

/**
 * @ingroup crypt_eal_rand
 * @brief Generate a random number, which is equivalent to CRYPT_EAL_DrbgbytesWithAdin(ctx, byte, len, NULL, 0).
 * This interface supports multi-thread access.
 *
 * @param drbgCtx  [IN] DRBG handle
 * @param byte     [OUT] Used to store output random numbers. the memory is provided by the user.
 * @param len      [IN] Required random number length. the range is (0, 65536].
 * @return CRYPT_SUCCESS, if successful.
 *         For other error codes, see the crypt_errno.h file.
 */
int32_t CRYPT_EAL_Drbgbytes(CRYPT_EAL_RndCtx *ctx, uint8_t *byte, uint32_t len);

/**
 * @ingroup crypt_eal_rand
 * @brief Regenerate the seed. The addtional data can be NULL. This interface supports multi-thread access.
 *
 * @param drbgCtx  [IN] DRBG handle
 * @param addin    [IN] Addtional data, which can be null.
 * @param addinLen [IN] Addtional data length. The maximum length is [0,0x7FFFFFF0].
 * @return CRYPT_SUCCESS, if successful.
 *         For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_DrbgSeedWithAdin(CRYPT_EAL_RndCtx *ctx, uint8_t *addin, uint32_t addinLen);

/**
 * @ingroup crypt_eal_rand
 * @brief Regenerate the seed, which is equivalent to CRYPT_EAL_DrbgSeedWithAdin(ctx, NULL, 0).
 * This interface supports multi-thread access.
 *
 * @param drbgCtx  [IN] DRBG handle.
 * @return  CRYPT_SUCCESS, if successful.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_DrbgSeed(CRYPT_EAL_RndCtx *ctx);

/**
 * @ingroup crypt_eal_rand
 * @brief   Check whether the id is valid Rand algorithm ID.
 *
 * @param   id [IN] Rand algorithm ID.
 *
 * @return true, if valid.
 *         false, if invalid.
 */
bool CRYPT_EAL_RandIsValidAlgId(CRYPT_RAND_AlgId id);

/**
 * @ingroup crypt_eal_rand
 * @brief   Create a DRBG context
 *
 * @param   id [IN] Rand algorithm ID.
 * @param   seedMeth [IN] Metohd structure of the RAND registration.
 * @param   seedCtx [IN] Context for DRBG methods.
 *
 * @return CRYPT_EAL_RndCtx, if created a context successfully.
 *         NULL, if failed to create a context.
 */
CRYPT_EAL_RndCtx *CRYPT_EAL_DrbgNew(CRYPT_RAND_AlgId id, CRYPT_RandSeedMethod *seedMeth, void *seedCtx);

/**
 * @ingroup crypt_eal_rand
 * @brief Instantiating a DRBG based on personalization string.
 * @brief This API does not support multiple threads.
 *
 * @param pers    Personalization string. The personalization string can be NULL.
 * @param persLen   Personalization string length
 *
 * @retval CRYPT_SUCCESS                  Instantiation succeeded.
 * @retval CRYPT_EAL_ERR_DRBG_REPEAT_INIT DRBG is initialized repeatedly.
 * @retval CRYPT_CMVP_NOT_APPROVED        Does not meet the standard requirements.
 * @retval CRYPT_DRBG_FAIL_GET_ENTROPY  Failed to obtain the entropy.
 * @retval CRYPT_DRBG_FAIL_GET_NONCE    Failed to obtain the nonce.
 * @retval Hash function error code:    Failed to invoke the hash function.
 */
int32_t CRYPT_EAL_DrbgInstantiate(CRYPT_EAL_RndCtx *rndCtx, const uint8_t *pers, uint32_t persLen);

/**
 * @ingroup crypt_eal_rand
 * @brief Set reseed properties for context
 *
 * @param opt rndCtx context to be set
 * @param opt options
 * @param val value
 * @param len length for val
 *
 * @retval CRYPT_SUCCESS    set successfully.
 * @retval CRYPT_INVALID_ARG Invalid arguments
 */
int32_t CRYPT_EAL_DrbgCtrl(CRYPT_EAL_RndCtx *rndCtx, int32_t opt, void *val, uint32_t len);

#ifdef __cplusplus
}
#endif
#endif
