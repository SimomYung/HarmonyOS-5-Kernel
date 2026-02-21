/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Audit kernel uapi interface header file
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 29 10:17:33 CST 2021
 */

#ifndef UAPI_HMKERNEL_HVGR_MMU_MODULE_H
#define UAPI_HMKERNEL_HVGR_MMU_MODULE_H

#include <hmkernel/types.h>

enum __hvgr_mmu_cmd {
	__HVGR_MMU_CMD_CBIT_CONFIG = 0,
	__HVGR_MMU_CMD_HAL_UNLOCK,
	__HVGR_MMU_CMD_HAL_ENABLE,
	__HVGR_MMU_CMD_HAL_DISABLE,
	__HVGR_MMU_CMD_HAL_FLUSH_PT,
	__HVGR_MMU_CMD_HAL_FLUSH_L2_CACHES,
	__HVGR_MMU_CMD_IRQ_MASK,
	__HVGR_MMU_CMD_IRQ_ENABLE,
	__HVGR_MMU_CMD_IRQ_DISABLE,
	__HVGR_MMU_CMD_CLEAR_FAULT,
	__HVGR_MMU_CMD_UNMASK_FAULT,
	__HVGR_MMU_CMD_TBL_MAP,
	__HVGR_MMU_CMD_TBL_UNMAP,
	__HVGR_MMU_CMD_TBL_FREE_PGD,
	__HVGR_MMU_CMD_FAULT_CLEAR_AS,
	__HVGR_MMU_CMD_MAX,
};

typedef struct {
	__u32 val;
} hvgr_mmu_para_input;

typedef struct {
	__u32 no;
	__u32 fault_type;
} hvgr_mmu_para_as;

struct hvgr_mmu_cfg {
	__u64 transcfg;
	__u64 transtab;
	__u64 memattr;
};

typedef struct {
	__u32 asid;
	__u64 pgd_pa;
} hvgr_mmu_para_enable;

typedef struct {
	__u32 asid;
	__u64 gva;
	__u32 pages;
	__u32 level;
} hvgr_mmu_para_flush_pt;

#define HVGR_MMU_GPA_NUM 512
#define HVGR_MMU_TBL_PA_NUM (3 + (HVGR_MMU_GPA_NUM / 512 + 1) + \
	(HVGR_MMU_GPA_NUM / (512 * 512) + 1))

typedef struct {
	__u32 tbl_pa_top;
	__u32 tbl_pa_num;
	__u64 tbl_pa_array[HVGR_MMU_TBL_PA_NUM];
} hvgr_mmu_tbl_stack;

typedef struct {
	__u64 pgd_pa;
	__u64 vpfn;
	__u64 pages;
	__u64 mmu_flag;
	__u64 flags;
	__u32 bitmap;
	hvgr_mmu_tbl_stack tbl_stack;
	__u64 gpa_array[HVGR_MMU_GPA_NUM];
	__u32 ppl_a_cnts;
} hvgr_mmu_tbl_map_s;

typedef struct {
	__u64 pgd_pa;
	__u64 vpfn;
	__u32 pages;
	__u32 bitmap;
	hvgr_mmu_tbl_stack tbl_stack;
	__u32 ppl_r_cnts;
} hvgr_mmu_unmap_paras;

typedef struct {
	__u32 asid;
	__u64 gva;
	__u32 pages;
	__u32 as_bits;
} hvgr_mmu_para_clear_as;

#endif
