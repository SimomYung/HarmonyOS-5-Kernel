/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2021. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_ksync.h"

#include <linux/moduleparam.h>
#include <linux/anon_inodes.h>
#include <linux/sync_file.h>
#include <linux/semaphore.h>
#include <linux/file.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/atomic.h>
#include <linux/syscalls.h>

#include "hvgr_datan_api.h"
#include "hvgr_log_api.h"
#include "hvgr_ksync_dma_fence.h"
#include "hvgr_msync.h"
#include "hvgr_softjob.h"
#include "hvgr_cq_schedule.h"
#include "hvgr_mem_api.h"
#include "hvgr_pm_api.h"
#include "hvgr_cq_schedule_submit.h"

int hvgr_ksync_fence_validate(struct hvgr_ctx * const ctx,
	struct hvgr_ioctl_fence_validate *validate)
{
#if defined(CONFIG_SYNC) || defined(CONFIG_SYNC_FILE)
	struct dma_fence *fence = sync_file_get_fence(validate->fd);

	if (fence == NULL) {
		hvgr_err(ctx->gdev, HVGR_FENCE, "%s ctx_%u get fence fail %d",
			__func__, ctx->id, validate->fd);
		return -EINVAL;
	}

	dma_fence_put(fence);

	return 0; /* valid */
#else
	return -ENOENT;
#endif
}

/**
 * Create a new sync fd, associate to ctx and return it.
 * Return fd if successful, else return error number.
 */
int hvgr_ksync_export_fd(struct hvgr_ctx * const ctx, struct dma_fence **new_fence)
{
	int fd = -1;
	struct dma_fence *fence = NULL;
	struct sync_file *sync_file = NULL;

	fence = hvgr_ksync_create_dma_fence(ctx);
	if (fence == NULL) {
		hvgr_err(ctx->gdev, HVGR_FENCE,
			"%s ctx_%u create dma fence failed!", __func__, ctx->id);
		return -EINVAL;
	}

	sync_file = sync_file_create(fence);
	if (sync_file == NULL) {
		hvgr_err(ctx->gdev, HVGR_FENCE,
			"%s ctx_%u create sync file failed!", __func__, ctx->id);
		hvgr_ksync_destroy_dma_fence(ctx, fence);
		return -EINVAL;
	}

	fd = get_unused_fd_flags(O_CLOEXEC);
	if (fd < 0) {
		hvgr_err(ctx->gdev, HVGR_FENCE,
			"%s ctx_%u get unused fd flag failed!", __func__, ctx->id);
		fput(sync_file->file);
		hvgr_ksync_destroy_dma_fence(ctx, fence);
		return -EINVAL;
	}

	fd_install((unsigned int)fd, sync_file->file);
	*new_fence = fence;

	return fd;
}

/**
 * Convert a sync fd to dma_fence object.
 * Return a dma_fence pointer if successful, else return NULL.
 */
struct dma_fence *hvgr_ksync_get_fence(struct hvgr_ctx * const ctx, int fd)
{
	return sync_file_get_fence(fd);
}

/**
 * Destory a dma_fence.
 * flag: 0: trigger, 1 wait.
 */
void hvgr_ksync_remove_fence(struct hvgr_ctx * const ctx, struct dma_fence * const fence,
	uint32_t token, uint32_t flag)
{
	hvgr_info(ctx->gdev, HVGR_FENCE, "%s ctx_%u, flag=%d, fence token=%u, remove\n",
		__func__, ctx->id, flag, token);
	dma_fence_put(fence);

	if (flag == WAIT_FENCE_FLAG) {
		atomic_dec(&ctx->ctx_cq.wait_uncomplte_cnt);
		atomic_inc(&ctx->ctx_cq.wait_complte_cnt);
	}
}

static uint32_t hvgr_ksync_create_fence(struct hvgr_ctx * const ctx,
	int *fd, struct dma_fence **fence)
{
	int new_fd;
	struct dma_fence *new_fence = NULL;

	new_fd = hvgr_ksync_export_fd(ctx, &new_fence);
	if (new_fd < 0) {
		hvgr_err(ctx->gdev, HVGR_FENCE,
			"Create fd fail when create fence ctx_%u", ctx->id);
		return -1;
	}

	*fd = new_fd;
	*fence = new_fence;

	return 0;
}

/**
 * Trigger a dma_fence.
 */
void hvgr_ksync_trigger_fence(struct hvgr_ctx * const ctx,
	struct dma_fence * const fence, int status, uint32_t token)
{
	if (status)
		dma_fence_set_error(fence, status);

	(void)dma_fence_signal(fence);
	atomic_dec(&ctx->ctx_cq.trigger_uncomplte_cnt);
	atomic_inc(&ctx->ctx_cq.trigger_complte_cnt);
	hvgr_info(ctx->gdev, HVGR_FENCE, "%s [fence_info] ctx_%u trigger fence token=%u, signaled.\n",
		__func__, ctx->id, token);
}

#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
static void hvgr_ksync_wait_fence_semp_record(struct hvgr_ctx * const ctx,
	uint64_t sem_addr, uint64_t sem_value)
{
	struct hvgr_cq_ctx_data *ctx_cq = &ctx->ctx_cq;
	uint32_t pos = ctx_cq->semp_record.pos;

	if(unlikely(pos >= HVGR_WAIT_FENCE_SEMP_RECORD_MAX)) {
		ctx_cq->semp_record.pos = 0U;
		hvgr_err(ctx->gdev, HVGR_FENCE, "%s pos overflow", __func__);
		return;
	}

	ctx_cq->semp_record.sem_addr[pos] = sem_addr;
	ctx_cq->semp_record.sem_value[pos] = sem_value;
	ctx_cq->semp_record.pos = (pos + 1U) % HVGR_WAIT_FENCE_SEMP_RECORD_MAX;
}

static void hvgr_ksync_wait_fence_submit(struct hvgr_ctx * const ctx,
	uint64_t sem_addr_set, uint64_t sem_value_set)
{
	unsigned long flags;
	unsigned long sch_flags;
	struct hvgr_cq_ctx_data *ctx_cq;
	struct hvgr_cq_ctx *cq_ctx = NULL;
	struct hvgr_cq_ctx *iter = NULL;
	struct hvgr_cq_ctx *tmp = NULL;
	struct hvgr_cq_dev *gcqctx;
	uint64_t sem_addr;

	gcqctx = &ctx->gdev->cq_dev;
	ctx_cq = &ctx->ctx_cq;

	spin_lock_irqsave(&gcqctx->schedule_lock, sch_flags);
	spin_lock_irqsave(&ctx_cq->wait_fence_lock, flags);
	hvgr_ksync_wait_fence_semp_record(ctx, sem_addr_set, sem_value_set);
	list_for_each_entry_safe(iter, tmp, &ctx_cq->fence_waiting_list, pending_item) {
		cq_ctx = iter;
		list_del(&cq_ctx->pending_item);
		hvgr_cq_mod_status_macro(cq_ctx, CQ_STATUS_IDLE);
		sem_addr = cq_ctx->wait_fence_sem_addr;
		cq_ctx->wait_fence_sem_addr = 0U;
		cq_ctx->wait_fence_sem_value = 0U;
		atomic_dec(&gcqctx->wait_fence_num);
		del_timer(&cq_ctx->in_fence_waiting_timer);
		(void)hvgr_cq_schedule_submit_without_lock(ctx, cq_ctx, true);
		hvgr_info(ctx->gdev, HVGR_SCH, "fence signal, ctx_%u, queueid_%u sem_0x%llx enqueue pending list.",
			cq_ctx->ctx->id, cq_ctx->queue_id, sem_addr);
	}
	spin_unlock_irqrestore(&ctx_cq->wait_fence_lock, flags);
	spin_unlock_irqrestore(&gcqctx->schedule_lock, sch_flags);
}
#endif

void hvgr_ksync_signal_wait_fence(struct hvgr_ctx * const ctx,
	uint64_t sem_addr, uint64_t sem_value, uint32_t token)
{
	hvgr_semaphore_set(ctx, sem_addr, sem_value);
#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
	hvgr_systrace_begin("hvgr_ksync_wait_fence_submit");
	hvgr_ksync_wait_fence_submit(ctx, sem_addr, sem_value);
	hvgr_systrace_end();
#endif
	hvgr_datan_jt_record_fence(ctx, token);
	hvgr_token_finish(ctx, token);
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
static void hvgr_ksync_wait_fence_callback_work(struct work_struct *data)
#else
static void hvgr_ksync_wait_fence_callback_work(struct kthread_work *data)
#endif
{
	struct token_callback_data *callback = container_of(data,
		struct token_callback_data, fence_callback_work);

	if (callback != NULL && callback->ctx != NULL) {
		hvgr_info(callback->ctx->gdev, HVGR_FENCE,
			"%s [fence_info] ctx_%u,token=%d has signaled.", __func__,
			callback->ctx->id, callback->cq_event.token);

		hvgr_ksync_signal_wait_fence(callback->ctx, callback->cq_event.sem_addr,
			callback->cq_event.sem_value, callback->cq_event.token);
	} else {
		pr_err("[HVGR_ERR] %s callback is null", __func__);
	}
}

static long hvgr_ksync_fence_callback_wq_init(struct hvgr_ctx *ctx)
{
	struct hvgr_cq_ctx_data *ctx_cq = &ctx->ctx_cq;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	ctx_cq->fence_callback_wq = alloc_workqueue("fence_callback_wq",
		WQ_MEM_RECLAIM | WQ_HIGHPRI | WQ_UNBOUND, 1);
	if (ctx_cq->fence_callback_wq == NULL) {
		hvgr_err(ctx->gdev, HVGR_CQ,
			"alloc workqueue fence_callback_wq fail.\n");
		return -EINVAL;
	}
#else
	ctx_cq->fence_callback_wq = kthread_create_worker(0, "fence_callback_wq");
	if (IS_ERR(ctx_cq->fence_callback_wq)) {
		hvgr_err(ctx->gdev, HVGR_CQ, "alloc workqueue fence_callback_wq fail.\n");
		return -EINVAL;
	}
	sched_set_fifo(ctx_cq->fence_callback_wq->task);
#endif

	return 0;
}

static void hvgr_ksync_fence_callback_wq_term(struct hvgr_ctx *ctx)
{
	struct hvgr_cq_ctx_data *ctx_cq = &ctx->ctx_cq;

	if (unlikely(ctx_cq->fence_callback_wq == NULL))
		return;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	destroy_workqueue(ctx_cq->fence_callback_wq);
#else
	kthread_destroy_worker(ctx_cq->fence_callback_wq);
#endif
	ctx_cq->fence_callback_wq = NULL;
}

void hvgr_fence_callback_create_work(struct hvgr_ctx *ctx,
	struct token_callback_data *callback)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	INIT_WORK(&callback->fence_callback_work, hvgr_ksync_wait_fence_callback_work);
	(void)queue_work(ctx->ctx_cq.fence_callback_wq, &callback->fence_callback_work);
#else
	kthread_init_work(&callback->fence_callback_work, hvgr_ksync_wait_fence_callback_work);
	(void)kthread_queue_work(ctx->ctx_cq.fence_callback_wq, &callback->fence_callback_work);
#endif
}

static void hvgr_ksync_wait_fence_callback(struct dma_fence *fence,
	struct dma_fence_cb *cb)
{
	struct token_callback_data *callback = container_of(cb,
		struct token_callback_data, fence_cb);

	hvgr_info(callback->ctx->gdev, HVGR_FENCE,
		"%s [fence_info] wait fence drv:%s timeline:%s seqno:%llu %d\n",
		__func__, fence->ops->get_driver_name(fence),
		fence->ops->get_timeline_name(fence),
		fence->seqno, (int)callback->ctx->tgid);

	hvgr_fence_callback_create_work(callback->ctx, callback);
}

long hvgr_ioctl_wait_fence(struct hvgr_ctx * const ctx,
	union hvgr_ioctl_para_wait_fence_fd * const para)
{
	union hvgr_ioctl_para_msync_fd fd_para;

	atomic_inc(&ctx->ctx_cq.wait_total_cnt);
	atomic_inc(&ctx->ctx_cq.wait_uncomplte_cnt);
	hvgr_info(ctx->gdev, HVGR_FENCE, "%s ctx_%u, in token=%d,fd=%d\n",
		__func__, ctx->id, para->in.token, para->in.fd);
	fd_para.in.token = para->in.token;
	fd_para.in.sem_addr = para->in.semaphore_addr;
	fd_para.in.sem_value = para->in.semaphore_value;
	fd_para.in.fd = para->in.fd;
	return hvgr_ioctl_cq_set_fd(ctx, &fd_para);
}

bool hvgr_trigger_fence_para_check(struct hvgr_ctx * const ctx,
	union hvgr_ioctl_para_trigger_fence_fd * const para)
{
	uint32_t token;
	uint64_t semp_addr_tmp = 0;

	token = para->in.token;
	if (!hvgr_token_is_valid(ctx, token))
		return false;

	if ((para->in.wait_token != CQ_TOKEN_INVALID) &&
		!hvgr_token_is_valid(ctx, para->in.wait_token)) {
		hvgr_err(ctx->gdev, HVGR_FENCE, "%s ctx_%u wait_token=%u not valid\n",
			__func__, ctx->id, para->in.wait_token);
		return false;
	}

	if (para->in.wait_token != CQ_TOKEN_INVALID) {
		semp_addr_tmp = hvgr_semaphore_get_addr(ctx, para->in.wait_semp_addr);
		if (semp_addr_tmp == 0) {
			hvgr_err(ctx->gdev, HVGR_FENCE,
				"%s ctx_%u semp_addr_tmp=0x%x not valid. \n",
				__func__, ctx->id, para->in.wait_semp_addr);
			return false;
		}
	}
	if (para->in.wait_token == para->in.token) {
		hvgr_err(ctx->gdev, HVGR_FENCE,
			"%s ctx_%u self dependency is not allowed. token = %u.\n",
			__func__, ctx->id, para->in.token);
		return false;
	}
	return true;
}

long hvgr_ioctl_trigger_fence(struct hvgr_ctx * const ctx,
	union hvgr_ioctl_para_trigger_fence_fd * const para)
{
	int fd;
	struct dma_fence *fence = NULL;
	struct hvgr_msync msync;
	uint32_t token;
	struct hvgr_msync dep_msync;
	long ret;

	if (!hvgr_trigger_fence_para_check(ctx, para)) {
		para->out.fd = -1;
		return -EINVAL;
	}

	mutex_lock(&ctx->ctx_lock);
	token = para->in.token;
	if (hvgr_ksync_create_fence(ctx, &fd, &fence) != 0) {
		hvgr_err(ctx->gdev, HVGR_FENCE,
			"%s create fence fail token=%u.\n", __func__, token);
		para->out.fd = -1;
		mutex_unlock(&ctx->ctx_lock);
		return -ENOMEM;
	}

	hvgr_info(ctx->gdev, HVGR_FENCE,
		"[fence_info] trig fence drv:%s timeline:%s seqno:%llu %d\n",
		fence->ops->get_driver_name(fence),
		fence->ops->get_timeline_name(fence),
		fence->seqno, (int)ctx->tgid);

	hvgr_info(ctx->gdev, HVGR_FENCE, "%s ctx_%u token=%d fd=%d wait_token=%d\n",
		__func__, ctx->id, para->in.token, fd, para->in.wait_token);

	atomic_inc(&ctx->ctx_cq.trigger_total_cnt);
	atomic_inc(&ctx->ctx_cq.trigger_uncomplte_cnt);

	msync.token = token;
	msync.sem_addr = 0;
	msync.sem_value = 0;
	msync.wait_token = para->in.wait_token;
	msync.check_wait_flag = true;
	ret = hvgr_token_register_trigger_fence(ctx, &msync, fence, para->in.ai_freq_enabled);
	if (ret == -EINVAL) {
		hvgr_ksync_remove_fence(ctx, fence, para->in.token, TRIGGER_FENCE_FLAG);
		para->out.fd = -1;
#ifndef CONFIG_LIBLINUX
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0)
		ksys_close((u32)fd);
#else
		sys_close((u32)fd);
#endif
#else
		__close_fd(NULL, (u32)fd);
#endif
		mutex_unlock(&ctx->ctx_lock);
		return ret;
	} else if (ret != 0) {
		goto exit;
	}

	dep_msync.token = para->in.wait_token;
	dep_msync.sem_addr = hvgr_semaphore_get_addr(ctx, para->in.wait_semp_addr);
	dep_msync.sem_value = para->in.wait_semp_value;
	if (hvgr_token_set_dependency(ctx, token, &dep_msync) != 0) {
		hvgr_err(ctx->gdev, HVGR_FENCE,
			"%s ctx_%u Set trigger fence dependency FAIL %x -> %x\n",
			__func__, ctx->id, token, dep_msync.token);

		hvgr_token_finish(ctx, token);
	}

	para->out.fd = fd;
	mutex_unlock(&ctx->ctx_lock);
	return 0;
exit:
	hvgr_ksync_trigger_fence(ctx, fence, 0, para->in.token);
	hvgr_ksync_remove_fence(ctx, fence, para->in.token, TRIGGER_FENCE_FLAG);

	hvgr_token_finish(ctx, token);
	para->out.fd = fd;
	mutex_unlock(&ctx->ctx_lock);
	return 0;
}

long hvgr_ksync_init(struct hvgr_ctx * const ctx)
{
	struct hvgr_cq_ctx_data *ctx_cq = &ctx->ctx_cq;

	ctx_cq->dma_fence_ctx = dma_fence_context_alloc(1);
	atomic64_set(&ctx_cq->dma_fence_seqno, 0);

	if (hvgr_ksync_fence_callback_wq_init(ctx) != 0)
		return -1;
	return 0;
}

void hvgr_ksync_term(struct hvgr_ctx * const ctx)
{
	hvgr_ksync_fence_callback_wq_term(ctx);
}

long hvgr_ioctl_cq_get_fd(struct hvgr_ctx * const ctx,
	union hvgr_ioctl_para_msync_fd * const para)
{
	int fd;
	struct dma_fence *fence = NULL;
	struct hvgr_msync msync;
	uint32_t token;
	long ret;

	if (unlikely((ctx == NULL) || (para == NULL)))
		return -EINVAL;

	token = para->in.token;
	if (!hvgr_token_is_valid(ctx, token))
		goto exit_err;

	msync.sem_addr = hvgr_semaphore_get_addr(ctx, para->in.sem_addr);
	if (msync.sem_addr == 0) {
		hvgr_err(ctx->gdev, HVGR_FENCE, "%s ctx_%u semp_addr_tmp=0x%x not valid. \n",
			__func__, ctx->id, para->in.sem_addr);
		goto exit_err;
	}

	mutex_lock(&ctx->ctx_lock);
	if (hvgr_ksync_create_fence(ctx, &fd, &fence) != 0) {
		hvgr_err(ctx->gdev, HVGR_FENCE, "Token %x create fence fail.\n", para->in.token);
		mutex_unlock(&ctx->ctx_lock);
		goto exit_err;
	}

	msync.sem_value = para->in.sem_value;
	msync.token = para->in.token;
	msync.fd = para->in.fd;
	msync.wait_token = 0;
	msync.check_wait_flag = false;
	ret = hvgr_token_register_trigger_fence(ctx, &msync, fence, para->in.ai_freq_enabled);
	if (ret != 0) {
		hvgr_debug(ctx->gdev, HVGR_FENCE,
			"[cq]: %s: ctx_%u: resigster fence callback FAIL. fd = %d",
			__func__, ctx->id, fd);
		if (ret != -EINVAL) {
			hvgr_ksync_trigger_fence(ctx, fence, 0, para->in.token);
			hvgr_token_finish(ctx, token);
		}
		hvgr_ksync_remove_fence(ctx, fence, para->in.token, TRIGGER_FENCE_FLAG);
		para->out.fd = -1;
#ifndef CONFIG_LIBLINUX
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0)
		ksys_close((u32)fd);
#else
		sys_close((u32)fd);
#endif
#else
		__close_fd(NULL, (u32)fd);
#endif
		mutex_unlock(&ctx->ctx_lock);
		return ret;
	}

	para->out.fd = fd;
	mutex_unlock(&ctx->ctx_lock);
	return 0;
exit_err:
	para->out.fd = -1;
	return -EINVAL;
}

long hvgr_ioctl_cq_set_fd(struct hvgr_ctx * const ctx,
	union hvgr_ioctl_para_msync_fd * const para)
{
	int fd;
	struct dma_fence *fence = NULL;
	struct hvgr_msync msync;
	long ret;

	if (unlikely((ctx == NULL) || (para == NULL) ||
		!hvgr_token_is_valid(ctx, para->in.token)))
		return -EINVAL;

	fd = para->in.fd;
	if (fd < 0) {
		hvgr_err(ctx->gdev, HVGR_FENCE,
			"[cq]: %s: ctx_%u: fd is INVALID",
			__func__, ctx->id);
		return -EINVAL;
	}

	mutex_lock(&ctx->ctx_lock);
	fence = hvgr_ksync_get_fence(ctx, fd);
	if (fence == NULL) {
		hvgr_err(ctx->gdev, HVGR_FENCE,
			"[cq]: %s: ctx_%u: fence is NULL. fd = %d",
			__func__, ctx->id, fd);
		mutex_unlock(&ctx->ctx_lock);
		return -EINVAL;
	}

	hvgr_info(ctx->gdev, HVGR_FENCE, "[fence_info] wait fence drv:%s timeline:%s seqno:%llu %d\n",
		fence->ops->get_driver_name(fence),
		fence->ops->get_timeline_name(fence),
		fence->seqno, (int)ctx->tgid);

	hvgr_info(ctx->gdev, HVGR_FENCE, "%s ctx_%u, in token=%u,fd=%d\n",
		__func__, ctx->id, para->in.token, para->in.fd);

	msync.token = para->in.token;
	msync.sem_addr = hvgr_semaphore_get_addr(ctx, para->in.sem_addr);
	if (msync.sem_addr == 0) {
		hvgr_ksync_remove_fence(ctx, fence, para->in.token, WAIT_FENCE_FLAG);
		mutex_unlock(&ctx->ctx_lock);
		return -EINVAL;
	}

	msync.sem_value = para->in.sem_value;
	msync.fd = para->in.fd;
	ret = hvgr_token_register_wait_fence(ctx, &msync, fence, hvgr_ksync_wait_fence_callback);
	if (ret != 0) {
		hvgr_debug(ctx->gdev, HVGR_FENCE, "%s ctx_%u, in token=%u,fd=%d fail ret %d\n",
			__func__, ctx->id, para->in.token, para->in.fd, ret);
		hvgr_ksync_remove_fence(ctx, fence, para->in.token, WAIT_FENCE_FLAG);
		if (ret != -EINVAL) {
			hvgr_semaphore_set(ctx, msync.sem_addr, msync.sem_value);
			hvgr_datan_jt_record_fence(ctx, msync.token);
			if (ret != -EPERM)
				hvgr_token_finish(ctx, msync.token);
		}
	}

	mutex_unlock(&ctx->ctx_lock);
	return ret;
}

long hvgr_ioctl_sc_update_policy(struct hvgr_ctx * const ctx,
	union hvgr_ioctl_sc_update_policy * const para)
{
	struct hvgr_cq_ctx_data *ctx_cq = NULL;
	struct hvgr_msync msync;
	uint32_t token;
	struct hvgr_msync dep_msync;
	long ret;

	token = para->in.token;
	if (!hvgr_token_is_valid(ctx, token))
		return -EINVAL;

	ctx_cq = &ctx->ctx_cq;

	hvgr_info(ctx->gdev, HVGR_CQ, "%s ctx_%u token=%d",
		__func__, ctx->id, para->in.token);
	msync.token = token;
	msync.sem_addr = hvgr_semaphore_get_addr(ctx, para->in.semaphore_addr);
	msync.sem_value = para->in.semaphore_value;
	ret = hvgr_token_register_sc_policy_callback(ctx, &msync, para->in.policy_info,
		para->in.info_num);
	if (ret != 0)
		return -EINVAL;

	if ((para->in.wait_token == CQ_TOKEN_INVALID) ||
		(hvgr_cq_is_queue_error(&ctx_cq->cq_ctx[CQ_PRIORITY_LOW]) ||
		hvgr_cq_is_queue_error(&ctx_cq->cq_ctx[CQ_PRIORITY_HIGH]))) {
		hvgr_semaphore_set(ctx, msync.sem_addr, msync.sem_value);
		hvgr_token_finish(ctx, token);
	} else {
		dep_msync.token = para->in.wait_token;
		dep_msync.sem_addr = hvgr_semaphore_get_addr(ctx, para->in.wait_semp_addr);
		dep_msync.sem_value = para->in.wait_semp_value;
		if (hvgr_token_set_dependency(ctx, token, &dep_msync) != 0) {
			hvgr_err(ctx->gdev, HVGR_CQ,
				"%s ctx_%u Set trigger fence dependency FAIL %x -> %x\n",
				__func__, ctx->id, token, dep_msync.token);

			hvgr_semaphore_set(ctx, msync.sem_addr, msync.sem_value);
			hvgr_token_finish(ctx, token);
		}
	}
	return 0;
}

long hvgr_ioctl_cq_set_top_notify(struct hvgr_ctx * const ctx,
	union hvgr_ioctl_para_top_notify * const para)
{
	uint32_t token;
	struct token_callback_data *callback = NULL;
	unsigned long flags;

	token = para->in.notify_token;
	if (!hvgr_token_is_valid(ctx, token)) {
		hvgr_err(ctx->gdev, HVGR_CQ,
			"%s token %d not right ", __func__, token);
		return -1;
	}

	hvgr_callback_data_alloc_and_init(ctx, token);
	spin_lock_irqsave(&ctx->ctx_cq.callback_data_lock[token], flags);
	callback = ctx->ctx_cq.token_callback[token];
	if (unlikely(callback == NULL)) {
		hvgr_err(ctx->gdev, HVGR_CQ,
			"%s notify_token token=%d init failed", __func__, token);
		spin_unlock_irqrestore(&ctx->ctx_cq.callback_data_lock[token], flags);
		return -1;
	}
	spin_unlock_irqrestore(&ctx->ctx_cq.callback_data_lock[token], flags);

	return 0;
}
