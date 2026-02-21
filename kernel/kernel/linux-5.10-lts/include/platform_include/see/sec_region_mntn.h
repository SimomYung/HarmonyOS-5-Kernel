/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: sec_region_mntn.h.
 * Create: 2024-07-10
 */

#ifndef __SEC_REGION_MNTN_H__
#define __SEC_REGION_MNTN_H__

#include <linux/printk.h>

#ifdef CONFIG_SEC_REGION_MNTN
void sec_region_mntn_handler(void);
#else
void sec_region_mntn_handler(void) {}
#endif

#endif
