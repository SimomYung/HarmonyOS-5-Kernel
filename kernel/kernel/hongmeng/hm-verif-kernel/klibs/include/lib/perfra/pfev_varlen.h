/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Performance infrastructure event: variadic-sized event
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 5 16:52:49 2021
 */
#ifndef KLIBS_PERFRA_PFEV_VARLEN_H
#define KLIBS_PERFRA_PFEV_VARLEN_H

#include <lib/perfra/pfev_utils.h>

#ifndef __KERNEL__
#include <libhwsecurec/securec.h>
#endif

struct __pfev_varlen_desc_s {
	/* The recording cursor */
	__u64 cursor;
};

struct __pfev_varlen_chunk_s {
	__u8 data[0];
};

static inline
void __pfev_varlen_record_next_maybe(
	struct __pfev_varlen_desc_s *desc,
	__u64 data_size,
	__u64 chunk_size_mask,
	__u64 *next_cursor,
	__u64 *next_cursor_post,
	int *err)
{
	do {
		*next_cursor = __pfev_atomic_load(&desc->cursor);
		*next_cursor_post = (*next_cursor + data_size) & chunk_size_mask;
		if ((err != NULL) && (*next_cursor_post < *next_cursor)) {
			*err = E_HM_TOOMANY;
			break;
		}
	} while (!__pfev_atomic_cas(&desc->cursor, next_cursor, *next_cursor_post));
}

static inline
void __pfev_varlen_record_next(
	struct __pfev_varlen_desc_s *desc,
	__u64 data_size,
	__u64 chunk_size_mask,
	__u64 *cursor)
{
	__u64 next_cursor;
	__u64 next_cursor_post;
	__pfev_varlen_record_next_maybe(desc, data_size, chunk_size_mask, &next_cursor, &next_cursor_post, NULL);
	*cursor = next_cursor;
}

static inline
int __pfev_varlen_try_record_next(
	struct __pfev_varlen_desc_s *desc,
	__u64 data_size,
	__u64 chunk_size_mask,
	__u64 *cursor)
{
	int err = E_HM_OK;
	__u64 next_cursor;
	__u64 next_cursor_post;
	__pfev_varlen_record_next_maybe(desc, data_size, chunk_size_mask, &next_cursor, &next_cursor_post, &err);
	if (err == E_HM_OK) {
		*cursor = next_cursor;
	}
	return err;
}

static inline
void __pfev_varlen_record_save_eval(
	__u64 next_base,
	__u64 chunk_size_mask,
	__u64 data_size,
	__u64 *round_curr,
	__u64 *round_next)
{
	if (((next_base & chunk_size_mask) + data_size) <= chunk_size_mask + 1) {
		*round_curr = data_size;
		*round_next = 0;
	} else {
		*round_curr = chunk_size_mask + 1 - (next_base & chunk_size_mask);
		*round_next = data_size - *round_curr;
	}
}

static inline
void __pfev_varlen_record_save_copy(
	__u64 next_base,
	struct __pfev_varlen_chunk_s *chunk,
	__u64 chunk_size,
	__u64 round_curr,
	__u64 round_next,
	const void *data)
{
#ifdef __KERNEL__
	mem_cpy_p_to_p(&chunk->data[next_base], chunk_size - next_base, data, round_curr);
	mem_cpy_p_to_p(&chunk->data[0], chunk_size, (__u8 *)data + round_curr, round_next);
#else
	NOFAIL(memcpy_s(&chunk->data[next_base], (size_t)(chunk_size - next_base), data, (size_t)round_curr));
	NOFAIL(memcpy_s(&chunk->data[0], (size_t)chunk_size, (__u8 *)data + round_curr, (size_t)round_next));
#endif
}

static inline
void __pfev_varlen_record_save(
	struct __pfev_varlen_chunk_s *chunk,
	__u64 cursor,
	__u64 chunk_size_mask,
	const void *data,
	__u64 data_size)
{
	__u64 round_curr = 0;
	__u64 round_next = 0;
	__pfev_varlen_record_save_eval(cursor, chunk_size_mask, data_size, &round_curr, &round_next);
	__pfev_varlen_record_save_copy(cursor, chunk, chunk_size_mask + 1, round_curr, round_next, data);
}

static inline
int __pfev_varlen_record(
	struct __pfev_varlen_desc_s *desc,
	struct __pfev_varlen_chunk_s *chunk,
	__u32 chunk_size,
	const void *data,
	__u64 data_size)
{
	__u64 chunk_size_mask = chunk_size - 1;
	__u64 cursor = 0;
	__pfev_varlen_record_next(desc, data_size, chunk_size_mask, &cursor);
	__pfev_varlen_record_save(chunk, cursor, chunk_size_mask, data, data_size);
	return E_HM_OK;
}

static inline
int __pfev_varlen_try_record(
	struct __pfev_varlen_desc_s *desc,
	struct __pfev_varlen_chunk_s *chunk,
	__u32 chunk_size,
	const void *data,
	__u64 data_size)
{
	__u64 chunk_size_mask = chunk_size - 1;
	__u64 cursor = 0;
	int err = __pfev_varlen_try_record_next(desc, data_size, chunk_size_mask, &cursor);
	if (err == E_HM_OK) {
		__pfev_varlen_record_save(chunk, cursor, chunk_size_mask, data, data_size);
	}
	return err;
}

/*
 * Initialize the varlen instance
 * @param desc: the target chunk's description
 * @param chunk: the target chunk
 * @param chunk_size_order: the order of the size of the chunk
 * @return: return E_HM_OK
 */
static inline
int pfev_varlen_init(
	struct __pfev_varlen_desc_s *desc,
	struct __pfev_varlen_chunk_s *chunk,
	__u64 chunk_size_order)
{
	int err = E_HM_OK;
	if (__pfev_init_check_param(desc, chunk, chunk_size_order)) {
		__pfev_init_mark_inval(&err);
	} else {
		__pfev_init_mark_param(desc);
	}
	return err;
}

/*
 * Record variadic byte data into chunk.
 * @param desc: the target chunk's description
 * @param chunk: the target chunk
 * @param data: the target data
 * @param data_size: the size of the target data
 * @return: return E_HM_OK
 */
static inline
int pfev_varlen_record(
	struct __pfev_varlen_desc_s *desc,
	struct __pfev_varlen_chunk_s *chunk,
	__u32 chunk_size,
	const void *data,
	__u64 data_size)
{
	return __pfev_varlen_record(desc, chunk, chunk_size, data, data_size);
}

/*
 * Record variadic byte data into chunk.
 * @param desc: the target chunk's description
 * @param chunk: the target chunk
 * @param data: the target data
 * @param data_size: the size of the target data
 * @return: When the buffer is full after record, do not record and return E_HM_TOOMANY.
 *	Otherwise, return E_HM_OK and record.
 */
static inline
int pfev_varlen_try_record(
	struct __pfev_varlen_desc_s *desc,
	struct __pfev_varlen_chunk_s *chunk,
	__u32 chunk_size,
	const void *data,
	__u64 data_size)
{
	return __pfev_varlen_try_record(desc, chunk, chunk_size, data, data_size);
}

#endif
