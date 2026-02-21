/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: module update head file
 * Create: 2025/1/18
 */
#ifndef __MODULE_UPDATE_H__
#define __MODULE_UPDATE_H__

#include <linux/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define MODULE_UPDATE_BINDFILE_MAX_BYTES    0x3000
#define IMG_NAME_MAX_BYTES                  0x40
#define IMG_HASH_MAX_BYTES                  0x40
#define MODULE_UPDATE_ERR_FUNC_UNSUPPORT    0xffff

struct module_update_bindfile {
	u64 data_bytes;
	u8 data[MODULE_UPDATE_BINDFILE_MAX_BYTES];
};

struct module_update_img_hash {
	u64 img_name_bytes;
	char img_name[IMG_NAME_MAX_BYTES];
	u64 hash_bytes;
	u8 hash[IMG_HASH_MAX_BYTES];
};

#ifdef CONFIG_MODULE_UPDATE
s32 module_update_verify_bindfile(u32 cmd, uintptr_t arg);
s32 module_update_verify_hash(u32 cmd, uintptr_t arg);
#else /* CONFIG_MODULE_UPDATE */
static inline s32 module_update_verify_bindfile(u32 cmd, uintptr_t arg)
{
	(void)cmd;
	(void)arg;
	return MODULE_UPDATE_ERR_FUNC_UNSUPPORT;
}

static inline s32 module_update_verify_hash(u32 cmd, uintptr_t arg)
{
	(void)cmd;
	(void)arg;
	return MODULE_UPDATE_ERR_FUNC_UNSUPPORT;
}
#endif /* CONFIG_MODULE_UPDATE */

#ifdef __cplusplus
}
#endif

#endif /* __MODULE_UPDATE_H__ */
