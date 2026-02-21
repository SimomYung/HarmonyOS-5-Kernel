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
#include <linux/genalloc.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <soc_sc_interface.h>

#if defined(CONFIG_MM_LB_PLAT_V550)
#include "mm_lb_plat_v550.h"
#elif defined(CONFIG_MM_LB_PLAT_V560)
#include "mm_lb_plat_v560.h"
#else
#include "mm_lb_plat_v570.h"
#endif

#ifdef pr_fmt
#undef pr_fmt
#endif
#define pr_fmt(fmt) KBUILD_MODNAME ":%s: " fmt, __func__

#define LB_ERROR  1
#define LB_WARN   3
#define LB_INFO   5

static int lb_d_lvl = LB_WARN;

#define lb_print(level, x...)    \
	do {                         \
		if (lb_d_lvl >= (level)) \
			pr_err(x);           \
	} while (0)

#define slc_addr(addr, slc_idx) \
	((addr) + ((slc_idx) + 1) * 0x10000)

#define set_bits(mask, addr) writel(((mask) | (readl(addr))), (addr))
#define clr_bits(mask, addr) writel(((~(mask)) & (readl(addr))), (addr))
#define set_bit(nr, addr)    set_bits(BIT(nr), addr)
#define clr_bit(nr, addr)    clr_bits(BIT(nr), addr)

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

#define DFX_INFO_NUM          5
#define MPAM_ID_MAX 15
#define MPAM_REPLACE_POLICY_NUM 7
#define MAX_STATIS_NUM 4

#define NEED_REMOTE_SC_PWR_ON    1
#define NEED_REMOTE_SC_PWR_OFF   0

typedef enum {
	NON_ERR,
	SINGLE_BIT_ERR,
	MULTI_BIT_ERR,
	OTHER_ERR,
} ecc_err_type;

typedef struct {
	u32 intr;
	char *info_msg;
	void (*post_process)(u32 *err_info, u32 info_num);
} lb_dfx_type;

typedef struct {
	u32 intr;
	ecc_err_type err_type;
} lb_ecc_type;

extern lb_dfx_type lb_dfx_info[LB_DFX_INFO_NUM];
extern lb_ecc_type lb_ecc_info[LB_ECC_INFO_NUM];

typedef struct {
	cgid_t cgid;
	u64 pgid_map; /* for pgids that are enabled */
	u64 pgid_map_full; /* for all pgids attached to this cgid_t */
	u32 org_quota;
	spinlock_t lock;
} cgid_info;

typedef struct {
	struct list_head list;
	struct mutex mutex;
	struct gen_pool *pool;
} vm_info;

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
	ecc_err_type err_type;
	struct work_struct bbox_work;
} bbox_info;

typedef struct {
	u32 g_stat_bitmap;
	stat_info vector[MAX_STATIS_NUM];
	u32 stat_glb_en;
	u32 stat_time;
	u32 stat_time_acc;
} stat_repo;

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
	u32 syncbuf_base;
	pgid_repo pgid_repo;
	cgid_repo cgid_repo;
	bbox_info bbox;
	stat_repo stat_repo;
	spinlock_t lock;
#ifdef CONFIG_MM_LB_SUPPORT_REMOTE_SC
	spinlock_t remote_sc_lock;
	u64 using_remote_pgid; /* to avoid repeated power-on and power-off of the same pgid. */
#endif
} lb_device;

extern lb_device *lbdev;

bool is_pgid_valid(u32 pgid);
bool is_cgid_valid(u32 cgid);
pgid_t *get_pgid(u32 pgid);
cgid_t *get_cgid(u32 cgid);

/* Remote sc func declare */
void lb_remote_sc_enable(u32 cgid, u32 pgid);
void lb_remote_sc_bypass(u32 cgid, u32 pgid);
bool is_need_remote_sc(u32 pgid, u32 cgid_idx, u32 pwr_on);

void gid_save(void);
void gid_restore(void);

/* type_name: the name of the gid struct defined by "typedef". (pgid/cgid) */
#define define_sc_property(type_name, property) \
	u32 get_##type_name##_##property(const u32 gid) { return get_##type_name(gid)->property; } \
	void set_##type_name##_##property(const u32 gid, const u32 val, const u32 direct) { \
		if (is_##type_name##_valid(gid)) \
			get_##type_name(gid)->property = val; \
	}

/*
 * WARNING: The following get/set funcs does not implement any lock or muxtex.
 * It's the caller's duty to avoid concurrent operations on the same gid.
 * (Including lb_gid_enable/bypass func)
 */
#define declare_sc_property(type_name, property) \
	u32 get_##type_name##_##property(const u32 gid); \
	void set_##type_name##_##property(const u32 gid, const u32 val, const u32 direct);

declare_sc_property(pgid, small_burst_na)
declare_sc_property(pgid, paf_en)
declare_sc_property(pgid, prefetch_en)
declare_sc_property(pgid, autoclean_en)
declare_sc_property(pgid, schint_en)
declare_sc_property(pgid, keep_cleansh)
declare_sc_property(pgid, vacmo_en)
declare_sc_property(pgid, stash_en)
declare_sc_property(pgid, trigger_en)
declare_sc_property(pgid, mode)
declare_sc_property(pgid, sot_ignore)
declare_sc_property(pgid, passdirty_en)
declare_sc_property(pgid, writeevict_en)
declare_sc_property(pgid, revisit_age)
declare_sc_property(pgid, wr_policy)
declare_sc_property(pgid, rd_policy)
declare_sc_property(pgid, priority)
declare_sc_property(pgid, self_replaceable)
declare_sc_property(pgid, drop)
declare_sc_property(pgid, disable)
declare_sc_property(pgid, small_burst_size)
declare_sc_property(pgid, cgid_idx)

declare_sc_property(cgid, way_enable)
#endif
