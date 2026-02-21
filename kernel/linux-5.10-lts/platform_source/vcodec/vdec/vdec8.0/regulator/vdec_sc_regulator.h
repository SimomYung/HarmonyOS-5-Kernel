/*
 * vdec_sc_regulator.h
 *
 * This is for vdec sc related regulator
 *
 * Copyright (c) 2024-2024 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef VDEC_SC_REGULATOR_H
#define VDEC_SC_REGULATOR_H

#include "vcodec_types.h"

#define VDEC_RCHN_PXPC_START_SWID 21
#define VDEC_RCHN_PXPC_END_SWID   22 // vde_rchn_pxpc_sc + 1
#define VDEC_WCHN_PXPC_START_SWID 45
#define VDEC_WCHN_PXPC_END_SWID   47 // vde_wchn_pxpc1_sc + 1

#define VDEC_CACHE_AS_FUL_OFS 0x600
#define SMMU_TBU_SWID_CFG_OFS 0x100

typedef union {
	struct {
		uint32_t pref_num:8;
		uint32_t reserved:16;
		uint32_t pref_jump:4;
		uint32_t syscache_hint_sel:2;
		uint32_t pref_lock_msk:1;
		uint32_t pref_en:1;
	} bits;
	uint32_t u32;
} smmu_tbu_swid_cfg;

typedef union {
	struct {
		uint32_t cache_as_ful_nsafe:1;
		uint32_t sc_gid_hint_nsafe:6;
		uint32_t sc_new_hint_nsafe:4;
		uint32_t cache_nsafe:4;
		uint32_t domain_nsafe:2;
		uint32_t reserved:15;
	} bits;
	uint32_t u32;
} vdec_cache_as_ful;

void vdec_hal_cfg_pxpc_syscache(void);

#endif
