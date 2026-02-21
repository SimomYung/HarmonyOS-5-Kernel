/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide interfaces for hm ring buffer
 * Author: Huawei OS Kernel Lab
 * Create: Thu 05 Sep 2019 10:24:52 AM CST
 */
#ifndef ULIBS_LIBHMTRACE_RING_BUFFER_H
#define ULIBS_LIBHMTRACE_RING_BUFFER_H

#include <stdint.h>
#include <libhmsync/raw_scopedptr.h>

#define HMTRACE_DEFAULT_TRY_TIME 10
#define HMTRACE_MAX_ENTRY_LEN (1U << 12)

struct rb {
	int shm_id;		/* share memory id */
	uint16_t blk_num;	/* block number related to ring buffer */
	uint64_t key;           /* a key used to open a share memory */
	size_t rb_sz;		/* ring buffer size */
	uint32_t flags;		/* rb mode: drop new or overwrite */
	bool is_creator;	/* rb creator or not */
	void *map_buf;		/* mmap address */
	void *data;		/* private data, pointer to true ring buffer */
	struct raw_refcnt_nr refcnt_nr;
	uint32_t cnode_idx;     /* check whether the key belongs to cnode */
	union {
		int *futex_shared;	/* futex used */
		int *ftrace_enable;	/* enable/disable fucntion trace writing trace data */
	} private;
};

struct rb_vectors_s {
	const void *vector_buf;     /* data address */
	size_t vector_len;         /* vector size */
};

#define RB_DROP_MODE 0
#define RB_OVERWRITE_MODE 1

/*
 * create a ring buffer with the given size
 *
 * @param	size	size of ring buffer
 * @param	flags	rb mode
 *
 * @return	pointer of ring buffer if success, or NULL if fail
 */
struct rb *hm_ring_buffer_create(size_t size, uint32_t flags);

/*
 * write data to the ring buffer
 *
 * @param	rb	[I] pointer to the ring bufer
 * @param	data	[I] pointer to user data
 * @param	len	length of user data, note len <= HMTRACE_MAX_ENTRY_LEN
 *
 * @return	size of data written, positive if success, or 0 if fail
 */
size_t hm_ring_buffer_write_event(struct rb *rb, void *data, size_t len);

/*
 * write data vectors to the ring buffer
 *
 * @param	rb	[I] pointer to the ring bufer
 * @param	data	[I] pointer to user data
 * @param	num	number of data
 * @param	try_times	[I] write event try times
 * @param	wake	[I] true if waking blocked reader
 *
 * @return	size of data written, positive if success, or negatives if fail
 */
ssize_t hm_ring_buffer_write_vector(const struct rb *rb, const struct rb_vectors_s data[], size_t num,
				   unsigned int try_times, bool wake);
/*
 * read an event data from the ring buffer
 *
 * @param	rb	[I] pointer to the ring buffer
 * @param	data	[O] pointer to user data expected to store event
 * @param	len	length of user data
 *
 * @return	size of data read, positive if success, or 0 if fail
 */
size_t hm_ring_buffer_read_event(struct rb *rb, void *data, size_t len);

/*
 * write data to the ring buffer and wake blocked reader
 *
 * @param	rb	[I] pointer to the ring bufer
 * @param	data	[I] pointer to user data
 * @param	len	length of user data, note len <= HMTRACE_MAX_ENTRY_LEN
 * @param	try_times	[I] write event try times
 *
 * @return	size of data written, positive if success, or 0 if fail
 */
size_t hm_ring_buffer_write_event_unblock(struct rb *rb, const void *data, size_t len, unsigned int try_times);

/*
 * read keep waiting until new event data written into ring buffer
 *
 * @param	rb	[I] pointer to the ring buffer
 * @param	data	[O] pointer to user data expected to store event
 * @param	len	length of user data
 * @param	try_times	[I] read event try times
 *
 * @return	size of data read, positive if success, or 0 if fail
 */
size_t hm_ring_buffer_read_event_block(struct rb *rb, void *data, size_t len, unsigned int try_times);

/*
 * destory the given ring buffer and clean related resources
 *
 * @param	rb	[I] pointer to the ring buffer
 *
 * @return	none
 */
void hm_ring_buffer_destroy(struct rb *rb);

/*
 * init a ring buffer with the given rb
 *
 * @param	rb	[I/O] pointer to ring buffer
 *
 * @return	0 if success, or negative if fail
 * @note	this api is meant to be used by client, and
 * rb->shm_id and rb->blk_num should be prepared
 */
int hm_client_ring_buffer_init(struct rb *rb);
#endif /* #ifndef ULIBS_LIBHMTRACE_RING_BUFFER_H */
