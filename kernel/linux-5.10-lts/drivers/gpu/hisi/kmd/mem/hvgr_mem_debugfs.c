/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_mem_debugfs.h"

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/seq_file.h>
#include <securec.h>

#include "hvgr_debugfs_api.h"
#include "hvgr_dm_api.h"
#include "hvgr_log_api.h"


#define HVGR_MEM_PROFILE_MAX_SIZE ((64 + ((80 + (56 * 64)) * 3 * 15) + (56 * 6)))

static uint64_t hvgr_mem_get_jit_total_pages(struct hvgr_ctx *ctx)
{
	struct hvgr_mem_area *iter = NULL;
	struct hvgr_mem_area *temp = NULL;
	uint64_t jit_pages = 0;

	mutex_lock(&ctx->mem_ctx.jit_shrink_mutex);
	list_for_each_entry_safe(iter, temp, &ctx->mem_ctx.jit_pool_head, jit_node)
		jit_pages += iter->pages;

	list_for_each_entry_safe(iter, temp, &ctx->mem_ctx.jit_using_head, jit_node)
		jit_pages += iter->pages;
	mutex_unlock(&ctx->mem_ctx.jit_shrink_mutex);

	return jit_pages;
}

struct hvgr_mem_usage_stat {
	uint64_t summary;
	uint64_t grow;
	uint64_t pt;
	uint64_t kmd;
	uint64_t jit;
};

static void hvgr_mem_get_used_memory_statistic(struct hvgr_ctx *ctx,
	struct hvgr_mem_usage_stat *used)
{
	used->summary = ((uint64_t)atomic_read(&(ctx->mem_ctx.used_pages))) * PAGE_SIZE;
	used->grow = ((uint64_t)atomic_read(&(ctx->mem_ctx.used_grow_pages))) * PAGE_SIZE;
	used->pt = ((uint64_t)atomic_read(&(ctx->mem_ctx.used_pt_pages))) * PAGE_SIZE;
	used->kmd = ((uint64_t)atomic_read(&(ctx->mem_ctx.used_kmd_pages))) * PAGE_SIZE;
	used->jit = hvgr_mem_get_jit_total_pages(ctx) * PAGE_SIZE;
}


long hvgr_mem_add_profile(struct hvgr_ctx * const ctx, uint64_t user_buf, uint64_t size)
{
	char *kbuf = NULL;

	if (unlikely(user_buf == 0 || size == 0 || size > HVGR_MEM_PROFILE_MAX_SIZE)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "mem profile size %lu addr 0x%lx error",
			size, user_buf);
		return -EINVAL;
	}

	mutex_lock(&ctx->mem_ctx.profile_mutex);

	kbuf = kvzalloc(size, GFP_KERNEL);
	if (kbuf == NULL) {
		mutex_unlock(&ctx->mem_ctx.profile_mutex);
		hvgr_err(ctx->gdev, HVGR_MEM, "mem profile malloc fail");
		return -ENOMEM;
	}

	if (copy_from_user(kbuf, u64_to_user_ptr(user_buf), size) != 0) {
		mutex_unlock(&ctx->mem_ctx.profile_mutex);
		hvgr_err(ctx->gdev, HVGR_MEM, "mem profile copy from user fail");
		kvfree(kbuf);
		return -EFAULT;
	}


	if (ctx->mem_profile_buf != NULL)
		kvfree(ctx->mem_profile_buf);

	ctx->mem_profile_buf = kbuf;
	ctx->mem_profile_size = size;

	mutex_unlock(&ctx->mem_ctx.profile_mutex);
	return 0;
}

void hvgr_mem_remove_profile(struct hvgr_ctx * const ctx)
{
	mutex_lock(&ctx->mem_ctx.profile_mutex);

	if (ctx->mem_profile_buf != NULL)
		kvfree(ctx->mem_profile_buf);

	ctx->mem_profile_buf = NULL;
	ctx->mem_profile_size = 0;
	mutex_unlock(&ctx->mem_ctx.profile_mutex);
}

static void hvgr_mem_profile_kmsg(struct hvgr_ctx *ctx, struct hvgr_mem_usage_stat *used)
{
	char *buff = kvzalloc(ctx->mem_profile_size + 1, GFP_KERNEL);
	if (buff == NULL)
		return;

	pr_err("ctx_%u %10u %10u used summary:%llu grow:%llu driver:%llu kmd:%llu jit:%llu\n",
		ctx->id, ctx->pid, ctx->tgid,
		used->summary, used->grow, used->pt + used->kmd, used->kmd, used->jit);

	if (ctx->mem_profile_buf) {
		(void)memcpy_s(buff, ctx->mem_profile_size,
			ctx->mem_profile_buf, ctx->mem_profile_size);
		pr_err("%s\n", buff);
	}

	kvfree(buff);
}


int hvgr_mem_profile_all_show(struct seq_file *sfile, void *data)
{
	struct hvgr_device *dev = hvgr_get_device();
	struct hvgr_ctx *tmp = NULL;
	struct hvgr_ctx *ctx = NULL;
	struct hvgr_pool *pool = NULL;
	struct hvgr_mem_dev *mem_dev = NULL;
	struct hvgr_mem_usage_stat used = {0};
	uint32_t pool_pages = 0;
	uint32_t dev_pool_pages = 0;
	uint32_t i;

	if (unlikely(dev == NULL))
		return -EINVAL;

	mutex_lock(&dev->dm_dev.ctx_list_mutex);
	list_for_each_entry_safe(ctx, tmp, &dev->dm_dev.ctx_list, dev_link) {
		mutex_lock(&ctx->mem_ctx.profile_mutex);
		hvgr_mem_get_used_memory_statistic(ctx, &used);
		if (sfile != NULL) {
			seq_printf(sfile,
				"ctx_%u %10u %10u used summary:%llu grow:%llu "
				"driver:%llu kmd:%llu jit:%llu\n",
				ctx->id, ctx->pid, ctx->tgid, used.summary, used.grow,
				used.pt + used.kmd, used.kmd, used.jit);
			if (ctx->mem_profile_buf) {
				seq_write(sfile, ctx->mem_profile_buf,
					(size_t)ctx->mem_profile_size);
				seq_printf(sfile, "%s", "\n");
			}
		} else {
			hvgr_mem_profile_kmsg(ctx, &used);
		}
		mutex_unlock(&ctx->mem_ctx.profile_mutex);
		for (i = 0; i < MEM_POOL_NUM; i++) {
			pool = &ctx->mem_ctx.mem_pool[i].pool;
			pool_pages += pool->cur_size;
		}

	}
	mutex_unlock(&dev->dm_dev.ctx_list_mutex);

	mem_dev = &dev->mem_dev;
	for (i = 0; i < MEM_POOL_NUM; i++) {
		pool = &mem_dev->mem_pool[i].pool;
		dev_pool_pages += pool->cur_size;
	}

	if (sfile != NULL)
		seq_printf(sfile, "pool pages:%u %u\n", dev_pool_pages, pool_pages);
	else
		pr_err("pool pages:%u %u\n", dev_pool_pages, pool_pages);

	return 0;
}


void hvgr_mem_debugfs_init(struct hvgr_device * const gdev)
{
}

void hvgr_switch_prev_ktime_init(ktime_t *begin)
{
}

uint32_t hvgr_switch_get_diff_and_update_ktime(ktime_t *begin)
{
	return 0;
}

void hvgr_switch_fill_fsnode(struct hvgr_switch_fs *dst, struct hvgr_switch_fs *src)
{
}

void hvgr_switch_list_add_fsinfo(struct hvgr_ctx * const ctx, struct hvgr_switch_fs *fs)
{
}

void hvgr_switch_update_front_info(struct hvgr_ctx * const ctx, struct hvgr_switch_fs *fs)
{
}

void hvgr_switch_fill_in_used_time(struct hvgr_switch_fs *info, enum recycle_step step,
	ktime_t *begin)
{
}

void hvgr_switch_fill_in_page_info(uint32_t num, struct hvgr_switch_fs *info, struct hvgr_ctx *ctx)
{
}

void hvgr_switch_update_stat_info(struct hvgr_switch_fs *info, enum context_swtich_status sts,
	char *str)
{
}
