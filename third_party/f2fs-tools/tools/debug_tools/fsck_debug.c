/**
 * fsck_debug.c
 *
 * Copyright (C) 2024 Huawei Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include "fsck_debug.h"

void dump_sbi_info(struct f2fs_sb_info *sbi)
{
	if (sbi == NULL) {
		MSG(0, "sbi is null\n");
		return;
	}

	MSG(0, "\n");
	MSG(0, "+--------------------------------------------------------+\n");
	MSG(0, "| SBI                                                    |\n");
	MSG(0, "+--------------------------------------------------------+\n");
	MSG(0, "total_count %u\n", total_segments(sbi));
	MSG(0, "resvd_segs %u\n", reserved_segments(sbi));
	MSG(0, "overp_segs %u\n", overprov_segments(sbi));
	MSG(0, "valid_count %u\n", of_valid_block_count(sbi));
	MSG(0, "utilization %u\n", f2fs_utilization(sbi));
	MSG(0, "\n");
	hex_info_dump("f2fs_sb_info", sbi,
		sizeof(struct f2fs_sb_info));
	MSG(0, "\n");
}

#define LINE_MAX_LEN     80
#define LINE_MAX_INTS    16
#define BATCH_INTS       8
#define HEX_SHIFT_12     12
#define HEX_SHIFT_8      8
#define HEX_SHIFT_4      4
#define HEX_MASK         0x0F
#define U32_PER_SEG      64
void hex_info_dump(const char *prompts, const unsigned char *buf,
			unsigned int len)
{
	static const unsigned char hex_ascii[] = "0123456789abcdef";
	unsigned char line[LINE_MAX_LEN];
	unsigned int i, j, k, line_len;
	unsigned int rest = len;

	MSG(0, "===HEX DUMP START: %.25s, len %u===\n",
		prompts, len);
	for (i = 0; i < len; i += LINE_MAX_INTS) {
		line_len = rest > LINE_MAX_INTS ? LINE_MAX_INTS : rest;
		k = 0;
		line[k++] = hex_ascii[(i >> HEX_SHIFT_12) & HEX_MASK];
		line[k++] = hex_ascii[(i >> HEX_SHIFT_8) & HEX_MASK];
		line[k++] = hex_ascii[(i >> HEX_SHIFT_4) & HEX_MASK];
		line[k++] = hex_ascii[i & HEX_MASK];
		line[k++] = ':';
		for (j = 0; j < line_len; j++) {
			j % BATCH_INTS == 0 ? line[k++] = ' ' : 1;
			line[k++] = hex_ascii[(buf[i + j] >> HEX_SHIFT_4) & HEX_MASK];
			line[k++] = hex_ascii[(buf[i + j] & HEX_MASK)];
		}
		line[k++] = '\0';
		rest -= line_len;
		MSG(0, "%s\n", line);
	}
	MSG(0, "===HEX DUMP END===\n");
}

static void dump_one_segment(const char *bitmap, unsigned int segno)
{
	for (u32 i = 0; i < U32_PER_SEG; i++) {
		MSG(0, " %02x", *(bitmap + i + segno * U32_PER_SEG));
		
		if ((i + 1) % LINE_MAX_INTS == 0) {
			MSG(0, "\n");
		}
	}
}

static bool has_diff_segment(const char *sit_area_bitmap, const char *main_area_bitmap, unsigned int segno)
{
	for (u32 i = 0; i < U32_PER_SEG; i++) {
		if (*(main_area_bitmap + i + segno * U32_PER_SEG) != *(sit_area_bitmap + i + segno * U32_PER_SEG)) {
			return true;
		}
	}
	return false;
}

void dump_bitmap_diff(struct f2fs_sb_info *sbi, const char *sit_area_bitmap, const char *main_area_bitmap)
{
	struct seg_entry *se;
	unsigned int segno;
	int i;
	u32 main_segments = SM_I(sbi)->main_segments;

	if (sit_area_bitmap == NULL || main_area_bitmap == NULL)  {
		return;
	}

	for (segno = 0; segno < main_segments; segno++) {
		se = get_seg_entry(sbi, segno);
		if (se->valid_blocks == 0x0) {
			continue;
		}

		if (has_diff_segment(sit_area_bitmap, main_area_bitmap, segno)) {
			MSG(0, "segno: %u vblocks: %u seg_type: %d\n", segno, se->valid_blocks, se->type);
			MSG(0, "=Dump main bitmap=\n");
			dump_one_segment(main_area_bitmap, segno);
			MSG(0, "=Dump sit bitmap=\n");
			dump_one_segment(sit_area_bitmap, segno);
			return;
		}
	}
}