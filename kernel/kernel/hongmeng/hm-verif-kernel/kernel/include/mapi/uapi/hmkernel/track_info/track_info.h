/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: track info
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 2 08:00:40 PM 2024
 */
#ifndef UAPI_HMKERNEL_TRACK_INFO_H
#define UAPI_HMKERNEL_TRACK_INFO_H
#include <lib/utils.h>
#include <hmkernel/types.h>

/* hex ascii code of "TRKI" */
#define TRACK_INFO_HEADER_MAGIC		0x54524B49U
/* hex ascii code of "SLTM" */
#define TRACK_INFO_SLOT_HEADER_MAGIC	0x534C544DU
/* hex ascii code of "RGNM" */
#define TRACK_INFO_REGION_HEADER_MAGIC	0x52474E4DU
/* hex ascii code of "TKRB" */
#define TRACK_INFO_RB_HEADER_MAGIC	0x544B5242U

enum record_type {
#define DEFINE_TRACK_INFO(RECORD_TYPE, ...)	\
	RECORD_##RECORD_TYPE,
#include <hmkernel/track_info/track_info_list.h>
#undef DEFINE_TRACK_INFO
	RECORD_TYPE_NR
};

#define RECORD_COMMON_ENTRY	\
	__u64 timestamp;

struct record_common_s {
	RECORD_COMMON_ENTRY
} __attribute__((packed));

enum {
	SLOT_ID_A,
	SLOT_ID_B,
	NR_SLOTS,
};

/*
 +----+------+----------+-------+-------+---+------------+---+------+---+
 |info|slot a|region irq|rb cpu0|rb cpu1|...|region sched|...|slot b|...|
 +----+------+----------+-------+-------+---+------------+---+------+---+
 */
struct track_info_header_s {
	__u32 csum;
	__u32 magic;
	__u32 size;			/* the size of track_info excluding header */
	__u32 slot_offset[NR_SLOTS];	/* the offset of slots relative to data */
	__u32 slot_size;		/* the size of each slot */
	__u8 curr_slot;			/* the id of the slot in use */
	__u8 reserved[7];		/* pad to 8-bytes align explicitly */
	unsigned char data[];
} __attribute__((packed));

struct track_info_slot_header_s {
	__u32 csum;
	__u32 magic;
	__u32 size;				/* the size of slot excluding header */
	__u32 region_offset[RECORD_TYPE_NR];	/* the offset of regions relative to data */
	__u32 region_size[RECORD_TYPE_NR];
	__u8 types;				/* the number of types */
	__u8 reserved[3];			/* pad to 8-bytes align explicitly */
	unsigned char data[];
} __attribute__((packed));

struct track_info_region_header_s {
	__u32 csum;
	__u32 magic;
	__u32 size;		/* the size of region excluding header */
	__u32 rb_offset;	/* the offset of rb relative to data */
	__u8 cpus;		/* the number of cpus */
	__u8 type;		/* the record type of region */
	__u8 reserved[6];	/* pad to 8-bytes align explicitly */
	unsigned char data[];
} __attribute__((packed));

/* only one writer or reader at any time, write and read with fixed-length */
struct track_info_rb_header_s {
	__u32 csum;
	__u32 magic;
	__u32 size;		/* the size of rb excluding header */
	__u32 offset;		/* the offset of rb relative to data */
	__u32 max_count;	/* the maximum number of records */
	__u32 field_size;	/* the size of each record */
	__u8 cpu_id;		/* the cpu id of rb */
	__u8 reserved[3];	/* pad to 8-bytes align explicitly */
	/* checksum doesn't contain the following modified frequently members */
	__u32 w_index;		/* the next index of record to be written */
	__u32 r_index;		/* the next index of record to be read */
	__u32 count;		/* the number of unread records */
	unsigned char data[];
} __attribute__((packed));

#define TRACK_INFO_BITS_PER_UINT   32
#define TRACK_INFO_UINT_MAX       (~0U)

static inline __u32 checksum32(const __u32 *addr, size_t len)
{
	__u64 sum = 0;
	__u32 left = 0;

	for (size_t i = 0; i < (len / sizeof(__u32)); i++) {
		sum += *(addr + i);
	}

	for (size_t i = 0; i < (len % sizeof(__u32)); i++) {
		*((__u8 *)&left + i) = *((__u8 *)(addr + len / sizeof(__u32)) + i);
	}

	sum += left;

	return (__u32)((sum & TRACK_INFO_UINT_MAX) + (sum >> TRACK_INFO_BITS_PER_UINT));
}

#define DEFINE_HEADER_CHECKSUM32(header_type, member)						\
static inline __u32 header_type##_checksum32(const struct header_type##_s *header)		\
{												\
	__u32 *start_ptr = ptr_cast(header, __u32);						\
	size_t len = offset_of(struct header_type##_s, member);					\
												\
	return checksum32(start_ptr, len);							\
}												\
												\
static inline void header_type##_set_checksum(struct header_type##_s *header)			\
{												\
	header->csum = 0;									\
	header->csum = ~header_type##_checksum32(header);					\
}												\
												\
static inline bool is_##header_type##_csum_invalid(const struct header_type##_s *header)	\
{												\
	return (header_type##_checksum32(header) + 1U) != 0;					\
}

DEFINE_HEADER_CHECKSUM32(track_info_header, reserved)
DEFINE_HEADER_CHECKSUM32(track_info_slot_header, reserved)
DEFINE_HEADER_CHECKSUM32(track_info_region_header, reserved)
DEFINE_HEADER_CHECKSUM32(track_info_rb_header, reserved)

#endif
