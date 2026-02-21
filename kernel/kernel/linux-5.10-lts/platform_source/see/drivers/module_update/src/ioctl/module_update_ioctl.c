/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: module uapp provide service for user via ioctl
 * Create: 2022/01/18
 */
#include <platform_include/see/module_update.h>
#include <module_update_by_tee.h>
#include <module_update_errno.h>
#include <module_update_plat.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/completion.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <securec.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
#include <linux/compat.h>
#endif

#define MODULE_UPDATE_QUEUE_NAME                        "module_update_queue"
#define MODULE_UPDATE_BINDFILE_DATA_ELEMENT_NUM         (sizeof(struct module_update_bindfile) / sizeof(u64))
#define MODULE_UPDATE_HASH_DATA_ELEMENT_NUM             (sizeof(struct module_update_img_hash) / sizeof(u64))

struct module_update_work_ctx {
	s32 result;
	u64 *paras;
	size_t size;
	struct completion completion;
	struct work_struct work;
	struct workqueue_struct *queue;
};

typedef void (*module_update_work_fn)(struct work_struct *work);

static s32 module_update_work_ctx_init(struct module_update_work_ctx *ctx,
	u64 *paras, size_t size, module_update_work_fn fn)
{
	struct workqueue_struct *queue = NULL;

	queue = create_singlethread_workqueue(MODULE_UPDATE_QUEUE_NAME);
	if (IS_ERR_OR_NULL(queue)) {
		module_update_err("create_workqueue error\n");
		return -EFAULT;
	}

	ctx->queue = queue;
	ctx->paras = paras;
	ctx->size = size;
	INIT_WORK(&ctx->work, fn);
	init_completion(&ctx->completion);

	return 0;
}

static void module_secinfo_work_ctx_deinit(struct module_update_work_ctx *ctx)
{
	destroy_workqueue(ctx->queue);
	ctx->queue = NULL;
}

static void module_update_verify_hash_work(struct work_struct *work)
{
	u32 ret;
	struct module_update_work_ctx *ctx = NULL;

	ctx = container_of(work, struct module_update_work_ctx, work);
	if (ctx->paras == NULL || ctx->size != MODULE_UPDATE_HASH_DATA_ELEMENT_NUM) {
		module_update_err("error, paras size=%u\n", ctx->size);
		ctx->result = MODULE_UPDATE_ERR_NULL_POINTER;
		complete(&ctx->completion);
		return;
	}
	struct module_update_img_hash *img_hash = (struct module_update_img_hash *)(ctx->paras);
	ret = module_update_verify_hash_by_tee(img_hash->img_name, (uint32_t)img_hash->img_name_bytes,
		img_hash->hash, (uint32_t)img_hash->hash_bytes);

	module_update_err("exit, module update ret=0x%x!\n", ret);
	ctx->result = (s32)ret;
	complete(&ctx->completion);
}

static s32 module_update_verify_hash_by_queue(struct module_update_img_hash *data)
{
	s32 ret;
	struct module_update_work_ctx ctx = {0};
	size_t len = MODULE_UPDATE_HASH_DATA_ELEMENT_NUM;
	ret = module_update_work_ctx_init(&ctx, (u64 *)data, len, module_update_verify_hash_work);
	if (ret != 0) {
		module_update_err("error, init module update work %d!\n", ret);
		return MODULE_UPDATE_WORK_CTX_INIT_FAIL;
	}

	queue_work(ctx.queue, &ctx.work);
	wait_for_completion(&ctx.completion);
	ret = ctx.result;

	module_secinfo_work_ctx_deinit(&ctx);
	module_update_err("exit, ret=0x%x\n", ret);
	return ret;
}

static void module_update_verify_bindfile_work(struct work_struct *work)
{
	u32 ret;
	struct module_update_work_ctx *ctx = NULL;

	ctx = container_of(work, struct module_update_work_ctx, work);
	if (ctx->paras == NULL || ctx->size != MODULE_UPDATE_BINDFILE_DATA_ELEMENT_NUM) {
		module_update_err("error, paras size=%u\n", ctx->size);
		ctx->result = MODULE_UPDATE_ERR_NULL_POINTER;
		complete(&ctx->completion);
		return;
	}
	struct module_update_bindfile *bindfile = (struct module_update_bindfile *)(ctx->paras);
	uint8_t *data = bindfile->data;
	size_t size = (uint32_t)bindfile->data_bytes;
	ret = module_update_verify_bindfile_by_tee(data, size);

	module_update_err("exit, module update ret=0x%x!\n", ret);
	ctx->result = (s32)ret;
	complete(&ctx->completion);
}

static s32 module_update_verify_bindfile_by_queue(struct module_update_bindfile *data)
{
	s32 ret;
	struct module_update_work_ctx ctx = {0};
	size_t len = MODULE_UPDATE_BINDFILE_DATA_ELEMENT_NUM;
	ret = module_update_work_ctx_init(&ctx, (u64 *)data, len, module_update_verify_bindfile_work);
	if (ret != 0) {
		module_update_err("error, init module update work %d!\n", ret);
		return MODULE_UPDATE_WORK_CTX_INIT_FAIL;
	}

	queue_work(ctx.queue, &ctx.work);
	wait_for_completion(&ctx.completion);
	ret = ctx.result;

	module_secinfo_work_ctx_deinit(&ctx);
	module_update_err("exit, ret=0x%x\n", ret);
	return ret;
}

s32 module_update_verify_hash(u32 cmd, uintptr_t arg)
{
	s32 ret;
	void __user *argp = (void __user *)arg;
	uint32_t size_in_cmd = _IOC_SIZE(cmd);

	/* if declare and define argp, static check warning */
	if (!argp) {
		module_update_err("error, argp is NULL\n");
		return MODULE_UPDATE_PTR_NULL;
	}
	if (size_in_cmd != sizeof(struct module_update_img_hash)) {
		module_update_err("error, size_in_cmd=0x%08x\n", size_in_cmd);
		return MODULE_UPDATE_IO_SIZE_NOT_MATCH;
	}

	struct module_update_img_hash data = {0};
	ret = (s32)copy_from_user(&data, argp, sizeof(struct module_update_img_hash));
	if (ret != 0) {
		module_update_err("error, copy_from uapp file size ret=0x%08X\n", ret);
		return MODULE_UPDATE_COPY_FROM_USER_FAIL;
	}
	if (data.img_name_bytes > IMG_NAME_MAX_BYTES || data.hash_bytes > IMG_HASH_MAX_BYTES) {
		module_update_err("error, module update img hash size invalid\n");
		return MODULE_UPDATE_DATA_SIZE_EXCEED;
	}

	ret = module_update_verify_hash_by_queue(&data);
	if (ret != MODULE_UPDATE_OK) {
		module_update_err("error 0x%x, get uapp bindfile info\n", ret);
		return ret;
	}
	return 0;
}

s32 module_update_verify_bindfile(u32 cmd, uintptr_t arg)
{
	s32 ret;
	void __user *argp = (void __user *)arg;
	uint32_t size_in_cmd = _IOC_SIZE(cmd);

	/* if declare and define argp, static check warning */
	if (!argp) {
		module_update_err("error, argp is NULL\n");
		return MODULE_UPDATE_PTR_NULL;
	}

	if (size_in_cmd != sizeof(struct module_update_bindfile)) {
		module_update_err("error, size_in_cmd=0x%08x\n", size_in_cmd);
		return MODULE_UPDATE_IO_SIZE_NOT_MATCH;
	}

	struct module_update_bindfile *data =
		(struct module_update_bindfile *)vmalloc(sizeof(struct module_update_bindfile));
	if (data == NULL) {
		module_update_err("error, alloc uapp file data fail\n");
		return MODULE_UPDATE_PTR_NULL;
	}
	ret = (s32)copy_from_user(data, argp, sizeof(struct module_update_bindfile));
	if (ret != 0) {
		vfree(data);
		module_update_err("error, copy_from uapp file size ret=0x%08X\n", ret);
		return MODULE_UPDATE_COPY_FROM_USER_FAIL;
	}
	if (data->data_bytes >  MODULE_UPDATE_BINDFILE_MAX_BYTES) {
		vfree(data);
		module_update_err("error, module update bindfile size invalid\n");
		return MODULE_UPDATE_DATA_SIZE_EXCEED;
	}

	ret = module_update_verify_bindfile_by_queue(data);
	if (ret != MODULE_UPDATE_OK) {
		vfree(data);
		module_update_err("error 0x%x, get uapp bindfile info\n", ret);
		return ret;
	}
	vfree(data);
	return 0;
}
