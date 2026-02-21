/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: header file to provide plat utils
 * Create: 2025/01/18
 */

#ifndef __MODULE_UPDATE_PLAT_H__
#define __MODULE_UPDATE_PLAT_H__

#include <linux/kernel.h> /* pr_err & min & ARRAY_SIZE */

#define module_update_err(exp, ...)                pr_err("[%s]: "exp, __func__, ##__VA_ARGS__)
#define module_update_info(exp, ...)               pr_info("[%s]: "exp, __func__, ##__VA_ARGS__)

#endif /* __MODULE_UPDATE_PLAT_H__ */