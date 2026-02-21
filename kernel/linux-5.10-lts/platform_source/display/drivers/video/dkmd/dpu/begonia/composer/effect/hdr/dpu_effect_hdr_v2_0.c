/* Copyright (c) 2023-2023, Hisilicon Tech. Co., Ltd. All rights reserved.
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
#include <linux/atomic.h>
#include "effect/dpu_effect_init.h"
#include "dpu_effect_hdr.h"

#define DMCU_LTM_IHIST_BUF_ADDR 0x15EEC
#define dpu_ltm_hist_addr(base, y5)  ((base) + (DACC_OFFSET + DMCU_LTM_IHIST_BUF_ADDR + 0x4 * (y5)))

struct dkmd_hdr_ctrl {
	struct dkmd_hdr_statistic_info data;
	char __iomem *hdr_base;
	struct kthread_work gtm_work;
	struct kthread_work ltm_work;
	struct mutex data_lock;
	atomic_t mean;
	struct dpu_composer *dpu_comp;
};

void dpu_effect_hdr_set_mean(struct dpu_composer *dpu_comp, uint32_t hdr_mean)
{
	struct dkmd_hdr_ctrl *hdr_ctrl = NULL;
	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL");
	
	hdr_ctrl = (struct dkmd_hdr_ctrl *)dpu_comp->hdr_ctrl;
	dpu_check_and_no_retval(!hdr_ctrl, err, "hdr_ctrl is NULL");

	atomic_set(&hdr_ctrl->mean, hdr_mean);
}

static void dpu_effect_hdr_gtm_handle_work(struct kthread_work *work)
{
	uint64_t tv0;
	int i;
	struct dpu_composer *dpu_comp = NULL;
	struct dkmd_hdr_ctrl *hdr_ctrl = NULL;
	char __iomem *hdr_base = NULL;

	hdr_ctrl = container_of(work, struct dkmd_hdr_ctrl, gtm_work);
	dpu_check_and_no_retval(!hdr_ctrl, err, "hdr_ctrl is NULL");

	dpu_comp = hdr_ctrl->dpu_comp;
	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL");

	hdr_base = hdr_ctrl->hdr_base;
	dpu_trace_ts_begin(&tv0);

	down(&dpu_comp->comp.blank_sem);
	dpu_print_sem_count(&dpu_comp->comp.blank_sem, true);
	if (!dpu_comp->comp.power_on) {
		dpu_pr_debug("panel power off");
		dpu_print_sem_count(&dpu_comp->comp.blank_sem, false);
		up(&dpu_comp->comp.blank_sem);
		return;
	}

	dpu_comp_active_vsync(dpu_comp);

	mutex_lock(&hdr_ctrl->data_lock);

	for (i = 0; i < GTM_HIST_SIZE; i++)
		hdr_ctrl->data.gtm_hist[i] = inp32(DPU_HDR_GLOBAL_HIST_BUFF_ADDR(hdr_base, i));

	mutex_unlock(&hdr_ctrl->data_lock);
	dpu_comp_deactive_vsync(dpu_comp);
	dpu_print_sem_count(&dpu_comp->comp.blank_sem, false);
	up(&dpu_comp->comp.blank_sem);

	dpu_trace_ts_end(&tv0, "hdr read statistic finished!");
}

static void dpu_effect_hdr_ltm_handle_work(struct kthread_work *work)
{
	uint64_t tv0;
	int i;
	struct dpu_composer *dpu_comp = NULL;
	struct dkmd_hdr_ctrl *hdr_ctrl = NULL;
	char __iomem *hdr_base = NULL;
	char __iomem *dpu_base = NULL;

	hdr_ctrl = container_of(work, struct dkmd_hdr_ctrl, ltm_work);
	dpu_check_and_no_retval(!hdr_ctrl, err, "hdr_ctrl is NULL");

	dpu_comp = hdr_ctrl->dpu_comp;
	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL");

	hdr_base = hdr_ctrl->hdr_base;
	dpu_base = dpu_comp->comp_mgr->dpu_base;
	dpu_trace_ts_begin(&tv0);

	down(&dpu_comp->comp.blank_sem);
	dpu_print_sem_count(&dpu_comp->comp.blank_sem, true);
	if (!dpu_comp->comp.power_on) {
		dpu_pr_debug("panel power off");
		dpu_print_sem_count(&dpu_comp->comp.blank_sem, false);
		up(&dpu_comp->comp.blank_sem);
		return;
	}

	dpu_comp_active_vsync(dpu_comp);
	mutex_lock(&hdr_ctrl->data_lock);
	for (i = 0; i < LTM_HIST_SIZE; i++)
		hdr_ctrl->data.ltm_hist[i] = inp32(dpu_ltm_hist_addr(dpu_base, i));

	mutex_unlock(&hdr_ctrl->data_lock);
	dpu_comp_deactive_vsync(dpu_comp);
	dpu_print_sem_count(&dpu_comp->comp.blank_sem, false);
	up(&dpu_comp->comp.blank_sem);

	dpu_trace_ts_end(&tv0, "hdr read ltm statistic finished!");
}

void dpu_effect_hdr_init(struct dpu_composer *dpu_comp, uint32_t hdr_offset)
{
	struct dkmd_hdr_ctrl *hdr_ctrl = NULL;
	char __iomem *dpu_base = NULL;
	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL");

	dpu_comp->hdr_ctrl = kzalloc(sizeof(struct dkmd_hdr_ctrl), GFP_KERNEL);
	dpu_check_and_no_retval(!dpu_comp->hdr_ctrl, err, "hdr_ctrl is NULL");

	hdr_ctrl = dpu_comp->hdr_ctrl;
	dpu_base = dpu_comp->comp_mgr->dpu_base;
	hdr_ctrl->hdr_base = dpu_base + hdr_offset;
	hdr_ctrl->dpu_comp = dpu_comp;

	kthread_init_work(&hdr_ctrl->ltm_work, dpu_effect_hdr_ltm_handle_work);
	kthread_init_work(&hdr_ctrl->gtm_work, dpu_effect_hdr_gtm_handle_work);
	mutex_init(&hdr_ctrl->data_lock);
	atomic_set(&hdr_ctrl->mean, 0);
}

void dpu_effect_hdr_deinit(struct dpu_composer *dpu_comp)
{
	struct dkmd_hdr_ctrl *hdr_ctrl = NULL;
	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL");
	hdr_ctrl = (struct dkmd_hdr_ctrl *)dpu_comp->hdr_ctrl;
	dpu_check_and_no_retval(!hdr_ctrl, err, "hdr_ctrl is NULL");

	mutex_destroy(&hdr_ctrl->data_lock);
	kfree(dpu_comp->hdr_ctrl);
	dpu_comp->hdr_ctrl = NULL;
}

void dpu_effect_hdr_gtm_queue_work(struct dpu_composer *dpu_comp)
{
	struct dkmd_hdr_ctrl *hdr_ctrl = NULL;
	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL");

	hdr_ctrl = (struct dkmd_hdr_ctrl *)dpu_comp->hdr_ctrl;
	dpu_check_and_no_retval(!hdr_ctrl, err, "hdr_ctrl is NULL");

	kthread_queue_work(&dpu_comp->effect_worker, &hdr_ctrl->gtm_work);
}

void dpu_effect_hdr_ltm_queue_work(struct dpu_composer *dpu_comp)
{
	struct dkmd_hdr_ctrl *hdr_ctrl = NULL;
	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL");

	hdr_ctrl = (struct dkmd_hdr_ctrl *)dpu_comp->hdr_ctrl;
	dpu_check_and_no_retval(!hdr_ctrl, err, "hdr_ctrl is NULL");

	kthread_queue_work(&dpu_comp->effect_worker, &hdr_ctrl->ltm_work);
}

int dpu_effect_hdr_get_mean(struct composer *comp, uint32_t *hdr_mean)
{
	void_unused(comp);
	void_unused(hdr_mean);
	return 0;
}

int dpu_effect_hdr_get_statistic(struct composer *comp, void __user* argp)
{
	int ret = 0;
	struct dpu_composer *dpu_comp = NULL;
	struct dkmd_hdr_ctrl *hdr_ctrl = NULL;
	dpu_check_and_return(!comp, -1, err, "comp is null pointer");
	dpu_check_and_return(!argp, -1, err, "argp is NULL");

	dpu_comp = to_dpu_composer(comp);
	dpu_check_and_return(!dpu_comp, -1, err, "dpu_comp is null pointer");
	hdr_ctrl = (struct dkmd_hdr_ctrl *)dpu_comp->hdr_ctrl;
	dpu_check_and_return(!hdr_ctrl, -1, err, "hdr_ctrl is null pointer");

	mutex_lock(&hdr_ctrl->data_lock);
	hdr_ctrl->data.hdr_mean = atomic_read(&hdr_ctrl->mean);

	if (copy_to_user(argp, &hdr_ctrl->data, sizeof(struct dkmd_hdr_statistic_info)) != 0) {
		dpu_pr_err("[hdr] copy_to_user failed(param)! ret=%d.", ret);
		mutex_unlock(&hdr_ctrl->data_lock);
		return -1;
	}

	mutex_unlock(&hdr_ctrl->data_lock);
	return 0;
}