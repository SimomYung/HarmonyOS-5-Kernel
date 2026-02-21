/**
 * @defgroup    hitls_uio_ssl.h
 * @ingroup     tls
 * @copyright   Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * @brief
 */
#ifndef HITLS_BIO_H
#define HITLS_BIO_H

#include <stdint.h>
#include "hitls_type.h"
#include "bsl_uio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup bsl
 * @brief   Returns the SSL UIO method, this function returns a BSL_UIO_Method structure of the SSL type.
 *
 * @retval BSL_UIO_Method
 * @retval NULL, if failed.
 */
const BSL_UIO_Method *BSL_UIO_GetMethodForSSL(void);

/**
 * @ingroup bsl
 * @brief Create a context for config, bind a new UIO, and return the UIO.
 *
 * @param config    [IN] Configure.
 * @param isClient  [IN] Client or Not.
 *
 * @retval Newly created UIO object.
 */
BSL_UIO *BSL_UIO_NewCtx(HITLS_Config *config, int32_t isClient);

#ifdef __cplusplus
}
#endif

#endif /* HITLS_BIO_H */
