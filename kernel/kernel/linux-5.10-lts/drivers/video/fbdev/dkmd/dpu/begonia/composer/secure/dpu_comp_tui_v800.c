
/* Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "dpu_comp_mgr.h"
#include "tui.h"
#include "dpu_comp_tui.h"
#include "dkmd_tui_interface.h"
#include <linux/workqueue.h>
#include <linux/of_irq.h>
#include "dpu_config_utils.h"
#include "dpu/dpu_base_addr.h"

struct workqueue_struct *g_dpu_tui_wq;
struct work_struct g_dpu_tui_isr_work;

#define DPU_TUI_EVENT_ADDR(base)         ((base)  + DACC_OFFSET + (0x15FF0))
#define DPU_LOW_POWER_STAE_ADDR(base)    ((base)  + DACC_OFFSET + (0x15FF4))
#define DPU_TUI_LEVEL1_XY_ADDR(base)     ((base)  + DACC_OFFSET + (0x15FF8))
#define DPU_TUI_LEVEL1_WH_ADDR(base)     ((base)  + DACC_OFFSET + (0x15FFC))

__attribute__((unused)) uint32_t get_tui_tee_event_type(void)
{
    uint32_t type;
    char __iomem *dpu_base = dpu_config_get_ip_base(DISP_IP_BASE_DPU);

    type = inp32(DPU_TUI_EVENT_ADDR(dpu_base));
	dpu_pr_debug("get_tui_tee_event_type = %u", type);
    return type;
}

void dpu_tui_set_lowpower_status(uint32_t user_state)
{
    uint32_t map_lp_state[USER_STATE_NUM] = {
        ENTER_PARTIAL_UPDATE, // USER_STATE_EXIT_ALL
        EXIT_PARTIAL_UPDATE,  // USER_STATE_LEVEL0_RUN
        EXIT_PARTIAL_UPDATE,  // USER_STATE_LEVEL1_IN_DISP
        ENTER_PARTIAL_UPDATE, // USER_STATE_LEVEL1_OUT_DISP
    };
    uint32_t lowpower_status;
    char __iomem *dpu_base = dpu_config_get_ip_base(DISP_IP_BASE_DPU);

	if (user_state >= USER_STATE_NUM) {
        dpu_pr_warn("invalid tui lowpower status %u", user_state);
		return;
	}

    lowpower_status = inp32(DPU_LOW_POWER_STAE_ADDR(dpu_base));
    if (lowpower_status != map_lp_state[user_state]) {
        outp32(dpu_base + DACC_OFFSET + 0x15FF4, map_lp_state[user_state]);
		dpu_pr_info("cur_lowpower_status = %d, user_state = %d", lowpower_status, user_state);
        dpu_pr_info("tui set lowpower status %u", map_lp_state[user_state]);
    }
}

int32_t dpu_get_tui_level1_layer_info(struct composer *comp, struct tui_level1_layer_info *info)
{
    struct dpu_composer *dpu_comp = NULL;
	char __iomem *dpu_base;

	if (!info || (!comp->power_on)) {
		dpu_pr_err("param invalid");
		return -EINVAL;
	}

	dpu_comp = to_dpu_composer(comp);
	if (!dpu_comp->comp_mgr) {
		dpu_pr_err("comp_mgr invalid");
		return -EINVAL;
	}

	dpu_base = dpu_comp->comp_mgr->dpu_base;
	if (!dpu_base) {
		dpu_pr_info("dpu_base is nullptr");
		return -EINVAL;
	}

    info->tui_level1_display_rect.x = inp32(DPU_TUI_LEVEL1_XY_ADDR(dpu_base)) & 0xFFFF;
    info->tui_level1_display_rect.y = (inp32(DPU_TUI_LEVEL1_XY_ADDR(dpu_base)) >> 16) & 0xFFFF;
    info->tui_level1_display_rect.w = inp32(DPU_TUI_LEVEL1_WH_ADDR(dpu_base)) & 0xFFFF;
    info->tui_level1_display_rect.h = (inp32(DPU_TUI_LEVEL1_WH_ADDR(dpu_base)) >> 16) & 0xFFFF;

	dpu_pr_info("[TUI]level1_layer_rect x, y, w, h=[%d %d %u %u]",
		info->tui_level1_display_rect.x, info->tui_level1_display_rect.y, info->tui_level1_display_rect.w, info->tui_level1_display_rect.h);

    return 0;
}

static void dpu_tui_notify_work_handler(struct work_struct *work)
{
	struct dpu_secure *secure_ctrl = NULL;
	struct dpu_composer *dpu_comp = NULL;
	uint32_t type;

	dpu_comp = get_active_dpu_comp();
	if (unlikely(!dpu_comp)) {
		dpu_pr_err("dpu_comp is NULL");
		return;
	}

	secure_ctrl = &dpu_comp->secure_ctrl;
 
	dpu_tui_switch_event(&dpu_comp->comp, USER_EVENT_REFRESH);
	dpu_pr_info("tui_mode[%u]: secure_status = %d, secure_event = %d, tui_request = %d ---", secure_ctrl->tui_mode,
	    secure_ctrl->secure_status, secure_ctrl->secure_event, USER_EVENT_REFRESH);
	return;
}
 
static int32_t dpu_tui_workqueue_register(void)
{
	g_dpu_tui_wq = create_singlethread_workqueue("dpu_tui_wq");

	dpu_check_and_return(!g_dpu_tui_wq, -EINVAL, err, "g_dpu_tui_wq is NULL");
	INIT_WORK(&g_dpu_tui_isr_work, dpu_tui_notify_work_handler);
	return 0;
}
 
static irqreturn_t dpu_tui_schedule_wq(int32_t irq, void *dev_id)
{
	dpu_pr_info("dpu_secure_notify irq %d", irq);
	queue_work(g_dpu_tui_wq, &g_dpu_tui_isr_work);
	return IRQ_HANDLED;
}
 
 
int32_t dpu_tui_irq_register(struct platform_device *pdev)
{
	int32_t ret;
	uint32_t irq_num;
 
	if (unlikely(pdev == NULL)) {
		dpu_pr_err("dpu_tui_irq_register pdev error");
		return -1;
	}
	
	irq_num = irq_of_parse_and_map(pdev->dev.of_node, 3);
    if (unlikely(irq_num == 0)) {
        dpu_pr_err("dpu_tui_irq_register irq num %d", irq_num);
        return -1;
    }
 
	ret = devm_request_irq(&(pdev->dev), irq_num, dpu_tui_schedule_wq, IRQF_NO_SUSPEND, pdev->name, NULL);
	if (unlikely(ret != 0)) {
		dpu_pr_err("devm_request_irq error %d", irq_num);
		return -1;
	}
 
	return dpu_tui_workqueue_register();
}

void dpu_tui_set_power_mode(uint32_t power_type)
{
	char __iomem *sctrl_base = dpu_config_get_ip_base(DISP_IP_BASE_SCTRL);
	uint32_t val_get, val_set;

	if (unlikely(sctrl_base == NULL)) {
		dpu_pr_err("sctrl_base is NULL");
		return;
	}

	/* use reg to record dpu power mode, then teeos can get unsecure power status
	 * sctrl 0x438, bit[24]:dpu power status
	 */
	val_get = inp32(SOC_SCTRL_SCBAKDATA11_ADDR(sctrl_base));
	val_set = (power_type == DPU_POWER_ON) ? 1 : 0; // dss power on is 1; off is 0
	set_reg(SOC_SCTRL_SCBAKDATA11_ADDR(sctrl_base), val_set, 1, 24);
	dpu_pr_debug("dpu power status val_get = %u, val_set = %u", val_get, inp32(SOC_SCTRL_SCBAKDATA11_ADDR(sctrl_base)));
}

void dpu_tui_frame_start_isr_handler(struct dpu_composer *dpu_comp)
{
    void_unused(dpu_comp);
	return;
}

bool dpu_tui_no_need_disable_ldi(struct dpu_composer *dpu_comp)
{
	void_unused(dpu_comp);
	return true;
}

void dpu_tui_request_frame_update(struct dpu_composer *dpu_comp)
{
	void_unused(dpu_comp);	
}