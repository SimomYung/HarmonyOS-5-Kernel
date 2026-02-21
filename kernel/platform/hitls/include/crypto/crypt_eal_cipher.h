/**
 * @defgroup    crypt_eal_cipher crypt_eal_cipher.h
 * @ingroup     crypto
 * @copyright   Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * @brief       External API of the symmetric encryption and decryption algorithm
 */

#ifndef CRYPT_EAL_CIPHER_H
#define CRYPT_EAL_CIPHER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "crypt_algid.h"
#include "crypt_types.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct CRYPT_EAL_CipherCtxLocal CRYPT_EAL_CipherCtx;

/**
  * @ingroup crypt_eal_cipher
  * @brief Symmetrical compliance checking
  *
  * @param id [IN] Symmetric algorithm ID.
  *
  * @return  true, success.
  *          false, failure.
  */
typedef bool (*CRYPT_EAL_CipherC2)(CRYPT_CIPHER_AlgId id);

/**
 * @ingroup crypt_eal_cipher
 * @brief   Callback registration interface for symmetric compliance checking, which is used to check whether
 * algorithm parameters are compliant and whether the module status is available. The EAL hash interface calls
 * this callback interface before performing the operation.
 *         Callback failure will cause CRYPT_EAL_CipherNewCtx/CRYPT_EAL_CipherInit/CRYPT_EAL_CipherReinit/
 * /CRYPT_EAL_CipherUpdate/CRYPT_EAL_CipherFinal/ CRYPT_EAL_CipherCtrl/CRYPT_EAL_CipherSetPadding API fails.
 *         After the CMVP feature is enabled, the CMVP provides the default implementation. Any algorithm can be
 * used in non-approved mode, Only CRYPT_CIPHER_AES128_CBC/CRYPT_CIPHER_AES192_CBC/CRYPT_CIPHER_AES256_CBC/
 * CRYPT_CIPHER_AES128_CTR/CRYPT_CIPHER_AES192_CTR/CRYPT_CIPHER_AES256_CTR/CRYPT_CIPHER_AES128_ECB/
 * CRYPT_CIPHER_AES192_ECB/CRYPT_CIPHER_AES256_ECB/CRYPT_CIPHER_AES128_XTS/CRYPT_CIPHER_AES256_XTS is allowed
 * in ISO19790 mode.
 *
 * @param   func     [IN] Symmetric compliance checking callback
 *
 * @return  None
 */
void CRYPT_EAL_RegCipherC2(CRYPT_EAL_CipherC2 func);

/**
 * @ingroup crypt_eal_cipher
 * @brief   Check whether the given symmetric algorithm ID is valid.
 *
 * @param   id [IN] Symmetric algorithm ID.
 *
 * @return Valid, true is returned.
 *         Invalid, false is returned.
 */
bool CRYPT_EAL_CipherIsValidAlgId(CRYPT_CIPHER_AlgId id);

/**
 * @ingroup crypt_eal_cipher
 * @brief Generate symmetric encryption and decryption handles.
 * @attention If the function is called by an external user and the error stack is concerned,
 * it is recommended that BSL_ERR_ClearError() be called before this function is called.
 *
 * @param id [IN] Symmetric encryption/decryption algorithm ID.
 * @return Success: cipher ctx.
 *         Fails: NULL.
 */
CRYPT_EAL_CipherCtx* CRYPT_EAL_CipherNewCtx(CRYPT_CIPHER_AlgId id);

/**
 * @ingroup crypt_eal_cipher
 * @brief Release the symmetric encryption/decryption handle. Clear sensitive information before releasing the handle.
 * @attention If the function is called by an external user and the error stack is concerned, it is recommended
 * that BSL_ERR_ClearError() be called before this function is called.
 *
 * @param ctx [IN] Symmetric encryption/decryption handle. The CTX is set null by the caller.
 * @return None
 */
void CRYPT_EAL_CipherFreeCtx(CRYPT_EAL_CipherCtx *ctx);

/**
 * @ingroup crypt_eal_cipher
 * @brief Initialize the symmetric encryption/decryption handle. The key cannot be null. Except the ECB mode,
 * other modes iv cannot be null.
 * The length of iv must be the same as the block length (this requirement is not required in ECB mode).
 * The block length can be obtained through CRYPT_CTRL_GET_BLOCKSIZE of CRYPT_EAL_CipherCtrl.
 * CRYPT_EAL_CipherInit can be called repeatedly at any stage, resets the key and iv, and clears the cached data.
 * <table border="1">
 * <tr><th>     algorithm ID         </th><th>  keyLen  </th><th>  ivLen  </th></tr>
 * <tr><td>  CRYPT_CIPHER_AES128_CBC  </td><td>   16     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES192_CBC  </td><td>   24     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES256_CBC  </td><td>   32     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES128_CTR  </td><td>   16     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES192_CTR  </td><td>   24     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES256_CTR  </td><td>   32     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES128_ECB  </td><td>   16     </td><td>  0      </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES192_ECB  </td><td>   24     </td><td>  0      </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES256_ECB  </td><td>   32     </td><td>  0      </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES128_XTS  </td><td>   32     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES256_XTS  </td><td>   64     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES128_CCM  </td><td>   16     </td><td>  7 <= ivLen <= 13     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES192_CCM  </td><td>   24     </td><td>  7 <= ivLen <= 13     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES256_CCM  </td><td>   32     </td><td>  7 <= ivLen <= 13     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES128_GCM  </td><td>   16     </td><td>  1 ≤ ivLen ≤ 2^32 - 1 (byte)     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES192_GCM  </td><td>   24     </td><td>  1 ≤ ivLen ≤ 2^32 - 1 (byte)     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES256_GCM  </td><td>   32     </td><td>  1 ≤ ivLen ≤ 2^32 - 1 (byte)     </td></tr>
 * <tr><td>  CRYPT_CIPHER_CHACHA20_POLY1305  </td><td>   32     </td><td>  8 or 12     </td></tr>
 * <tr><td>  CRYPT_CIPHER_SM4_CBC     </td><td>   16     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_SM4_ECB     </td><td>   16     </td><td>  0      </td></tr>
 * <tr><td>  CRYPT_CIPHER_SM4_CTR     </td><td>   16     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_SM4_GCM     </td><td>   16     </td><td>  1 ≤ ivLen ≤ 2^32 - 1 (byte)     </td></tr>
 * <tr><td>  CRYPT_CIPHER_SM4_CFB     </td><td>   16     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_SM4_OFB     </td><td>   16     </td><td>  a nonce      </td></tr>
 * <tr><td>  CRYPT_CIPHER_SM4_XTS     </td><td>   32     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES128_CFB  </td><td>   16     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES192_CFB  </td><td>   24     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES256_CFB  </td><td>   32     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES128_OFB  </td><td>   16     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES192_OFB  </td><td>   24     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_AES256_OFB  </td><td>   32     </td><td>  16     </td></tr>
 * <tr><td>  CRYPT_CIPHER_TDES_ECB    </td><td>   24     </td><td>  0      </td></tr>
 * <tr><td>  CRYPT_CIPHER_TDES_CBC    </td><td>   24     </td><td>  8 or 24      </td></tr>
 * <tr><td>  CRYPT_CIPHER_TDES_OFB    </td><td>   24     </td><td>  8 or 24      </td></tr>
 * <tr><td>  CRYPT_CIPHER_TDES_CFB    </td><td>   24     </td><td>  8 or 24      </td></tr>
 * </table>
 * @attention If the function is called by an external user and the error stack is concerned,
 * you are advised to call BSL_ERR_ClearError() before calling this function.
 *
 * @param ctx [IN] Symmetric encryption/decryption handle
 * @param key [IN] Key
 * @param keyLen [IN] Key length
 * @param iv [IN] Initialization vector
 * @param ivLen [IN] Initialize the vector length.
 * @param enc [IN] True: encryption; False: decryption
 * @return CRYPT_SUCCESS,success
 *         For other error codes, see crypt_errno.h.
 */
int32_t CRYPT_EAL_CipherInit(CRYPT_EAL_CipherCtx *ctx, const uint8_t *key, uint32_t keyLen, const uint8_t *iv,
    uint32_t ivLen, bool enc);

/**
 * @ingroup crypt_eal_cipher
 * @brief Deinitialize the handle and restore the handle to the state,
 * when the CRYPT_EAL_CipherNewCtx function is called.
 * @attention If the function is called by an external user and the error stack is concerned,
 * you are advised to call BSL_ERR_ClearError() before calling this function.
 *
 * @param ctx [IN] Symmetric encryption/decryption handle
 * @return None
 */
void CRYPT_EAL_CipherDeinit(CRYPT_EAL_CipherCtx *ctx);

/**
 * @ingroup crypt_eal_cipher
 * @brief Re-initialize the handle, retain the key, reset the IV, and clear the cache and sensitive data.
 * Except the ECB mode, other modes iv cannot be null. The setting of iv must be based on the corresponding
 * algorithm ID. For details, see the mapping in CRYPT_EAL_CipherInit.
 *
 * @param ctx [IN] Symmetric encryption/decryption handle
 * @param iv [IN] Vector
 * @param ivlen [IN] Vector length
 * @return None
 */
int32_t CRYPT_EAL_CipherReinit(CRYPT_EAL_CipherCtx *ctx, uint8_t *iv, uint32_t ivLen);

/**
 * @ingroup crypt_eal_cipher
 * @brief Continuously enter encrypted and decrypted data.
 * CRYPT_EAL_CipherUpdate should be used in conjunction with CRYPT_EAL_CipherFinal, after one or more calls to
 * CRYPT_EAL_CipherUpdate, Call CRYPT_EAL_CipherFinal. With the exception of SM4_XTS mode, multiple calls to
 * CRYPT_EAL_CipherUpdate and CRYPT_EAL_CipherFinal are not supported.
 * @attention If the function is called by an external user and the error stack is concerned, it is recommended
 * that BSL_ERR_ClearError() be called before this function is called.
 *
 * @param ctx [IN] Symmetric encryption and decryption handle
 * @param in [IN] Continuously input data
 * @param inLen [IN] Length of continuously input data
 * @param out [OUT] Output data
 * @param outLen [IN/OUT] Input: For CBC and ECB block encryption, you are advised to set outLen > inLen + blockSize.
 * For CTR and XTS stream encryption, you are advised to set outLen >= inLen. blockSize can be obtained by using
 * CRYPT_CTRL_GET_BLOCKSIZE of CRYPT_EAL_CipherCtrl.
 * Output: Length of the encrypted data. If the block encryption algorithm is used and the length of the last data
 *         to be processed is insufficient, the output value of outLen is 0.
 * eg: CBC and ECB block encryption
 *     1. Encrypted data is input for the first time, and inLen is less than blockSize.
 *        In this case, the output value of outLen is 0.
 *     2. In the first input encrypted data length, inLen is an integer multiple of blockSize.
 *        In this case, outLen is equal to inLen.
 *     3. In the first input encrypted data length, inLen > blockSize and not an integer multiple of blockSize.
 *        In this case, outLen < inLen.
 *     4. Enter the encrypted data for multiple times. (inLen% blockSize) + cache (CTX cache data) >= blockSize.
 *        At this point outLen = (inlen / blockSize) * blockSize + blockSize
 *        CTR outLen equals inLen.
 *     In XTS mode, update reserves the last two blocks for final processing, If the total length of the input data
 * plus the buffer is less than 32 blocks, the output is 0.
 *     1. When data is input for the first time, outLen = (inLen / 16 - 2) * 16.
 *     2. Enter the encrypted data for multiple times. At this time, outLen = ((inLen + cache) / 16 - 2) * 16.
 *     In SM4_XTS mode, after calling CRYPT_EAL_CipherUpdate, you need to use CRYPT_EAL_CipherInit or
 * CRYPT_EAL_CipherReinit to reset the key or iv.
 * @return  CRYPT_SUCCESS, success.
 *          Other error codes see the crypt_errno.h.
 */
int32_t CRYPT_EAL_CipherUpdate(CRYPT_EAL_CipherCtx *ctx, const uint8_t *in, uint32_t inLen, uint8_t *out,
    uint32_t *outLen);

/**
 * @ingroup crypt_eal_cipher
 * @brief Fill the data with the size of the block and output the encrypted data; the AEAD tag is obtained
 * through CRYPT_EAL_CipherCtrl.
 *        For block encryption algorithms such as CBC and ECB, padding must be set, In XTS mode, final needs
 * to be called to obtain the last two blocks.
 * @attention If the function is called by an external user and the error stack is concerned,
 *            you are advised to call BSL_ERR_ClearError() before calling this function.
 *
 * @param ctx [IN] Symmetric encryption/decryption handle
 * @param out [OUT] Output the encrypted data
 * @param outLen [IN/OUT] Input: outLen >= blockSize
 * Output: The output value for stream encryption is 0.
 *   If padding is set for CBC and ECB block encryption, the output value of outLen is blockSize.
 *   If the padding is not set for CBC and ECB block encryption and CTX contains cached data, an error is reported.
 *   If padding is not set for CBC and ECB block encryption, and no data is cached in the CTX, the output value of
 * outLen is 0.
 * @return Success return CRYPT_SUCCESS.
 *         Other error codes see the crypt_errno.h.
 */
int32_t CRYPT_EAL_CipherFinal(CRYPT_EAL_CipherCtx *ctx, uint8_t *out, uint32_t *outLen);

/**
 * @ingroup crypt_eal_cipher
 * @brief Set the mode ctx parameters in the CTX.
 *         parameter           data type         Length(len):number of data bytes
 * CRYPT_CTRL_GET_IV         uint8_t array   The length of the IV depends on the corresponding algorithm,
                                             see the mapping in CRYPT_EAL_CipherInit
 * CRYPT_CTRL_SET_IV         uint8_t array   The length of the IV depends on the corresponding algorithm,
                                             see the mapping in CRYPT_EAL_CipherInit.
 * CRYPT_CTRL_SET_AAD        uint8_t array   It is used only for AEAD calculation.
                                             The length is related to the corresponding AEAD algorithm.
 * CRYPT_CTRL_GET_TAG        uint8_t array   It is used only for AEAD calculation.
                                             The length is the tagLen value set by the user.
 * CRYPT_CTRL_SET_COUNT      uint8_t[4]      length(len) 4
 * CRYPT_CTRL_SET_TAGLEN     uint32_t        length(len) 4
 * CRYPT_CTRL_SET_MSGLEN     uint64_t        length(len) 8
 * CRYPT_CTRL_SET_FEEDBACKSIZE     uint32_t            length(len) 4
 * CRYPT_CTRL_GET_FEEDBACKSIZE     uint32_t pointer    sizeof(*uint32_t)
 * CRYPT_CTRL_GET_BLOCKSIZE        uint32_t            length(len) 4
 * @attention If the function is called by an external user and the error stack is concerned,
 * it is recommended that BSL_ERR_ClearError() be called before this function is called.
 * @param ctx [IN] Symmetric encryption/decryption handle
 * @param type [IN] Parameter type
 * @param data [IN/OUT] Input and output data
 * @param len [IN] Data length
 * @return Success response: CRYPT_SUCCESS
 *         error codes see the crypt_errno.h
 */
int32_t CRYPT_EAL_CipherCtrl(CRYPT_EAL_CipherCtx *ctx, CRYPT_CipherCtrl type, void *data, uint32_t len);

/**
 * @ingroup crypt_eal_cipher
 * @brief Set the padding mode.
 *
 * @param ctx Symmetric encryption/decryption handle
 * @param type Padding type
 * @return Success response: CRYPT_SUCCESS
 *         Error codes see crypt_errno.h
 */
int32_t CRYPT_EAL_CipherSetPadding(CRYPT_EAL_CipherCtx *ctx, CRYPT_PaddingType type);

/**
 * @ingroup crypt_eal_cipher
 * @brief Obtain the padding type.
 *
 * @param ctx Symmetric encryption/decryption handle
 * @return Return mode
 */
int32_t CRYPT_EAL_CipherGetPadding(CRYPT_EAL_CipherCtx *ctx);

/**
  * @ingroup crypt_eal_cipher
  * @brief Obtain the type of an algorithm based on the algorithm ID.
  *
  * @param id [IN] Symmetric algorithm ID.
  * @param type [IN] Attribute type to be obtained.
  * @param infoValue [OUT] Obtained attribute data.
  * @return CRYPT_SUCCESS, success
  *         Other error codes see crypt_errno.h
  */
int32_t CRYPT_EAL_CipherGetInfo(CRYPT_CIPHER_AlgId id, CRYPT_INFO_TYPE type, uint32_t *infoValue);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif