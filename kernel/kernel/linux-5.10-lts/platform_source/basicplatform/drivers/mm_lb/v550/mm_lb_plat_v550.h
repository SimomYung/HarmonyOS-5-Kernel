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

#ifndef __MM_LB_PLAT_V550_H
#define __MM_LB_PLAT_V550_H

#include <linux/types.h>
#include <linux/of.h>

#define dfx_err_info_req_mid(info) ((info[3] & 0xFE000000UL) >> 25)

#define LB_DFX_INFO_NUM 16
#define LB_ECC_INFO_NUM 6
#define SC_CGID_CNT_ADDR SOC_SC_CGID_CNT_ADDR

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

typedef struct {
	u32 way_search : 16;
	u32 way_enable : 16;
	u32 quota : 16;
	u32 bypass : 1;
	u32 self_replaceable : 1;
	u32 tag_pgid_chg_en : 1;
	u32 way_allc_lp : 16;
} cgid_t;

typedef union {
    u32 val;
    struct {
        u32 statis_mid        : 8;
        u32 statis_mid_mask_n : 8;
        u32 gid_statis_en     : 16;
    } reg;
} stat_ctrl0;

typedef union {
    u32 val;
    struct {
        u32 type_statis_en     : 11;
        u32 statis_mode        : 1;
        u32 bypass_statis_en   : 3;
        u32 slice_statis_en    : 1;
        u32 reserved           : 16;
    } reg;
} stat_ctrl1;

typedef struct {
	stat_ctrl0 ctrl0;
	stat_ctrl1 ctrl1;
	spinlock_t lock;
} stat_info;

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
} pgid_t;

int register_check(u64 addr, u32 mask, u32 val, u32 timeout);
void pack_pgid_attr(pgid_t *pgid, hw_reg_pgid *pgid_reg);
void cgid_validate(u32 cgid_idx);
u32 clear_quota(u32 cgid);
void set_quota(u32 cgid, u32 quota);
void split_and_set_drv_cfg(u32 pgid, u32 val);
void split_and_set_mst_cfg(u32 pgid, u32 value);
int load_cgid_cross_access_en(struct device_node *np, u32 cgid_idx);

#endif
