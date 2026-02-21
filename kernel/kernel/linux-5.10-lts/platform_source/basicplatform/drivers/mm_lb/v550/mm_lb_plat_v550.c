/*
 *
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

#include <linux/mm_lb/mm_lb_v550.h>

#include "mm_lb_priv_v550.h"
#include "mm_lb_dfx_err_handle.h"

#define CGID_BYP_TIMEOUT_US   1000

lb_dfx_type lb_dfx_info[LB_DFX_INFO_NUM] = {
	{1,  "access deep sleep tag ram", NULL},
	{2,  "sf ram 1 bit ecc err", NULL},
	{3,  "sf ram 1 bit ecc err hard", NULL},
	{4,  "sf ram 2 bit ecc err", NULL},
	{5,  "tag ram 1 bit ecc err", NULL},
	{6,  "tag ram 1 bit ecc err hard", NULL},
	{7,  "tag ram 2 bit ecc err", NULL},
	{8,  "ns invalid cfgcmo for sec gid, inv has been converted to clean&inv", NULL},
	{9,  "tag multi dirty", NULL},
	{11, "tag hit, cgid unmatch", post_process_pgid_hit_cgid_mismatch},
	{13, "buffer mode gid alloc cacheline fail", NULL},
	{78, "access deep sleep waygrp", NULL},
	{79, "access way which is in initing status", NULL},
	{80, "access way in static low power state", NULL},
	{82, "dram 1 bit ecc err", NULL},
	{83, "dram 2 bit ecc err", NULL},
};

lb_ecc_type lb_ecc_info[LB_ECC_INFO_NUM] = {
	{2, SINGLE_BIT_ERR},
	{4, MULTI_BIT_ERR},
	{5, SINGLE_BIT_ERR},
	{7, MULTI_BIT_ERR},
	{82, SINGLE_BIT_ERR},
	{83, MULTI_BIT_ERR},
};

define_sc_property(pgid, initial_age0)
define_sc_property(pgid, initial_age1)

/* Besides NCH, no chip plat use remote sc and should do nothing */
bool is_need_remote_sc(u32 pgid, u32 cgid_idx, u32 pwr_on) { return false; }
void lb_remote_sc_enable(u32 cgid, u32 pgid) { }
void lb_remote_sc_bypass(u32 cgid, u32 pgid) { }

void pack_pgid_attr(pgid_t *pgid, hw_reg_pgid *pgid_reg)
{
	if (!pgid || !pgid_reg) {
		lb_print(LB_ERROR, "input NULL!");
		return;
	}

	// pack pgid_drv_cfg
	pgid_reg->pgid_drv_cfg.value = 0;
	pgid_reg->pgid_drv_cfg.reg.pgid_small_burst_na = pgid->small_burst_na;
	pgid_reg->pgid_drv_cfg.reg.pgid_paf_en = pgid->paf_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_prefetch_en = pgid->prefetch_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_autoclean_en = pgid->autoclean_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_schint_en = pgid->schint_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_keep_cleansh = pgid->keep_cleansh;
	pgid_reg->pgid_drv_cfg.reg.pgid_vacmo_en = pgid->vacmo_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_stash_en = pgid->stash_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_trigger_en = pgid->trigger_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_mode = pgid->mode;
	pgid_reg->pgid_drv_cfg.reg.pgid_sot_ignore = pgid->sot_ignore;
	pgid_reg->pgid_drv_cfg.reg.pgid_passdirty_en = pgid->passdirty_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_cgid_idx =  pgid->cgid_idx;
#ifndef CONFIG_MM_LB_GID_PBHA
	/*
	 * Hardware sgid attr is only used for PA mode.
	 * On PBHA mode, it should be RES0.
	 * We init sgid in dt probe only for gpu's lb_alloc/free_pages.
	 */
	pgid_reg->pgid_drv_cfg.reg.pgid_sgid_idx = pgid->sgid_idx;
#endif
	pgid_reg->pgid_drv_cfg.reg.pgid_writeevict_en = pgid->writeevict_en;

	// pack pgid_mst_cfg
	pgid_reg->pgid_mst_cfg.value = 0;
	pgid_reg->pgid_mst_cfg.reg.pgid_revisit_age = pgid->revisit_age;
	pgid_reg->pgid_mst_cfg.reg.pgid_initial_age0 = pgid->initial_age0;
	pgid_reg->pgid_mst_cfg.reg.pgid_initial_age1 = pgid->initial_age1;
	pgid_reg->pgid_mst_cfg.reg.pgid_wr_policy = pgid->wr_policy;
	pgid_reg->pgid_mst_cfg.reg.pgid_rd_policy = pgid->rd_policy;
	pgid_reg->pgid_mst_cfg.reg.pgid_priority = pgid->priority;
	pgid_reg->pgid_mst_cfg.reg.pgid_self_replaceable = pgid->self_replaceable;
	pgid_reg->pgid_mst_cfg.reg.pgid_drop = 0; // do not set Read Only reg
	pgid_reg->pgid_mst_cfg.reg.pgid_drop_clr = !(pgid->drop); // W1PULSE
	pgid_reg->pgid_mst_cfg.reg.pgid_drop_set = pgid->drop; // W1PULSE
	pgid_reg->pgid_mst_cfg.reg.pgid_disable = 0; // do not set Read Only reg
	pgid_reg->pgid_mst_cfg.reg.pgid_disable_clr = !(pgid->disable); // W1PULSE
	pgid_reg->pgid_mst_cfg.reg.pgid_disable_set = pgid->disable; // W1PULSE
}

static void pack_cgid_attr(cgid_t *cgid, hw_reg_cgid *cgid_reg)
{
	if (!cgid || !cgid_reg) {
		lb_print(LB_ERROR, "input NULL!");
		return;
	}

	cgid_reg->cgid_way_allc.value = 0;
	if (!cgid->bypass) {
		cgid_reg->cgid_way_allc.reg.cgid_way_enable = cgid->way_enable;
		cgid_reg->cgid_way_allc.reg.cgid_way_search = cgid->way_search;
	}

	cgid_reg->cgid_cfg.value = 0;
	if (!cgid->bypass)
		cgid_reg->cgid_cfg.reg.cgid_quota = cgid->quota;
	cgid_reg->cgid_cfg.reg.cgid_self_replaceable = cgid->self_replaceable;
	cgid_reg->cgid_cfg.reg.cgid_bypass_en = cgid->bypass;
	cgid_reg->cgid_cfg.reg.cgid_tag_pgid_chg_en = cgid->tag_pgid_chg_en;

	lb_print(LB_ERROR, "cgid_way_allc: 0x%lx, cgid_cfg: 0x%lx\n",
		cgid_reg->cgid_way_allc.value, cgid_reg->cgid_cfg.value);
}

static int lb_cgid_bypass_config(u32 cgid, u32 bypass_en)
{
	u32 i;
	u64 addr;
	u32 quota;
	hw_reg_cgid cgid_reg;

	cgid_reg.cgid_cfg.value = readl(SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid));
	/*
	 * When enabling cgid, we should cfg bypass to 0
	 * before assigning any non-zero quota.
	 * We also assume that when the input bypass_en is 0,
	 * the register value of cgid_reg.bypass_en is currently 1,
	 * and the register value of cgid_reg.quota is 0.
	 */
	quota = cgid_reg.cgid_cfg.reg.cgid_quota;
	WARN(quota != 0, "quota is assigned 0x%x before setting bypass_en to 0", quota);

	cgid_reg.cgid_cfg.reg.cgid_bypass_en = bypass_en;
	writel(cgid_reg.cgid_cfg.value, SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid));

	for (i = 0; i < lbdev->slc_idx; i++) {
		addr = slc_addr(SOC_SC_CGID_BYP_SWITCH_STATUS_ADDR((u64)lbdev->base, cgid), i);
		if (register_check(addr, BIT(0), bypass_en ? 1 : 0, CGID_BYP_TIMEOUT_US)) {
			lb_print(LB_ERROR, "set cgid %u bypass %u failed!\n", cgid, bypass_en);
			return -1;
		}
	}

	return 0;
}

void cgid_validate(u32 cgid_idx)
{
	cgid_t *cgid = NULL;
	hw_reg_cgid cgid_reg = {0};

	if (!lbdev->cgid_repo.ops) {
		lb_print(LB_ERROR, "error: cgid_repo.ops NULL!");
		return;
	}

	cgid = lbdev->cgid_repo.ops->get(cgid_idx);
	if (!cgid) {
		lb_print(LB_ERROR, "error: cgid null!");
		return;
	}

	pack_cgid_attr(cgid, &cgid_reg);
	writel(cgid_reg.cgid_way_allc.value, SOC_SC_CGID_WAY_ALLC_ADDR(lbdev->base, cgid_idx));
	if (!cgid->bypass) {
		/* When enabling cgid, config bypass_en before assigning non-zero quota */
		if (lb_cgid_bypass_config(cgid_idx, cgid->bypass))
			WARN(1, "cgid bypass failed!");
	}
	writel(cgid_reg.cgid_cfg.value, SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid_idx));
}

u32 clear_quota(u32 cgid)
{
	u32 r_quota;
	hw_reg_cgid cgid_reg;

	cgid_reg.cgid_cfg.value = readl(SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid));
	r_quota = cgid_reg.cgid_cfg.reg.cgid_quota;
	cgid_reg.cgid_cfg.reg.cgid_quota = 0;
	writel(cgid_reg.cgid_cfg.value, SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid));

	return r_quota;
}

void set_quota(u32 cgid, u32 quota)
{
	hw_reg_cgid cgid_reg;

	cgid_reg.cgid_cfg.value = readl(SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid));
	cgid_reg.cgid_cfg.reg.cgid_quota = quota;
	writel(cgid_reg.cgid_cfg.value, SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid));
}

void split_and_set_drv_cfg(u32 pgid, u32 val)
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

void split_and_set_mst_cfg(u32 pgid, u32 value)
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

int load_cgid_cross_access_en(struct device_node *np, u32 cgid_idx)
{
	return 0;
}

static inline void start_flow(void)
{
	writel(1, SOC_SC_STATIS_GLB_EN_ADDR(lbdev->base));
}

u32 lb_set_flow(struct lb_flow_ctrl *flow, u32 statis_num)
{
	SOC_SC_STATIS_SLICE_CTRL0_UNION ctrl0 = {0};
	SOC_SC_STATIS_SLICE_CTRL0_UNION ctrl1 = {0};

	if (statis_num >= MAX_STATIS_NUM) {
		lb_print(LB_ERROR, "invalid statis num\n");
		return -1;
	}
	ctrl0.value = flow->ctrl0;
	ctrl1.value = flow->ctrl1;
	ctrl0.reg.gid_statis_en = BIT(flow->pid);

	writel(ctrl0.value, SOC_SC_STATIS_SLICE_CTRL0_ADDR(lbdev->base, statis_num));
	writel(ctrl1.value, SOC_SC_STATIS_SLICE_CTRL1_ADDR(lbdev->base, statis_num));
	start_flow();

	return 0;
}

int lb_mpam_set_replace_policy(u32 mpam_id, u32 revisit_age)
{
	return 0;
}

u32 lb_mpam_get_replace_policy(u32 mpam_id)
{
	return 0;
}

int lb_mpam_set_evict_en(u32 mpam_id, u32 enable)
{
	return 0;
}

u32 lb_mpam_get_evict_en(void)
{
	return 0;
}

int lb_alloc_syncbuf(u32 size)
{
	return 0;
}

int lb_release_syncbuf(void)
{
	return 0;
}