/**
  * @defgroup    crypt_eal_md    crypt_eal_md.h
  * @ingroup     crypto
  * @copyright   Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
  * @brief       Message Digest (MD) Algorithm external interface file
  */

#ifndef CRYPT_EAL_MD_H
#define CRYPT_EAL_MD_H

#include <stdbool.h>
#include <stdint.h>
#include "crypt_algid.h"
#include "crypt_types.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct EAL_MdCtx CRYPT_EAL_MdCTX;

/**
  * @ingroup crypt_eal_md
  * @brief Hash compliance checking
  *
  * @param   id [IN] Hash algorithm ID.
  *
  * @return  true, if successful.
  *          false, if failed.
  */
typedef bool (*CRYPT_EAL_MdC2)(CRYPT_MD_AlgId id);

/**
 * @ingroup crypt_eal_md
 * @brief Callback registration interface for checking the hash compliance, which is used to check
 * whether the algorithm parameters are compliant and whether the module status is available. the EAL
 * hash interface calls this callback before excute the operation.
 *    If the callback fails, the CRYPT_EAL_MdNewCtx/CRYPT_EAL_MdInit/CRYPT_EAL_MdUpdate/CRYPT_EAL_MdFinal
 * interface fails. After the CMVP feature is enabled, the CMVP provides the default implementation.
 *    Any algorithm can be used in non-approved mode, And only CRYPT_MD_SHA224/CRYPT_MD_SHA256/
 * CRYPT_MD_SHA384/CRYPT_MD_SHA512 is allowed in ISO19790 mode.
 *
 * @param   func     [IN] Hash compliance check callback
 *
 * @return  None
 */
void CRYPT_EAL_RegMdC2(CRYPT_EAL_MdC2 func);

/**
 * @ingroup crypt_eal_md
 * @brief   Create the MD context.
 * After the allocation is complete, call the CRYPT_EAL_MdFreeCtx interface to release the memory.
 *
 * @param   id [IN] Algorithm ID
 *
 * @return  CRYPT_EAL_MdCTX, MD context pointer.
 *          NULL, if the operation fails.
 */
CRYPT_EAL_MdCTX *CRYPT_EAL_MdNewCtx(CRYPT_MD_AlgId id);

/**
 * @ingroup crypt_eal_md
 * @brief Check whether the id is valid MD algorithm ID.
 *
 * @param   id [IN] MD algorithm ID.
 *
 * @return  true, If the value is valid.
 *          false, If the value is invalid.
 */
bool CRYPT_EAL_MdIsValidAlgId(CRYPT_MD_AlgId id);

/**
 * @ingroup crypt_eal_md
 * @brief   Return the MD algorithm ID.
 *
 * @param   pkey [IN] MD context
 *
 * @return  ID, MD algorithm ID.
 *          CRYPT_MD_MAX, which indicates invalid ID or the input parameter is null.
 */
CRYPT_MD_AlgId CRYPT_EAL_MdGetId(CRYPT_EAL_MdCTX *ctx);

/**
 * @ingroup crypt_eal_md
 * @brief  Copy the MD context.
 *
 * @param   to [IN/OUT] Target MD context
 * @param   from [IN] Source MD context
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_MdCopyCtx(CRYPT_EAL_MdCTX *to, const CRYPT_EAL_MdCTX *from);

/**
 * @ingroup crypt_eal_md
 * @brief   Copy the MD context.
 * Note that need to call the CRYPT_EAL_MdFreeCtx interface to release the memory After the duplication is complete.
 *
 * @param   ctx [IN] Source MD context
 *
 * @return  CRYPT_EAL_MdCTX, MD context pointer.
 *          NULL, if the operation fails.
 */
CRYPT_EAL_MdCTX *CRYPT_EAL_MdDupCtx(const CRYPT_EAL_MdCTX *ctx);

/**
 * @ingroup crypt_eal_md
 * @brief  Release the MD context.
 *
 * @param   ctx [IN] MD context. which is created by using the CRYPT_EAL_MdNewCtx interface and need to be set
 * NULL by caller.
 *
 * @return  Void, no return value.
 */
void CRYPT_EAL_MdFreeCtx(CRYPT_EAL_MdCTX *ctx);

/**
 * @ingroup crypt_eal_md
 * @brief  Initialize the MD context.
 *
 * @param   ctx [IN/OUT] MD context, which is created by using the CRYPT_EAL_MdNewCtx interface.
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_MdInit(CRYPT_EAL_MdCTX *ctx);

/**
 * @ingroup crypt_eal_md
 * @brief   Continuously input the data to be digested.
 *
 * @param   ctx [IN/OUT] MD context, which is created by using the CRYPT_EAL_MdNewCtx interface.
 * @param   data [IN] Data to be digested.
 * @param   len [IN] Data length.
 *                   The maximum length of sha384 and sha512 is [0, 2^128 bits).
 *                   The maximum total length of sha1, sha224, sha256, sm3, and md5 is [0, 2^64 bits).
 *                   The maximum length at a time is [0, 0xffffffff].
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_MdUpdate(CRYPT_EAL_MdCTX *ctx, const uint8_t *data, uint32_t len);

/**
 * @ingroup crypt_eal_md
 * @brief   Complete the digest and output the final digest result.
 *
 * @param   ctx [IN/OUT] MD context, which is created by using the CRYPT_EAL_MdNewCtx interface.
 * @param   out [OUT] Digest result cache, which needs to be created and managed by users.
 * @param   len [IN/OUT] The input parameter indicates the length of the buffer marked as "out", and the output
 * parameter indicates the valid length of the obtained "out". The length must be greater than or equal to
 * the hash length of the corresponding algorithm, the hash length can be obtained through the
 * CRYPT_EAL_MdGetDigestSize interface.
 * Requires user creation management.
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_MdFinal(CRYPT_EAL_MdCTX *ctx, uint8_t *out, uint32_t *len);

/**
 * @ingroup crypt_eal_md
 * @brief   Obtain the digest length of the algorithm output.
 *
 * @param   id [IN] Algorithm ID
 *
 * @return  Digest length, if successful.
 *          0, if failed(in this case, the ID is invalid).
 */
uint32_t CRYPT_EAL_MdGetDigestSize(CRYPT_MD_AlgId id);

/**
 * @ingroup crypt_eal_md
 * @brief   Calculate the data digest
 *
 * @param   id [IN] Algorithm ID
 * @param   in [IN] Data to be digested
 * @param   len [IN] Data length
 * @param   out [OUT] Digest result
 * @param   len [IN/OUT] The input parameter indicates the length of the buffer marked as "out", and the output
 * parameter indicates the valid length of the obtained "out".
 *
 * @return  CRYPT_SUCCESS.
 *          For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_Md(CRYPT_MD_AlgId id, const uint8_t *in, uint32_t inLen, uint8_t *out, uint32_t *outLen);

/**
 * @ingroup crypt_eal_md
 * @brief   Deinitialize the function.
 * If need to be calculated after the CRYPT_EAL_MdDeinit is called, it needs to be initialized again.
 * @param   ctx [IN] Md Context
 */
uint32_t CRYPT_EAL_MdDeinit(CRYPT_EAL_MdCTX *ctx);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // CRYPT_EAL_MD_H
