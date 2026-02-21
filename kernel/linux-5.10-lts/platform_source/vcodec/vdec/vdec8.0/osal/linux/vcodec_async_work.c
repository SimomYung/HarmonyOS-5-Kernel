#ifdef CONFIG_VCODEC_OHOS
#include <trace/hooks/liblinux.h>
#endif
#include "dbg.h"
#include "vcodec_platform_osal.h"
#include "vcodec_event_queue.h"

#define VCODEC_WORK_NORMAL 0
#define VCODEC_WORK_DETACH 1

struct vcodec_async_work_node {
	int32_t result;
	uint32_t detach_flag;
	struct completion completion;
	int32_t (*proc)(void *);
	void *arg;
};

struct vcodec_fifo_task
{
	spinlock_t *lock;
	vcodec_event event;
	DECLARE_KFIFO_PTR(fifo, struct vcodec_async_work_node *);
};

struct vcodec_async_work_ctx {
	struct vcodec_fifo_task *task_fifo;
	struct task_struct *task;
};

static int vcodec_async_task_proc(void *data)
{
	int ret;
	struct vcodec_async_work_ctx *ctx = (struct vcodec_async_work_ctx *)data;
	struct vcodec_async_work_node *work_node = NULL;	

	dprint(PRN_ALWS, "enter vcodec async task");
	while (!kthread_should_stop()) {
		dprint(PRN_ALWS, "before wait event interruptible");
		ret = wait_event_interruptible(ctx->task_fifo->event.queue_head,
			!queue_is_empty(ctx->task_fifo) || kthread_should_stop());
		if (ret) {
			dprint(PRN_ERROR, "wait interrupt...");
			continue;
		}

		if (!queue_is_empty(ctx->task_fifo)) {
			ret = pop(ctx->task_fifo, &work_node);
			if (ret) {
				dprint(PRN_ERROR, "pop work node fail...");
				continue;
			}

			ret = work_node->proc(work_node->arg);
			if (work_node->detach_flag) {
				kfree(work_node);
			} else {
				work_node->result = ret;
				complete(&work_node->completion);
			}
			work_node = NULL;
		}
	}

	dprint(PRN_ALWS, "exit vcodec async task");
	return 0;
}

void *vcodec_work_ctx_create(const char *name)
{
	struct vcodec_async_work_ctx *ctx = kzalloc(sizeof(struct vcodec_async_work_ctx), GFP_KERNEL);
	if (ctx) {
		ctx->task_fifo = create_queue(struct vcodec_fifo_task);
		if (IS_ERR_OR_NULL(ctx->task_fifo)) {
			dprint(PRN_ERROR, "create work queue fail");
			kfree(ctx);
			return NULL;
		}
		if (alloc_queue(ctx->task_fifo, 8)) {
			dprint(PRN_ERROR, "alloc work queue fail");
			destroy_queue(ctx->task_fifo);
			kfree(ctx);
			return NULL;
		}

		ctx->task = kthread_run(vcodec_async_task_proc, ctx, name);
		if (IS_ERR(ctx->task)) {
			dprint(PRN_ERROR, "start work thread fail");
			free_queue(ctx->task_fifo);
			destroy_queue(ctx->task_fifo);
			kfree(ctx);
			return NULL;
		}
#ifdef CONFIG_VCODEC_OHOS
		trace_ldk_rvh_set_vip_prio(ctx->task, 1); /* prio 41 */
#endif
	}

	return ctx;
}

void vcodec_work_ctx_destroy(void *ctx)
{
	struct vcodec_async_work_ctx *work_ctx = NULL;

	if (!ctx)
		return;

	work_ctx = (struct vcodec_async_work_ctx *)ctx;
	if (!IS_ERR_OR_NULL(work_ctx->task_fifo)) {
		kthread_stop(work_ctx->task);
		free_queue(work_ctx->task_fifo);
		destroy_queue(work_ctx->task_fifo);
	}
	kfree(ctx);
}

static void *vcodec_work_submit_internal(void *ctx, int32_t (*proc)(void *),
	void *arg, uint32_t detach, uint32_t gfp_flags)
{
	struct vcodec_async_work_node *node = NULL;
	struct vcodec_async_work_ctx *work_ctx = (struct vcodec_async_work_ctx *)ctx;

	if (unlikely(!work_ctx || !work_ctx->task_fifo || !proc))
		return NULL;

	node = kzalloc(sizeof(struct vcodec_async_work_node), gfp_flags);
	if (!node)
		return NULL;

	node->proc = proc;
	node->arg = arg;
	node->detach_flag = detach;
	init_completion(&node->completion);

	if (push(work_ctx->task_fifo, &node)) {
		kfree(node);
		return NULL;
	}

	vcodec_give_event(&work_ctx->task_fifo->event);
	return node;
}

void *vcodec_work_submit(void *ctx, int32_t (*proc)(void *), void *arg)
{
	return vcodec_work_submit_internal(ctx, proc, arg, VCODEC_WORK_NORMAL, GFP_KERNEL);
}

int32_t vcodec_work_submit_detach(void *ctx, int32_t (*proc)(void *), void *arg)
{
	if (!vcodec_work_submit_internal(ctx, proc, arg, VCODEC_WORK_DETACH, GFP_KERNEL))
		return -EFAULT;

	return 0;
}

int32_t vcodec_work_submit_irq(void *ctx, int32_t (*proc)(void *), void *arg)
{
	if (!vcodec_work_submit_internal(ctx, proc, arg, VCODEC_WORK_DETACH, GFP_KERNEL|GFP_ATOMIC))
		return -EFAULT;

	return 0;
}

int32_t vcodec_work_finish(void *node)
{
	int32_t ret;
	struct vcodec_async_work_node *work_node = (struct vcodec_async_work_node *)node;

	if (unlikely(!work_node || work_node->detach_flag))
		return -EINVAL;
	wait_for_completion(&work_node->completion);
	ret = work_node->result;
	kfree(node);

	return ret;
}
