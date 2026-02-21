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

#include <linux/kthread.h>
#include <uapi/linux/sched/types.h>
#include "dpu_effect_init.h"

#define UEVENT_BUF_LEN 120

static void dpu_mdp_wbuf_uevent(struct kthread_work *work)
{
	struct dpu_composer *dpu_comp = NULL;
	char *envp[2] = {"WBufErr=1", NULL};
	struct composer *comp = NULL;

	dpu_comp = container_of(work, struct dpu_composer, wbuf_work);
	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL");

	comp = &dpu_comp->comp;
	kobject_uevent_env(&(comp->base.peri_device->dev.kobj), KOBJ_CHANGE, envp);
}

static void dpu_mdp_ddic_uevent(struct kthread_work *work)
{
	struct dpu_composer *dpu_comp = NULL;

	dpu_comp = container_of(work, struct dpu_composer, ddic_work);
	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL");

	notify_de_event(dpu_comp->comp.base.id, DDIC_IRC_FORBID_PARTIALUPDATE);
}

void dpu_effect_thread_setup(struct dpu_composer *dpu_comp)
{
	struct sched_param param = {
		.sched_priority = MAX_RT_PRIO - 1,
	};
	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL");

	kthread_init_worker(&dpu_comp->effect_worker);
	kthread_init_work(&dpu_comp->wbuf_work, dpu_mdp_wbuf_uevent);
	kthread_init_work(&dpu_comp->ddic_work, dpu_mdp_ddic_uevent);
	dpu_comp->effect_thread = kthread_create(kthread_worker_fn, &dpu_comp->effect_worker, "effect thread");
	if (IS_ERR_OR_NULL(dpu_comp->effect_thread)) {
		dpu_pr_err("failed to create effect_thread!");
		return;
	}
	(void)sched_setscheduler_nocheck(dpu_comp->effect_thread, SCHED_FIFO, &param);
	(void)wake_up_process(dpu_comp->effect_thread);
}

void dpu_effect_thread_relase(struct dpu_composer *dpu_comp)
{
	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL");
	if (IS_ERR_OR_NULL(dpu_comp->effect_thread))
		return;

	kthread_stop(dpu_comp->effect_thread);
	dpu_comp->effect_thread = NULL;
}

bool is_vstate_in_vfp(char __iomem *dpu_base)
{
	uint32_t vstate = inp32(DPU_DSI_VSTATE_ADDR(dpu_base));
	if (vstate & (LDI_VSTATE_VSW | LDI_VSTATE_VBP | LDI_VSTATE_VACTIVE0 | LDI_VSTATE_V_WAIT_TE0))
		return false;

	return true;
}

int32_t notify_de_event(uint32_t panel_id, uint32_t event)
{
	int32_t ret = -1;
	int32_t index = 0;
	char *envp[2] = {NULL};
	char event_buf[UEVENT_BUF_LEN] = {0};
	struct composer *comp = NULL;
	struct composer_manager *comp_manager = get_dpu_composer_manager();

	if (unlikely(comp_manager == NULL)) {
		dpu_pr_err("composer manager is null");
		return -EINVAL;
	}

	if (unlikely(panel_id >= PANEL_ID_MAX_NUM)) {
		dpu_pr_err("panel_id=%u is out of range", panel_id);
		return -EINVAL;
	}

	for (; index < DEVICE_COMP_MAX_COUNT; index++) {
		if (comp_manager->dpu_comps[index] == NULL)
			continue;

		if (comp_manager->dpu_comps[index]->comp.base.id == panel_id) {
			comp = &comp_manager->dpu_comps[index]->comp;
			break;
		}
	}

	if (unlikely(comp == NULL)) {
		dpu_pr_err("composer is null");
		return -EINVAL;
	}

	ret = snprintf_s(event_buf, UEVENT_BUF_LEN, UEVENT_BUF_LEN - 1,
	    "DE_EVENT=%u, COMP_IDX=%u\n", event, comp->index);
	if (ret < 0) {
		dpu_pr_err("format string failed, truncation occurs");
		return -EINVAL;
	}

	envp[0] = event_buf;
	kobject_uevent_env(&(comp->base.peri_device->dev.kobj), KOBJ_CHANGE, envp);

	dpu_pr_info("upload DE event=%u", event);
	return 0;
}