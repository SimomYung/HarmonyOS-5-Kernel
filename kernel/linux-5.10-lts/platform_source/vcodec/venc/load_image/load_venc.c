/*
 *  Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 *  Description: kernel CA bsp
 *  Create : 2023/12/27
 */
#include "load_venc.h"
#include <linux/device.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/completion.h>
#include <platform_include/see/efuse_driver.h>
#include <platform_include/see/sec_auth_ca.h>

#define SEC_AUTH_BSP_QUEUE_NAME         "venc_sec_auth_tc"

struct bsp_work_ctx {
	int result;
	void *image_buffer;
	size_t image_size;
	struct completion completion;
	struct work_struct work;
	struct workqueue_struct *queue;
};

static void bsp_work(struct work_struct *work)
{
	int ret;
	struct bsp_work_ctx *ctx = NULL;

	ctx = container_of(work, struct bsp_work_ctx, work);
	ret = load_verify_venc_image(ctx->image_buffer, ctx->image_size);
	pr_err("[%s]exit, ret=0x%x!\n", __func__, ret);
	ctx->result = ret;
	complete(&ctx->completion);
}

static int bsp_work_ctx_init(struct bsp_work_ctx *ctx)
{
	struct workqueue_struct *queue = NULL;

	queue = create_singlethread_workqueue(SEC_AUTH_BSP_QUEUE_NAME);
	if (IS_ERR_OR_NULL(queue)) {
		pr_err("%s, create_workqueue error\n", __func__);
		return -EFAULT;
	}

	ctx->queue = queue;
	INIT_WORK(&ctx->work, bsp_work);
	init_completion(&ctx->completion);

	return 0;
}

static void bsp_work_ctx_deinit(struct bsp_work_ctx *ctx)
{
	destroy_workqueue(ctx->queue);
}

static int venc_image_buffer_validate(void *image_buffer, size_t image_size)
{
	if (!image_buffer)
		return -EINVAL;

	if (image_size <= SEC_AUTH_CERT_SIZE)
		return -EINVAL;

	return 0;
}

int load_venc_image(void *image_buffer, size_t image_size)
{
	int ret;
	struct bsp_work_ctx ctx = {0};

	pr_err("enter %s\n", __func__);

	ret = venc_image_buffer_validate(image_buffer, image_size);
	if (ret) {
		pr_err("error, invalid image buffer %d!\n", ret);
		return ret;
	}
	ctx.image_buffer = image_buffer;
	ctx.image_size = image_size;
	ret = bsp_work_ctx_init(&ctx);
	if (ret) {
		pr_err("error, init bsp work %d!\n", ret);
		return ret;
	}

	queue_work(ctx.queue, &ctx.work);
	pr_err("wait bsp work complete\n");
	wait_for_completion(&ctx.completion);

	ret = ctx.result;

	bsp_work_ctx_deinit(&ctx);

	return ret;
}