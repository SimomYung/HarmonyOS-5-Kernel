/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _FSCK_QUEUE_H_
#define _FSCK_QUEUE_H_

#ifdef POSIX_FADV_WILLNEED
#include "f2fs.h"

struct ra_work {
	struct list_head entry;
	block_t blkaddr;
};

/* queue.c */
extern void queue_reada_block(struct f2fs_sb_info *sbi, block_t blkaddr, int type);
extern void init_reada_queue(struct f2fs_sb_info *sbi);
extern void exit_reada_queue(struct f2fs_sb_info *sbi);
extern void build_sum_cache_list(struct f2fs_sb_info *sbi);
extern void destroy_sum_cache_list(struct f2fs_sb_info *sbi);
extern struct f2fs_summary_block *get_sum_node_block_from_cache(struct f2fs_sb_info *sbi,
				unsigned int segno, int *type);
extern struct f2fs_summary_block *get_sum_data_block_from_cache(struct f2fs_sb_info *sbi,
				unsigned int segno, int *type);
extern void add_sum_block_to_cache(struct f2fs_sb_info *sbi, unsigned int segno,
				int type, struct f2fs_summary_block *blk);
#else // POSIX_FADV_WILLNEED
#define queue_reada_block(sbi, blkaddr, type) dev_reada_block(blkaddr)
#define init_reada_queue(sbi) MSG(0, "Info: readahead queue is not enabled\n")
#define exit_reada_queue(sbi)
#define build_sum_cache_list(sbi)
#define destroy_sum_cache_list(sbi)
#define get_sum_node_block_from_cache(sbi, segno, type)
#define get_sum_data_block_from_cache(sbi, segno, type)
#define add_sum_block_to_cache(segno, type, blk)
#endif // POSIX_FADV_WILLNEED

#endif // _FSCK_QUEUE_H_
