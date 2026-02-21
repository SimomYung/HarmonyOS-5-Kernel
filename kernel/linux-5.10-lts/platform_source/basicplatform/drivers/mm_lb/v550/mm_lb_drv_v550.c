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

#include <linux/platform_device.h>
#include <linux/moduleparam.h>
#include <linux/of.h>
#include <asm/cacheflush.h>
#include <linux/dma-direction.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/genalloc.h>
#include <dsm/dsm_pub.h>
#include <linux/mm_lb/mm_lb.h>

#include "mm_lb_sec_ops_v550.h"
#include "mm_lb_plat_v550.h"
#include "mm_lb_drv_v550.h"

#define CFGCMO_PGID_GRP_SIZE 16
#define CFGCMO_PGID_GRP_MASK ((1UL << CFGCMO_PGID_GRP_SIZE) - 1)
#define MPAM_ID_MAX 15

enum lb_prio {
	LB_CACHE_LOW_PRIO = 0x0,
	LB_CACHE_HIGH_PRIO = 0x3,
};

typedef enum {
	NORMAL_SCENE = 0,
	STANDBY_SCENE,
	ALL_SCENE,
	MODEM_SCENE,
	NR_SCENE_TYPE,
} scene_type;

lb_device *lbdev;

static inline bool is_pgid_valid(u32 pgid)
{
	if (pgid < MAX_PGID_CNT)
		return true;

	lb_print(LB_ERROR, "invalid pgid %u\n", pgid);
	return false;
}

static inline bool is_cgid_valid(u32 cgid)
{
	if(cgid < MAX_CGID_CNT)
		return true;

	lb_print(LB_ERROR, "invalid cgid %u\n", cgid);
	return false;
}

/* type_name: the name of the gid struct defined by "typedef". (pgid/cgid) */
#define define_sc_property(type_name, property) \
	u32 get_##type_name##_##property(const u32 gid) { return get_##type_name(gid)->property; } \
	void set_##type_name##_##property(const u32 gid, const u32 val, const u32 direct) { \
		if (is_##type_name##_valid(gid)) \
			get_##type_name(gid)->property = val; \
	}

define_sc_property(cgid, way_search)
define_sc_property(cgid, way_enable)
define_sc_property(cgid, quota)
define_sc_property(cgid, bypass)
define_sc_property(cgid, self_replaceable)
define_sc_property(cgid, tag_pgid_chg_en)

define_sc_property(pgid, cgid_idx)
define_sc_property(pgid, sgid_idx)
define_sc_property(pgid, disable)
define_sc_property(pgid, drop)
define_sc_property(pgid, self_replaceable)
define_sc_property(pgid, priority)
define_sc_property(pgid, rd_policy)
define_sc_property(pgid, wr_policy)
define_sc_property(pgid, initial_age0)
define_sc_property(pgid, initial_age1)
define_sc_property(pgid, revisit_age)
define_sc_property(pgid, nwr_revisit_age)
define_sc_property(pgid, initial_age)
define_sc_property(pgid, wr_revisit_age)
define_sc_property(pgid, scene)
define_sc_property(pgid, drop_en)
define_sc_property(pgid, mode)
define_sc_property(pgid, trigger_en)
define_sc_property(pgid, stash_en)
define_sc_property(pgid, vacmo_en)
define_sc_property(pgid, keep_cleansh)
define_sc_property(pgid, writeevict_en)
define_sc_property(pgid, sot_ignore)
define_sc_property(pgid, passdirty_en)
define_sc_property(pgid, schint_en)
define_sc_property(pgid, autoclean_en)
define_sc_property(pgid, prefetch_en)
define_sc_property(pgid, paf_en)
define_sc_property(pgid, small_burst_na)
define_sc_property(pgid, hit_age_byp)
define_sc_property(pgid, small_burst_size)

cgid_info *get_cgid_info(u32 cgid)
{
	if (!is_cgid_valid(cgid))
		return NULL;

	return &lbdev->cgid_repo.vector[cgid];
}

cgid_t *get_cgid(u32 cgid)
{
	if (!is_cgid_valid(cgid))
		return NULL;

	return &lbdev->cgid_repo.vector[cgid].cgid;
}

static inline u32 get_cgid_idx(cgid_t *cgid)
{
	return cgid - get_cgid(0);
}

pgid_info *get_pgid_info(u32 pgid)
{
	if (!is_pgid_valid(pgid))
		return NULL;

	return &lbdev->pgid_repo.vector[pgid];
}

pgid_t *get_pgid(u32 pgid)
{
	if (!is_pgid_valid(pgid))
		return NULL;

	return &lbdev->pgid_repo.vector[pgid].pgid;
}

static inline u32 get_pgid_idx(pgid_t *pgid)
{
	return pgid - get_pgid(0);
}

int get_cpu_pgid(u32 pgid_idx)
{
	pgid_t *pgid = NULL;

	pgid = get_pgid(pgid_idx);
	if (!pgid) {
		lb_print(LB_ERROR, "%s: error pgid: %u\n", pgid_idx);
		return -EINVAL;
	}

	return (int)pgid->sgid_idx;
}

static cgid_repo_ops c_repo_ops = {
	.get = get_cgid,
	.valid = is_cgid_valid,
	.idx = get_cgid_idx,
};

static pgid_repo_ops p_repo_ops = {
	.get = get_pgid,
	.valid = is_pgid_valid,
	.idx = get_pgid_idx,
};

static inline void sc_repo_init(lb_device *lb_dev)
{
	/*
	 * We need init global lbdev pointer here
	 * because it will be used in the load_p/cgids
	 * process by calling set_p/cgid_xxx();
	 */
	lbdev = lb_dev;
	lb_dev->pgid_repo.ops = &p_repo_ops;
	lb_dev->cgid_repo.ops = &c_repo_ops;
}

static int load_lbdev_data(const struct device_node *dt_node, lb_device *lbd)
{
	u32 prot;
	int ret;

	ret = of_property_read_u32(dt_node, "max-quota", &prot);
	if (ret < 0) {
		lb_print(LB_ERROR, "failed to get max-quota\n");
		return -EINVAL;
	}
	lbd->max_quota = prot;

	ret = of_property_read_u32(dt_node, "max-size", &prot);
	if (ret < 0) {
		lb_print(LB_ERROR, "failed to get max-size\n");
		return -EINVAL;
	}
	lbd->max_size = prot;

	ret = of_property_read_u32(dt_node, "slc-idx", &prot);
	if (ret < 0) {
		lb_print(LB_ERROR, "failed to get slice idx\n");
		return -EINVAL;
	}
	lbd->slc_idx = prot;

	ret = of_property_read_u32(dt_node, "way-en", &prot);
	if (ret < 0) {
		lb_print(LB_ERROR, "failed to get enable way\n");
		return -EINVAL;
	}
	lbd->way_en = prot;

	return 0;
}

#ifdef CONFIG_MM_LB_V560
static void split_and_set_drv_cfg(u32 pgid, u32 val)
{
	hw_reg_pgid pgid_ent = {0};

	pgid_ent.pgid_drv_cfg.value = val;
	set_pgid_paf_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_paf_en, FALSE);
	set_pgid_prefetch_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_prefetch_en, FALSE);
	set_pgid_autoclean_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_autoclean_en, FALSE);
	set_pgid_schint_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_schint_en, FALSE);
	set_pgid_keep_cleansh(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_keep_cleansh, FALSE);
	set_pgid_vacmo_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_vacmo_en, FALSE);
	set_pgid_stash_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_stash_en, FALSE);
	set_pgid_trigger_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_trigger_en, FALSE);
	set_pgid_mode(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_mode, FALSE);
	set_pgid_sot_ignore(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_sot_ignore, FALSE);
	/*
	 * Ignore cgid_idx and sgid_idx bits in dts's drv_cfg attr.
	 * These two attrs are read from independent dts node attr for
	 * the convenience of human reading and maintanence.
	 */
	set_pgid_passdirty_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_passdirty_en, FALSE);
	/* pgid_cgid_idx/pgid_sgid_idx configured separately on the dts */
	set_pgid_writeevict_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_writeevict_en, FALSE);
	set_pgid_hit_age_byp(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_hit_age_byp, FALSE);
}

static void split_and_set_mst_cfg(u32 pgid, u32 value)
{
	hw_reg_pgid pgid_ent = {0};

	pgid_ent.pgid_mst_cfg.value = value;
	set_pgid_wr_revisit_age(pgid, pgid_ent.pgid_mst_cfg.reg.pgid_wr_revisit_age, FALSE);
	set_pgid_nwr_revisit_age(pgid, pgid_ent.pgid_mst_cfg.reg.pgid_nwr_revisit_age, FALSE);
	set_pgid_initial_age(pgid, pgid_ent.pgid_mst_cfg.reg.pgid_initial_age, FALSE);
	set_pgid_wr_policy(pgid, pgid_ent.pgid_mst_cfg.reg.pgid_wr_policy, FALSE);
	set_pgid_rd_policy(pgid, pgid_ent.pgid_mst_cfg.reg.pgid_rd_policy, FALSE);
	set_pgid_priority(pgid, pgid_ent.pgid_mst_cfg.reg.pgid_priority, FALSE);
	set_pgid_self_replaceable(pgid, pgid_ent.pgid_mst_cfg.reg.pgid_self_replaceable, FALSE);
	/* ignore disable bits in the mst_cfg attr, and set it to disable when loading from dts */
	set_pgid_drop(pgid, FALSE, FALSE);
	set_pgid_disable(pgid, TRUE, FALSE);
	set_pgid_small_burst_size(pgid, pgid_ent.pgid_mst_cfg.reg.pgid_small_burst_size, FALSE);
	set_pgid_small_burst_na(pgid, pgid_ent.pgid_mst_cfg.reg.pgid_small_burst_na, FALSE);
}
#else
static void split_and_set_drv_cfg(u32 pgid, u32 val)
{
	hw_reg_pgid pgid_ent = {0};

	pgid_ent.pgid_drv_cfg.value = val;
	set_pgid_small_burst_na(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_small_burst_na, FALSE);
	set_pgid_paf_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_paf_en, FALSE);
	set_pgid_prefetch_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_prefetch_en, FALSE);
	set_pgid_autoclean_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_autoclean_en, FALSE);
	set_pgid_schint_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_schint_en, FALSE);
	set_pgid_keep_cleansh(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_keep_cleansh, FALSE);
	set_pgid_vacmo_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_vacmo_en, FALSE);
	set_pgid_stash_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_stash_en, FALSE);
	set_pgid_trigger_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_trigger_en, FALSE);
	set_pgid_mode(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_mode, FALSE);
	set_pgid_sot_ignore(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_sot_ignore, FALSE);
	/*
	 * Ignore cgid_idx and sgid_idx bits in dts's drv_cfg attr.
	 * These two attrs are read from independent dts node attr for
	 * the convenience of human reading and maintanence.
	 */
	set_pgid_passdirty_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_passdirty_en, FALSE);
	/* pgid_cgid_idx/pgid_sgid_idx configured separately on the dts */
	set_pgid_writeevict_en(pgid, pgid_ent.pgid_drv_cfg.reg.pgid_writeevict_en, FALSE);
}

static void split_and_set_mst_cfg(u32 pgid, u32 value)
{
	hw_reg_pgid pgid_ent = {0};

	pgid_ent.pgid_mst_cfg.value = value;
	set_pgid_revisit_age(pgid, pgid_ent.pgid_mst_cfg.reg.pgid_revisit_age, FALSE);
	set_pgid_initial_age0(pgid, pgid_ent.pgid_mst_cfg.reg.pgid_initial_age0, FALSE);
	set_pgid_initial_age1(pgid, pgid_ent.pgid_mst_cfg.reg.pgid_initial_age1, FALSE);
	set_pgid_wr_policy(pgid, pgid_ent.pgid_mst_cfg.reg.pgid_wr_policy, FALSE);
	set_pgid_rd_policy(pgid, pgid_ent.pgid_mst_cfg.reg.pgid_rd_policy, FALSE);
	set_pgid_priority(pgid, pgid_ent.pgid_mst_cfg.reg.pgid_priority, FALSE);
	set_pgid_self_replaceable(pgid, pgid_ent.pgid_mst_cfg.reg.pgid_self_replaceable, FALSE);
	/* ignore disable bits in the mst_cfg attr, and set it to disable when loading from dts */
	set_pgid_drop(pgid, FALSE, FALSE);
	set_pgid_disable(pgid, TRUE, FALSE);
}
#endif

static void set_pgid_org_prio(u32 pgid, u32 value)
{
	pgid_info *p_info = NULL;
	hw_reg_pgid pgid_ent = {0};

	pgid_ent.pgid_mst_cfg.value = value;
	p_info = get_pgid_info(pgid);
	if (!p_info) {
		lb_print(LB_ERROR, "invalid pgid %u\n", pgid);
		return;
	}

	p_info->org_prio = pgid_ent.pgid_mst_cfg.reg.pgid_priority;
}

static void set_cgid_org_quota(u32 cgid, u32 value)
{
	cgid_info *c_info = NULL;

	c_info = get_cgid_info(cgid);
	if (!c_info) {
		lb_print(LB_ERROR, "invalid cgid %u\n", cgid);
		return;
	}

	c_info->org_quota = value;
}

static void load_pgids(const struct device_node *dev_node, lb_device *lbd)
{
	u32 ret;
	u32 pgid_idx;
	u32 prot;
	u32 i;
	struct device_node *pgid_node = NULL;
	struct device_node *np = NULL;
	const char *name = NULL;

	pgid_node = of_get_child_by_name(dev_node, "lb-pgids");
	if (!pgid_node) {
		lb_print(LB_ERROR, "failed to get lb-pgids\n");
		return;
	}

	for_each_child_of_node(pgid_node, np) {
		ret = of_property_read_u32(np, "pgid", &pgid_idx);
		if (ret < 0) {
			lb_print(LB_ERROR, "failed to get pgid idx\n");
			continue;
		}

		if (!is_pgid_valid(pgid_idx)) {
			lb_print(LB_ERROR, "invalid pgid_idx %u\n", pgid_idx);
			continue;
		}

		ret = of_property_read_string(np, "name", &name);
		if (ret < 0) {
			lb_print(LB_ERROR, "failed to get name, pgid = %u\n", pgid_idx);
			continue;
		}
		lbd->pgid_repo.vector[pgid_idx].name = name;

		ret = of_property_read_u32(np, "cgid_idx", &prot);
		if (ret < 0 || !is_cgid_valid(prot)) {
			lb_print(LB_ERROR, "failed to get cgid_idx, pgid = %u\n", pgid_idx);
			continue;
		}
		set_pgid_cgid_idx(pgid_idx, prot, FALSE);
		lbd->cgid_repo.vector[prot].pgid_map_full |= BIT(pgid_idx);

		ret = of_property_read_u32(np, "sgid_idx", &prot);
		if (ret < 0 || !is_cgid_valid(prot)) {
			lb_print(LB_ERROR, "failed to get sgid_idx, pgid = %u\n", pgid_idx);
			continue;
		}
		set_pgid_sgid_idx(pgid_idx, prot, FALSE);

		ret = of_property_read_u32(np, "drv_cfg", &prot);
		if (ret < 0) {
			lb_print(LB_ERROR, "failed to get drv_cfg, pgid = %u\n", pgid_idx);
			continue;
		}
		split_and_set_drv_cfg(pgid_idx, prot);

		ret = of_property_read_u32(np, "mst_cfg", &prot);
		if (ret < 0) {
			lb_print(LB_ERROR, "failed to get mst_cfg, pgid = %u\n", pgid_idx);
			continue;
		}
		split_and_set_mst_cfg(pgid_idx, prot);
		set_pgid_org_prio(pgid_idx, prot);

		ret = of_property_read_u32(np, "scene", &prot);
		if (ret < 0) {
			lb_print(LB_ERROR, "failed to get scene, pgid = %u\n", pgid_idx);
			continue;
		}
		set_pgid_scene(pgid_idx, prot, FALSE);
		pgid_validate(pgid_idx);
	}

	for (i = 0; i < MAX_PGID_CNT; i++) {
		set_pgid_disable(i, TRUE, FALSE);
		spin_lock_init(&(get_pgid_info(i)->lock));
	}
}

static void load_cgids(const struct device_node *dev_node)
{
	u32 ret;
	u32 cgid_idx;
	u32 prot;
	struct device_node *cgid_node = NULL;
	struct device_node *np = NULL;

	cgid_node = of_get_child_by_name(dev_node, "lb-cgids");
	if (!cgid_node) {
		lb_print(LB_ERROR, "failed to get lb-cgids\n");
		return;
	}

	for_each_child_of_node(cgid_node, np) {
		ret = of_property_read_u32(np, "cgid", &cgid_idx);
		if (ret < 0) {
			lb_print(LB_ERROR, "failed to get pgid idx\n");
			continue;
		}

		if (!is_cgid_valid(cgid_idx)) {
			lb_print(LB_ERROR, "invalid cgid_idx %u\n", cgid_idx);
			continue;
		}
		// todo: load cgid
		ret = of_property_read_u32(np, "quota", &prot);
		if (ret < 0) {
			lb_print(LB_ERROR, "failed to get quota, cgid = %u\n", cgid_idx);
			continue;
		}
		set_cgid_quota(cgid_idx, prot, FALSE);
		set_cgid_org_quota(cgid_idx, prot);

		ret = of_property_read_u32(np, "self_replace", &prot);
		if (ret < 0) {
			lb_print(LB_ERROR, "failed to get self_replace, cgid = %u\n", cgid_idx);
			continue;
		}
		set_cgid_self_replaceable(cgid_idx, prot, FALSE);

		set_cgid_tag_pgid_chg_en(cgid_idx, TRUE, FALSE);
		set_cgid_way_search(cgid_idx, lbdev->way_en & lbdev->way_efuse, FALSE);
		set_cgid_way_enable(cgid_idx, lbdev->way_en & lbdev->way_efuse, FALSE);
		spin_lock_init(&(get_cgid_info(cgid_idx)->lock));
	}
}

static int load_attr_from_dts(struct platform_device *pdev, lb_device *lbd)
{
	const struct device_node *dt_node = pdev->dev.of_node;

	lb_print(LB_INFO, "begin\n");
	if (load_lbdev_data(dt_node, lbd))
		return -EINVAL;

	load_pgids(dt_node, lbd);
	load_cgids(dt_node);

	lb_print(LB_INFO, "end\n");

	return 0;
}

u32 is_lb_available(void)
{
	return (u32)(lbdev && lbdev->is_available);
}

static int change_size_from_efuse(lb_device *lbd)
{
	int i = 0, j = 0;
	u32 way = lbd->way_en;
	u32 way_efuse = lbd->way_efuse;

	while (way) {
		if ((way & BIT(0)) == (way_efuse & BIT(0)))
			j++;

		way >>= 1;
		way_efuse >>= 1;
		i++;
	}

	if (unlikely(i == 0))
		return -EINVAL;

	lbd->max_quota = lbd->max_quota / i * j;
	lbd->max_size = lbd->max_size / i * j;

	lb_print(LB_ERROR, "way:0x%x, efuse:0x%x, quota:0x%x, size:0x%x\n",
		lbd->way_en, lbd->way_efuse, lbd->max_quota, lbd->max_size);

	return 0;
}

static inline u32 get_max_quota(void)
{
	if (!lbdev) {
		lb_print(LB_ERROR, "lbdev is null\n");
		return 0;
	} else {
		return lbdev->max_quota;
	}
}

int lb_set_master_policy(u32 pid, u32 quota, u32 prio)
{
	pgid_info *p_info = NULL;

	if (pid >= PID_MAX) {
		lb_print(LB_ERROR, "pid 0x%x invalid\n", pid);
		return -EINVAL;
	}

	if (prio > LB_CACHE_HIGH_PRIO)
		return -EINVAL;

	p_info = get_pgid_info(pid);
	spin_lock(&p_info->lock);
	set_pgid_priority(pid, prio, FALSE);
	pgid_validate(pid);
	spin_unlock(&p_info->lock);

	return 0;
}

int lb_reset_master_policy(u32 pid)
{
	pgid_info *p_info = NULL;

	if (pid >= PID_MAX) {
		lb_print(LB_ERROR, "pid %u invalid\n", pid);
		return -EINVAL;
	}

	p_info = get_pgid_info(pid);
	spin_lock(&p_info->lock);
	set_pgid_priority(pid, p_info->org_prio, FALSE);
	pgid_validate(pid);
	spin_unlock(&p_info->lock);

	return 0;
}

void lb_pgid_enable(u32 pgid)
{
	u32 cgid_idx;
	cgid_info *c_info = NULL;
	bool need_en_cgid = false;

	set_pgid_disable(pgid, FALSE, FALSE);
	cgid_idx = get_pgid_cgid_idx(pgid);
	c_info = get_cgid_info(cgid_idx);

	spin_lock(&c_info->lock);
	if (c_info->pgid_map == 0)
		need_en_cgid = true;
	c_info->pgid_map |= BIT(pgid);

	pgid_validate(pgid);
	if (need_en_cgid) {
		set_cgid_bypass(cgid_idx, FALSE, FALSE);
		cgid_validate(cgid_idx);
	}
	if (pgid == 0)
		lb_exclusive_enable();
	spin_unlock(&c_info->lock);
}

static int cfgcmo_for_cgid_bypass(u32 cgid_idx, u64 map, ops_type ops)
{
	u64 left_pgids;
	u64 pgids_in_grp;
	u64 grp_id = 0;
	unsigned long flags;

	spin_lock_irqsave(&lbdev->lock, flags);
	set_cgid_tag_pgid_chg_en_direct(cgid_idx, FALSE);
	left_pgids = map;

	while(left_pgids) {
		pgids_in_grp = left_pgids & CFGCMO_PGID_GRP_MASK;
		if (pgids_in_grp) {
			if (lb_ops_cache(ops, CMO_BY_GID,
				pgids_in_grp | (grp_id << CFGCMO_PGID_GRP_SIZE), 0, 0)) {
				spin_unlock_irqrestore(&lbdev->lock, flags);
				return -1;
			}
		}
		left_pgids >>= CFGCMO_PGID_GRP_SIZE;
		grp_id++;
	}
	lb_cmo_sync();
	set_cgid_tag_pgid_chg_en_direct(cgid_idx, TRUE);
	spin_unlock_irqrestore(&lbdev->lock, flags);
	return 0;
}

u32 lb_pgid_cnt(u32 pgid_idx)
{
	unsigned long flags;
	u32 grp_id;
	u32 ret;

	grp_id = pgid_idx / CFGCMO_PGID_GRP_SIZE;
	spin_lock_irqsave(&lbdev->lock, flags);
	if (lb_ops_cache(QUOTA_CNT, CMO_BY_GID,
		BIT(pgid_idx % CFGCMO_PGID_GRP_SIZE) | (grp_id << CFGCMO_PGID_GRP_SIZE), 0, 0)) {
		spin_unlock_irqrestore(&lbdev->lock, flags);
		return -1;
	}
	lb_cmo_sync();

	ret = check_pgid_cnt();
	spin_unlock_irqrestore(&lbdev->lock, flags);
	lb_print(LB_INFO, "pgid-%u total cnt: 0x%x\n", pgid_idx, ret);
	return ret;
}

void lb_pgid_bypass(u32 pgid)
{
	u32 cgid_idx;
	cgid_info *c_info = NULL;

	set_pgid_disable(pgid, TRUE, FALSE);
	cgid_idx = get_pgid_cgid_idx(pgid);
	c_info = get_cgid_info(cgid_idx);

	spin_lock(&c_info->lock);
	if (pgid == 0)
		lb_exclusive_bypass();
	c_info->pgid_map &= ~BIT(pgid);
	if (!c_info->pgid_map) {
		/* set software flag to bypass and prepare for validate in register */
		set_cgid_bypass(cgid_idx, TRUE, FALSE);
		/* clear quota by writing regs directly. Avoid new cacheline incoming */
		clear_quota(cgid_idx);
		cfgcmo_for_cgid_bypass(cgid_idx, c_info->pgid_map_full, CI);
		cgid_validate(cgid_idx);
	}
	pgid_validate(pgid);
	spin_unlock(&c_info->lock);
}

static void lb_invalid_cache(u32 pgid, u64 pa, size_t sz)
{
	unsigned long flags = 0;

	if (!lbdev) {
		lb_print(LB_ERROR, "lbdev is null\n");
		return;
	}

	spin_lock_irqsave(&lbdev->lock, flags);

	if (lb_ops_cache(INV, CMO_BY_4XKPA, pgid, pa, sz))
		goto unlock;

	/* ensure cmo ops complete */
	mb();

	lb_cmo_sync();
unlock:
	spin_unlock_irqrestore(&lbdev->lock, flags);
}

void lb_cmo_by_gid(u32 pgid, u32 ops)
{
	u32 cgid_idx;
	cgid_info *c_info = NULL;

	if (!is_pgid_valid(pgid)) {
		lb_print(LB_ERROR, "invalid pgid %u\n", pgid);
		return;
	}

	cgid_idx = get_pgid_cgid_idx(pgid);
	c_info = get_cgid_info(cgid_idx);
	if (!c_info) {
		lb_print(LB_ERROR, "invalid cgid %u\n", cgid_idx);
		return;
	}

	cfgcmo_for_cgid_bypass(cgid_idx,
		c_info->pgid_map_full, ops << CMD_OP_TYPE_SHIFT);
}
EXPORT_SYMBOL(lb_cmo_by_gid);

int lb_mpam_way_set(u32 mpam_id, u32 way)
{
	if (mpam_id > MPAM_ID_MAX)
		return -EINVAL;
 
	if (way & ~(lbdev->way_en & lbdev->way_efuse))
		return -EINVAL;
 
	lb_mpam_way_enable(mpam_id, way);
	return 0;
}

int lb_mpam_way_reset(u32 mpam_id)
{
	if (mpam_id > MPAM_ID_MAX)
		return -EINVAL;
 
	lb_mpam_way_enable(mpam_id, lbdev->way_en & lbdev->way_efuse);
	return 0;
}

#ifdef CONFIG_MM_LB_L3_EXTENSION
void lb_ci_cache_exclusive(void)
{
	u32 r_quota;
	unsigned long flags = 0;

	if (!is_lb_available())
		return;

	if (!lbdev) {
		lb_print(LB_ERROR, "lbdev is null\n");
		return;
	}

	spin_lock_irqsave(&lbdev->lock, flags);

	/*
	 * save gid0 quota
	 * set gid0 quota 0
	 */
	r_quota = clear_quota(0);

	if (lb_ops_cache(CI, CMO_BY_GID, BIT(0), 0, 0))
		goto unlock;

	/* ensure cmo ops complete */
	mb();

	lb_cmo_sync();

	/* recover gid0 quota */
	set_quota(0, r_quota);

unlock:
	spin_unlock_irqrestore(&lbdev->lock, flags);
}
#endif

#ifndef CONFIG_LIBLINUX
static void vm_uninit(lb_device *lbd)
{
	int i;
	vm_info *vm = NULL;
	lb_area *vm_ar = NULL;
	lb_area *tmp_ar = NULL;
	pgid_info *p_info = NULL;

	for (i = 0; i < MAX_PGID_CNT; i++) {
		p_info = get_pgid_info(i);
		vm = p_info->vm;
		if (!vm)
			continue;

		list_for_each_entry_safe(vm_ar, tmp_ar, &vm->list, next) {
			list_del(&vm_ar->next);

			if (vm_ar->area)
				free_vm_area(vm_ar->area);

			kfree(vm_ar);
		}

		if (vm->pool)
			gen_pool_destroy(vm->pool);

		mutex_destroy(&vm->mutex);

		kfree(vm);
		p_info->vm = NULL;
	}
}

static int gid_vm_init(lb_device *lbd)
{
	int i;
	vm_info *vm = NULL;
	pgid_info *p_info = NULL;

	for (i = 0; i < MAX_PGID_CNT; i++) {
		vm = kzalloc(sizeof(vm_info), GFP_KERNEL);
		if (!vm) {
			lb_print(LB_ERROR, "kzalloc failed\n");
			goto vm_free;
		}
		p_info = get_pgid_info(i);
		p_info->vm = vm;

		mutex_init(&(vm->mutex));
		INIT_LIST_HEAD(&(vm->list));

		vm->pool = gen_pool_create(PAGE_SHIFT, -1);
		if (!vm->pool) {
			lb_print(LB_ERROR, "gen pool create failed\n");
			goto vm_free;
		}

		if (add_gid_vm(vm, SZ_64M)) {
			lb_print(LB_ERROR, "add pool failed\n");
			goto vm_free;
		}
	}

	return 0;

vm_free:
	vm_uninit(lbd);
	return -ENOMEM;
}
#endif

int set_page(u32 pgid, struct page *page_alloc, size_t count)
{
	void *vaddr = NULL;
	struct page *pg = page_alloc;
	struct page **pages = NULL;
	struct page **tmp = NULL;
	size_t i;
	pgprot_t lbva_prot;
	pgid_info *p_info = NULL;

#ifdef CONFIG_MM_LB_L3_EXTENSION
	change_secpage_range(page_to_phys(page_alloc),
		(uintptr_t)page_address(page_alloc),
		count << PAGE_SHIFT, __pgprot(PROT_DEVICE_nGnRE));
	flush_tlb_kernel_range((uintptr_t)page_to_virt(page_alloc),
		(uintptr_t)page_to_virt(pg) + (count << PAGE_SHIFT));
#endif

	/*
	 * invalid acpu cache by normal va
	 * it is necessary by normal va
	 * cache |--va--| |--va with gid--|
	 * lb             |-------lb------|
	 * ddr   |------|
	 */
	__dma_unmap_area(page_to_virt(pg), count << PAGE_SHIFT,
			DMA_FROM_DEVICE);

	pages = kcalloc(count, sizeof(struct page *), GFP_KERNEL);
	if (!pages) {
		lb_print(LB_ERROR, "zalloc page\n");
		goto change_mapping;
	}

	tmp = pages;
	for (i = 0; i < count; i++)
		*(tmp++) = pg++;

	p_info = get_pgid_info(pgid);

	lbva_prot = lb_get_page_osh_flag(page_alloc) ? PAGE_OUTER_SHARED : __pgprot(PROT_NORMAL_NC);
#ifndef CONFIG_LIBLINUX
	vaddr = vm_map(p_info->vm, pages, count, pgprot_lb(lbva_prot, pgid));
#else
	vaddr = vmap(pages, count, VM_MAP, pgprot_lb(lbva_prot, pgid));
#endif
	if (!vaddr) {
		lb_print(LB_ERROR, "vm map failed\n");
		goto free_page;
	}

	kfree(pages);

	/* set the page privite */
	pg = page_alloc;
	for (i = 0; i < count; i++) {
		set_page_private(pg, lb_get_page_osh_flag(pg) | (uintptr_t)vaddr | pgid);
		SetPagePrivate(pg);
		SetPageLB(pg);
		pg++;
		vaddr += PAGE_SIZE;
	}

	return 0;

free_page:
	kfree(pages);

change_mapping:
#ifdef CONFIG_MM_LB_L3_EXTENSION
	change_secpage_range(page_to_phys(page_alloc),
		(uintptr_t)page_address(page_alloc),
		count << PAGE_SHIFT, PAGE_KERNEL);
	flush_tlb_kernel_range((uintptr_t)page_to_virt(page_alloc),
		(uintptr_t)page_to_virt(page_alloc) + (count << PAGE_SHIFT));
#endif

	return -ENOMEM;
}

#ifdef CONFIG_DMABUF_UDK_EN
int set_page_from_udk(u32 pgid, struct page *page_alloc, size_t count)
{
	void *vaddr = NULL;
	struct page *pg = page_alloc;
	struct page **pages = NULL;
	struct page **tmp = NULL;
	size_t i;
	pgprot_t lbva_prot;
	pgid_info *p_info = NULL;

	pages = kcalloc(count, sizeof(struct page *), GFP_KERNEL);
	if (!pages) {
		lb_print(LB_ERROR, "zalloc page\n");
		goto change_mapping;
	}

	tmp = pages;
	for (i = 0; i < count; i++)
		*(tmp++) = pg++;

	p_info = get_pgid_info(pgid);

	lbva_prot = lb_get_page_osh_flag(page_alloc) ? PAGE_OUTER_SHARED : __pgprot(PROT_NORMAL_NC);
#ifndef CONFIG_LIBLINUX
	vaddr = vm_map(p_info->vm, pages, count, pgprot_lb(lbva_prot, pgid));
#else
	vaddr = vmap(pages, count, VM_MAP, pgprot_lb(lbva_prot, pgid));
#endif
	if (!vaddr) {
		lb_print(LB_ERROR, "vm map failed\n");
		goto free_page;
	}

	kfree(pages);

	/* set the page privite */
	pg = page_alloc;
	for (i = 0; i < count; i++) {
		set_page_private(pg, lb_get_page_osh_flag(pg) | (uintptr_t)vaddr | pgid);
		SetPagePrivate(pg);
		SetPageLB(pg);
		pg++;
		vaddr += PAGE_SIZE;
	}

	return 0;

free_page:
	kfree(pages);

change_mapping:
	return -ENOMEM;
}
#endif /* CONFIG_DMABUF_UDK_EN */

static void inv_cache_with_nc_gid_va(u32 pgid, struct page *page, size_t count)
{
	u32 i;
	struct page *pg = page;
	struct page **pages = NULL;
	struct page **tmp = NULL;
	pgid_info *p_info = NULL;
	u64 size = count << PAGE_SHIFT;
	void *vaddr = NULL;

	if (!lb_get_page_osh_flag(pg))
		return;

	pages = kcalloc(count, sizeof(struct page *), GFP_KERNEL);
	if (!pages) {
		lb_print(LB_ERROR, "alloc pages fail!\n");
		return;
	}

	tmp = pages;
	for (i = 0; i < count; i++)
		*(tmp++) = pg++;

	p_info = get_pgid_info(pgid);
#ifndef CONFIG_LIBLINUX
	vaddr = vm_map(p_info->vm, pages, count, pgprot_lb(__pgprot(PROT_DEVICE_nGnRE), pgid));
#else
	vaddr = vmap(pages, count, VM_MAP, pgprot_lb(__pgprot(PROT_DEVICE_nGnRE), pgid));
#endif
	if (!vaddr) {
		lb_print(LB_ERROR, "vm map failed\n");
		goto free_pages;
	}

	__dma_unmap_area(vaddr, size, DMA_FROM_DEVICE);
#ifndef CONFIG_LIBLINUX
	vm_unmap(p_info->vm, vaddr, size);
#else
	vunmap(vaddr);
#endif
free_pages:
	kfree(pages);
}

void reset_page(u32 pgid, struct page *page, size_t count)
{
	int i;
	u32 pg_pgid;
	void *vaddr = NULL;
	pgid_info *p_info = NULL;
	struct page *pg = page;
	phys_addr_t phys = page_to_phys(pg);
	u64 size = count << PAGE_SHIFT;

	if (!page) {
		lb_print(LB_ERROR, "reset page falied\n");
		return;
	}

	WARN_ON(!PageLB(pg));
	pg_pgid = lb_page_to_gid(pg);
	WARN(pgid != pg_pgid, "pgid:%u != pg_pgid:%u\n", pgid, pg_pgid);

	/*
	 * invalid acpu cache by lb va
	 * it is necessary by lb va
	 * cache |--va--| |--va with gid--|
	 * lb             |-------lb------|
	 * ddr   |------|
	 */
	vaddr = lb_page_to_virt(page);
	__dma_unmap_area(vaddr, size, DMA_FROM_DEVICE);

	p_info = get_pgid_info(pgid);
#ifndef CONFIG_LIBLINUX
	vm_unmap(p_info->vm, vaddr, size);
#else
	vunmap(vaddr);
#endif

	inv_cache_with_nc_gid_va(pgid, page, count);

	for (i = 0; i < (int)count; i++) {
		set_page_private(pg, 0);
		ClearPagePrivate(pg);
		ClearPageLB(pg);
		pg++;
	}

	/* invalid system cache */
	lb_invalid_cache(pg_pgid, phys, size);

#ifdef CONFIG_MM_LB_L3_EXTENSION
	change_secpage_range(page_to_phys(page),
		(uintptr_t)page_address(page),
		count << PAGE_SHIFT, PAGE_KERNEL);
	flush_tlb_kernel_range((uintptr_t)page_to_virt(page),
		(uintptr_t)page_to_virt(page) + (count << PAGE_SHIFT));
#endif
}

#ifdef CONFIG_DMABUF_UDK_EN
void reset_page_from_udk(u32 pgid, struct page *page, size_t count)
{
	int i;
	u32 pg_pgid;
	void *vaddr = NULL;
	pgid_info *p_info = NULL;
	struct page *pg = page;
	phys_addr_t phys = page_to_phys(pg);
	u64 size = count << PAGE_SHIFT;

	if (!page) {
		lb_print(LB_ERROR, "reset page falied\n");
		return;
	}

	WARN_ON(!PageLB(pg));
	pg_pgid = lb_page_to_gid(pg);
	WARN(pgid != pg_pgid, "pgid:%u != pg_pgid:%u\n", pgid, pg_pgid);

	p_info = get_pgid_info(pgid);

	vaddr = lb_page_to_virt(page);
	__dma_unmap_area(vaddr, size, DMA_FROM_DEVICE);

#ifndef CONFIG_LIBLINUX
	vm_unmap(p_info->vm, vaddr, size);
#else
	vunmap(vaddr);
#endif

	inv_cache_with_nc_gid_va(pgid, page, count);

	for (i = 0; i < (int)count; i++) {
		set_page_private(pg, 0);
		ClearPagePrivate(pg);
		ClearPageLB(pg);
		pg++;
	}

	/* invalid system cache */
	lb_invalid_cache(pg_pgid, phys, size);
}
#endif /* CONFIG_DMABUF_UDK_EN */

static int irq_init(struct platform_device *pdev, lb_device *lbd)
{
	int irq;

	lb_print(LB_INFO, "begin\n");

	/* register intr */
	irq = platform_get_irq_byname(pdev, "dfx-intr");
	if (irq < 0) {
		lb_print(LB_ERROR, "get dfx irq failed\n");
		return -EINVAL;
	}
	lbd->dfx_irq = irq;

	/* enable intr reg */
	if (init_irq(lbd)) {
		lb_print(LB_ERROR, "reset ip failed\n");
		return -EINVAL;
	}

	lb_print(LB_INFO, "end\n");

	return 0;
}

static void dsm_lb_handle_work(struct work_struct *work)
{
	struct dsm_client *ai_client;

	ai_client = dsm_find_client("dsm_ai");
	if (!ai_client) {
		lb_print(LB_ERROR, "lb dsm is null\n");
		return;
	}
	lbdev->dsm.ai_client = ai_client;
}

static int dsm_init(lb_device *lbd)
{
	INIT_WORK(&(lbd->dsm.dsm_work), dsm_lb_handle_work);

	return 0;
}

static int lb_probe(struct platform_device *pdev)
{
	struct resource *res = NULL;
	lb_device *lb_dev = NULL;
	struct device *dev = &pdev->dev;

	lb_print(LB_ERROR, "begin\n");

	lb_dev = devm_kzalloc(dev, sizeof(*lb_dev), GFP_KERNEL);
	if (!lb_dev) {
		lb_print(LB_ERROR, "failed to allocate lb_dev\n");
		return -ENOMEM;
	}
	lb_dev->dev = dev;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		lb_print(LB_ERROR, "failed to get resource\n");
		return -EINVAL;
	}

	lb_dev->base = devm_ioremap_resource(dev, res);
	if (!lb_dev->base) {
		lb_print(LB_ERROR, "io remap failed\n");
		return -EINVAL;
	}

	lb_dev->way_efuse = get_lb_efuse();

	lb_dev->is_available = lb_dev->way_efuse ? true : false;
	if (!lb_dev->is_available) {
		lb_print(LB_ERROR, "lb unusable\n");
		lbdev = lb_dev;
		return 0;
	}

	spin_lock_init(&lb_dev->lock);

	sc_repo_init(lb_dev);
	/* get the gid set from dts, relies on repo init */
	if (load_attr_from_dts(pdev, lb_dev)) {
		lb_print(LB_ERROR, "failed to get gid set from dts\n");
		return -EINVAL;
	}

	if (change_size_from_efuse(lb_dev)) {
		lb_print(LB_ERROR, "change size from efuse failed\n");
		return -EINVAL;
	}

	if (irq_init(pdev, lb_dev)) {
		lb_print(LB_ERROR, "lb init failed\n");
		return -EINVAL;
	}

#ifndef CONFIG_LIBLINUX
	if (gid_vm_init(lb_dev)) {
		lb_print(LB_ERROR, "gid init failed\n");
		return -EINVAL;
	}
#endif

	if (lb_pmu_init(pdev, lb_dev)) {
		lb_print(LB_ERROR, "pmu init failed\n");
		return -EINVAL;
	}

	if (dsm_init(lb_dev)) {
		lb_print(LB_ERROR, "dsm init failed\n");
		return -EINVAL;
	}

	/* register to local device */
	lbdev = lb_dev;
	lb_print(LB_ERROR, "lb init success\n");
	return 0;
}

static int lb_remove(struct platform_device *pdev)
{
	lbdev = NULL;
	return 0;
}

static void gid_save(void)
{
	u32 i;
	pgid_info *p_info = NULL;
	unsigned long flags = 0;

	for (i = 0; i < MAX_PGID_CNT; i++) {
		p_info = get_pgid_info(i);
		spin_lock_irqsave(&p_info->lock, flags);
		if (!p_info->pgid.disable) {
			lb_print(LB_ERROR, "pgid %u\n", i);
			if (p_info->pgid.scene == NORMAL_SCENE) {
				lb_pgid_bypass(i);
			} else if (p_info->pgid.scene == STANDBY_SCENE) {
				lb_pgid_enable(i);
				lb_pgid_request(i);
			} else {
				lb_print(LB_INFO, "all scene\n");
			}
			set_pgid_disable(i, FALSE, FALSE);
		}
		spin_unlock_irqrestore(&p_info->lock, flags);
	}
}

static void gid_restore(void)
{
	u32 i;
	pgid_info *p_info = NULL;
	unsigned long flags = 0;

	for (i = 0; i < MAX_PGID_CNT; i++) {
		p_info = get_pgid_info(i);
		if (p_info->pgid.scene != MODEM_SCENE)
			pgid_validate(i);
	}

	for (i = 0; i < MAX_PGID_CNT; i++) {
		p_info = get_pgid_info(i);
		spin_lock_irqsave(&p_info->lock, flags);
		if (!p_info->pgid.disable) {
			lb_print(LB_ERROR, "pgid %u\n", i);
			if (p_info->pgid.scene == NORMAL_SCENE) {
				lb_pgid_enable(i);
			} else if (p_info->pgid.scene == STANDBY_SCENE) {
				// buffer mode to cache mode
				set_pgid_mode(i, 0x0, FALSE);
				pgid_validate(i);
				lb_pgid_bypass(i);
				lb_pgid_release(i);
				set_pgid_mode(i, 0x1, FALSE);
			} else {
				lb_print(LB_INFO, "all scene\n");
			}
			set_pgid_disable(i, FALSE, FALSE);
		}
		spin_unlock_irqrestore(&p_info->lock, flags);
	}
}

static int lb_suspend(struct device *dev)
{
	lb_print(LB_ERROR, "begin\n");

	if (!is_lb_available())
		return 0;

	gid_save();

	lb_print(LB_ERROR, "end\n");

	return 0;
}

static int lb_resume(struct device *dev)
{
	lb_print(LB_ERROR, "begin\n");

	if (!is_lb_available())
		return 0;

	gid_restore();

	lb_print(LB_ERROR, "end\n");

	return 0;
}

static SIMPLE_DEV_PM_OPS(lb_pm_ops, lb_suspend, lb_resume);

static const struct of_device_id lb_match_table[] = {
	{.compatible = "mm,mm-lb" },
	{},
};

static struct platform_driver lb_driver = {
	.probe = lb_probe,
	.remove = lb_remove,
	.driver = {
		.name = "mm-lb",
		.owner = THIS_MODULE,
		.of_match_table = lb_match_table,
		.pm = &lb_pm_ops,
	},
};

static int lb_init_ns(void)
{
	return platform_driver_register(&lb_driver);
}
subsys_initcall(lb_init_ns);
module_param_named(debug_level, lb_d_lvl, uint, 0644);
