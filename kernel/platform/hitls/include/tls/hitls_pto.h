/**
 * @defgroup  hitls_pto hitls_pto.h
 * @ingroup   tls
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * @brief     Separation of bearer and control feature(process based TLS offload)
 */

#ifndef HITLS_PTO_H
#define HITLS_PTO_H

#include <stdint.h>
#include <stdbool.h>
#include "bsl_uio.h"
#include "hitls.h"
#include "hitls_crypt_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup hitls_pto
 * @brief   Obtain the status of the PTO feature.
 *
 * @param   ctx [IN] TLS object.
 *
 * @retval  False, disable.
 * @retval  True, enabled.
 */
bool HITLS_PTO_GetSwitch(const HITLS_Ctx *ctx);

/**
 * @ingroup hitls_pto
 * @brief   Set the status of the PTO feature.
 *
 * @attention This feature supports only DTLS over SCTP.
 *
 * @param   ctx [IN] TLS object.
 * @param   isEnable [IN] switch <true: enabled; false: disable>.
 *
 * @retval  HITLS_SUCCESS, if successful.
 * @retval  For other error codes, see hitls_error.h
 */
int32_t HITLS_PTO_SetSwitch(HITLS_Ctx *ctx, bool isEnable);

/**
 * @ingroup hitls_pto
 * @brief   Backup TLS and UIO objects.
 * @details Backup TLS and UIO objects, and construct the context content into a TLV format message.
 *           - TLV format:
 *           - <--TLV Header--><---TLV Body--->
 *           - | type | length |     value    |
 *           - Constructed message is as follows:
 *           - | UIO_OBJECT | Data Length | UIO Backup Data | TLS_OBJECT | Data Length | TLS Backup Data |
 *
 * @param   ctx     [IN] TLS object.
 * @param   cipher  [IN] Key parameters.
 * @param   data    [OUT] Message memory.
 * @param   length  [IN] Memory length.
 * @param   usedLen [OUT] Message length.
 *
 * @retval  HITLS_SUCCESS, if successful.
 * @retval  For other error codes, see hitls_error.h.
 */
int32_t HITLS_PTO_Backup(HITLS_Ctx *ctx, const HITLS_CipherParameters *cipher,
    uint8_t *data, uint32_t length, uint32_t *usedLen);

/**
 * @ingroup hitls_pto
 * @brief   Parse the context content from the TLV format message, restore the UIO object.
 *
 * @param   data   [IN] Message memory.
 * @param   length [IN] Message length.
 * @param   cipher [IN] Key parameters.
 * @param   uio    [IN] UIO structure.
 * @param   ctx    [OUT] TLS object.
 *
 * @retval  HITLS_SUCCESS, if successful.
 * @retval  For other error codes, see hitls_error.h.
 */
int32_t HITLS_PTO_Restore(const uint8_t *data, uint32_t length, const HITLS_CipherParameters *cipher,
    BSL_UIO *uio, HITLS_Ctx **ctx);

#ifdef __cplusplus
}
#endif

#endif /* HITLS_PTO_H */
