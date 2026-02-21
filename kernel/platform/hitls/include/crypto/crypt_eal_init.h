/**
 * @defgroup    crypt_eal_init    crypt_eal_init.h
 * @ingroup     crypto
 * @copyright   Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef CRYPT_EAL_INIT_H
#define CRYPT_EAL_INIT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define CRYPT_INIT_ABILITY_CPU_POS   0
#define CRYPT_INIT_ABILITY_BSL_POS   1
#define CRYPT_INIT_ABILITY_RAND_POS  2
#define CRYPT_INIT_ABILITY_LOCK_POS  3

/**
 * @ingroup crypt_method
 * @brief CRYPTO initialization
 *
 * @param opts   [IN] Bit information to be initialized, the four bits are used at present.
 *                    The first bit is CRYPT_INIT_ABILITY_CPU_POS, the second bit is CRYPT_INIT_ABILITY_BSL_POS,
 *                    the third bit is CRYPT_INIT_ABILITY_RAND_POS,
 *                    and the fourth bit is CRYPT_INIT_ABILITY_LOCK_POS
.
 * @return CRYPT_SUCCESS, if successful.
 *         For other error codes, see the crypt_errno.h file.
 */
int32_t CRYPT_EAL_Init(uint64_t opts);

/**
 * @ingroup crypt_method
 * @brief   release the CRYPTO initialization memory.
 *
 * @param opts   [IN] information about the bits to be deinitialized, which is the same as that of CRYPT_EAL_Init.
 */
void CRYPT_EAL_Cleanup(uint64_t opts);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // CRYPT_EAL_INIT_H
