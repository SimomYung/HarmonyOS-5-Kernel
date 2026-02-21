/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: dev_secinfo do things in tee header
 * Create: 2023/03/31
 */

#ifndef __DEV_SECINFO_BY_TEE_H__
#define __DEV_SECINFO_BY_TEE_H__
#include <linux/types.h>

u32 set_uapp_enable_state_by_tee(u32 state);
u32 dev_secinfo_update_all_nvcnt_by_tee(void);
u32 huk2_enhance_update_cnt_by_tee(void);

#endif /* __DEV_SECINFO_BY_TEE_H__ */