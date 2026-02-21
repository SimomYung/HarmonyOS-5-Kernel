/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Provide user mode ring buffer function for hongmeng
 * Author: Huawei OS Kernel Lab
 * Create: Tue 03 Sep 2019 05:45:03 PM CST
 */

#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>
#include <libhmtrace/ring_buffer/ring_buffer.h>
#include <libhmsrv_sys/hm_futex.h>
#include <libhmsync/atomic.h>
#include <vsync/queue/rbuffer.h>
#include <vsync/atomic.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <fcntl.h>

#ifdef CONFIG_HMTRACE
#define WRITE_UNBLOCK_VALUE	100
#define WRITE_UNBLOCK_ACTIVE	0
#define WRITE_UNBLOCK_NUM	1
#define RB_OFFSET_IN_SHM	8

static void prepare_params(uint64_t *shm_oflags, uint32_t *mmap_prot, uint32_t *rb_mode, const struct rb *rb)
{
	/*
	 * Non-creator also needs to write to ring buffer. Because
	 * some read info is in the header of ring buffer.
	 */
	*mmap_prot = PROT_READ | PROT_WRITE;

	if (rb->is_creator) {
		*shm_oflags = O_CREAT | O_RDWR;
		*rb_mode = RB_RDWR;
	} else {
		*shm_oflags = O_RDWR;
		*rb_mode = RB_RDONLY;
	}

	if (rb->flags == RB_DROP_MODE) {
		*rb_mode |= RB_DROP_NEW;
	}
}

#ifdef __HOST_UT__
static int hm_ring_buffer_init_ret;
void set_hm_ring_buffer_init_ret(int ret)
{
	hm_ring_buffer_init_ret = ret;
}

static int __hm_ring_buffer_init(struct rb *rb)
{
	return hm_ring_buffer_init_ret;
}
#else
static int __hm_ring_buffer_init(struct rb *rb)
{
	int rc = E_HM_INVAL;
	uint32_t mmap_prot = 0U;
	uint64_t shm_oflags = 0U;
	uint32_t rb_mode = 0U;
	size_t ret_len = 0U;

	rb->data = (void *)malloc(sizeof(struct rbuffer_s));
	if (rb->data == NULL) {
		hm_warn("malloc rb data failed, no mem\n");
		goto fail_malloc;
	}

	prepare_params(&shm_oflags, &mmap_prot, &rb_mode, rb);

	rc = hm_ashm_open_auth(rb->key, rb->cnode_idx, rb->rb_sz, shm_oflags, &ret_len);
	if (rc < 0) {
		hm_warn("shm open failed: %s\n", hmstrerror(rc));
		goto fail_shm_open;
	}
	rb->shm_id = rc;
	rb->rb_sz = ret_len;

	rb->map_buf = hm_mem_mmap(NULL, ret_len, mmap_prot,
				  MAP_SHARED | MAP_ANONYMOUS,
				  rb->shm_id, 0);
	if (rb->map_buf == MAP_FAILED) {
		hm_warn("mmap failed\n");
		rc = E_HM_INVAL;
		goto fail_mmap;
	}
	/*
	 *   +------+
	 *   |FUTEX |
	 *   |SHARED|----------v
	 *   +------+        +---+---+---+---+----+
	 *                   | 8 |TRUE RING BUFFER|
	 *                   +---+---+---+---+----+
	 *                   ^                    ^
	 *                   |                    |
	 *                   +---SHARED MEMORY----+
	 */
	rc = rb_init((struct rbuffer_s *)rb->data, (void *)((uintptr_t)rb->map_buf + RB_OFFSET_IN_SHM),
		     rb->rb_sz, rb->blk_num, rb_mode);
	if (rc < 0) {
		hm_warn("init rb failed\n");
		rc = E_HM_INVAL;
		goto fail_rb_init;
	}

	rb->private.futex_shared = (int *)rb->map_buf;
	return E_HM_OK;

fail_rb_init:
	(void)hm_mem_munmap(rb->map_buf, rb->rb_sz);
fail_mmap:
	(void)hm_ashm_close(rb->shm_id);
	(void)hm_ashm_unlink(rb->shm_id);
fail_shm_open:
	free(rb->data);
	rb->data = NULL;
fail_malloc:
	return rc;
}
#endif

struct rb *hm_ring_buffer_create(size_t size, uint32_t flags)
{
	int rc;
	struct rb *rb = NULL;
	unsigned int zeros;

	if (__builtin_popcountl((unsigned long)size) != 1) {
		hm_warn("invalid size\n");
		return NULL;
	}

	rb = (struct rb *)malloc(sizeof(struct rb));
	if (rb == NULL) {
		hm_warn("malloc rb failed, no mem\n");
		return NULL;
	}

	mem_zero_s(*rb);
	rb->rb_sz = size;
	rb->flags = flags;
	rb->is_creator = true;

	zeros = (unsigned int)__builtin_ctzl((unsigned long)size);
	/*
	 * blk_num should be power of 2.
	 * User should be carefully with the blk_num to make block size greater than the maximum entry size.
	 * If size<=(1<<22), sets blk_num to 1<<(zeros/3), which is [16,16,16,32,32,32,64,64,64,128,128]
	 * when the number of digits of size ranges from 12 to 22.
	 * Otherwise, sets blk_num to rb_sz>>15, which starts from 256 and has been increased by a multiple of 2.
	 */
	rb->blk_num = zeros > 22 ? (uint16_t)(((unsigned int)rb->rb_sz) >> 15) : (uint16_t)(1 << (zeros / 3));
	rc = __hm_ring_buffer_init(rb);
	if (rc < 0) {
		free(rb);
		return NULL;
	}

	return rb;
}

size_t hm_ring_buffer_write_event(struct rb *rb, void *data, size_t len)
{
	if (rb == NULL || data == NULL || len > HMTRACE_MAX_ENTRY_LEN) {
		return 0;
	}
	return rb_write_entry((struct rbuffer_s *)rb->data, data,
			      len, HMTRACE_DEFAULT_TRY_TIME);
}

size_t hm_ring_buffer_read_event(struct rb *rb, void *data, size_t len)
{
	if (rb == NULL || data == NULL) {
		return 0;
	}

	size_t size = 0;
	int try_times = 0;

	do {
		size = rb_try_read_entry((struct rbuffer_s *)rb->data,
					 data, len, NULL);
		try_times++;
	} while (size == 0 && try_times < HMTRACE_DEFAULT_TRY_TIME);

	return size;
}

static int water_line_add(const struct rb *rb, size_t len, struct entry_s *entry, struct rbuffer_s *rbuffer)
{
	int ret = E_HM_OK;
	int water_size = 0;

	water_size = rb_get_write_offset(rbuffer, entry, len);
	hm_debug("water size is %d\n", water_size);
	if (water_size > WRITE_UNBLOCK_VALUE) {
		ret = hm_futex_wake(rb->private.futex_shared, WRITE_UNBLOCK_NUM, FUTEX_TYPE_SHARED);
		if (ret < 0) {
			hm_debug("hm_futex_wake num is %d\n", ret);
			return E_HM_INVAL;
		}
	}
	hm_debug("hm_futex_wake end\n");
	*rb->private.futex_shared = WRITE_UNBLOCK_ACTIVE;
	return E_HM_OK;
}

static ssize_t rb_get_data_size(const struct rb_vectors_s *data, size_t num, size_t max_size)
{
	size_t i;
	size_t size = 0;
	if (data == NULL) {
		return E_HM_INVAL;
	}
	for (i = 0; i < num; i++) {
		if (unlikely(data[i].vector_buf == NULL)) {
			return E_HM_INVAL;
		}
		if (unlikely(data[i].vector_len == 0)) {
			return E_HM_INVAL;
		}
		if (unlikely(max_size - size < data[i].vector_len)) {
			return E_HM_NOMEM;
		}
		size += data[i].vector_len;
	}
	return (ssize_t)size;
}

ssize_t hm_ring_buffer_write_vector(const struct rb *rb, const struct rb_vectors_s data[], size_t num,
				    unsigned int try_times, bool wake)
{
	ssize_t err;
	unsigned int cnt = 0;
	struct entry_s *entry = NULL;
	struct rbuffer_s *rbuffer = NULL;
	size_t size = 0;

	if (rb == NULL) {
		return E_HM_INVAL;
	}
	rbuffer = (struct rbuffer_s *)rb->data;
	if (rbuffer == NULL) {
		return E_HM_INVAL;
	}
	err = rb_get_data_size(data, num, HMTRACE_MAX_ENTRY_LEN);
	if (err < 0) {
		return err;
	}
	size = (size_t)err;
	do {
		entry = rb_try_alloc_entry(rbuffer, size);
		cnt++;
	} while (entry == NULL && cnt < try_times);

	if (entry == NULL) {
		hm_warn("alloc entry of ring buffer failed\n");
		return E_HM_NOMEM;
	}
	size_t i;
	void *dst = entry_get_data(entry);
	for (i = 0; i < num; i++) {
		NOFAIL(memcpy_s(dst, data[i].vector_len, data[i].vector_buf, data[i].vector_len));
		dst = (char *)dst + data[i].vector_len;
	}
	entry_commit(rbuffer, entry);
	if (wake) {
		err = (ssize_t)water_line_add(rb, size, entry, rbuffer);
		if (err != E_HM_OK) {
			hm_warn("write event size water failed\n");
		}
	}
	return (ssize_t)size;
}

size_t hm_ring_buffer_write_event_unblock(struct rb *rb, const void *data, size_t len, unsigned int try_times)
{
	int err = E_HM_OK;
	unsigned int cnt = 0;
	struct entry_s *entry = NULL;
	struct rbuffer_s *rbuffer = NULL;

	if (rb == NULL) {
		return 0;
	}
	rbuffer = (struct rbuffer_s *)rb->data;
	if (rbuffer == NULL) {
		return 0;
	}

	do {
		entry = rb_try_alloc_entry(rbuffer, len);
		cnt++;
	} while (entry == NULL && cnt < try_times);

	if (entry == NULL) {
		hm_warn("alloc entry of ring buffer failed\n");
		return 0;
	}

	void *dst = entry_get_data(entry);
	err = memcpy_s(dst, len, data, len);
	if (err != 0) {
		hm_warn("copy event data to ring buffer failed, ret=%d\n", err);
		return 0;
	}
	entry_commit(rbuffer, entry);

	/* if water line is full wake */
	err = water_line_add(rb, len, entry, rbuffer);
	if (err != E_HM_OK) {
		hm_warn("write event size water failed\n");
	}

	return len;
}

size_t hm_ring_buffer_read_event_block(struct rb *rb, void *data, size_t len, unsigned int try_times)
{
	int err = E_HM_OK;

	if (rb == NULL || data == NULL) {
		return 0;
	}
	size_t size = 0;
	unsigned int cnt;

again:
	cnt = 0;
	do {
		size = rb_try_read_entry((struct rbuffer_s *) rb->data,
					 data, len, NULL);
		cnt++;
	} while (size == 0 && cnt < try_times);

	/*
	 * If reader try to read several times, but it still doesn't get any data.
	 * Then ring buffer is supposed to be empty and reader goes to futex wait
	 * until writer wakes it up and then tries to read again.
	 */
	if (size == 0) {
		err = hm_futex_wait(rb->private.futex_shared, 0, NULL, FUTEX_TYPE_SHARED);
		if (err > 0) {
			hm_warn("hm_futex_wake failed, ret = %d\n", err);
		}
		hm_debug("hm_ring_buffer_read_event wake end\n");
		goto again;
	}

	return size;
}

void hm_ring_buffer_destroy(struct rb *rb)
{
	if (rb == NULL) {
		return;
	}

	(void)hm_mem_munmap(rb->map_buf, rb->rb_sz);
	(void)hm_ashm_close(rb->shm_id);

	if (rb->is_creator) {
		/* only creator can unlink shm */
		(void)hm_ashm_unlink(rb->shm_id);
	}

	free(rb->data);

	if (rb->is_creator) {
		/*
		 * When non-creator inits rb, it doesn't malloc
		 * rb during the call. So only creator needs to
		 * free rb.
		 */
		free(rb);
	}
}

int hm_client_ring_buffer_init(struct rb *rb)
{
	int rc = E_HM_OK;

	if (rb == NULL) {
		hm_warn("rb is NULL\n");
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rb->rb_sz = 0;
		rb->is_creator = false;
		rb->flags = RB_OVERWRITE_MODE;
		rc = __hm_ring_buffer_init(rb);
	}
	return rc;
}
#else /* CONFIG_HMTRACE */
struct rb *hm_ring_buffer_create(size_t size, uint32_t flags)
{
	UNUSED(size);
	UNUSED(flags);
	return NULL;
}

size_t hm_ring_buffer_write_event(struct rb *rb, void *data, size_t len)
{
	UNUSED(rb, data, len);
	return 0;
}

size_t hm_ring_buffer_read_event(struct rb *rb, void *data, size_t len)
{
	UNUSED(rb, data, len);
	return 0;
}

ssize_t hm_ring_buffer_write_vector(const struct rb *rb, const struct rb_vectors_s data[], size_t num,
				    unsigned int try_times, bool wake)
{
	UNUSED(rb, data, num, try_times, wake);
	return 0;
}

size_t hm_ring_buffer_write_event_unblock(struct rb *rb, const void *data, size_t len, unsigned int try_times)
{
	UNUSED(rb, data, len, try_times);
	return 0;
}

size_t hm_ring_buffer_read_event_block(struct rb *rb, void *data, size_t len, unsigned int try_times)
{
	UNUSED(rb, data, len, try_times);
	return 0;
}

void hm_ring_buffer_destroy(struct rb *rb)
{
	UNUSED(rb);
}

int hm_client_ring_buffer_init(struct rb *rb)
{
	UNUSED(rb);
	return E_HM_NOSYS;
}
#endif
