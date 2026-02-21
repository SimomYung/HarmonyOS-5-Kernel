/*
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
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

#ifndef __MM_LB_PRIV_V550_H
#define __MM_LB_PRIV_V550_H

#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>
#include <soc_sc_interface.h>

#include "../mm_lb_common.h"

#ifdef pr_fmt
#undef pr_fmt
#endif
#define pr_fmt(fmt) KBUILD_MODNAME ":%s: " fmt, __func__

#ifndef BIT
#define BIT(n) (0x1UL << (n))
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAX_PGID_CNT 64
#define MAX_CGID_CNT 16

#define LB_PRIV_GID_WIDTH 6
#define LB_PRIV_GID_MASK (BIT(LB_PRIV_GID_WIDTH) - 1UL)
#define LB_PRIV_OSH_SHIFT 6
#define LB_VA_MASK (~(LB_PRIV_GID_MASK | BIT(LB_PRIV_OSH_SHIFT)))

#ifdef CONFIG_MM_LB_V560
typedef struct {
	union {
		u32 value;
		struct {
			u32 rsv0 : 1;
			u32 pgid_paf_en : 1;
			u32 pgid_prefetch_en : 1;
			u32 pgid_autoclean_en : 1;
			u32 pgid_schint_en : 1;
			u32 pgid_keep_cleansh : 1;
			u32 pgid_vacmo_en : 1;
			u32 pgid_stash_en : 1;
			u32 pgid_trigger_en : 1;
			u32 pgid_mode : 1;
			u32 pgid_sot_ignore : 1;
			u32 pgid_passdirty_en : 1;
			u32 pgid_cgid_idx : 4;
			u32 rsv1 : 4;
			u32 pgid_writeevict_en : 1;
			u32 rsv2 : 10;
			u32 pgid_hit_age_byp : 1;
		} reg;
	} pgid_drv_cfg;

	union {
		u32 value;
		struct {
			u32 pgid_wr_revisit_age : 3;
			u32 rsv0 : 1;
			u32 pgid_nwr_revisit_age : 3;
			u32 rsv1 : 1;
			u32 pgid_initial_age : 3;
			u32 rsv2 : 1;
			u32 pgid_wr_policy : 2;
			u32 pgid_rd_policy : 2;
			u32 pgid_priority : 2;
			u32 pgid_self_replaceable : 1;
			u32 rsv3 : 1;
			u32 pgid_drop : 1; // RO
			u32 pgid_drop_clr : 1; // W1_PULSE
			u32 pgid_drop_set : 1; // W1_PULSE
			u32 rsv4 : 1;
			u32 pgid_disable : 1; // RO
			u32 pgid_disable_clr : 1; // W1_PULSE
			u32 pgid_disable_set : 1; // W1_PULSE
			u32 rsv5 : 1;
			u32 pgid_small_burst_size : 2;
			u32 pgid_small_burst_na : 1;
			u32 rsv6 : 1;
		} reg;
	} pgid_mst_cfg;
} hw_reg_pgid;

typedef struct {
	union {
		u32 value;
		struct {
			u32 cgid_way_enable : 16;
			u32 rsv0 : 16;
		} reg;
	} cgid_way_allc;

	union {
		u32 value;
		struct {
			u32 cgid_way_search : 16;
			u32 rsv0 : 16;
		} reg;
	} cgid_way_search;

	union {
		u32 value;
		struct {
			u32 cgid_self_replaceable : 1;
			u32 cgid_bypass_en : 1;
			u32 cgid_tag_pgid_chg_en : 1;
			u32 rsv0 : 29;
		} reg;
	} cgid_cfg;

	union {
		u32 value;
		struct {
			u32 cgid_quota : 16;
			u32 rsv0 : 16;
		} reg;
	} cgid_quota;
} hw_reg_cgid;
#else
typedef struct {
	union {
		u32 value;
		struct {
			u32 pgid_small_burst_na : 1;
			u32 pgid_paf_en : 1;
			u32 pgid_prefetch_en : 1;
			u32 pgid_autoclean_en : 1;
			u32 pgid_schint_en : 1;
			u32 pgid_keep_cleansh : 1;
			u32 pgid_vacmo_en : 1;
			u32 pgid_stash_en : 1;
			u32 pgid_trigger_en : 1;
			u32 pgid_mode : 1;
			u32 pgid_sot_ignore : 1;
			u32 pgid_passdirty_en : 1;
			u32 pgid_cgid_idx : 4;
			u32 pgid_sgid_idx : 4;
			u32 pgid_writeevict_en : 1;
			u32 rsv1 : 11;
		} reg;
	} pgid_drv_cfg;

	union {
		u32 value;
		struct {
			u32 pgid_revisit_age : 3;
			u32 rsv0 : 1;
			u32 pgid_initial_age0 : 3;
			u32 rsv1 : 1;
			u32 pgid_initial_age1 : 3;
			u32 rsv2 : 1;
			u32 pgid_wr_policy : 2;
			u32 pgid_rd_policy : 2;
			u32 pgid_priority : 2;
			u32 pgid_self_replaceable : 1;
			u32 rsv3 : 1;
			u32 pgid_drop : 1; // RO
			u32 pgid_drop_clr : 1; // W1_PULSE
			u32 pgid_drop_set : 1; // W1_PULSE
			u32 rsv4 : 1;
			u32 pgid_disable : 1; // RO
			u32 pgid_disable_clr : 1; // W1_PULSE
			u32 pgid_disable_set : 1; // W1_PULSE
			u32 rsv5 : 5;
		} reg;
	} pgid_mst_cfg;
} hw_reg_pgid;

typedef struct {
	union {
		u32 value;
		struct {
			u32 cgid_way_enable : 16;
			u32 cgid_way_search : 16;
		} reg;
	} cgid_way_allc;

	union {
		u32 value;
		struct {
			u32 cgid_quota : 16;
			u32 cgid_self_replaceable : 1;
			u32 cgid_bypass_en : 1;
			u32 cgid_tag_pgid_chg_en : 1;
			u32 rsv : 13;
		} reg;
	} cgid_cfg;
} hw_reg_cgid;
#endif

typedef struct {
	u32 way_search : 16;
	u32 way_enable : 16;
	u32 quota : 16;
	u32 bypass : 1;
	u32 self_replaceable : 1;
	u32 tag_pgid_chg_en : 1;
} cgid_t;

typedef struct {
	u32 cgid_idx : 4;
	u32 sgid_idx : 4;
	u32 disable : 1;
	u32 drop : 1;
	u32 self_replaceable : 1;
	u32 priority : 2;
	u32 rd_policy : 2;
	u32 wr_policy : 2;
	u32 initial_age0 : 3;
	u32 initial_age1 : 3;
	u32 revisit_age : 3;
	u32 nwr_revisit_age : 3;
	u32 initial_age : 3;
	u32 wr_revisit_age : 3;
	u32 scene: 2;
	u32 drop_en : 1;
	u32 mode : 1;
	u32 trigger_en : 1;
	u32 stash_en : 1;
	u32 vacmo_en : 1;
	u32 keep_cleansh : 1;
	u32 writeevict_en : 1;
	u32 sot_ignore : 1;
	u32 passdirty_en : 1;
	u32 schint_en : 1;
	u32 autoclean_en : 1;
	u32 prefetch_en : 1;
	u32 paf_en : 1;
	u32 small_burst_size : 2;
	u32 small_burst_na : 1;
	u32 hit_age_byp : 1;
} pgid_t;

typedef struct {
	cgid_t cgid;
	u64 pgid_map; /* for pgids that are enabled */
	u64 pgid_map_full; /* for all pgids attached to this cgid_t */
	u32 org_quota;
	spinlock_t lock;
} cgid_info;

typedef struct {
	const char *name;
	pgid_t pgid;
	u64 page_count;
	vm_info *vm;
	u32 org_prio;
	spinlock_t lock;
} pgid_info;

typedef struct {
	pgid_t *(*get)(u32 pgid);
	bool (*valid)(u32 pgid);
	u32 (*idx)(pgid_t *pgid);
} pgid_repo_ops;

typedef struct {
	pgid_info vector[MAX_PGID_CNT];
	pgid_repo_ops *ops;
} pgid_repo;

typedef struct {
	cgid_t *(*get)(u32 cgid);
	bool (*valid)(u32 cgid);
	u32 (*idx)(cgid_t *cgid);
} cgid_repo_ops;

typedef struct {
	cgid_info vector[MAX_CGID_CNT];
	cgid_repo_ops *ops;
} cgid_repo;

typedef struct {
	struct dsm_client *ai_client;
	struct work_struct dsm_work;
} dsm_info;

typedef struct {
	struct device *dev;
	void __iomem *base;
	u32 is_available;
	u32 dfx_irq;
	u32 max_quota;
	u32 max_size;
	u32 way_en;
	u32 way_efuse;
	u32 slc_idx;
	pgid_repo pgid_repo;
	cgid_repo cgid_repo;
	dsm_info dsm;
	spinlock_t lock;
} lb_device;

extern lb_device *lbdev;

#define set_bits(mask, addr) writel(((mask) | (readl(addr))), (addr))
#define clr_bits(mask, addr) writel(((~(mask)) & (readl(addr))), (addr))
#define set_bit(nr, addr)    set_bits(BIT(nr), addr)
#define clr_bit(nr, addr)    clr_bits(BIT(nr), addr)

#endif