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

#ifndef __MM_LB_DRV_V550_H
#define __MM_LB_DRV_V550_H

#include <linux/platform_device.h>
#include "mm_lb_priv_v550.h"

u32 is_lb_available(void);
int lb_set_master_policy(u32 pid, u32 quota, u32 prio);
int lb_reset_master_policy(u32 pid);
int set_page(u32 pgid, struct page *page_alloc, size_t count);
void reset_page(u32 pgid, struct page *page, size_t count);
pgid_info *get_pgid_info(u32 pgid);
cgid_info *get_cgid_info(u32 cgid);
cgid_t *get_cgid(u32 cgid);
pgid_t *get_pgid(u32 pgid);
int get_cpu_pgid(u32 pgid_idx);
void lb_pgid_enable(u32 pgid);
void lb_pgid_bypass(u32 pgid);
int lb_pmu_init(struct platform_device *pdev, lb_device *lbd);
void set_cgid_quota(const u32 gid, const u32 val, const u32 direct);
u32 get_pgid_cgid_idx(const u32 gid);

#ifdef CONFIG_DMABUF_UDK_EN
int set_page_from_udk(u32 pgid, struct page *page_alloc, size_t count);
void reset_page_from_udk(u32 pgid, struct page *page, size_t count);
#else
static inline int set_page_from_udk(u32 pgid, struct page *page_alloc, size_t count) {return -ENOMEM;}
static inline void reset_page_from_udk(u32 pgid, struct page *page, size_t count) {}
#endif

/*
 * WARNING: The following get/set funcs does not implement any lock or muxtex.
 * It's the caller's duty to avoid concurrent operations on the same gid.
 * (Including lb_gid_enable/bypass func)
 */
#define declare_sc_property(type_name, property) \
	u32 get_##type_name##_##property(const u32 gid); \
	void set_##type_name##_##property(const u32 gid, const u32 val, const u32 direct);

#endif
