/**
 * fsck_debug.h
 *
 * Copyright (C) 2024 Huawei Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _FSCK_DEBUG_H_
#define _FSCK_DEBUG_H_

#include "f2fs.h"

void dump_sbi_info(struct f2fs_sb_info *);
void hex_info_dump(const char *prompts, const unsigned char *buf,
	unsigned int len);
void dump_bitmap_diff(struct f2fs_sb_info *, const char *, const char *);
extern struct seg_entry *get_seg_entry(struct f2fs_sb_info *, unsigned int);

static inline unsigned int total_segments(struct f2fs_sb_info *sbi)
{
	return sbi->blocks_per_seg == 0 ?
		0 : ((unsigned int)sbi->user_block_count) /
				((unsigned int)sbi->blocks_per_seg);
}

static inline unsigned int reserved_segments(struct f2fs_sb_info *sbi)
{
	return sbi->sm_info == NULL ? 0 : sbi->sm_info->reserved_segments;
}

static inline unsigned int overprov_segments(struct f2fs_sb_info *sbi)
{
	return sbi->sm_info == NULL ? 0 : sbi->sm_info->ovp_segments;
}

static inline block_t of_valid_block_count(struct f2fs_sb_info *sbi)
{
	return sbi->total_valid_block_count;
}

static inline unsigned int f2fs_utilization(struct f2fs_sb_info *sbi)
{
	/* valid block percentage of sbi */
	return sbi->user_block_count == 0 ?
		0 : (of_valid_block_count(sbi) * 100) / sbi->user_block_count;
}

#endif /* _FSCK_DEBUG_H_ */