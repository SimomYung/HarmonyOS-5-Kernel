/**
 * hmfs_tools.c
 *
 * Copyright (C) 2024 Huawei Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "hmfs_tools.h"
#include "../../fsck/f2fs.h"
#include "../../fsck/fsck.h"

static __le32 fsck_type_val[] = {0, F2FS_SUPER_MAGIC, HMFS_SUPER_MAGIC};

bool is_valid_hmfs_magic(unsigned long hmfs_magic)
{
	if (hmfs_magic != HMFS_SUPER_MAGIC) {
		MSG(0, "HMFS Magic Mismatch, valid(0x%x) - read(0x%x)\n",
			 HMFS_SUPER_MAGIC, hmfs_magic);
		return false;
	}

	return true;
}

enum convert_type parse_convert_config(const char *optarg)
{
	enum convert_type ret = CONVERT_TYPE_NONE;
	if (optarg != NULL) {
		if (!strcmp("hmfs", optarg)) {
			ret = CONVERT_TYPE_HMFS;
		} else if (!strcmp("f2fs", optarg)) {
			ret = CONVERT_TYPE_F2FS;
		} else {
			MSG(0, "Unknown file system type %s\n", optarg);
			return CONVERT_TYPE_NONE;
		}
		MSG(0, "Info: convert to %s\n", optarg);
	}
	return ret;
}

void mkfs_hmfs_msg(unsigned int fs_magic)
{
	if (fs_magic == HMFS_SUPER_MAGIC) {
		MSG(0, "Info: make hmfs type\n");
	}
}

bool convert_fs(struct f2fs_sb_info *sbi, enum convert_type convert)
{
	struct f2fs_super_block *sb = sbi->raw_super;

	c.fs_magic = sb->magic;
	if ((convert == CONVERT_TYPE_HMFS && sb->magic != HMFS_SUPER_MAGIC) ||
		(convert == CONVERT_TYPE_F2FS && sb->magic != F2FS_SUPER_MAGIC)) {
		MSG(0, "Info: convert maigic 0x%X to 0x%X\n", sb->magic, fsck_type_val[convert]);
		/* update sb magic and sb crc */
		sb->magic = fsck_type_val[convert];
		return true;
	}
	return false;
}

void update_currcp_crc(struct f2fs_sb_info *sbi)
{
	struct f2fs_super_block *sb = F2FS_RAW_SUPER(sbi);
	struct f2fs_checkpoint *cp = F2FS_CKPT(sbi);
	unsigned long long cp_blk_no;
	int ret;
	uint32_t crc;
	void *cp_pack_page;

	/* update the 1st cp block in this CP pack */
	crc = f2fs_checkpoint_chksum(cp);
	*((__le32 *)((unsigned char *)cp + get_cp(checksum_offset))) =
							cpu_to_le32(crc);
	cp_blk_no = get_sb(cp_blkaddr);
	if (sbi->cur_cp == 2)
		cp_blk_no += 1 << get_sb(log_blocks_per_seg);
	ret = dev_write_block(cp, cp_blk_no);
	ASSERT(ret >= 0);

	/* update the 2nd cp block in this CP pack */
	cp_blk_no += get_cp(cp_pack_total_block_count) - 1;
	cp_pack_page = malloc(F2FS_BLKSIZE);
	ASSERT(cp_pack_page);
	if (dev_read_block(cp_pack_page, cp_blk_no) < 0)
		ASSERT(0);
	crc = f2fs_checkpoint_chksum(cp_pack_page);
	*((__le32 *)((unsigned char *)cp_pack_page + get_cp(checksum_offset))) =
							cpu_to_le32(crc);
	ret = dev_write_block(cp_pack_page, cp_blk_no);
	free(cp_pack_page);
	ASSERT(ret >= 0);

	ret = f2fs_fsync_device();
	ASSERT(ret >= 0);
}

void hmfs_enable_large_nat_bitmap(struct f2fs_sb_info *sbi)
{
	/* has large_nat_bitmap before */
    if (sbi != NULL) {
        struct f2fs_checkpoint *cp = F2FS_CKPT(sbi);
        u32 flags = get_cp(ckpt_flags);
        if (flags & CP_LARGE_NAT_BITMAP_FLAG) {
            c.large_nat_bitmap = 1;
        }
    }
}