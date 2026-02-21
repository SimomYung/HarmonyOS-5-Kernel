/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: ringbuffer for network
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 10 14:26:28 CST 2024
 */
#include <sys/types.h>
#include <hongmeng/errno.h>
#include <libhwsecurec/securec.h>
#include "ux_netlog.h"
#include "ux_ringbuffer.h"
/*
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |R| |                           regions                         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |R|rw ->
 *            |R|r->-------data--------w
 * ----------------w             |R|r->----------------data---------
 * Ring buffer
 *    It supports continuous intra-block addresses and discontinuous inter-block addresses.
 *    It supports not overwriting when the queue is full.
 *    It supports discarding the remaining packets that are not completely read.
 *    It supports the peek function.
 *    R: Reserved Byte
 *      - Reserved. This byte is not used to determine the region status (empty or full).
 *      - if empty:
 *          w == r;
 *          - if has data:
 *              Must w != r
 *          - if full:
 *              r >= w: r - w - 1 < data_len
 *              r < w: r + len - w < data_len
 */

#define min(a, b) \
	({ __typeof__ (a) _a = (a); \
	   __typeof__ (b) _b = (b); \
	   _a < _b ? _a : _b; })

static int __rb_write_data(struct ringbuffer_s *rb, void *src, unsigned int src_len,
			    int (*copy_fun)(void *dst, const void *src, unsigned long n))
{
	int ret = E_HM_OK;
	unsigned int rb_size = rb->rb_size;
	void *__src = src;
	unsigned int src_left = src_len;

	while (src_left > 0U) {
		unsigned int dst_left = rb_size - rb->w_offset;
		unsigned int copy_size = min(src_left, dst_left);
		void *__dst = ptr_add(rb->regions, rb->w_offset);
		ret = copy_fun(__dst, __src, (unsigned long)copy_size);
		if (ret != E_HM_OK) {
			break;
		}

		src_left -= copy_size;
		rb->w_offset = (rb->w_offset + copy_size) % rb_size;
		__src = ptr_add(__src, copy_size);
	}

	return ret;
}

bool ux_rb_is_full(struct ringbuffer_s *rb, unsigned int src_len)
{
	unsigned int w_offset = rb->w_offset;
	unsigned int r_offset = rb->r_offset;
	unsigned int rb_len = rb->rb_size - 1;

	return (src_len > rb_len) ||
	       (r_offset < w_offset && r_offset + rb_len - w_offset < src_len) ||
	       (r_offset >= w_offset && r_offset - w_offset - 1 < src_len);
}

bool ux_rb_is_empty(struct ringbuffer_s *rb)
{
	return rb->w_offset == rb->r_offset;
}

int ux_rb_write_nolock(struct ringbuffer_s *rb, void *src, unsigned int src_len,
		int (*copy_fun)(void *dst, const void *src, unsigned long n))
{
	int ret = E_HM_OK;

	if (ux_rb_is_full(rb, src_len)) {
		ret = E_HM_AGAIN;
	}

	if (ret == E_HM_OK) {
		ret = __rb_write_data(rb, src, src_len, copy_fun);
	}

	return ret;
}

static int __rb_read_data(struct ringbuffer_s *rb, int msglen, void *dst, int dst_len,
			  int (*copy_fun)(void *dst, const void *src, unsigned long n))
{
	int ret = E_HM_OK;
	unsigned int _dst_len = (unsigned int)dst_len;
	unsigned int _msglen = (unsigned int)msglen;
	unsigned int rb_size = rb->rb_size;
	unsigned int copied = 0;
	void *_dst = dst;

	while (_msglen > 0 && _dst_len > 0) {
		unsigned int _left = rb_size - rb->r_offset;
		unsigned int copy_size = min(_msglen, _left);
		copy_size = min(copy_size, _dst_len);
		void *_src = ptr_add(rb->regions, rb->r_offset);
		ret = copy_fun(_dst, _src, (unsigned long)copy_size);
		if (ret != E_HM_OK) {
			break;
		}

		_dst_len -= copy_size;
		_dst = ptr_add(_dst, copy_size);
		_msglen -= copy_size;
		rb->r_offset = (rb->r_offset + copy_size) % rb_size;
		copied += copy_size;
	}

	return ret == E_HM_OK ? (int)copied : ret;
}

int ux_rb_read_nolock(struct ringbuffer_s *rb, int msglen, void *dst, int dst_len,
		     int (*copy_fun)(void *dst, const void *src, unsigned long n))
{
	if (ux_rb_is_empty(rb)) {
		net_error(HMNET_UNIX, "unix rb is empty, r_offset = w_offset = %u\n", rb->w_offset);
		return E_HM_AGAIN;
	}

	return __rb_read_data(rb, msglen, dst, dst_len, copy_fun);
}

void ux_rb_discard_nolock(struct ringbuffer_s *rb, int msglen)
{
	unsigned int _msglen = (unsigned int)msglen;
	rb->r_offset = (rb->r_offset + _msglen) % rb->rb_size;
}

void ux_rb_backup_nolock(struct ringbuffer_s *rb)
{
	rb->r_offset_backup = rb->r_offset;
	rb->w_offset_backup = rb->w_offset;
}

void ux_rb_rollback_nolock(struct ringbuffer_s *rb)
{
	rb->r_offset = rb->r_offset_backup;
	rb->w_offset = rb->w_offset_backup;
}

void ux_rb_init(struct ringbuffer_s *rb, unsigned int rb_size)
{
	rb->w_offset = 0;
	rb->r_offset = 0;
	rb->rb_size = rb_size;
}
