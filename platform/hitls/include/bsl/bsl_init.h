/**
 * @defgroup    bsl_init bsl_init.h
 * @ingroup     bsl
 * @copyright   Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * @brief       This API is called by the user to perform global initialization and de-initialization.
 */

#ifndef BSL_INIT_H
#define BSL_INIT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup bsl_init
 * @brief Initialize the BSL module.
 *
 * @par Description:
 * The user must call this interface to initialize.
 *
 * @attention None.
 * @retval #BSL_SUCCESS, error code module is successfully initialized.
 * @retval #BSL_SAL_ERR_NO_MEMORY, memory space is insufficient and thread lock space cannot be applied for.
 * @retval #BSL_SAL_ERR_UNKNOWN, thread lock initialization failed.
 */
int32_t BSL_GLOBAL_Init(void);

/**
 * @ingroup bsl_init
 * @brief Deinitialize the BSL module.
 *
 * @par Description:
 * The user calls this interface when the process exits.
 *
 * @attention None
 */
int32_t BSL_GLOBAL_DeInit(void);

#ifdef __cplusplus
}
#endif

#endif