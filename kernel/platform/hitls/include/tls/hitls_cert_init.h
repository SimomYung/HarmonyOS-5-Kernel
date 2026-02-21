/**
 * @defgroup    hitls_cert_init hitls_cert_init.h
 * @ingroup     tls
 * @copyright   Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * @brief       TLS certificate abstraction layer initialization
 */
#ifndef HITLS_CERT_INIT_H
#define HITLS_CERT_INIT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup hitls
 *
 * @brief   Certificate initialization interface, default use the PSE interface.
 * @attention If PSE not be used, do not need to call this interface.
 * @param   NA
 * @return  void
 */
int32_t HITLS_CertMethodInit(void);

/**
 * @ingroup hitls
 *
 * @brief   Deinitialize the certificate, set the certificate registration interface to NULL.
 * @param   NA
 * @return  void
 */
void HITLS_CertMethodDeinit(void);

#ifdef __cplusplus
}
#endif

#endif /* HITLS_CRYPT_CERT_H */
