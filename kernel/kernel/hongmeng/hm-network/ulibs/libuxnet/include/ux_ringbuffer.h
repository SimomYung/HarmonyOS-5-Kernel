/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: ringbuffer for network
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 10 14:26:28 CST 2024
 */

#ifndef UX_RINGBUFFER_H
#define UX_RINGBUFFER_H

#include <hmkernel/types.h>

struct ringbuffer_s {
	unsigned int w_offset;
	unsigned int r_offset;
	unsigned int rb_size;
	unsigned int w_offset_backup;
	unsigned int r_offset_backup;
	void *regions;
};

bool ux_rb_is_full(struct ringbuffer_s *rb, unsigned int src_len);
bool ux_rb_is_empty(struct ringbuffer_s *rb);
void ux_rb_init(struct ringbuffer_s *rb, unsigned int rb_size);
int ux_rb_write_nolock(struct ringbuffer_s *rb, void *src, unsigned int src_len,
		       int (*copy_fun)(void *dst, const void *src, unsigned long n));
int ux_rb_read_nolock(struct ringbuffer_s *rb, int msglen, void *dst, int dst_len,
		      int (*copy_fun)(void *dst, const void *src, unsigned long n));
void ux_rb_backup_nolock(struct ringbuffer_s *rb);
void ux_rb_rollback_nolock(struct ringbuffer_s *rb);
void ux_rb_discard_nolock(struct ringbuffer_s *rb, int msglen);

#endif /* UX_RINGBUFFER_H */
