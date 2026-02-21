/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2022-2022.
 * All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */
#include "hvgr_hts.h"

#include <linux/delay.h>
#include <linux/vmalloc.h>

#include "hvgr_hts.h"
#include "hvgr_defs.h"
#include "hvgr_regmap.h"
#include "hvgr_mem_api.h"
#include "hvgr_sch_api.h"
#include "hvgr_dm_api.h"
#include "hvgr_log_api.h"
#include "hvgr_pm_api.h"
#include "hvgr_cq_queue.h"
#include "uniform_id.h"
#include "hts_interface.h"

static void hvgr_hts_event_table_cmd(struct hvgr_device *gdev, uint32_t cmd)
{
	uint32_t try_times = HVGR_HTS_TRY_TIMES_MAX;
	uint32_t value;
	struct hvgr_ctx *ctx = gdev->hts_dev.ctx;
	struct hvgr_mem_area *area = gdev->hts_dev.area;

	if (unlikely(area == NULL))
		return;
	/*
	 * Use the same ctx with fcp load task. mmu is enable by fcp load task,
	 * so do not need to enable mmu again.
	 */

	/* Config gpu va */
	hvgr_write_reg(gdev, cq_control_reg(gdev, CQ_HTS_EVENT_LS_ADDR_L),
		(uint32_t)(area->gva & (uint64_t)0xFFFFFFFF));
	hvgr_write_reg(gdev, cq_control_reg(gdev, CQ_HTS_EVENT_LS_ADDR_H),
		(uint32_t)(area->gva >> 32));

	/* Config ASID */
	hvgr_write_reg(gdev, cq_control_reg(gdev, CQ_HTS_EVENT_LS_ASID), ctx->asid);
	/* Start */
	hvgr_write_reg(gdev, cq_control_reg(gdev, CQ_HTS_EVENT_CMD), cmd | HTS_EVENT_CMD_START);
	/* Wait cmd process done. */
	while (try_times > 0) {
		value = hvgr_read_reg(gdev, cq_control_reg(gdev, CQ_HTS_EVENT_CMD_DONE));
		if (value & CQ_HTS_EVENT_CMD_FINISH)
			break;
		udelay(1);
		try_times--;
	}
	if (try_times == 0)
		hvgr_err(gdev, HVGR_HTS, "%s cq hts event cmd done time out", __func__);
	else if (try_times < (HVGR_HTS_TRY_TIMES_MAX - 100))
		hvgr_err(gdev, HVGR_HTS, "%s cq hts event cmd done. try_times=%u",
			__func__, HVGR_HTS_TRY_TIMES_MAX - try_times);
	else
		hvgr_info(gdev, HVGR_HTS, "%s cq hts event cmd done.", __func__);
}

/*
 * load hts event refcnt from DDR
 * Constraints: this api must be called after hvgr_fcp_load_by_poll, Otherwise the mmu is
 * not enable and load failed.
 */
void hvgr_hts_load_event_table(struct hvgr_device * const gdev)
{
	hvgr_info(gdev, HVGR_HTS, "%s in", __func__);
	/* firstly enable MMU, then load hts event */
	if (unlikely(gdev->hts_dev.ctx == NULL)) {
		hvgr_debug(gdev, HVGR_HTS, "%s fcp load task not finish", __func__);
		return;
	}
	(void)hvgr_mmu_hal_enable(gdev->hts_dev.ctx);
	hvgr_hts_event_table_cmd(gdev, HTS_EVENT_REF_LOAD);
	(void)hvgr_mmu_hal_disable(gdev->hts_dev.ctx);
	hvgr_info(gdev, HVGR_HTS, "%s out", __func__);
}

void hvgr_hts_store_event_through_register(struct hvgr_device * const gdev)
{
	int i;
	uint32_t *kva = NULL;

	hvgr_info(gdev, HVGR_HTS, "%s in", __func__);
	if (unlikely(gdev->hts_dev.area == NULL)) {
		hvgr_err(gdev, HVGR_HTS, "hts area is null");
		return;
	}
	kva = (uint32_t *)u64_to_ptr(gdev->hts_dev.area->kva);
	if (unlikely(kva == NULL)) {
		hvgr_err(gdev, HVGR_HTS, "hts memory is not mapped");
		return;
	}

	for (i = 0; i < HTS_EVENT_REGISTER_MAX; i++)
		kva[i] = hvgr_read_reg(gdev,
			cq_control_reg(gdev, HTS_EVENT_REFCNT_BASE + i * 4));
	hvgr_info(gdev, HVGR_HTS, "%s out", __func__);
}

/*
 * store event refcnt to DDR
 * Constraints: this api must be called after hvgr_fcp_load_by_poll, Otherwise the mmu is
 * not enable and store failed.
 */
void hvgr_hts_store_event_table(struct hvgr_device * const gdev)
{
	hvgr_info(gdev, HVGR_HTS, "%s in", __func__);
	hvgr_hts_event_table_cmd(gdev, HTS_EVENT_REF_STORE);
	hvgr_info(gdev, HVGR_HTS, "%s out", __func__);
}

/* alloc memory for events refcounts */
int hvgr_hts_alloc_events_memory(struct hvgr_device * const gdev, struct hvgr_ctx *ctx)
{
	struct hvgr_mem_area *area = NULL;
	struct hvgr_mmu_setup_paras mmu_para = {0};
	uint32_t level = 0;

	area = kzalloc(sizeof(*area), GFP_KERNEL);
	if (area == NULL) {
		hvgr_err(gdev, HVGR_HTS, "Create hts area failed.");
		goto fail_area;
	}

	area->zone = &ctx->mem_ctx.zones[0];
	area->pages = HVGR_HTS_EVENT_PAGES;
	area->max_pages = HVGR_HTS_EVENT_PAGES;
	kref_init(&area->kref);

	if (hvgr_mem_alloc_pages(area) != 0) {
		hvgr_err(gdev, HVGR_HTS, "Alloc hts page fail.");
		goto fail_pages;
	}

	area->attribute = HVGR_MEM_ATTR_GPU_RD | HVGR_MEM_ATTR_GPU_WR;
	if (!hvgr_mem_map_kva(area)) {
		hvgr_err(gdev, HVGR_HTS, "hts task kmap page fail");
		goto fail_kmap;
	}

	area->gva = area->kva;
	hvgr_mem_attr2flag(area);

	mmu_para.gva = area->gva;
	mmu_para.page_array = area->page_array;
	mmu_para.pages = area->pages;
	mmu_para.mmu_flag = area->mmu_flag;
	mmu_para.flags = area->flags;

	if (hvgr_mmu_set_up(ctx, &mmu_para, &level, area) != 0) {
		hvgr_err(gdev, HVGR_HTS, "hts task set up pgd fail");
		goto fail_pgd;
	}
	gdev->hts_dev.area = area;
	gdev->hts_dev.ctx = ctx;
	return 0;
fail_pgd:
	hvgr_mem_unmap_kva(area);
fail_kmap:
	(void)hvgr_mem_free_pages(area);
fail_pages:
	kfree(area);
fail_area:
	return -ENOMEM;
}

/* free events memory */
void hvgr_hts_free_events_memory(struct hvgr_device * const gdev)
{
	struct hvgr_ctx *ctx = gdev->hts_dev.ctx;
	struct hvgr_mem_area *area = gdev->hts_dev.area;
	struct hvgr_mmu_takedown_paras paras;

	if (unlikely(area == NULL))
		return;

	paras.va = area->gva;
	paras.pages = HVGR_HTS_EVENT_PAGES;
	paras.bitmap = 0U;
	INIT_LIST_HEAD(&paras.free_list);
	(void)hvgr_mmu_take_down(ctx, &paras, area);
	hvgr_mmu_free_pt_pages(ctx, &paras.free_list);
	hvgr_mem_unmap_kva(area);
	(void)hvgr_mem_free_pages(area);
	kfree(area);
	gdev->hts_dev.ctx = NULL;
	gdev->hts_dev.area = NULL;
}

/* lock and cache hts events */
void hvgr_hts_event_lock(struct hvgr_device *gdev)
{
	uint32_t value;
	int times = 10;

	hvgr_info(gdev, HVGR_HTS, "[HTS-DEBUG]%s hts_event_lock in.", __func__);
	if (hts_event_outbuff_enable_by_dev(DEV_GPU) != 0) {
		hvgr_err(gdev, HVGR_HTS, "hts event lock fail.");
		return;
	}

	hvgr_info(gdev, HVGR_HTS, "[HTS-DEBUG]%s hts_event_lock done.", __func__);
	/* wait gpu receive event and store to ram */
	do {
		value = hvgr_read_reg(gdev, gpu_control_reg(gdev,GPU_STATUS));
		times--;
	} while (((value & GPU_STATUS_CQ_ACTIVE) != 0) && (times > 0));
	if (times == 0)
		hvgr_err(gdev, HVGR_HTS, "wait cq idle timeout.");
}

/* unlock hts events */
void hvgr_hts_event_unlock(void)
{
	struct hvgr_device *gdev = hvgr_get_device();

	if (hts_event_outbuff_disable_by_dev(DEV_GPU) != 0) {
		hvgr_err(gdev, HVGR_HTS, "hts event unlock fail.");
		return;
	}

	hvgr_info(gdev, HVGR_HTS, "[HTS-DEBUG]%s out.", __func__);
}

uint32_t hvgr_hts_get_ref_cnt(struct hvgr_device *gdev, uint32_t event_id)
{
	uint32_t reg_offset, int_offset;
	uint32_t ref_cnt;

	reg_offset = event_id / sizeof(uint32_t);
	int_offset = event_id % sizeof(uint32_t);
	ref_cnt = hvgr_read_reg(gdev,
		cq_control_reg(gdev, HTS_EVENT_REFCNT_BASE + reg_offset * 4));
	ref_cnt = (ref_cnt >> (int_offset * 8)) & (0xff);
	hvgr_info(gdev, HVGR_HTS, "%s out. ref_cnt = %d, event_id = %d", __func__, ref_cnt, event_id);
	return ref_cnt;
}

static bool hvgr_hts_del_timeout_cqctx_in_wait_queue(struct hvgr_device *gdev,
	struct hvgr_cq_ctx *cq_ctx)
{
	struct hvgr_hts_dev *hts_dev = &gdev->hts_dev;
	unsigned long flags;
	struct hvgr_cq_ctx *iter = NULL;
	struct hvgr_cq_ctx *tmp = NULL;

	spin_lock_irqsave(&hts_dev->hts_lock, flags);
	list_for_each_entry_safe(iter, tmp, &hts_dev->hts_list_head, hts_list) {
		if (iter == cq_ctx) {
			list_del(&iter->hts_list);
			spin_unlock_irqrestore(&hts_dev->hts_lock, flags);
			return true;
		}
	}
	spin_unlock_irqrestore(&hts_dev->hts_lock, flags);
	return false;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0)
static void hvgr_hts_wait_timeout_callback(struct timer_list *timer)
{
#else
static void hvgr_hts_wait_timeout_callback(unsigned long addr)
{
	struct timer_list *timer = (struct timer_list *)(uintptr_t)addr;
#endif
	struct hvgr_cq_ctx *cq_ctx = NULL;
	struct hvgr_device *gdev = hvgr_get_device();
	struct hvgr_pm_reset *pm_reset = NULL;
	struct hvgr_hts_dev *hts_dev = NULL;
	struct hvgr_cq_dev *gcqctx = NULL;
	unsigned long flags;

	cq_ctx = container_of(timer, struct hvgr_cq_ctx, hts_timer);
	if (unlikely(cq_ctx == NULL || gdev == NULL))
		return;
	pm_reset = &gdev->pm_dev.pm_reset;
	hts_dev = &gdev->hts_dev;

	gcqctx = &gdev->cq_dev;
	spin_lock_irqsave(&gcqctx->schedule_lock, flags);
	if (unlikely((cq_ctx->flags & CQ_KMD_CTX_START_SUBMIT) == 0) ||
		(cq_ctx->status == CQ_STATUS_PENDING)) {
		hvgr_debug(gdev, HVGR_CQ, "qid(%d) flags=0x%x status=%u do not need to reset gpu\n",
			cq_ctx->queue_id, cq_ctx->flags, cq_ctx->status);
		goto out;
	}

	if (atomic_read(&pm_reset->reset_status) != GPU_RESET_END ||
		(atomic_cmpxchg(&hts_dev->hts_timeout_flag, 0, 1) != 0)) {
		(void)mod_timer(&cq_ctx->hts_timer, jiffies + (uint32_t)HTS_WAIT_TIMEOUT * HZ);
		hvgr_err(gdev, HVGR_HTS, "%s gpu reset is pending, delay 5 seconds.", __func__);
		goto out;
	}

	hts_dev->hts_timeout_cq_ctx = cq_ctx;
	hvgr_info(gdev, HVGR_HTS, "%s store hts timeout cq ctx: qid=%u, ctx_%u", __func__,
		cq_ctx->queue_id, cq_ctx->ctx->id);

	if (hvgr_hts_del_timeout_cqctx_in_wait_queue(gdev, cq_ctx)) {
		hvgr_err(gdev, HVGR_HTS, "%s del wait queue and reset gpu.", __func__);
		hvgr_pm_gpu_reset(gdev, GPU_RESET_TYPE_IMMEDIATELY);
	}
out:
	spin_unlock_irqrestore(&gcqctx->schedule_lock, flags);
}

static void hvgr_hts_wait_list_add(struct hvgr_cq_ctx * const cq_ctx, struct list_head *hts_head)
{
	unsigned long flags;
	struct hvgr_hts_dev *hts_dev = &cq_ctx->ctx->gdev->hts_dev;

	spin_lock_irqsave(&hts_dev->hts_lock, flags);
	list_add_tail(&cq_ctx->hts_list, hts_head);
	(void)mod_timer(&cq_ctx->hts_timer, jiffies + (uint32_t)HTS_WAIT_TIMEOUT * HZ);
	spin_unlock_irqrestore(&hts_dev->hts_lock, flags);
}

void hvgr_hts_wait_list_in_schedule(struct hvgr_device * const gdev, enum sch_type type)
{
	struct hvgr_hts_dev *hts_dev = &gdev->hts_dev;
	unsigned long flags;
	uint32_t ref_cnt;
	int event_id;
	struct hvgr_cq_ctx *iter = NULL;
	struct hvgr_cq_ctx *tmp = NULL;
	struct list_head del_timer_list;

	INIT_LIST_HEAD(&del_timer_list);

	spin_lock_irqsave(&hts_dev->hts_lock, flags);
	if (list_empty(&hts_dev->hts_list_head)) {
		hvgr_info(gdev, HVGR_HTS, "%s hts wait queue is null", __func__);
		spin_unlock_irqrestore(&hts_dev->hts_lock, flags);
		return;
	}

	list_for_each_entry_safe(iter, tmp, &hts_dev->hts_list_head, hts_list) {
		event_id = iter->hts_event_id;
		ref_cnt = hvgr_hts_get_ref_cnt(gdev, event_id);
		if (ref_cnt == 0) {
			hvgr_info(gdev, HVGR_HTS, "%s ref_cnt equal 0. type = %d, event_id = %d",
				__func__, type, event_id);
			continue;
		}
		hvgr_info(gdev, HVGR_HTS, "%s event_id=%d ,ref_cnt=%d  will enqueue", __func__, event_id, ref_cnt);

		list_del(&iter->hts_list);
		hvgr_hts_sch_schedule(gdev, iter, type);
		list_add(&iter->hts_list, &del_timer_list);
	}
	spin_unlock_irqrestore(&hts_dev->hts_lock, flags);

	list_for_each_entry_safe(iter, tmp, &del_timer_list, hts_list) {
		list_del(&iter->hts_list);
		del_timer(&iter->hts_timer);
	}
}

bool hvgr_hts_wait_entry_enqueue(struct hvgr_cq_ctx * const cq_ctx)
{
	struct hvgr_wait_entry_info *entry_info = NULL;
	struct hvgr_device *gdev = cq_ctx->ctx->gdev;
	struct hvgr_hts_dev *hts_dev = &gdev->hts_dev;
	uint32_t ref_cnt;
	uint32_t event_id;
	int i;

	entry_info = (struct hvgr_wait_entry_info *) &cq_ctx->sw_wr_ctx->wait_entry_info[0];
	for (i = 0; i < WAIT_ENTRY_MAX_NUM - 1; i++) {
		if (entry_info[i].hts_en == 0)
			continue;
		event_id = entry_info[i].event_id;
		if (unlikely(event_id > HTS_EVENT_MAX_NUM || event_id == 0)) {
			hvgr_err(gdev, HVGR_HTS, "event id is invalid, event id = %u, entry id = %u",
				event_id, entry_info[i].entry_id);
			event_id = 0;
		}
		ref_cnt = hvgr_hts_get_ref_cnt(gdev, event_id);
		if (ref_cnt != 0) {
			hvgr_info(gdev, HVGR_HTS, "receive hts event, add to hts wait queue is not needed.");
			return false;
		}
		hvgr_debug(gdev, HVGR_HTS, "not receive hts event, add eventid_%d to hts wait queue.",
			event_id);
		cq_ctx->hts_event_id = event_id;
		hvgr_hts_wait_list_add(cq_ctx, &hts_dev->hts_list_head);
		return true;
	}
	hvgr_info(gdev, HVGR_HTS, "No hts wait event find.");
	return false;
}

void hvgr_hts_wait_pendinglist_clear(struct hvgr_device *gdev, struct hvgr_cq_ctx * const cq_ctx)
{
	unsigned long flags;
	struct hvgr_hts_dev *hts_dev = &gdev->hts_dev;
	struct hvgr_cq_ctx *iter = NULL;
	struct hvgr_cq_ctx *tmp = NULL;
	bool is_empty = false;
	struct list_head del_timer_list;

	INIT_LIST_HEAD(&del_timer_list);

	spin_lock_irqsave(&hts_dev->hts_lock, flags);

	list_for_each_entry_safe(iter, tmp, &hts_dev->hts_list_head, hts_list) {
		if (iter == cq_ctx) {
			hvgr_err(gdev, HVGR_HTS, "[HTS-DEBUG]%s find cq_ctx and clear it.", __func__);
			list_del(&iter->hts_list);
			list_add(&iter->hts_list, &del_timer_list);
			break;
		}
	}
	if (list_empty(&hts_dev->hts_list_head))
		is_empty = true;
	spin_unlock_irqrestore(&hts_dev->hts_lock, flags);

	list_for_each_entry_safe(iter, tmp, &del_timer_list, hts_list) {
		list_del(&iter->hts_list);
		del_timer_sync(&iter->hts_timer);
	}

	if (is_empty) {
		hvgr_info(gdev, HVGR_HTS, "hts wait quque is empty and try to power off gpu.");
		hvgr_pm_try_power_off_gpu(gdev);
	}
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
static void hvgr_hts_schedule_queue(struct work_struct *data)
#else
static void hvgr_hts_schedule_queue(struct kthread_work *data)
#endif
{
	struct hvgr_device *gdev = NULL;

	gdev = container_of(data, struct hvgr_device, hts_dev.no_consumer_wq_work);
	hvgr_hts_sch_schedule(gdev, NULL, HVGR_SCH_SCHEDULE_ALL);
}

int hvgr_hts_init(struct hvgr_device * const gdev)
{
	struct hvgr_hts_dev *hts_dev = &gdev->hts_dev;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	hts_dev->hts_wq = alloc_workqueue("hvgr_no_consumer", WQ_HIGHPRI | WQ_UNBOUND, 1);
	if (hts_dev->hts_wq == NULL) {
		hvgr_err(gdev, HVGR_HTS, "alloc workqueue hvgr_no_consumer fail.");
		return -ENOMEM;
	}
	INIT_WORK(&hts_dev->no_consumer_wq_work, hvgr_hts_schedule_queue);
#else
	hts_dev->hts_wq = kthread_create_worker(0, "hvgr_no_consumer");
	if (IS_ERR(hts_dev->hts_wq)) {
		hvgr_err(gdev, HVGR_HTS, "alloc workqueue hvgr_no_consumer fail.");
		return -ENOMEM;
	}
	sched_set_fifo(hts_dev->hts_wq->task);
	kthread_init_work(&hts_dev->no_consumer_wq_work, hvgr_hts_schedule_queue);
#endif

	INIT_LIST_HEAD(&hts_dev->hts_list_head);
	spin_lock_init(&hts_dev->hts_lock);

	atomic_set(&hts_dev->hts_timeout_flag, 0);
	hts_dev->hts_timeout_cq_ctx = NULL;

	return 0;
}

void hvgr_hts_term(struct hvgr_device *gdev)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	if (gdev->hts_dev.hts_wq != NULL)
		destroy_workqueue(gdev->hts_dev.hts_wq);
#else
	if (gdev->hts_dev.hts_wq != NULL)
		kthread_destroy_worker(gdev->hts_dev.hts_wq);
#endif
}

void hvgr_hts_no_consumer_irq(struct hvgr_device * const gdev)
{
	uint32_t irq_no_consumer;
	irq_no_consumer = hvgr_read_reg(gdev, cq_control_reg(gdev, CQ_IRQ_VCQ1));
	hvgr_info(gdev, HVGR_HTS, "%s irq_no_consumer = 0x%x.\n", __func__, irq_no_consumer);
	if (irq_no_consumer == 0)
		return;

	if ((irq_no_consumer & NO_CONSUMER_IRQ) != 0) {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		(void)queue_work(gdev->hts_dev.hts_wq, &gdev->hts_dev.no_consumer_wq_work);
#else
		(void)kthread_queue_work(gdev->hts_dev.hts_wq, &gdev->hts_dev.no_consumer_wq_work);
#endif
		hvgr_write_reg(gdev, cq_control_reg(gdev, CQ_IRQ_TOP_CLEAR), NO_CONSUMER_IRQ);
	}
}

bool hvgr_hts_is_waitqueue_empty(struct hvgr_device * const gdev)
{
	unsigned long flags;

	spin_lock_irqsave(&gdev->hts_dev.hts_lock, flags);

	if (list_empty(&gdev->hts_dev.hts_list_head)) {
		spin_unlock_irqrestore(&gdev->hts_dev.hts_lock, flags);
		hvgr_info(gdev, HVGR_HTS, "%s deep power off.", __func__);
		return true;
	}

	spin_unlock_irqrestore(&gdev->hts_dev.hts_lock, flags);
	return false;
}

void hvgr_hts_get_ssid(struct hvgr_ctx *ctx)
{
	uint16_t ssid;

	ssid = ffrt_get_uniform_id(ctx->tgid);
	if (ssid == FFRT_INVALID_UNIFORM_ID || ssid > FFRT_UNIFORM_ID_END) {
		hvgr_err(ctx->gdev, HVGR_HTS, "ctx->tgid = %d, get ssid failed.", ctx->tgid);
		return;
	}
	ctx->ssid = ssid;
}

void hvgr_hts_set_ssid(struct hvgr_ctx *ctx, uint32_t channel_id)
{
	hvgr_write_reg(ctx->gdev, cq_control_reg(ctx->gdev, cq_bind_pid(channel_id)), (uint32_t)ctx->ssid);
	hvgr_info(ctx->gdev, HVGR_HTS, "[HTS-DEBUG]%s out. ctx_%u tgid=%u ssid=%u",__func__, ctx->id,
		ctx->tgid, ctx->ssid);
}

void hvgr_hts_no_consumer_irq_enable(struct hvgr_device * const gdev)
{
	hvgr_write_reg(gdev, cq_control_reg(gdev, CQ_IRQ_TOP_CLEAR), 0x1);
	hvgr_write_reg(gdev, cq_control_reg(gdev, CQ_IRQ_TOP_MASK), 0x1);
}

void hvgr_hts_no_consumer_irq_disable(struct hvgr_device * const gdev)
{
	hvgr_write_reg(gdev, cq_control_reg(gdev, CQ_IRQ_TOP_CLEAR), 0x1);
	hvgr_write_reg(gdev, cq_control_reg(gdev, CQ_IRQ_TOP_MASK), 0x0);
}

int hvgr_hts_ctx_init(struct hvgr_cq_ctx *cq_ctx)
{
	hvgr_timer_setup(&cq_ctx->hts_timer, hvgr_hts_wait_timeout_callback);
	return 0;
}

static bool hvgr_hts_event_id_is_valid(struct hvgr_ctx *ctx, uint32_t event_id)
{
	struct dev_event_info event_info;
	uint32_t i;

	if (unlikely(event_id > HTS_EVENT_MAX_NUM || event_id == 0)) {
		hvgr_err(ctx->gdev, HVGR_HTS, "event id is invalid, event id = %u", event_id);
		return false;
	}

	event_info.dev_id = DEV_GPU;
	event_info.uid = ctx->ssid;

	if (ip_get_hts_uid_event(&event_info) != 0) {
		hvgr_err(ctx->gdev, HVGR_HTS, "ip_get_hts_uid_event fail, event id = %u", event_id);
		return false;
	}

	if (event_info.event_count >= HTS_INTERFACE_MAX_EVENT_BUFF) {
		hvgr_err(ctx->gdev, HVGR_HTS, "event_count overflow, event id = %u", event_id);
		return false;
	}

	for (i = 0; i < event_info.event_count; i++) {
		hvgr_info(ctx->gdev, HVGR_HTS, "%s event_info.events[%u]=%u ", __func__, i,
			event_info.events[i]);
		if (event_info.events[i] == event_id)
			return true;
	}

	return false;
}

/* if GPU BL is power on, clean GPU ram, otherwise, clean ddr. */
static void hvgr_hts_clear_event_id(struct hvgr_device * const gdev, uint32_t event_id)
{
	uint32_t reg_offset;
	uint32_t int_offset;
	uint32_t ref_cnt;
	uint8_t *event_table = NULL;

	mutex_lock(&gdev->pm_dev.pm_pwr.power_lock);
	if (hvgr_pm_get_gpu_status(gdev) == HVGR_PM_POWER_OFF ||
		hvgr_pm_get_gpu_status(gdev) == HVGR_PM_CORE_ERROR) {
		event_table = (uint8_t *)u64_to_ptr(gdev->hts_dev.area->kva);
		if (unlikely(event_table == NULL)) {
			hvgr_err(gdev, HVGR_HTS, "hts memory is not mapped");
			mutex_unlock(&gdev->pm_dev.pm_pwr.power_lock);
			return;
		}
		hvgr_debug(gdev, HVGR_HTS, "%s ddr ref_cnt = %u", __func__, event_table[event_id]);
		ref_cnt = 0;
		event_table[event_id] = 0;
	} else {
		reg_offset = event_id / sizeof(uint32_t);
		int_offset = event_id % sizeof(uint32_t);
		ref_cnt = hvgr_read_reg(gdev,
			cq_control_reg(gdev, HTS_EVENT_REFCNT_BASE + reg_offset * 4));
		hvgr_debug(gdev, HVGR_HTS, "%s ram ref_cnt = %u", __func__, ref_cnt);
		ref_cnt = ref_cnt & (~(0xff << (int_offset * 8)));
		hvgr_write_reg(gdev,
			cq_control_reg(gdev, HTS_EVENT_REFCNT_BASE + reg_offset * 4), ref_cnt);
	}
	mutex_unlock(&gdev->pm_dev.pm_pwr.power_lock);
	hvgr_debug(gdev, HVGR_HTS, "%s out.ref_cnt=%d,event_id=%d", __func__, ref_cnt, event_id);
}

/* clean all event_id and rules of current process in hts outbuff */
void hvgr_hts_block_proc(struct hvgr_ctx * const ctx)
{
	struct dev_pid_info info;

	if (ctx->ssid < HVGR_HTS_SSID_START) {
		hvgr_info(ctx->gdev, HVGR_HTS, "%s ctx_%u ssid not valid", __func__, ctx->id);
		return;
	}

	info.dev_id = DEV_GPU;
	info.pid = ctx->tgid;

	if (hts_block_proc(info) != 0)
		hvgr_err(ctx->gdev, HVGR_HTS, "%s fail hts_uid=%u", __func__, info.pid);

	hvgr_info(ctx->gdev, HVGR_HTS, "%s ctx_%u hts_block_proc done", __func__, ctx->id);
}

long hvgr_ioctl_hts_clear_event(struct hvgr_ctx * const ctx,
	union hvgr_ioctl_para_hts_clear_event * const para)
{
	uint32_t event_id;

	if (unlikely((ctx == NULL) || (para == NULL)))
		return -EINVAL;

	event_id = para->in.event_id;

	if (!hvgr_hts_event_id_is_valid(ctx, event_id)) {
		hvgr_err(ctx->gdev, HVGR_HTS, "%s event_id not current process", __func__);
		return -EINVAL;
	}

	hvgr_hts_clear_event_id(ctx->gdev, event_id);
	return 0;
}