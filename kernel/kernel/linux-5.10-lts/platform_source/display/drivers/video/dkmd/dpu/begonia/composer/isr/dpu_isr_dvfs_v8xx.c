/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include "dpu/dpu_base_addr.h"
#include "dpu_config_utils.h"
#include "dpu_isr_dvfs.h"
#include "dkmd_log.h"
#include "dkmd_object.h"
#include "dpu_comp_mgr.h"
#include "dpu_isr.h"

#ifdef CONFIG_DKMD_DEBUG_ENABLE
irqreturn_t dpu_dvfs_isr(int32_t irq, void *ptr)
{
	uint32_t debug_cfg;
	char __iomem *pmctrl_base = NULL;

	if (g_debug_dvfs_isr_enable == 0)
		return IRQ_HANDLED;

	pmctrl_base = dpu_config_get_ip_base(DISP_IP_BASE_PMCTRL);
	dpu_check_and_return(!pmctrl_base, IRQ_HANDLED, err, "pmctrl_base is NULL!");

	dpu_print_timestamp("dvfs finish");

	dpu_print_dvfs_vote_status();

	debug_cfg = inp32(SOC_PMCTRL_APB_DEBUG_CFG_ADDR(pmctrl_base));

	debug_cfg |= BIT(11);

	/* must clear dvfs isr at last */
	outp32(SOC_PMCTRL_APB_DEBUG_CFG_ADDR(pmctrl_base), debug_cfg);

	return IRQ_HANDLED;
}

void dkmd_dvfs_isr_enable(struct composer_manager *comp_mgr)
{
	uint32_t debug_cfg;
	struct ukmd_isr *isr_ctrl = &comp_mgr->dvfs_isr_ctrl;
	char __iomem *pmctrl_base = NULL;
	SOC_PMCTRL_DSS_DVFS_INTR_CFG_UNION sctrl_cfg;

	if (g_debug_dvfs_isr_enable == 0)
		return;

	dpu_pr_info("+\n");

	if (unlikely(!isr_ctrl)) {
		dpu_pr_err("isr_ctrl is null ptr\n");
		return;
	}

	if (unlikely(!isr_ctrl->handle_func)) {
		dpu_pr_err("isr_ctrl->handle_func is null ptr\n");
		return;
	}

	pmctrl_base = dpu_config_get_ip_base(DISP_IP_BASE_PMCTRL);
	dpu_check_and_no_retval(!pmctrl_base, err, "pmctrl_base is NULL!");

	/* 1. interrupt mask */
	sctrl_cfg.value = 0;
	sctrl_cfg.reg.apb_intr_sf_dvfs_bypass = 0x1;
	sctrl_cfg.reg.apb_intr_hw_dvfs_bypass = 0x1;
	sctrl_cfg.reg.apb_inter_instr_bypass = 0x1;
	outp32(SOC_PMCTRL_DSS_DVFS_INTR_CFG_ADDR(pmctrl_base), sctrl_cfg.value);
	set_reg(SOC_PMCTRL_PERI_M1_INTR_M3ORCPU_MSK_ADDR(pmctrl_base), 0xFFFF, 16, 16);
	set_reg(SOC_PMCTRL_PERI_M2_INTR_M3ORCPU_MSK_ADDR(pmctrl_base), 0xFFFF, 16, 16);
	set_reg(SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_ADDR(pmctrl_base), 0xFFFF, 16, 16);
	set_reg(SOC_PMCTRL_BUCK5_INTR_M3ORCPU_MSK_ADDR(pmctrl_base), 0xFF, 8, 0);
	set_reg(SOC_PMCTRL_VDDC_MEM_INTR_M3ORCPU_MSK_ADDR(pmctrl_base), 0xFFFF, 16, 16);

	/* 2. enable irq */
	isr_ctrl->handle_func(isr_ctrl, UKMD_ISR_ENABLE);

	/* 3. interrupt clear */
	debug_cfg = inp32(SOC_PMCTRL_APB_DEBUG_CFG_ADDR(pmctrl_base));
	debug_cfg |= BIT(11);
	outp32(SOC_PMCTRL_APB_DEBUG_CFG_ADDR(pmctrl_base), debug_cfg);

	/* 4. interrupt umask */
	sctrl_cfg.value = 0;
	sctrl_cfg.reg.apb_intr_sf_dvfs_bypass = 0x1;
	sctrl_cfg.reg.apb_intr_hw_dvfs_bypass = 0x1;
	sctrl_cfg.reg.apb_inter_instr_bypass = 0x0;
	outp32(SOC_PMCTRL_DSS_DVFS_INTR_CFG_ADDR(pmctrl_base), sctrl_cfg.value);

	dpu_pr_info("-\n");
}

void dkmd_dvfs_isr_disable(struct composer_manager *comp_mgr)
{
	struct ukmd_isr *isr_ctrl = &comp_mgr->dvfs_isr_ctrl;
	char __iomem *pmctrl_base = NULL;
	SOC_PMCTRL_DSS_DVFS_INTR_CFG_UNION sctrl_cfg;

	if (g_debug_dvfs_isr_enable == 0)
		return;

	dpu_pr_info("+\n");

	if (unlikely(!isr_ctrl)) {
		dpu_pr_err("isr_ctrl is null ptr\n");
		return;
	}

	if (unlikely(!isr_ctrl->handle_func)) {
		dpu_pr_err("isr_ctrl->handle_func is null ptr\n");
		return;
	}

	pmctrl_base = dpu_config_get_ip_base(DISP_IP_BASE_PMCTRL);
	dpu_check_and_no_retval(!pmctrl_base, err, "pmctrl_base is NULL!");

	/* 1. interrupt bypass */
	sctrl_cfg.value = 0;
	sctrl_cfg.reg.apb_intr_sf_dvfs_bypass = 0x1;
	sctrl_cfg.reg.apb_intr_hw_dvfs_bypass = 0x1;
	sctrl_cfg.reg.apb_inter_instr_bypass = 0x1;
	outp32(SOC_PMCTRL_DSS_DVFS_INTR_CFG_ADDR(pmctrl_base), sctrl_cfg.value);

	/* 2. disable irq */
	isr_ctrl->handle_func(isr_ctrl, UKMD_ISR_DISABLE);

	dpu_pr_info("-\n");
}
#else
irqreturn_t dpu_dvfs_isr(int32_t irq, void *ptr)
{
	void_unused(irq);
	void_unused(ptr);
	return IRQ_HANDLED;
}

void dkmd_dvfs_isr_enable(struct composer_manager *comp_mgr)
{
	void_unused(comp_mgr);
	return;
}

void dkmd_dvfs_isr_disable(struct composer_manager *comp_mgr)
{
	void_unused(comp_mgr);
	return;
}
#endif