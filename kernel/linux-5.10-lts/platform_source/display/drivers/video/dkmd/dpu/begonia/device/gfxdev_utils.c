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
#include <linux/syscalls.h>
#include <linux/atomic.h>
#include <dpu_format.h>
#include <dpu/soc_dpu_define.h>

#include "dpu_ion_mem.h"
#include "res_mgr.h"
#include "dkmd_log.h"
#include "dkmd_dpu.h"
#include "cmdlist_interface.h"
#include "gfxdev_mgr.h"
#include "gfxdev_utils.h"
#include "dkmd_notify.h"
#include "ukmd_fence_utils.h"
#include "dpu_comp_mgr.h"
#include "secure/dpu_comp_tui.h"
#include <platform_include/basicplatform/linux/dfx_bbox_diaginfo.h>

int32_t dpu_gfxdev_set_safe_frm_rate(struct composer *comp, void __user *argp)
{
	int32_t ret = -EINVAL;
	uint32_t safe_frm_rate = 0;

	if (unlikely(!comp || !argp)) {
		dpu_pr_err("comp or argp is null");
		return ret;
	}

	if (!comp->set_safe_frm_rate) {
		dpu_pr_err("set safe frm rate is not inited");
		return ret;
	}

	if (copy_from_user(&safe_frm_rate, argp, sizeof(safe_frm_rate)) != 0) {
		dpu_pr_err("copy from user safe frm rate info failed");
		return ret;
	}

	ret = comp->set_safe_frm_rate(comp, safe_frm_rate);
	return ret;
}

int32_t dpu_gfxdev_create_fence(struct composer *comp, void __user *argp)
{
	struct disp_present_fence fence = { 0 };
	fence.fence_fd = -1;
	fence.fence_pt = 0;

	if (unlikely((!comp) || (!argp))) {
		dpu_pr_err("comp or argp is null");
		return -1;
	}

	if (unlikely(!comp->power_on)) {
		dpu_pr_info("comp is power down");
		return -1;
	}

	if (comp->create_fence) {
		if (comp->create_fence(comp, &fence) != 0) {
			dpu_pr_info("comp create fence fail");
			return -1;
		}
	}

	if (copy_to_user(argp, &fence, sizeof(fence)) != 0) {
		dpu_pr_err("%s copy fence to user fail, fence_fd=%d", comp->base.name, fence.fence_fd);
		if (fence.fence_fd > 0)
			ksys_close((uint32_t)fence.fence_fd);
		return -1;
	}

	return 0;
}

int32_t dpu_gfxdev_release_fence(struct composer *comp, void __user *argp)
{
	int32_t fence_fd = -1;

	if (argp == NULL || comp == NULL) {
		dpu_pr_err("argp or comp is nullptr");
		return -1;
	}

	if (copy_from_user(&fence_fd, argp, sizeof(fence_fd)) != 0) {
		dpu_pr_err("copy from user fail");
		return -1;
	}

	if (comp->release_fence)
		return comp->release_fence(comp, fence_fd);

	return 0;
}

int32_t dpu_gfxdev_blank(struct composer *comp, int32_t blank_mode)
{
	int32_t ret = 0;
	struct dkmd_event event;
	uint64_t blank_tv;

	if (unlikely(!comp)) {
		dpu_pr_err("comp is null");
		return -1;
	}

	event.data = NULL;
	event.panel_id = (int32_t)comp->base.id;
	event.value = blank_mode;

	if (is_offline_panel(&comp->base)) {
		dpu_pr_warn("offline comp not support blank");
		return 0;
	}

	if ((blank_mode == DISP_BLANK_UNBLANK) &&
		is_dp_panel(&comp->base) && (comp->base.connect_status == DP_DISCONNECTED)) {
		dpu_pr_warn("dp has plugged out before power on finish, direct return");
		return -1;
	}

	dpu_trace_ts_begin(&blank_tv);
	dpu_pr_info("%s enter, power on=%d, blank mode = %d", comp->base.name, comp->power_on, blank_mode);

	switch (blank_mode) {
	case DISP_BLANK_UNBLANK:
		dkmd_notifier_call_chain(DKMD_EARLY_EVENT_BLANK, (void *)&event);
		ret = gfxdev_blank_power_on(comp);
		dkmd_notifier_call_chain(DKMD_EVENT_BLANK, (void *)&event);
		break;
	case DISP_BLANK_FAKE_OFF:
#ifdef CONFIG_DKMD_DPU_OHOS
		// for listeners blank mode is powerdown
		dpu_pr_info("dkmd notifier call chain on fake off case");
		event.value = DISP_BLANK_POWERDOWN;
		dkmd_notifier_call_chain(DKMD_EVENT_BLANK, (void *)&event);
#endif
		ret = gfxdev_blank_fake_off(comp);
		break;
	case DISP_BLANK_VSYNC_SUSPEND:
	case DISP_BLANK_HSYNC_SUSPEND:
	case DISP_BLANK_POWERDOWN:
		dkmd_notifier_call_chain(DKMD_EVENT_BLANK, (void *)&event);
		ret = gfxdev_blank_power_off(comp, COMPOSER_OFF_MODE_BLANK);
		break;
	case DISP_BLANK_PERI_UNBLANK:
		ret = gfxdev_blank_peri_handle(comp, DISP_BLANK_PERI_UNBLANK);
		break;
	case DISP_BLANK_PERI_POWERDOWN:
		ret = gfxdev_blank_peri_handle(comp, DISP_BLANK_PERI_POWERDOWN);
		break;
	default:
		dpu_pr_err("error power mode=%d", blank_mode);
		break;
	}

	dpu_trace_ts_end(&blank_tv, "set power mode");
	return ret;
}

static int32_t frame_valid_check(struct composer *comp, const struct dkmd_object_info *pinfo, struct disp_frame *frame)
{
	const struct dkmd_rect *rect = &frame->disp_rect;

	if (frame->layer_count > DISP_LAYER_MAX_COUNT) {
		dpu_pr_err("layer_count=%u is invalid", frame->layer_count);
		return -1;
	}

	if (frame->async_mode >= ASYNC_MODE_INVALID) {
		dpu_pr_err("async_mode=%u is invalid", frame->async_mode);
		return -1;
	}

	if (frame->effect_params.effect_num > EFFECT_PARAMS_MAX_NUM) {
		dpu_pr_err("effect_num = %u is invalid", frame->effect_params.effect_num);
		return -1;
	}
	if (is_offline_panel(&comp->base))
		return 0;

	if (unlikely(rect->w > pinfo->xres) || unlikely(rect->h > pinfo->yres) || unlikely(rect->x < 0) ||
		unlikely(rect->x >= pinfo->xres) || unlikely(rect->y < 0) || unlikely(rect->y >= pinfo->yres) ||
		unlikely((rect->x + rect->w) > pinfo->xres) || unlikely((rect->y + rect->h) > pinfo->yres)) {
		dpu_pr_err("disp_rect [x y w h]=[%d %d %u %u] is invalid, xres=%u yres=%u",
			rect->x, rect->y, rect->w, rect->h, pinfo->xres, pinfo->yres);
		return -1;
	}
	return 0;
}

static int32_t dpu_gfxdev_pre_present(struct composer *comp, const struct dkmd_object_info *pinfo)
{
	if (!comp->power_on) {
		if (is_offline_panel(&comp->base))
			return comp->on(comp);
		return -1;
	}

	if (is_dp_panel(&comp->base) || is_hdmi_panel(&comp->base)) {
		mutex_lock(pinfo->pluggable_connect_mutex);
		if (pinfo->connect_status != DP_CONNECTED) {
			mutex_unlock(pinfo->pluggable_connect_mutex);
			dpu_pr_info("dptx or hdmitx is not connected, connected status: %d", pinfo->connect_status);
			return -EINVAL;
		}
	}

	return 0;
}

static inline void dpu_gfxdev_post_present(struct composer *comp, const struct dkmd_object_info *pinfo)
{
	/* comp_frame_index may differ from frame.frame_index when UT invoke to kernel */
	comp->comp_frame_index++;

	if (is_dp_panel(&comp->base) || is_hdmi_panel(&comp->base))
		mutex_unlock(pinfo->pluggable_connect_mutex);
}

int32_t dpu_gfxdev_present(struct composer *comp, const struct dkmd_object_info *pinfo,
	const void __user* argp)
{
	int32_t ret = -1;
	struct disp_frame frame;

	if (unlikely(!comp || !pinfo || !argp)) {
		dpu_pr_err("comp or pinfo or argp is null");
		return -1;
	}

	if (copy_from_user(&frame, argp, sizeof(frame)) != 0) {
		dpu_pr_err("copy from user frame info fail");
		return -EINVAL;
	}

	if (frame_valid_check(comp, pinfo, &frame) != 0) {
		dpu_pr_err("frame valid check fail");
		ukmd_cmdlist_release_locked(dpu_res_get_cmdlist_dev_id(),
			(uint32_t)frame.scene_id, frame.cmdlist_id);
		return -1;
	}

	/* if there is no poweron, need to call poweron here, and then call poweroff at task end */
	down(&comp->blank_sem);
	dpu_print_sem_count(&comp->blank_sem, true);
	if (dpu_gfxdev_pre_present(comp, pinfo) != 0) {
		dpu_pr_warn("composer %s is power down or disconnected", comp->base.name);
		ukmd_cmdlist_release_locked(dpu_res_get_cmdlist_dev_id(),
			(uint32_t)frame.scene_id, frame.cmdlist_id);
		dpu_print_sem_count(&comp->blank_sem, false);
		up(&comp->blank_sem);
		return -1;
	}

	dpu_pr_debug("%s present info: scene_id=%d cmdlist_id=%u layer_count=%u frame_index=%u",
		comp->base.name, frame.scene_id, frame.cmdlist_id, frame.layer_count, frame.frame_index);

	ret = comp->present(comp, (void *)&frame);
	if (unlikely(ret != 0)) {
		dpu_pr_err("%s present fail: scene_id=%d cmdlist_id=%u layer_count=%u frame_index=%u",
			comp->base.name, frame.scene_id, frame.cmdlist_id, frame.layer_count, frame.frame_index);
		ukmd_cmdlist_release_locked(dpu_res_get_cmdlist_dev_id(),
			(uint32_t)frame.scene_id, frame.cmdlist_id);
	}

	dpu_gfxdev_post_present(comp, pinfo);
	dpu_print_sem_count(&comp->blank_sem, false);
	up(&comp->blank_sem);
	return ret;
}

int32_t dpu_gfxdev_dmd_report(struct composer *comp, void __user *argp)
{
	int32_t ret = -1;
	uint32_t i = 0;
	struct disp_dmd_list list;
 
	dpu_check_and_return(unlikely(!argp), -EINVAL, err, "argp is null pointer");
	if (unlikely(!comp->power_on)) {
		dpu_pr_info("comp is power down");
		return ret;
	}
 
	ret = copy_from_user(&list, argp, sizeof(list));
	if (unlikely(ret != 0)) {
		dpu_pr_err("copy_from_user failed ret=%d\n", ret);
		return ret;
	}

	struct disp_dmd dmd_info[list.dmd_count];
	ret = copy_from_user(&dmd_info, (void *)(list.dmd_info), sizeof(dmd_info));
	if (unlikely(ret != 0)) {
		dpu_pr_err("copy_from_user failed ret=%d\n", ret);
		return ret;
	}

	for (; i < list.dmd_count; i++) {
		if (dmd_info[i].dmd_id < DSS_DIAG_INFO_START || dmd_info[i].dmd_id > DSS_DIAG_INFO_END) {
			dpu_pr_debug("Invalid dmd id");
			continue;
		}

		uint32_t size = dmd_info[i].info_size + 1;
		if (size > DISP_DMD_MAX_LENGTH) {
			dpu_pr_err("report info size more than limit");
			continue;
		}

		char *info = kzalloc(size, GFP_KERNEL);
		if (unlikely(!info)) {
			dpu_pr_err("kzalloc info failed");
			return -1;
		}
		ret = copy_from_user(info, (void *)(dmd_info[i].dmd_report_info), size);
		if (unlikely(ret != 0)) {
			dpu_pr_err("copy_from_user failed ret=%d\n", ret);
			kfree(info);
			return -1;
		}

		bbox_diaginfo_record(dmd_info[i].dmd_id, NULL, info);
		kfree(info);
	}

	return 0;
}

int32_t gfxdev_mmap_table(struct sg_table *table, gfxdev_fix_screeninfo* fix, struct vm_area_struct *vma)
{
	uint32_t i = 0;
	int32_t ret = 0;
	struct scatterlist *sg = NULL;
	struct page *page = NULL;
	unsigned long size;
	unsigned long addr;
	unsigned long offset;
	unsigned long len;
	unsigned long remainder;

	if (unlikely(!table) || unlikely(!fix) || unlikely(!vma)) {
		dpu_pr_err("input is nullptr!");
		return -EINVAL;
	}

	vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

	addr = vma->vm_start;
	offset = vma->vm_pgoff * PAGE_SIZE;

	if (unlikely(vma->vm_end <= vma->vm_start)) {
		dpu_pr_err("vm_end minus vm_start is less than 0");
		return -EINVAL;
	}

	size = vma->vm_end - vma->vm_start;

	if (unlikely(size == 0)) {
		dpu_pr_err("vm_end - vm_start is 0");
		return -EINVAL;
	}

	if (unlikely(!fix)) {
		dpu_pr_err("fix is NULL");
		return -EINVAL;
	}

	if (size > fix->smem_len) {
		dpu_pr_err("vma size=%lu is out of range(%u)!", size, fix->smem_len);
		return -EFAULT;
	}

	for_each_sg(table->sgl, sg, table->nents, i) {
		if (unlikely(!sg)) {
		    dpu_pr_err("sg is NULL");
		    return -EINVAL;
		}

		page = sg_page(sg);
		if (unlikely(!page)) {
		    dpu_pr_err("page is NULL");
		    return -EINVAL;
		}
		remainder = vma->vm_end - addr;
		len = sg->length;

		if (offset >= sg->length) {
			offset -= sg->length;
			continue;
		}

		if (offset) {
			page += offset / PAGE_SIZE;
			len = sg->length - offset;
			offset = 0;
		}

		len = (unsigned long)min(len, remainder);
		ret = remap_pfn_range(vma, addr, page_to_pfn(page), len, vma->vm_page_prot);
		if (ret != 0) {
			dpu_pr_err("failed to remap_pfn_range! ret=%d", ret);
			return ret;
		}

		addr += len;
		if (addr >= vma->vm_end) {
			ret = 0;
			break;
		}
	}

	return ret;
}

/*
 * this function allocate physical memory,
 * and make them to scatter lista.
 * table is global .
 */
struct iommu_page_info *dpu_dma_create_node(void)
{
	/* alloc 8kb each time */
	unsigned order = 1;
	struct iommu_page_info *info = NULL;
	struct page *page = NULL;

	info = kzalloc(sizeof(struct iommu_page_info), GFP_KERNEL);
	if (!info) {
		dpu_pr_err("kzalloc info failed");
		return NULL;
	}
	page = alloc_pages(GFP_KERNEL, order);
	if (!page) {
		dpu_pr_err("alloc page error");
		kfree(info);
		info = NULL;
		return NULL;
	}
	info->page = page;
	info->order = order;
	INIT_LIST_HEAD(&info->list);

	return info;
}

struct sg_table *dpu_dma_alloc_mem(unsigned size)
{
	unsigned i = 0;
	unsigned alloc_size = 0;
	struct iommu_page_info *info = NULL;
	struct iommu_page_info *tmp_info = NULL;
	struct sg_table *table = NULL;
	struct scatterlist *sg = NULL;
	struct page *page = NULL;
	struct list_head pages;
	int32_t ret;

	if ((size > SZ_512M) || (size == 0))
		return NULL;

	INIT_LIST_HEAD(&pages);
	do {
		info = dpu_dma_create_node();
		if (!info)
			goto error;
		list_add_tail(&info->list, &pages);
		alloc_size += (1 << info->order) * PAGE_SIZE;
		i++;
	} while (alloc_size < size);

	table = kzalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (!table)
		goto error;

	ret = sg_alloc_table(table, i, GFP_KERNEL);
	if (ret) {
		kfree(table);
		table = NULL;
		goto error;
	}

	sg = table->sgl;
	list_for_each_entry_safe(info, tmp_info, &pages, list) {
		page = info->page;
		sg_set_page(sg, page, (1 << info->order) * PAGE_SIZE, 0);
		sg = sg_next(sg);
		list_del(&info->list);
		kfree(info);
		info = NULL;
	}

	dpu_pr_info("alloc total memory size 0x%x", alloc_size);
	return table;

error:
	list_for_each_entry_safe(info, tmp_info, &pages, list) {
		__free_pages(info->page, info->order);
		list_del(&info->list);
		kfree(info);
		info = NULL;
	}
	return NULL;
}

void dpu_dma_free_mem(struct sg_table *table)
{
	struct scatterlist *sg = NULL;
	unsigned mem_size = 0;
	uint32_t i;

	if (table) {
		for_each_sg(table->sgl, sg, table->nents, i) {
			__free_pages(sg_page(sg), (uint32_t)get_order(sg->length));
			mem_size += sg->length;
		}
		sg_free_table(table);
		kfree(table);
		table = NULL;
	}
	dpu_pr_info("free total memory size 0x%x", mem_size);
}

bool dpu_gfxdev_set_fastboot(struct composer *comp)
{
	dpu_pr_info("fastboot_display_enabled = %d", comp->fastboot_display_enabled);

	if ((comp->fastboot_display_enabled != 0) && comp->set_fastboot) {
		comp->power_on = comp->set_fastboot(comp);
		comp->fastboot_display_enabled = 0;
	}

	return comp->power_on;
}

int32_t dpu_gfxdev_set_active_rect(struct composer *comp, void __user *argp)
{
	int32_t ret = -EINVAL;
	uint32_t ppc_config_id = 0;
	struct dpu_composer *dpu_comp = NULL;

	if (unlikely(!comp || !argp)) {
		dpu_pr_err("comp or argp is null");
		return ret;
	}
	if (!comp->set_active_rect) {
		dpu_pr_err("set_active_rect is not init");
		return ret;
	}

	if (copy_from_user(&ppc_config_id, argp, sizeof(ppc_config_id)) != 0) {
		dpu_pr_err("copy from user active_rect_id info failed");
		return ret;
	}
	if (ppc_config_id > PPC_CONFIG_ID_G_MODE) {
		dpu_pr_err("invalid ppc_config_id:%u", ppc_config_id);
		return ret;
	}

	dpu_comp = to_dpu_composer(comp);
	if (dpu_tui_power_work(dpu_comp, DPU_CHANGE_ACT_DISPLAY) != 0)
		dpu_pr_warn("set active rect, quit tui failed"); /* not block set active rect */

	down(&comp->blank_sem);
	dpu_print_sem_count(&comp->blank_sem, true);
	dpu_pr_info("set active rect by ppc_config_id:%u", ppc_config_id);
	ret = comp->set_active_rect(comp, ppc_config_id);
	dpu_print_sem_count(&comp->blank_sem, false);
	up(&comp->blank_sem);
	return ret;
}

int32_t dpu_gfxdev_set_display_active_region(struct composer *comp, void __user *argp)
{
	int32_t ret = 0;
	struct dkmd_rect_coord user_region = {0};

	if (unlikely(!comp || !argp)) {
		dpu_pr_err("comp or argp is null");
		return -1;
	}

	if (copy_from_user(&user_region, argp, sizeof(user_region)) != 0) {
		dpu_pr_err("copy display active region from user fail");
		return -1;
	}

	if (comp->set_display_active_region != NULL) {
		ret = comp->set_display_active_region(comp, &user_region);
	}

	return ret;
}

int32_t dpu_gfxdev_power_ctl(uint32_t panel_id)
{
	// interface for lcdkit.
	int32_t ret = -1;
	uint32_t index;
	struct composer *comp = NULL;
	struct dpu_composer *dpu_comp = NULL;
	struct comp_online_present *present = NULL;
	struct composer_manager *comp_manager = get_dpu_composer_manager();

	if (!comp_manager) {
		dpu_pr_warn("get comp_manager fail.");
		return ret;
	}

	for (index = 0; index < DEVICE_COMP_MAX_COUNT; index++) {
		comp = &comp_manager->dpu_comps[index]->comp;
		if (comp && comp->base.id == panel_id) {
			dpu_comp = to_dpu_composer(comp);
			break;
		}
	}

	if (!dpu_comp) {
		dpu_pr_warn("get dpu_composer fail.");
		return ret;
	}

	if (dpu_comp->pmic_abnormal_flag) {
		dpu_pr_info("pmic abnormal is running.");
		return ret;
	}

	if (!comp->power_on) {
		dpu_pr_info("panel id:%u already power off, do not need reset!", panel_id);
		return ret;
	}

	present = (struct comp_online_present *)dpu_comp->present_data;
	if (!present) {
		dpu_pr_warn("get comp_online_present fail.");
		return ret;
	}
	kthread_queue_work(&dpu_comp->handle_worker, &present->pmic_abnormal_handle_work);

	dpu_pr_info("reset hardware finished!");
	return 0;
}

int32_t dpu_gfxdev_get_product_ext_config(struct composer *comp, void __user *argp)
{
	int32_t ret = 0;
	struct product_ext_config ext_config = {0};

	dpu_check_and_return(unlikely(!argp), -EINVAL, err, "argp is null pointer");

	if (comp->get_ext_info == NULL) {
		dpu_pr_info("comp get ext info function is nullptr");
		return 0;
	}

	ret = comp->get_ext_info(comp, &ext_config);
	if (unlikely(ret != 0)) {
		dpu_pr_err("%s get ext config fail", comp->base.name);
		return -1;
	}

	if (copy_to_user(argp, &ext_config, sizeof(ext_config)) != 0) {
		dpu_pr_err("copy dynamic config to user fail");
		return -1;
	}
	return 0;
}

static int32_t update_hardware_cursor_coord(struct composer *comp, int32_t scene_id, int32_t x, int32_t y)
{
	if (unlikely(comp->update_hardware_cursor_coord == NULL)) {
		dpu_pr_err("[hw cursor] update_hardware_cursor_coord is not init");
		return -EINVAL;
	}

	return comp->update_hardware_cursor_coord(comp, scene_id, x, y);
}

static int32_t enable_hardware_cursor_stats(struct composer *comp, int32_t scene_id)
{
	if (unlikely(comp->enable_hardware_cursor_stats == NULL)) {
		dpu_pr_err("[hw cursor] enable_hardware_cursor_stats is not init");
		return -EINVAL;
	}

	return comp->enable_hardware_cursor_stats(comp, scene_id, true);
}

static int32_t disable_hardware_cursor_stats(struct composer *comp, int32_t scene_id)
{
	if (unlikely(comp->enable_hardware_cursor_stats == NULL)) {
		dpu_pr_err("[hw cursor] enable_hardware_cursor_stats is not init");
		return -EINVAL;
	}

	return comp->enable_hardware_cursor_stats(comp, scene_id, false);
}

static int32_t get_hardware_cursor_stats(struct composer *comp, int32_t scene_id, uint32_t *user_frame_count, uint32_t *user_vsync_count)
{
	uint32_t frame_count;
	uint32_t vsync_count;
	int32_t ret;
	if (unlikely(comp->get_hardware_cursor_stats == NULL)) {
		dpu_pr_err("[hw cursor] get_hardware_cursor_stats is not init");
		return -EINVAL;
	}

	if (unlikely((user_frame_count == NULL) || (user_vsync_count == NULL))) {
		dpu_pr_err("[hw cursor] given frame_count or vsync_count should not be null");
		return -EINVAL;
	}

	ret = comp->get_hardware_cursor_stats(comp, scene_id, &frame_count, &vsync_count);
	if (ret != 0) {
		return ret;
	}

	if (copy_to_user(user_frame_count, &frame_count, sizeof(uint32_t))) {
		dpu_pr_err("[hw cursor] failed to copy frame_count to user space");
		return -EFAULT;
	}

	if (copy_to_user(user_vsync_count, &vsync_count, sizeof(uint32_t))) {
		dpu_pr_err("[hw cursor] failed to copy frame_count to user space");
		return -EFAULT;
	}

	dpu_pr_debug("[hw cursor] frame count:%d, vsync count:%d", frame_count, vsync_count);

	return 0;
}

int32_t dpu_gfxdev_update_hardware_cursor(struct composer *comp, void __user *argp)
{
	int32_t ret = 0;
	struct hardware_cursor_info info = {0};

	if (unlikely((comp == NULL) || (argp == NULL))) {
		dpu_pr_err("[hw cursor] comp or argp is null");
		return -EINVAL;
	}

	if (unlikely(!comp->power_on)) {
		dpu_pr_err("[hw cursor] cannot update hardware cursor, composer is power down");
		return -1;
	}

	if (copy_from_user(&info, argp, sizeof(info))) {
		dpu_pr_err("[hw cursor] failed to copy from user");
		return -EFAULT;
	}

	dpu_pr_debug("[hw cursor] cmd: %d, scene_id: %d, x: %d, y: %d", info.cmd, info.scene_id, info.coord.x, info.coord.y);

	switch(info.cmd) {
		case HARDWARE_CURSOR_UPDATE_COORDINATE:
			ret = update_hardware_cursor_coord(comp, info.scene_id, info.coord.x, info.coord.y);
			break;
		case HARDWARE_CURSOR_ENABLE_STATS:
			ret = enable_hardware_cursor_stats(comp, info.scene_id);
			break;
		case HARDWARE_CUSROR_DISABLE_STATS:
			ret = disable_hardware_cursor_stats(comp, info.scene_id);
			break;
		case HARDWARE_CURSOR_GET_STATS:
			ret = get_hardware_cursor_stats(comp, info.scene_id, info.stats.frame_count, info.stats.vsync_count);
			break;
		default:
			dpu_pr_err("[hw cursor] unsupported hardware cursor cmd: %x", info.cmd);
			return -EINVAL;
	}

	dpu_pr_debug("[hw cursor] ret: %d, scene_id: %d, x: %d, y: %d", ret, info.scene_id, info.coord.x, info.coord.y);

	return ret;
}