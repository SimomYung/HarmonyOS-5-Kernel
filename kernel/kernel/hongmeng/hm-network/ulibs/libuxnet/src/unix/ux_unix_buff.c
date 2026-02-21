/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Source file unix buffers
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct  5 11:02:56 2023
 */

#include "osal.h"
#include "ux_mempool.h"
#include "ux_unix_util.h"
#include "ux_unix_scm.h"

static void ux_unix_free_buffer(ux_unix_buf_t *buf)
{
	ux_unix_sock_t *usk = buf->allocator;

	if (usk != NULL) {
		(void)ux_unix_send_buffer_sub(usk, (int)buf->real_size);
		if (ux_unix_send_buffer_used(usk) < usk->sk.sndbuf) {
			ux_wq_wakeup_all_sync(&usk->sendwq);
			ux_sock_poll_common(&usk->sk, EPOLLOUT);
		}

		ux_sock_put(&usk->sk);
	}

	ux_mempool_free(buf, buf->real_size);
}

static void __unix_buf_release_func(struct raw_refcnt *ref)
{
	if (ref != NULL) {
		ux_unix_buf_t *unix_buf = container_of(ref, ux_unix_buf_t, ref);
		unix_scm_fds_destory(&unix_buf->scm_fd);
		ux_unix_free_buffer(unix_buf);
	}
}

ux_unix_buf_t *ux_unix_alloc_buffer(ux_unix_sock_t *allocator, size_t size)
{
	const size_t size_allocated = (sizeof(ux_unix_buf_t) + size);
	ux_unix_buf_t *unix_buf = (ux_unix_buf_t *)ux_mempool_alloc(1 * size_allocated);

	if (unix_buf != NULL) {
		NOFAIL(memset_s(unix_buf, sizeof(ux_unix_buf_t), 0, sizeof(ux_unix_buf_t)));
		raw_refcnt_init(&unix_buf->ref, 1, __unix_buf_release_func);

		ux_sock_get(&allocator->sk);
		unix_buf->allocator = allocator;

		unix_buf->real_size = size_allocated;
		(void)ux_unix_send_buffer_add(allocator, (int)size_allocated);

		if (ux_unix_send_buffer_used(allocator) >= allocator->sk.sndbuf) {
			ux_sock_poll_common(&allocator->sk, EPOLLOUT << CLEAN_EVENT_SHIFT);
		}
	}

	return unix_buf;
}

int ux_unix_buff_get(ux_unix_buf_t *buf)
{
	return raw_refcnt_get(&buf->ref);
}

void ux_unix_buff_put(ux_unix_buf_t *buf)
{
	(void)raw_refcnt_put(&buf->ref);
}
