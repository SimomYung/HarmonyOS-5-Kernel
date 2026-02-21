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
#include "ukmd_listener.h"
#include "dpu_comp_mgr.h"
#include "dpu_isr.h"

#define HDR_EN BIT(0)

static int32_t dpu_comp_hdr_handle_isr_notify(struct notifier_block *self, unsigned long action, void *data)
{
	struct ukmd_listener_data *listener_data = (struct ukmd_listener_data *)data;
	struct dpu_composer *dpu_comp = (struct dpu_composer *)(listener_data->data);
	char __iomem *hdr_base = NULL;
	uint32_t hdr_mean;
	uint32_t hdr_ctrl;
	hdr_base  = dpu_comp->comp_mgr->dpu_base + DPU_HDR_OFFSET;
	hdr_ctrl = inp32(DPU_HDR_CTRL_ADDR(hdr_base));
	if ((hdr_ctrl & HDR_EN) != HDR_EN) {
		dpu_pr_debug("hdr is not en no need handle wq");
		return 0;
	}

	if ((action & DSI_INT_VACT0_END) == DSI_INT_VACT0_END) {
		hdr_mean = inp32(DPU_HDR_MEAN_STT_ADDR(hdr_base));
		dpu_effect_hdr_set_mean(dpu_comp, hdr_mean);
	}

	if ((action & DSI_INT_VACT0_START) == DSI_INT_VACT0_START)
		dpu_effect_hdr_gtm_queue_work(dpu_comp);

	if ((action & LTM_IHIST_DONE) == LTM_IHIST_DONE)
		dpu_effect_hdr_ltm_queue_work(dpu_comp);
	
	return 0;
}

static struct notifier_block hdr_handle_isr_notifier = {
	.notifier_call = dpu_comp_hdr_handle_isr_notify,
};

void dpu_comp_hdr_handle_init(struct ukmd_isr *isr_ctrl, struct dpu_composer *dpu_comp)
{
	struct ukmd_isr *mdp_isr_ctrl = &dpu_comp->comp_mgr->mdp_isr_ctrl;
	ukmd_isr_register_listener(isr_ctrl, &hdr_handle_isr_notifier, DSI_INT_VACT0_END, dpu_comp);
	ukmd_isr_register_listener(isr_ctrl, &hdr_handle_isr_notifier, DSI_INT_VACT0_START, dpu_comp);
	ukmd_isr_register_listener(mdp_isr_ctrl, &hdr_handle_isr_notifier, LTM_IHIST_DONE, dpu_comp);
}

void dpu_comp_hdr_handle_deinit(struct ukmd_isr *isr_ctrl, struct dpu_composer *dpu_comp)
{
	struct ukmd_isr *mdp_isr_ctrl = &dpu_comp->comp_mgr->mdp_isr_ctrl;
	ukmd_isr_unregister_listener(isr_ctrl, &hdr_handle_isr_notifier, DSI_INT_VACT0_END);
	ukmd_isr_unregister_listener(isr_ctrl, &hdr_handle_isr_notifier, DSI_INT_VACT0_START);
	ukmd_isr_unregister_listener(mdp_isr_ctrl, &hdr_handle_isr_notifier, LTM_IHIST_DONE);
}

void dkmd_sdp_isr_get_unmask(uint32_t *unmask)
{
	*unmask = ~(DPU_DPP0_HIACE_NS_INT | DPU_SMART_DMA0_NS_INT |
		DPU_SMART_DMA1_NS_INT | DPU_SMART_DMA2_NS_INT);
}
