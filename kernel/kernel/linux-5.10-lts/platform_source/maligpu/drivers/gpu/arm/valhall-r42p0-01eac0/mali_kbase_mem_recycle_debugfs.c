/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2025. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include <mali_kbase.h>
#include "mali_kbase_mem_recycle_debugfs.h"
#include <linux/seq_file.h>
#ifdef CONFIG_DFX_DEBUG_FS
#include <linux/debugfs.h>
#endif
#include <securec.h>

#ifdef CONFIG_MALI_GPU_RECLAIM
void kbase_switch_prev_ktime_init(ktime_t *begin)
{
	*begin = ktime_get();
}

uint32_t kbase_switch_get_diff_and_update_ktime(ktime_t *begin)
{
	uint32_t diff;
	ktime_t now = ktime_get();

	diff = (uint32_t)ktime_to_us(ktime_sub(now, *begin));
	*begin = now;

	return diff;
}

void kbase_switch_fill_fsnode(struct kbase_switch_fs *dst, struct kbase_switch_fs *src)
{
	dst->same_va_total_pages = src->same_va_total_pages;
	dst->real_swaped_pages = src->real_swaped_pages;
	dst->tgid = src->tgid;
	dst->id = src->id;
	dst->status = src->status;
	(void)memcpy_s(dst->status_str, sizeof(dst->status_str),
		src->status_str, sizeof(src->status_str));
	(void)memcpy_s(dst->diff_time, sizeof(dst->diff_time),
		src->diff_time, sizeof(src->diff_time));
}

void kbase_switch_list_add_fsinfo(struct kbase_context * const ctx, struct kbase_switch_fs *fs)
{
	struct kbase_switch_fs *fs_node = NULL;
	struct kbase_switch_fs *head = NULL;
	struct kbase_device *gdev = ctx->kbdev;

	fs_node = kvzalloc(sizeof(*fs_node), GFP_KERNEL);
	if (unlikely(fs_node == NULL)) {
		dev_err(ctx->kbdev->dev, "[ctx_sw]alloc fs node failed, tgid:%d", ctx->tgid);
		return;
	}

	kbase_switch_fill_fsnode(fs_node, fs);

	mutex_lock(&gdev->switch_fs_mutex);
	if (gdev->count > MAX_RECORD_COUNT) {
		head = list_first_entry(&gdev->switch_fs_list, struct kbase_switch_fs, node);
		list_del(&head->node);
	} else {
		gdev->count++;
	}
	list_add_tail(&fs_node->node, &gdev->switch_fs_list);
	mutex_unlock(&gdev->switch_fs_mutex);
}

void kbase_switch_update_front_info(struct kbase_context * const ctx, struct kbase_switch_fs *fs)
{
	struct kbase_device *gdev = ctx->kbdev;
	struct kbase_switch_fs *iter = NULL;
	struct kbase_switch_fs *tmp = NULL;
	enum recycle_step i;

	mutex_lock(&gdev->switch_fs_mutex);
	list_for_each_entry_safe(iter, tmp, &gdev->switch_fs_list, node) {
		if (iter->tgid == fs->tgid) {
			for (i = FG_DATA_CHECK; i < MAX_STEP; i++)
				iter->diff_time[i] = fs->diff_time[i];
			(void)memset_s(iter->status_str, sizeof(iter->status_str), 0,
				sizeof(iter->status_str));
			(void)memcpy_s(iter->status_str, sizeof(iter->status_str),
				fs->status_str, strlen(fs->status_str));
			iter->status = fs->status;
			break;
		}
	}
	mutex_unlock(&gdev->switch_fs_mutex);
}

void kbase_switch_fill_in_used_time(struct kbase_switch_fs *info, enum recycle_step step,
	ktime_t *begin)
{
	uint32_t diff = kbase_switch_get_diff_and_update_ktime(begin);

	info->diff_time[step] = diff;
}

void kbase_switch_update_stat_info(struct kbase_switch_fs *info, enum context_swtich_status sts,
	char *str)
{
	info->status = sts;
	(void)memset_s(info->status_str, sizeof(info->status_str), 0, sizeof(info->status_str));
	(void)memcpy_s(info->status_str, sizeof(info->status_str), str, strlen(str));
}

#ifdef CONFIG_DFX_DEBUG_FS
static int kbase_recycle_info_debugfs_show(struct seq_file *file, void *data)
{
	struct kbase_device *kbdev = file->private;
	struct kbase_switch_fs *iter = NULL;
	struct kbase_switch_fs *tmp = NULL;
	ssize_t ret = 0;
	uint32_t to_back_total_times;
	uint32_t to_front_total_times;

	mutex_lock(&kbdev->switch_fs_mutex);
	list_for_each_entry_safe(iter, tmp, &kbdev->switch_fs_list, node) {
		to_back_total_times = 0;
		to_front_total_times = 0;
		seq_printf(file,
			"kbase-ctx_%u, pid : %u, sameva all pages : %u swaped pages: %u\n",
			iter->id, iter->tgid, iter->same_va_total_pages, iter->real_swaped_pages);
		seq_printf(file, "status : %u, reason: %s\n", iter->status, iter->status_str);
		to_back_total_times = iter->diff_time[BG_DATA_CHECK] + iter->diff_time[CLEAN_POOL_CYCLE] +
			iter->diff_time[TAKE_DOWN_PAGE] + iter->diff_time[ARRAY_OUT_PAGE] + iter->diff_time[FREE_PAGE];
		seq_printf(file,
			"BG Data check:%u us, clean pool:%u us, take down page:%u us, array out:%u us, free page:%u us, total:%u us.\n",
			iter->diff_time[BG_DATA_CHECK], iter->diff_time[CLEAN_POOL_CYCLE], iter->diff_time[TAKE_DOWN_PAGE],
			iter->diff_time[ARRAY_OUT_PAGE], iter->diff_time[FREE_PAGE], to_back_total_times);
		to_front_total_times = iter->diff_time[FG_DATA_CHECK] + iter->diff_time[SETUP_ALLOC_PAGE] +
			iter->diff_time[ARRAY_IN_PAGE];
		seq_printf(file,
			"FG Data check:%u us, step up page:%u us, array in:%u us, total:%u us\n", iter->diff_time[FG_DATA_CHECK],
			iter->diff_time[SETUP_ALLOC_PAGE], iter->diff_time[ARRAY_IN_PAGE], to_front_total_times);
	}
	mutex_unlock(&kbdev->switch_fs_mutex);
	return ret;	
}

static int kbase_recycle_info_debugfs_open(struct inode *in, struct file *file)
{
	return single_open(file, kbase_recycle_info_debugfs_show,
			   in->i_private);
}

static const struct file_operations kbase_recycle_info_fops = {
	.open = kbase_recycle_info_debugfs_open,
	.read = seq_read,
	.release = single_release,
};

void kbase_recycle_info_init(struct kbase_device *kbdev)
{
	struct dentry *file;
	const mode_t mode = 0444;

	if (WARN_ON(!kbdev || IS_ERR_OR_NULL(kbdev->mali_debugfs_directory)))
		return;

	file = debugfs_create_file("recycle_info", mode,
				   kbdev->mali_debugfs_directory, kbdev,
				   &kbase_recycle_info_fops);
	if (IS_ERR_OR_NULL(file)) {
		dev_warn(kbdev->dev,
			 "Unable to create dvfs debugfs entry");
	}
}
#else
void kbase_recycle_info_init(struct kbase_device *kbdev)
{
}
#endif
#endif
