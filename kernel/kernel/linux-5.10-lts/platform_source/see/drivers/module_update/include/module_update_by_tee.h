/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: module_update do things in tee header
 * Create: 2025/01/18
 */

#ifndef __MODULE_UPDATE_BY_TEE_H__
#define __MODULE_UPDATE_BY_TEE_H__
#include <linux/types.h>

u32 module_update_verify_bindfile_by_tee(u8 *data, u32 size);
u32 module_update_verify_hash_by_tee(char *img_name, u32 img_name_size, u8 *data, u32 size);

#endif /* __MODULE_UPDATE_BY_TEE_H__ */