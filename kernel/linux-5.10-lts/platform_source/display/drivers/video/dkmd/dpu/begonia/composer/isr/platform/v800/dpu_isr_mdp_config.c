/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
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
#include <dpu/soc_dpu_define.h>
#include "ukmd_listener.h"
#include "dpu_comp_mgr.h"
#include "dpu_isr.h"
#include "dpu_isr_mdp.h"

void dpu_mdp_dacc_async_tx_isr_handler(struct composer_manager *comp_mgr, struct ukmd_isr *isr_ctrl)
{
	struct dpu_composer *dpu_comp = NULL;
	uint32_t comp_idx = 0;
	uint32_t async_tx_done = read_reg_with_retry(DPU_ASYNC_TX_DONE_ADDR(comp_mgr->dpu_base));
	if (async_tx_done == 0)
		return;

	if (async_tx_done == DSS_ERR_CODE_DEADBEED) {
		dpu_pr_warn("deadbeed async_tx_done %u", async_tx_done);
		return;
	}

	outp32(DPU_ASYNC_TX_DONE_ADDR(comp_mgr->dpu_base), 0);

	comp_idx = read_reg_with_retry(DPU_ASYNC_TX_REL_SCENE_ADDR(comp_mgr->dpu_base));
	if (comp_idx > DEVICE_COMP_BUILTIN_ID) {
		dpu_pr_warn("invalid comp_idx %u", comp_idx);
		return;
	}

	dpu_pr_debug("comp_idx=%u", comp_idx);
	dpu_comp = comp_mgr->dpu_comps[comp_idx];
	if (!dpu_comp) {
		dpu_pr_err("dpu_comp is null");
		return;
	}

	pipeline_next_ops_handle(dpu_comp->conn_info->conn_device, dpu_comp->conn_info, CMDS_ASYNC_TX_DONE, NULL);
}

static void dpu_update_dfr_base_timestamp(char __iomem *dpu_base, uint64_t current_time, uint32_t isr2_state)
{
	bool is_mismatch = false;
	uint32_t base_timestamp_low;
	uint32_t base_timestamp_high;
 
	base_timestamp_high = (current_time >> 32) & (0xFFFFFFFF);
	base_timestamp_low = current_time & (0xFFFFFFFF);
	outp32(DFR_BASE_TIMESTAMP_LOW32(dpu_base), base_timestamp_low);
	outp32(DFR_BASE_TIMESTAMP_HIGH32(dpu_base), base_timestamp_high);
	if (isr2_state & MISMATCH_TE)
		is_mismatch = true;
	dpu_pr_debug("frmId=%u misFLag=%u, update base timestamp=%llu, low_32=%u ,vsync_high=%u ",
		((inp32(DFR_MISMATCHED_FRAME_INFO_DISPLAYED(dpu_base))& 0xFFFFFFFC) >> 2) , is_mismatch,
		current_time, base_timestamp_low, base_timestamp_high);
 
	/* bit0-bit1: timestamp type(0: disable  1:absolute time 2:relative Time);
	bit2-bit31: displayed frm idx corresponding expected timestamp */
	outp32(DFR_MISMATCHED_FRAME_INFO_DISPLAYED(dpu_base), 0);
}

static bool has_vsync_registered(struct composer_manager *comp_mgr)
{
	if (unlikely(!comp_mgr)) {
		dpu_pr_warn("not exist comp_mgr");
		return false;
	}

	if (comp_mgr->dpu_comps[DEVICE_COMP_BUILTIN_ID] == NULL ||
		!composer_manager_get_scene_switch(comp_mgr->dpu_comps[DEVICE_COMP_BUILTIN_ID]))
		return true;

	/* Check whether the vsync listener has been registered. */
	if (comp_mgr->dpu_comps[DEVICE_COMP_BUILTIN_ID]->comp_scene_id == DPU_SCENE_ONLINE_1)
		return comp_mgr->dpu_comps[DEVICE_COMP_PRIMARY_ID]->has_dfr_related_listener_registered;
	else
		return comp_mgr->dpu_comps[DEVICE_COMP_PRIMARY_ID]->has_dfr_related_listener_registered |
			comp_mgr->dpu_comps[DEVICE_COMP_BUILTIN_ID]->has_dfr_related_listener_registered;
}

void dpu_mdp_dacc_isr_handler(char __iomem *dpu_base, struct ukmd_isr *isr_ctrl)
{
	uint64_t current_time;
	struct composer_manager *comp_mgr = NULL;
	uint8_t power_status = 0;
	uint32_t isr2_state = inp32(DPU_GLB_DACC_NS_INT_O_ADDR(dpu_base + DPU_GLB0_OFFSET));
	outp32(DPU_GLB_DACC_NS_INT_O_ADDR(dpu_base + DPU_GLB0_OFFSET), isr2_state);
	dpu_check_and_no_retval(!isr_ctrl, err, "isr ctrl is null");

	current_time = (uint64_t)ktime_to_ns(ktime_get());
	dpu_pr_debug("isr2 intr:%#x, cur_time = %llu ns", isr2_state, current_time);
	/**
     * isr intr:
     * 0x2: valid vsync, need to report vsync and count fps
     * 0x8: count vysnc, need to report vsync
     * 0x10: self refresh, need to count fps
     * 0x20: only dimming done, need to update dimming done flag
     * 0x18: vsync and self refresh, need to report vsync and count fps
     * 0x28: vsync and dimming done, need to report vsync and update dimming done flag
     * 0x38: vsync and self refresh, dimming done, need to report vsync and update dimming done flag
     **/
	if (((isr2_state & (VALID_VSYNC | COUNT_VSYNC)) != 0) && has_vsync_registered(isr_ctrl->parent)) {
		dpu_pr_debug("report vsync, cur_time = %lld ms", ktime_to_ms(ktime_get()));
		ukmd_isr_notify_listener(isr_ctrl, NOTIFY_BOTH_VSYNC_TIMELINE);
	}
	if (((isr2_state & (VALID_VSYNC | SELF_REFRESH)) != 0) && has_vsync_registered(isr_ctrl->parent)) {
		dpu_pr_debug("self refresh , cur_time = %lld ms", ktime_to_ms(ktime_get()));
		/* fps count only for LTPO screen */
		ukmd_isr_notify_listener(isr_ctrl, NOTIFY_REFRESH);
		dpu_update_dfr_base_timestamp(dpu_base, current_time, isr2_state);
	}
	if (isr2_state & (DIMMING_DONE)) {
		dpu_pr_info("frm rate dimming done, cur_time = %llu ms", ktime_to_ms(ktime_get()));
		comp_mgr = isr_ctrl->parent;
		if (likely(comp_mgr))
			power_status = comp_mgr->power_status.refcount.value[DEVICE_COMP_PRIMARY_ID]
				| comp_mgr->power_status.refcount.value[DEVICE_COMP_BUILTIN_ID];
		if (unlikely(power_status == 0)) {
			dpu_pr_info("already power off");
			return;
		}
		ukmd_isr_notify_listener(isr_ctrl, NOTIFY_DIMMING_DONE);
	}
	if (isr2_state & LTM_IHIST_DONE) {
		dpu_pr_debug("riscv have already finished read ihist");
		ukmd_isr_notify_listener(isr_ctrl, LTM_IHIST_DONE);
	}
}

void dkmd_mdp_isr_enable(struct composer_manager *comp_mgr)
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
		dpu_pr_err("isr_ctrl->handle_func is null ptr");
		return;
	}

	/* 1. interrupt mask */
	outp32(DPU_GLB_NS_MDP_TO_GIC_MSK_ADDR(dpu_base + DPU_GLB0_OFFSET), mask);
	outp32(DPU_GLB_WCH0_NS_INT_MSK_ADDR(dpu_base + DPU_GLB0_OFFSET), mask);

	/* 2. enable irq */
	isr_ctrl->handle_func(isr_ctrl, UKMD_ISR_ENABLE);

	/* 3. interrupt clear */
	outp32(DPU_GLB_NS_MDP_TO_GIC_O_ADDR(dpu_base + DPU_GLB0_OFFSET), mask);

	/* 4. interrupt umask */
	dpu_pr_info("unmask 0x%x", isr_ctrl->unmask);
	outp32(DPU_GLB_NS_MDP_TO_GIC_MSK_ADDR(dpu_base + DPU_GLB0_OFFSET), isr_ctrl->unmask);
	set_reg(DPU_GLB_DACC_NS_INT_MSK_ADDR(dpu_base + DPU_GLB0_OFFSET), 0x0, DACC_SECONDARY_ISR_TE, 0);
	set_reg(DPU_GLB_DACC_NS_INT_MSK_ADDR(dpu_base + DPU_GLB0_OFFSET), 0x0, 1, DPU_DACC_SECONDARY_ISR_LTM_IHIST);
	outp32(DPU_GLB_WCH0_NS_INT_MSK_ADDR(dpu_base + DPU_GLB0_OFFSET), ~(WCH_FRM_END_INTS));

	dpu_pr_info("-");
}
