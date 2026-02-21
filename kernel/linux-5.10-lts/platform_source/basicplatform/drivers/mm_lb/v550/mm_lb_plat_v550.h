
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

#ifndef __MM_LB_PLAT_V550_H
#define __MM_LB_PLAT_V550_H

#include <linux/mm_lb/mm_lb_v550.h>
#include "mm_lb_priv_v550.h"

#define slc_addr(addr, slc_idx) \
	((addr) + ((slc_idx) + 1) * 0x10000)

#define CMD_OP_TYPE_SHIFT 1
typedef enum {
	INV = 0 << CMD_OP_TYPE_SHIFT,
	CLEAN = 1 << CMD_OP_TYPE_SHIFT,
	CI = 2 << CMD_OP_TYPE_SHIFT,
	QUOTA_CNT = 3 << CMD_OP_TYPE_SHIFT,
} ops_type;

typedef enum {
	CMO_BY_WAY = 0,
	CMO_BY_GID,
	CMO_BY_64PA,
	CMO_BY_128PA,
	CMO_BY_4XKPA,
} cmo_type;

/* function */
void pgid_validate(u32 pgid_idx);
void cgid_validate(u32 cgid);
u32 clear_quota(u32 cgid);
void set_quota(u32 cgid, u32 quota);
void set_cgid_tag_pgid_chg_en_direct(u32 cgid_idx, bool enable);
int lb_ops_cache(ops_type ops, cmo_type by_x, u32 bitmap, u64 pa, size_t sz);
void lb_cmo_sync(void);
u32 check_pgid_cnt(void);
u32 lb_cgid_cfg(u32 cgid);
u32 lb_pgid_mst_cfg(u32 pgid);
u32 lb_pgid_drv_cfg(u32 pgid);
int init_irq(lb_device *lbd);
#endif
