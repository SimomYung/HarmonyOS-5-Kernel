/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Author: Huawei Gpu Kmd
 * Create: Tue February 20 09:15:58 2024
 */
#include <malloc.h>
#include <hmasm/page.h>
#include <hmkernel/rq.h>
#include <hmkernel/types.h>
#include <hmkernel/capability.h>
#include <hmkernel/hvgr_mmu_module.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libsysif/utils.h>
#include <libstrict/strict.h>
#include <libsysif/devmgr/api.h>
#include <libhmsync/raw_mutex.h>
#include <libhmucap/ucap.h>
#include <libhmactv/actv.h>
#include <libhmsrv_sys/hm_module.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libsysif/devhost/server.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhwsecurec/securec.h>
#include <libpreempt/preempt.h>
#include <devhost/log.h>

int liblinux_ext_hvgr_mmu_cbit_config(void)
{
	return sysfast_hvgr_mmu_ctrl(__HVGR_MMU_CMD_CBIT_CONFIG,
		NULL, 0, NULL, 0);
}

int liblinux_ext_hvgr_mmu_irq_mask(uint32_t mask)
{
	hvgr_mmu_para_input input;

	input.val = mask;
	return sysfast_hvgr_mmu_ctrl(__HVGR_MMU_CMD_IRQ_MASK,
		&input, sizeof(input), NULL, 0);
}

int liblinux_ext_hvgr_mmu_irq_enable(void)
{
	return sysfast_hvgr_mmu_ctrl(__HVGR_MMU_CMD_IRQ_ENABLE,
		NULL, 0, NULL, 0);
}

int liblinux_ext_hvgr_mmu_irq_disable(void)
{
	return sysfast_hvgr_mmu_ctrl(__HVGR_MMU_CMD_IRQ_DISABLE,
		NULL, 0, NULL, 0);
}

int liblinux_ext_hvgr_mmu_clear_fault(uint32_t no, uint32_t fault_type)
{
	hvgr_mmu_para_as input;

	input.no = no;
	input.fault_type = fault_type;
	return sysfast_hvgr_mmu_ctrl(__HVGR_MMU_CMD_CLEAR_FAULT,
		&input, sizeof(input), NULL, 0);
}

int liblinux_ext_hvgr_mmu_unmask_fault(uint32_t no, uint32_t fault_type)
{
	hvgr_mmu_para_as input;

	input.no = no;
	input.fault_type = fault_type;
	return sysfast_hvgr_mmu_ctrl(__HVGR_MMU_CMD_UNMASK_FAULT,
		&input, sizeof(input), NULL, 0);
}

int liblinux_ext_hvgr_mmu_hal_unlock(uint32_t asid)
{
	hvgr_mmu_para_input input;

	input.val = asid;
	return sysfast_hvgr_mmu_ctrl(__HVGR_MMU_CMD_HAL_UNLOCK,
		&input, sizeof(input), NULL, 0);
}

int liblinux_ext_hvgr_mmu_hal_enable(uint64_t pgd, uint32_t asid)
{
	hvgr_mmu_para_enable input;

	input.asid = asid;
	input.pgd_pa = pgd;
	return sysfast_hvgr_mmu_ctrl(__HVGR_MMU_CMD_HAL_ENABLE,
		&input, sizeof(input), NULL, 0);
}

int liblinux_ext_hvgr_mmu_hal_disable(uint32_t asid)
{
	hvgr_mmu_para_input input;

	input.val = asid;
	return sysfast_hvgr_mmu_ctrl(__HVGR_MMU_CMD_HAL_DISABLE,
		&input, sizeof(input), NULL, 0);
}

int liblinux_ext_hvgr_mmu_hal_flush_pt(uint32_t asid, uint64_t gva,
	uint32_t pages, uint32_t level)
{
	hvgr_mmu_para_flush_pt input;

	input.asid = asid;
	input.gva = gva;
	input.pages = pages;
	input.level = level;
	return sysfast_hvgr_mmu_ctrl(__HVGR_MMU_CMD_HAL_FLUSH_PT,
		&input, sizeof(input), NULL, 0);
}

int liblinux_ext_hvgr_mmu_hal_flush_l2_caches(void)
{
	return sysfast_hvgr_mmu_ctrl(__HVGR_MMU_CMD_HAL_FLUSH_L2_CACHES,
		NULL, 0, NULL, 0);
}

int liblinux_ext_hvgr_mmu_tbl_map(void *para)
{
	hvgr_mmu_tbl_map_s *input = (hvgr_mmu_tbl_map_s *)para;

	return sysfast_hvgr_mmu_ctrl(__HVGR_MMU_CMD_TBL_MAP,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_ext_hvgr_mmu_tbl_unmap(void *para)
{
	hvgr_mmu_unmap_paras *input = (hvgr_mmu_unmap_paras *)para;

	return sysfast_hvgr_mmu_ctrl(__HVGR_MMU_CMD_TBL_UNMAP,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_ext_hvgr_mmu_tbl_free_pgd(void *para)
{
	hvgr_mmu_unmap_paras *input = (hvgr_mmu_unmap_paras *)para;

	return sysfast_hvgr_mmu_ctrl(__HVGR_MMU_CMD_TBL_FREE_PGD,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_ext_hvgr_mmu_fault_clear_as(uint32_t asid, uint64_t gva,
	uint32_t pages, uint32_t as_bits)
{
	hvgr_mmu_para_clear_as input;

	input.asid = asid;
	input.gva = gva;
	input.pages = pages;
	input.as_bits = as_bits;

	return sysfast_hvgr_mmu_ctrl(__HVGR_MMU_CMD_FAULT_CLEAR_AS,
		&input, sizeof(input), NULL, 0);
}