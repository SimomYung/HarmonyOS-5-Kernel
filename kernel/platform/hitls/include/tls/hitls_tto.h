/**
 * @defgroup  hitls_pto hitls_pto.h
 * @ingroup   tls
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * @brief     TLS traffic offload
 */
#ifndef HITLS_TTO_H
#define HITLS_TTO_H

#include <stdint.h>
#include "bsl_uio.h"
#include "hitls_type.h"
#include "hitls_crypt_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TTO_SET_FD = 1,
    TTO_GET_FD = 2,
    TTO_SET_READ_CB = 3,
    TTO_SET_WRITE_CB = 4,
    TTO_SET_READ_APP_DATA = 5,
    TTO_SET_WRITE_APP_DATA = 6,
    TTO_RANDOM_IV_NEEDED_FOR_WRITE = 7,
    TTO_GEN_RANDOM_IV_FOR_WRITE = 8,
    TTO_SET_RANDOM_IV_FOR_WRITE = 9,
    TTO_GET_WRITE_RECORD_LEN = 10,
    TTO_GET_PENDING_APP_DATA = 11,
    TTO_GET_CONNECTION_STATE = 12
} HITLS_TTO_CtrlCmd;

int32_t HITLS_TTO_Backup(HITLS_Ctx *ctx, const HITLS_CipherParameters *cipher,
    uint8_t *data, uint32_t length, uint32_t *usedLen);

HITLS_Ctx *HITLS_TTO_Restore(const uint8_t *data, uint32_t length, const HITLS_CipherParameters *cipher);

int32_t HITLS_TTO_Ctrl(HITLS_Ctx *ctx, HITLS_TTO_CtrlCmd cmd, void *arg, uint32_t argLen);

#ifdef __cplusplus
}
#endif

#endif