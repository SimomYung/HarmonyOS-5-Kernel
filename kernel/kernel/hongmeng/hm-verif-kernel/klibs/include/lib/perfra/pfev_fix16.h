/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Performance infrastructure event: 16 byte fixed-sized event
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 5 16:52:49 2021
 */
#ifndef KLIBS_PERFRA_PFEV_FIX16_H
#define KLIBS_PERFRA_PFEV_FIX16_H

#include <lib/perfra/pfev_utils.h>

/* fix16 == Fixed 16 byte */
#define __PFEV_FIX16_DATA_SIZE	16
struct __pfev_fix16_desc_s {
	/* The recording cursor */
	__u64 cursor;
};
struct __pfev_fix16_data_s {
	__u8 data[__PFEV_FIX16_DATA_SIZE];
};
struct __pfev_fix16_chunk_s {
	struct __pfev_fix16_data_s data[0];
};

struct __pfev_fix16_u64x2_data_s {
	__u64 data[2];
};
struct __pfev_fix16_u64x2_chunk_s {
	struct __pfev_fix16_u64x2_data_s data[0];
};

static inline
void __pfev_fix16_record_next_maybe(
	struct __pfev_fix16_desc_s *desc,
	__u64 chunk_size_mask,
	__u64 *next_cursor,
	__u64 *next_cursor_post,
	int *err)
{
	do {
		*next_cursor = __pfev_atomic_load(&desc->cursor);
		*next_cursor_post = (*next_cursor + 1) & chunk_size_mask;
		if ((err != NULL) && (*next_cursor_post == 0)) {
			*err = E_HM_TOOMANY;
			break;
		}
	} while (!__pfev_atomic_cas(&desc->cursor, next_cursor, *next_cursor_post));
}

static inline
void __pfev_fix16_record_next(
	struct __pfev_fix16_desc_s *desc,
	__u64 chunk_size_mask,
	__u64 *cursor)
{
	__u64 next_cursor;
	__u64 next_cursor_post;
	__pfev_fix16_record_next_maybe(desc, chunk_size_mask, &next_cursor, &next_cursor_post, NULL);
	*cursor = next_cursor;
}

static inline
int __pfev_fix16_try_record_next(
	struct __pfev_fix16_desc_s *desc,
	__u64 chunk_size_mask,
	__u64 *cursor)
{
	int err = E_HM_OK;
	__u64 next_cursor;
	__u64 next_cursor_post;
	__pfev_fix16_record_next_maybe(desc, chunk_size_mask, &next_cursor, &next_cursor_post, &err);
	if (err == E_HM_OK) {
		*cursor = next_cursor;
	}
	return err;
}

static inline
void __pfev_fix16_record_save(
	__u64 cursor,
	struct __pfev_fix16_chunk_s *chunk,
	const struct __pfev_fix16_data_s *data)
{
	chunk->data[cursor] = *data;
}

static inline
void __pfev_fix16_u64x2_record_save(
	__u64 cursor,
	struct __pfev_fix16_chunk_s *chunk,
	__u64 data0, __u64 data1)
{
	((struct __pfev_fix16_u64x2_chunk_s *)chunk)->data[cursor] = (struct __pfev_fix16_u64x2_data_s) {
		.data = {
			data0, data1,
		},
	};
}

static inline
int __pfev_fix16_record(
	struct __pfev_fix16_desc_s *desc,
	struct __pfev_fix16_chunk_s *chunk,
	__u32 chunk_size,
	const struct __pfev_fix16_data_s *data)
{
	__u64 chunk_size_mask = (chunk_size / __PFEV_FIX16_DATA_SIZE) - 1;
	__u64 cursor = 0;
	__pfev_fix16_record_next(desc, chunk_size_mask, &cursor);
	__pfev_fix16_record_save(cursor, chunk, data);
	return E_HM_OK;
}

static inline
int __pfev_fix16_u64x2_record(
	struct __pfev_fix16_desc_s *desc,
	struct __pfev_fix16_chunk_s *chunk,
	__u32 chunk_size,
	__u64 data0, __u64 data1)
{
	__u64 chunk_size_mask = (chunk_size / __PFEV_FIX16_DATA_SIZE) - 1;
	__u64 cursor = 0;
	__pfev_fix16_record_next(desc, chunk_size_mask, &cursor);
	__pfev_fix16_u64x2_record_save(cursor, chunk, data0, data1);
	return E_HM_OK;
}

static inline
int __pfev_fix16_try_record(
	struct __pfev_fix16_desc_s *desc,
	struct __pfev_fix16_chunk_s *chunk,
	__u32 chunk_size,
	const struct __pfev_fix16_data_s *data)
{
	__u64 chunk_size_mask = (chunk_size / __PFEV_FIX16_DATA_SIZE) - 1;
	__u64 cursor = 0;
	int err = __pfev_fix16_try_record_next(desc, chunk_size_mask, &cursor);
	if (err == E_HM_OK) {
		__pfev_fix16_record_save(cursor, chunk, data);
	}
	return err;
}

/*
 * Initialize the fix16 instance
 * @param desc: the target chunk's description
 * @param chunk: the target chunk
 * @param chunk_size_order: the order of the size of the chunk
 * @return: return E_HM_OK
 */
static inline
int pfev_fix16_init(
	struct __pfev_fix16_desc_s *desc,
	struct __pfev_fix16_chunk_s *chunk,
	__u64 chunk_size_order)
{
	int err = E_HM_OK;
	if(__pfev_init_check_param(desc, chunk, chunk_size_order)) {
		__pfev_init_mark_inval(&err);
	} else {
		__pfev_init_mark_param(desc);
	}
	return err;
}

/*
 * Record 16 byte data into chunk.
 * @param desc: the description of the chunk
 * @param chunk: the chunk
 * @param data: the target data
 * @return: return E_HM_OK
 */
static inline
int pfev_fix16_record(
	struct __pfev_fix16_desc_s *desc,
	struct __pfev_fix16_chunk_s *chunk,
	__u32 chunk_size,
	const void *rawdata)
{
	const struct __pfev_fix16_data_s *data = (const struct __pfev_fix16_data_s *)rawdata;
	return __pfev_fix16_record(desc, chunk, chunk_size, data);
}

/*
 * Record 16 byte data into chunk.
 * @param desc: the description of the chunk
 * @param chunk: the chunk
 * @param data0, data1: the target data
 * @return: return E_HM_OK
 */
static inline
int pfev_fix16_u64x2_record(
	struct __pfev_fix16_desc_s *desc,
	struct __pfev_fix16_chunk_s *chunk,
	__u32 chunk_size,
	__u64 data0, __u64 data1)
{
	return __pfev_fix16_u64x2_record(desc, chunk, chunk_size, data0, data1);
}

/*
 * Try to record 16 byte data into chunk.
 * @param desc: the description of the chunk
 * @param chunk: the chunk
 * @param data: the target data
 * @return: When the buffer is full after record, do not record and return E_HM_TOOMANY.
 *	Otherwise, return E_HM_OK and record.
 */
static inline
int pfev_fix16_try_record(
	struct __pfev_fix16_desc_s *desc,
	struct __pfev_fix16_chunk_s *chunk,
	__u32 chunk_size,
	const void *rawdata)
{
	const struct __pfev_fix16_data_s *data = (const struct __pfev_fix16_data_s *)rawdata;
	return __pfev_fix16_try_record(desc, chunk, chunk_size, data);
}

#endif
