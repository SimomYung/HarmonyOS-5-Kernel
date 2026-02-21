/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_MEM_DEBUGFS_H
#define HVGR_MEM_DEBUGFS_H

#include "hvgr_defs.h"

long hvgr_mem_add_profile(struct hvgr_ctx * const ctx, uint64_t user_buf, uint64_t size);

void hvgr_mem_remove_profile(struct hvgr_ctx * const ctx);

void hvgr_mem_debugfs_init(struct hvgr_device * const gdev);

void hvgr_mtrack_debugfs_init(struct hvgr_device * const gdev);

void hvgr_mtrack_debugfs_term(struct hvgr_device * const gdev);

void hvgr_switch_prev_ktime_init(ktime_t *begin);

uint32_t hvgr_switch_get_diff_and_update_ktime(ktime_t *begin);

void hvgr_switch_fill_fsnode(struct hvgr_switch_fs *dst, struct hvgr_switch_fs *src);

void hvgr_switch_list_add_fsinfo(struct hvgr_ctx * const ctx, struct hvgr_switch_fs *fs);

void hvgr_switch_update_front_info(struct hvgr_ctx * const ctx, struct hvgr_switch_fs *fs);

void hvgr_switch_fill_in_used_time(struct hvgr_switch_fs *info, enum recycle_step step,
	ktime_t *begin);

void hvgr_switch_fill_in_page_info(uint32_t num, struct hvgr_switch_fs *info, struct hvgr_ctx *ctx);

void hvgr_switch_update_stat_info(struct hvgr_switch_fs *info, enum context_swtich_status sts,
	char *str);

#endif
