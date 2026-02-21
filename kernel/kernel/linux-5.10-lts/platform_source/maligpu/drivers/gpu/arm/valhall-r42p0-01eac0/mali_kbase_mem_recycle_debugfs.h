/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2025. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef KBASE_MEM_RECYCLE_DEBUGFS_H
#define KBASE_MEM_RECYCLE_DEBUGFS_H

#include "mali_kbase_defs.h"

#define MAX_RECORD_COUNT 10

#ifdef CONFIG_MALI_GPU_RECLAIM
void kbase_switch_prev_ktime_init(ktime_t *begin);

uint32_t kbase_switch_get_diff_and_update_ktime(ktime_t *begin);

void kbase_switch_list_add_fsinfo(struct kbase_context * const ctx, struct kbase_switch_fs *fs);

void kbase_switch_update_front_info(struct kbase_context * const ctx, struct kbase_switch_fs *fs);

void kbase_switch_fill_in_used_time(struct kbase_switch_fs *info, enum recycle_step step,
	ktime_t *begin);

void kbase_switch_update_stat_info(struct kbase_switch_fs *info, enum context_swtich_status sts,
	char *str);

void kbase_recycle_info_init(struct kbase_device *kbdev);
#endif

#endif /* KBASE_MEM_RECYCLE_DEBUGFS_H */
