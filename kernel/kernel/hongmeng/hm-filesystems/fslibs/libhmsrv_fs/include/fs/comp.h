/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of comp
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 24 11:24:49 2019
 */
#ifndef LIBHMSRV_FS_COMP_H
#define LIBHMSRV_FS_COMP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <endian.h>
#include <sys/ioctl.h>

/*
 * On-disk compressed file format structures
 *
 * .-----------------.
 * |   comp header   |
 * |-----------------|
 * |  comp entry 1   |
 * |-----------------|
 * | data of entry 1 |
 * |-----------------|
 * | ... ... ... ... |
 * |-----------------|
 * |  comp entry n   |
 * |-----------------|
 * | data of entry n |
 * '-----------------'
 */

#define COMP_MAGIC 0x434F4D50	/* ASCII: COMP */
#define ZTMPFS_IOC_SETCOMP	_IOW('Z', 1, int)

typedef uint32_t __le32;
typedef uint64_t __le64;

enum comp_type {
	COMP_TYPE_UNCOMP,
	COMP_TYPE_LZMA,
	COMP_TYPE_LZMA_NOCOMP,
	COMP_TYPE_LZMA_FILTER,
	COMP_TYPE_MAX
};

#define DEFAULT_COMP_TYPE	COMP_TYPE_LZMA
#define DEFAULT_COMP_UNIT	(128 * 1024)
#define MAX_COMP_UNIT		(8 * 1024 * 1024)
/* ratio = (uncomp_sz / comp_sz) * 10, the larger the harder */
#define DEFAULT_COMP_RATIO	(11)
/* for 128K unit, its 1T */
#define MAX_ENT_CNT		0x800000U

/*
 * Compress entry->data only when:
 *  1. compressed data size is less than uncompressed data size, and
 *  2. ratio is larger than or equal to DEFAULT_COMP_RATIO
 */
#define should_compress(uncomp_sz, comp_sz) (((uncomp_sz) > (comp_sz)) && \
		((uncomp_sz) >= DEFAULT_COMP_RATIO) && \
		((comp_sz) <= (((uncomp_sz) / DEFAULT_COMP_RATIO) * 10)))

struct file_header {
	__le32 magic;
	/*
	 * The highest bit indicates if the file has extension options,
	 * the other 7 bits is the compress type of the file
	 */
	uint8_t comp_type;
	uint8_t comp_unit_log;	/* compress unit log2() */
	__le32 ent_cnt;		/* number of entry count */
	__le64 file_size;	/* size of plain file */
	uint8_t extension[];	/* extension for algrithm */
} __attribute__((packed));

#define COMP_HDR_SZ sizeof(struct file_header)

struct ext_struct {
	uint8_t ext_size;
	uint8_t options[];
} __attribute__((packed));

struct file_entry {
	__le32 key;
	/*
	 * The highest bit indicates the entry is compressed or not,
	 * the other 31 bits is the data length of the entry
	 */
	__le32 data_len;	/* should be less than data_unit */
} __attribute__((packed));

#define COMP_ENT_SZ sizeof(struct file_entry)

#define EXTENSION_SHIFT	7U
#define EXTENSION_BIT	(1U << EXTENSION_SHIFT)
#define COMP_TYPE_MASK	(EXTENSION_BIT - 1U)

#define get_comp_type(h) ((uint8_t)((h)->comp_type & (uint8_t)COMP_TYPE_MASK))
#define set_comp_type(h, c, t) do { \
	if ((c)->ext_size > 0) \
		(h)->comp_type = (uint8_t)(EXTENSION_BIT | ((t) & ~EXTENSION_BIT)); \
	else \
		(h)->comp_type = (uint8_t)((t) & ~EXTENSION_BIT); \
} while (0)

#define ENT_COMP_SHIFT	31U
#define ENT_COMP_BIT	(1U << ENT_COMP_SHIFT)
#define ENT_COMP_MASK	(ENT_COMP_BIT - 1U)

#define is_entry_compressed(e) (((le32toh((e)->data_len) & ENT_COMP_BIT) != 0) ? 1 : 0)
#define get_entry_datalen(e) (le32toh((e)->data_len) & ENT_COMP_MASK)
#define get_entry_key(e) (le32toh((e)->key))

#define GET_FILE_ENTRY(file_data, offs) ((struct file_entry *)(void *) \
		((char *)(file_data) + sizeof(struct file_header) + \
		(offs)))

#define GET_ENTRY_DATA(fent) ((char *)(fent) + sizeof(struct file_entry))

static inline bool all_zero(const char *buf, size_t size)
{
	size_t i;

	for (i = 0; i < size; i++) {
		if (buf[i] != 0) {
			return false;
		}
	}

	return true;
}

struct lzma_args {
	uint32_t preset;
	uint32_t filter_id;
};

#endif
