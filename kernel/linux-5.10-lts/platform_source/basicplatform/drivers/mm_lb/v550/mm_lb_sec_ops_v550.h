/*
 * Copyright (c) 2022-2022 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __MM_LB_SEC_OPS_V550_H
#define __MM_LB_SEC_OPS_V550_H
#include <linux/types.h>

u32 get_lb_efuse(void);
u32 lb_exclusive_enable(void);
u32 lb_exclusive_bypass(void);
u32 lb_pgid_request(u32 pgid);
u32 lb_pgid_release(u32 pgid);
u32 lb_mpam_way_enable(u32 mpam_id, u32 way);

u32 sec_read(u32 addr);
u32 sec_write(u32 value, u32 addr);

#endif
