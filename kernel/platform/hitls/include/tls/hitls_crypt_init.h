/**
 * @defgroup    hitls_crypt_reg hitls_crypt_reg.h
 * @ingroup     tls
 * @copyright   Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * @brief       TLS algorithm abstraction layer initialization
 */
#ifndef HITLS_CRYPT_INIT_H
#define HITLS_CRYPT_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup hitls
 *
 * @brief   Initialize the algorithm interface. By default, the hicrypto interface is used.
 * @attention If hicrypto is not used, you do not need to call this API.
 * @param   NA
 * @return  void
 */
void HITLS_CryptMethodInit(void);

#ifdef __cplusplus
}
#endif

#endif /* HITLS_CRYPT_INIT_H */