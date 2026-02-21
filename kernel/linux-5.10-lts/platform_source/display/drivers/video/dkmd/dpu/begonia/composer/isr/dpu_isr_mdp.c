/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
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
#include <linux/ktime.h>
#include <linux/of_irq.h>
#include <linux/kernel.h>
#include "ukmd_listener.h"
#include "dpu_comp_mgr.h"
#include "dpu_isr.h"
#include "dpu_isr_mdp.h"

static void dpu_mdp_wch0_isr_handler(char __iomem *dpu_base, struct ukmd_isr *isr_ctrl)
{
	uint32_t isr2_state = inp32(DPU_GLB_WCH0_NS_INT_O_ADDR(dpu_base + DPU_GLB0_OFFSET));
	outp32(DPU_GLB_WCH0_NS_INT_O_ADDR(dpu_base + DPU_GLB0_OFFSET), isr2_state);

	dpu_pr_debug("isr2 intr:%#x", isr2_state);

	if ((isr2_state & WCH_FRM_END_INTS) == WCH_FRM_END_INTS)
		dpu_pr_debug("online wb done");
}

static void dpu_mdp_wbuf_isr_handler(struct composer_manager *comp_mgr)
{
	struct dpu_composer *dpu_comp = comp_mgr->dpu_comps[DEVICE_COMP_PRIMARY_ID];
	if (!dpu_comp)
		return;

	kthread_queue_work(&dpu_comp->effect_worker, &dpu_comp->wbuf_work);
}

irqreturn_t dpu_mdp_isr(int32_t irq, void *ptr)
{
	uint32_t isr1_state;
	char __iomem *dpu_base = NULL;
	struct ukmd_isr *isr_ctrl = (struct ukmd_isr *)ptr;
	struct composer_manager *comp_mgr = isr_ctrl->parent;

	dpu_check_and_return(!comp_mgr, IRQ_NONE, err, "comp_mgr is null!");
	dpu_base = comp_mgr->dpu_base;

	isr1_state = inp32(DPU_GLB_NS_MDP_TO_GIC_O_ADDR(dpu_base + DPU_GLB0_OFFSET));
	dpu_pr_debug("isr1 intr:%#x", isr1_state);

	if ((isr1_state & DPU_DACC_NS_INT) == DPU_DACC_NS_INT) {
		dpu_mdp_dacc_isr_handler(dpu_base, isr_ctrl);
		dpu_mdp_dacc_async_tx_isr_handler(comp_mgr, isr_ctrl);
	}

	if ((isr1_state & DPU_WCH0_NS_INT) == DPU_WCH0_NS_INT)
		dpu_mdp_wch0_isr_handler(dpu_base, isr_ctrl);

	if ((isr1_state & DPU_WBUF_NS_INT) == DPU_WBUF_NS_INT) {
		dpu_mdp_wbuf_isr_handler(comp_mgr);
		dpu_pr_info("wch0 wbuf err intr:%#x", isr1_state);
	}

	/* must clear mdp 1st level isr at last */
	outp32(DPU_GLB_NS_MDP_TO_GIC_O_ADDR(dpu_base + DPU_GLB0_OFFSET), isr1_state);
	return IRQ_HANDLED;
}

void dkmd_mdp_isr_disable(struct composer_manager *comp_mgr)
{
	uint32_t mask = ~0;
	struct ukmd_isr *isr_ctrl = &comp_mgr->mdp_isr_ctrl;
	char __iomem *dpu_base = comp_mgr->dpu_base;

	dpu_pr_info("+");

	if (unlikely(!isr_ctrl)) {
		dpu_pr_err("isr_ctrl is null ptr");
		return;
	}

	if (unlikely(!isr_ctrl->handle_func)) {
		dpu_pr_warn("isr_ctrl->handle_func is null ptr");
		return;
	}

	if (unlikely(isr_ctrl->irq_no <= 0)) {
		dpu_pr_warn("irq_no[%d] is invalid, maybe no need", isr_ctrl->irq_no);
		return;
	}

	/* 1. interrupt mask */
	outp32(DPU_GLB_NS_MDP_TO_GIC_MSK_ADDR(dpu_base + DPU_GLB0_OFFSET), mask);
	outp32(DPU_GLB_WCH0_NS_INT_MSK_ADDR(dpu_base + DPU_GLB0_OFFSET), mask);

	/* 2. disable irq */
	isr_ctrl->handle_func(isr_ctrl, UKMD_ISR_DISABLE);
	dpu_pr_info("-");
}
