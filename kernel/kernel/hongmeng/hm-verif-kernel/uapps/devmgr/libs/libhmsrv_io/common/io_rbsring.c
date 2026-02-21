/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Ring buffer based on shm_node in hongmeng
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 22 16:51:34 2021
 */

#include <hongmeng/errno.h>
#include <libmem/utils.h>
#include <libhmsync/raw_sem.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/bitops.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libsysif/utils.h>

#include <libhmsrv_io/common/io_rbsring.h>
#include <vsync/atomic.h>
#include <vsync/queue/rbuffer.h>

struct sring_buf_allocator {
	struct rbuffer_s que;
	volatile unsigned int cont_sqe;
};

struct io_uring_info {
	unsigned int flags;
	unsigned int obj_num;
	unsigned int obj_size;

	unsigned int real_obj_size;
	unsigned int metadata_size;
	unsigned int sq_size;
	unsigned int real_cq_obj_size;
	unsigned int cq_size;
	unsigned int total_mem_size;

	unsigned int sq_offset;
	unsigned int cq_offset;

	volatile int cq_futex;
	volatile int sq_futex;

	struct rbuffer_s rb_sq;
	struct rbuffer_s rb_cq;
};

struct io_uring {
	int uring_fd;
	unsigned int flags;
	unsigned int obj_num;
	unsigned int obj_size;

	unsigned int real_obj_size;
	unsigned int metadata_size;
	unsigned int sq_size;
	unsigned int real_cq_obj_size;
	unsigned int cq_size;
	unsigned int total_mem_size;

	struct io_uring_info *shared_info;
	struct sring_buf_allocator sq;
	struct sring_buf_allocator cq;
};

static inline bool __sq_queue_is_sleep(struct io_uring *uring)
{
	if (vatomic32_inc_get_rlx((vatomic32_t *)(uintptr_t)&(uring->shared_info->sq_futex)) == 1U) {
		return true;
	}
	return false;
}

static inline bool __cq_queue_is_sleep(struct io_uring *uring)
{
	if (vatomic32_inc_get_rlx((vatomic32_t *)(uintptr_t)&(uring->shared_info->cq_futex)) == 1U) {
		return true;
	}
	return false;
}

static inline bool __uring_should_stop(const struct io_uring *ring)
{
	if ((ring->shared_info->flags & IO_URING_SHOULD_STOP) != 0) {
		return true;
	}
	return false;
}

static int __io_uring_init_sq_sring(struct io_uring *uring, bool is_import)
{
	int err = E_HM_OK;
	void *vaddr = NULL;
	struct sring_buf_allocator *alloc = &(uring->sq);

	vaddr = ptr_add(uring->shared_info, uring->shared_info->sq_offset);
	if (is_import) {
		alloc->que = uring->shared_info->rb_sq;
		alloc->que.bh = ptr_to_type(vaddr, struct buffer_head_s *);
		alloc->que.g_blk = ptr_to_type(alloc->que.bh + 1, uint8_t *);
	} else {
		err = rb_init(&alloc->que, vaddr, uring->sq_size, (uint16_t)uring->obj_num, RB_RDWR | RB_DROP_NEW);
		if (err != 0) {
			return E_HM_RESFAULT;
		}
	}
	if (alloc->que.blk_size < uring->real_obj_size) {
		return E_HM_INVAL;
	}
	alloc->cont_sqe = 0;

	return E_HM_OK;
}

static int __io_uring_init_cq_sring(struct io_uring *uring, bool is_import)
{
	int err = E_HM_OK;
	void *vaddr = NULL;
	struct sring_buf_allocator *alloc = &(uring->cq);

	if ((uring->flags & IO_URING_WITHOUT_CQ) != 0U) {
		return E_HM_OK;
	}

	vaddr = ptr_add(uring->shared_info, uring->shared_info->cq_offset);
	if (is_import) {
		alloc->que = uring->shared_info->rb_cq;
		alloc->que.bh = ptr_to_type(vaddr, struct buffer_head_s *);
		alloc->que.g_blk = ptr_to_type(alloc->que.bh + 1, uint8_t *);
	} else {
		err = rb_init(&alloc->que, vaddr, uring->cq_size, (uint16_t)uring->obj_num, RB_RDWR | RB_DROP_NEW);
		if (err != 0) {
			return E_HM_RESFAULT;
		}
	}
	if (alloc->que.blk_size < uring->real_cq_obj_size) {
		return E_HM_INVAL;
	}
	alloc->cont_sqe = 0;

	return E_HM_OK;
}

static void __init_uring_info(struct io_uring *uring)
{
	uring->shared_info->flags = uring->flags;
	uring->shared_info->obj_num = uring->obj_num;
	uring->shared_info->obj_size = uring->obj_size;
	uring->shared_info->real_obj_size = uring->real_obj_size;
	uring->shared_info->metadata_size = uring->metadata_size;
	uring->shared_info->sq_size = uring->sq_size;
	uring->shared_info->real_cq_obj_size = uring->real_cq_obj_size;
	uring->shared_info->cq_size = uring->cq_size;
	uring->shared_info->total_mem_size = uring->total_mem_size;

	uring->shared_info->sq_futex = 0;
	uring->shared_info->cq_futex = 0;

	uring->shared_info->sq_offset = uring->metadata_size; /* metadata offset is 0 */
	uring->shared_info->cq_offset = uring->sq_size + uring->shared_info->sq_offset;
}

static inline void __real_sring_obj_size(unsigned int obj_size, unsigned int obj_cnt,
					 unsigned int *real_obj_size, unsigned int *sring_size)
{
	*real_obj_size = (unsigned int)(obj_size + sizeof(struct entry_s) + sizeof(struct block_head_s));
	*sring_size = (unsigned int)PAGE_ALIGN_UP((*real_obj_size) * obj_cnt + sizeof(struct buffer_head_s));
}

static inline unsigned int __uring_queue_futex_type(struct io_uring_info *shared_info)
{
	return (shared_info->flags & IO_URING_INSIDE_PROCESS) ? FUTEX_TYPE_PRIVATE : FUTEX_TYPE_SHARED;
}

#define URING_OBJ_MAX_SIZE	(256U)
#define URING_OBJ_MAX_NUM	(1024U * 32U)
static int __alloc_uring_total_mem(struct io_uring *uring)
{
	unsigned int total_mem_size;
	unsigned int real_obj_size;
	unsigned int sq_size;
	unsigned int cq_size;
	int shm_fd;
	int err;
	unsigned int metadata_size = PAGE_SIZE;
	unsigned int obj_size = uring->obj_size;
	unsigned int obj_cnt = uring->obj_num;
	void *vaddr = NULL;

	if ((metadata_size < sizeof(struct io_uring_info)) ||
	    (obj_size > URING_OBJ_MAX_SIZE) ||
	    (obj_cnt > URING_OBJ_MAX_NUM)) {
		hm_error("uring obj info is invalid\n");
		return E_HM_INVAL;
	}
	uring->metadata_size = metadata_size;

	__real_sring_obj_size(obj_size, obj_cnt, &real_obj_size, &sq_size);
	uring->sq_size = sq_size;
	uring->real_obj_size = real_obj_size;

	__real_sring_obj_size((unsigned int)sizeof(struct uring_cqe), obj_cnt, &real_obj_size, &cq_size);
	cq_size = ((uring->flags & IO_URING_WITHOUT_CQ) != 0) ? 0 : cq_size;
	uring->cq_size = cq_size;
	uring->real_cq_obj_size = real_obj_size;

	total_mem_size = metadata_size + sq_size + cq_size;
	shm_fd = hm_ashm_open_auth(0, 0U, total_mem_size, SHM_O_CREAT | SHM_O_RDWR, NULL);
	if (shm_fd < 0) {
		hm_error("alloc mem for uring failed, ret = %d\n", shm_fd);
		return E_HM_NOMEM;
	}
	vaddr = hm_mem_mmap_posix(NULL, total_mem_size, PROT_READ | PROT_WRITE,
				  MAP_SHARED | MAP_ANONYMOUS, shm_fd, 0, &err);
	if (err < 0) {
		hm_error("map uring shared mem failed, ret = %d\n", err);
		(void)hm_ashm_close(shm_fd);
		(void)hm_ashm_unlink(shm_fd);
		return E_HM_RESFAULT;
	}
	uring->uring_fd = shm_fd;
	uring->total_mem_size = total_mem_size;
	uring->shared_info = vaddr;

	__init_uring_info(uring);
	return E_HM_OK;
}

struct io_uring *io_uring_create(unsigned int item_cnt, unsigned int item_size, unsigned int flags)
{
	struct io_uring *uring = NULL;
	int err = E_HM_INVAL;

	uring = malloc(sizeof(struct io_uring));
	if (uring == NULL) {
		return NULL;
	}
	mem_zero_b(uring, sizeof(struct io_uring));

	uring->flags = flags;
	uring->obj_num = item_cnt;
	uring->obj_size = sizeof(struct uring_sqe) + item_size;
	err = __alloc_uring_total_mem(uring);
	if (err != E_HM_OK) {
		free(uring);
		return NULL;
	}

	err = __io_uring_init_sq_sring(uring, false);
	if (err != E_HM_OK) {
		hm_error("create sq ring failed, ret = %d\n", err);
		io_uring_destroy(uring);
		return NULL;
	}
	err = __io_uring_init_cq_sring(uring, false);
	if (err != E_HM_OK) {
		hm_error("create cq ring failed, ret = %d\n", err);
		io_uring_destroy(uring);
		return NULL;
	}
	uring->shared_info->rb_sq = uring->sq.que;
	uring->shared_info->rb_cq = uring->cq.que;

	return uring;
}

void io_uring_stop(struct io_uring *uring)
{
	if ((uring == NULL) || (uring->shared_info == NULL)) {
		return;
	}
	uring->shared_info->flags |= IO_URING_SHOULD_STOP;
	/* send a dummy msg to cq & sq */
	(void)__sq_queue_is_sleep(uring);
	(void)hm_futex_wake(&(uring->shared_info->sq_futex), 1, __uring_queue_futex_type(uring->shared_info));
	(void)__cq_queue_is_sleep(uring);
	(void)hm_futex_wake(&(uring->shared_info->cq_futex), 1, __uring_queue_futex_type(uring->shared_info));
}

void io_uring_destroy(struct io_uring *uring)
{
	if (uring == NULL) {
		return;
	}
	(void)hm_mem_munmap(uring->shared_info, uring->total_mem_size);
	(void)hm_ashm_close(uring->uring_fd);
	(void)hm_ashm_unlink(uring->uring_fd);
	free(uring);
}

int io_uring_grant_to_caller(const struct io_uring *uring, uint64_t *ret_key)
{
	int ret;
	if (uring == NULL) {
		return E_HM_INVAL;
	}

	if (sysif_actv_src_cnode_idx() == hm_ucap_self_cnode_idx()) {
		uring->shared_info->flags |= IO_URING_INSIDE_PROCESS;
		*ret_key = ptr_to_u64(uring->shared_info);
		return E_HM_OK;
	}

	ret = hm_ashm_grant_to_caller(uring->uring_fd, SHM_GRANT_RDWR, ret_key);
	if (ret < 0) {
		hm_error("grant io uring to caller failed\n");
		return ret;
	}

	return ret;
}

int io_uring_grant_by_xref(const struct io_uring *uring, xref_t xref, uint64_t *ret_key)
{
	int ret;
	if (uring == NULL) {
		return E_HM_INVAL;
	}

	ret = hm_ashm_grant(uring->uring_fd, SHM_GRANT_RDWR, xref, ret_key);
	if (ret < 0) {
		hm_error("grant io uring by xref failed\n");
		return ret;
	}

	return ret;
}

static struct io_uring_info *__import_all_shm_mem(uint64_t key, size_t size, unsigned int cnode_idx,
						  int *ret_id)
{
	int err;
	int shm_id;
	void *vaddr = NULL;
	size_t shm_len;

	if (cnode_idx == hm_ucap_self_cnode_idx()) {
		struct io_uring_info *uring = u64_to_ptr(key, struct io_uring_info);
		if (!(uring->flags & IO_URING_INSIDE_PROCESS)) {
			hm_error("shared_info bad flags %x\n", uring->flags);
			return NULL;
		}

		*ret_id = -1;
		return uring;
	}

	shm_id = hm_ashm_open_auth(key, cnode_idx, 0, SHM_O_RDWR, &shm_len);
	if (shm_id < 0) {
		hm_error("open shm fd %"PRIu64" failed %s\n", key, hmstrerror(shm_id));
		return NULL;
	}
	if (size > shm_len) {
		(void)hm_ashm_close(shm_id);
		return NULL;
	}

	vaddr = hm_mem_mmap_posix(NULL, shm_len, PROT_READ | PROT_WRITE,
				  MFLAG_MAP_SHARED | MFLAG_MAP_ANONYMOUS, shm_id, 0, &err);
	if (err < 0) {
		hm_error("map uring failed %s\n", hmstrerror(err));
		(void)hm_ashm_close(shm_id);
		return NULL;
	}

	*ret_id = shm_id;
	return (struct io_uring_info *)vaddr;
}

struct io_uring *io_uring_import(uint64_t fd, uint32_t cnode_idx)
{
	int err;
	int shm_id;
	struct io_uring *uring = NULL;
	struct io_uring_info *uring_info = NULL;

	uring = (struct io_uring *)malloc(sizeof(struct io_uring));
	if (uring == NULL) {
		return NULL;
	}
	mem_zero_b(uring, sizeof(struct io_uring));

	uring_info = __import_all_shm_mem(fd, sizeof(struct io_uring_info), cnode_idx,
					  &shm_id);
	if (uring_info == NULL) {
		free(uring);
		return NULL;
	}
	uring->shared_info = uring_info;
	uring->uring_fd = shm_id;
	uring->flags = uring_info->flags;
	uring->obj_num = uring_info->obj_num;
	uring->obj_size = uring_info->obj_size;
	uring->real_obj_size = uring_info->real_obj_size;
	uring->metadata_size = uring_info->metadata_size;
	uring->sq_size = uring_info->sq_size;
	uring->real_cq_obj_size = uring_info->real_cq_obj_size;
	uring->cq_size = uring_info->cq_size;
	uring->total_mem_size = uring_info->total_mem_size;

	err = __io_uring_init_sq_sring(uring, true);
	if (err != E_HM_OK) {
		hm_error("init sq ring failed, ret = %d\n", err);
		io_uring_destroy(uring);
		return NULL;
	}
	err = __io_uring_init_cq_sring(uring, true);
	if (err != E_HM_OK) {
		hm_error("init cq ring failed, ret = %d\n", err);
		io_uring_destroy(uring);
		return NULL;
	}

	return uring;
}

#define POLLING_READ_TRY_TIMES	(10)
#define POLLING_WRITE_TRY_TIMES	(1000)
int io_uring_submit_sqe(struct io_uring *uring, struct uring_sqe *sqe, unsigned int submit_sz)
{
	int err;
	size_t sz;
	if ((uring == NULL) || (sqe == NULL) ||
	    (submit_sz > uring->obj_size)) {
		return E_HM_INVAL;
	}
	if (__uring_should_stop(uring)) {
		return E_HM_NOMSG;
	}

	sz = rb_write_entry(&uring->sq.que, sqe, submit_sz, POLLING_WRITE_TRY_TIMES);
	if (sz == 0U) {
		hm_error("submit sqe failed\n");
		return E_HM_NOMEM;
	}

	if (__sq_queue_is_sleep(uring)) {
		err = hm_futex_wake(&(uring->shared_info->sq_futex), 1, __uring_queue_futex_type(uring->shared_info));
		if (err < 0) {
			hm_error("notify customer failed\n");
			return err;
		}
	}
	return E_HM_OK;
}

static size_t __uring_try_read_ent(struct rbuffer_s *rb, void *ent, unsigned int sz)
{
	size_t size = 0;
	int try_read_timers = POLLING_READ_TRY_TIMES;
	do {
		size = rb_try_read_entry(rb, ent, sz, NULL);
		try_read_timers--;
	} while (size == 0U && try_read_timers != 0);

	return size;
}

#define WAKE_CQ_THREAD_THRESHOLD	(10U)
#define CQ_WARN_FLUSH_RATIO		(3U)
int io_uring_wait_sqe(struct io_uring *uring, struct uring_sqe *sqe, unsigned int read_sz)
{
	size_t sz;

	if ((uring == NULL) || (sqe == NULL) ||
	    (read_sz > uring->obj_size)) {
		return E_HM_INVAL;
	}

	do {
		sz = __uring_try_read_ent(&uring->sq.que, sqe, read_sz);
		if (sz == 0U) {
			if (uring->cq.cont_sqe % WAKE_CQ_THREAD_THRESHOLD != 0U) {
				(void)hm_futex_wake(&(uring->shared_info->cq_futex), 1, __uring_queue_futex_type(uring->shared_info));
			}

			if (__uring_should_stop(uring)) {
				return E_HM_NOMSG;
			}
			(void)hm_futex_wait(&(uring->shared_info->sq_futex), 0, NULL, __uring_queue_futex_type(uring->shared_info));
			continue;
		}
	} while (sz == 0U);
	vatomic32_dec_rlx((vatomic32_t *)(uintptr_t)&(uring->shared_info->sq_futex));

	return E_HM_OK;
}

int io_uring_wait_cqe(struct io_uring *uring, struct uring_cqe *cqe)
{
	size_t sz = 0;

	if ((uring == NULL) || (cqe == NULL)) {
		return E_HM_INVAL;
	}
	if ((uring->flags & IO_URING_WITHOUT_CQ) != 0U) {
		return E_HM_NOMSG;
	}

	do {
		sz = __uring_try_read_ent(&uring->cq.que, cqe, sizeof(*cqe));
		if (sz == 0U) {
			if (__uring_should_stop(uring)) {
				return E_HM_NOMSG;
			}
			(void)hm_futex_wait(&(uring->shared_info->cq_futex), 0, NULL, __uring_queue_futex_type(uring->shared_info));
			continue;
		} else {
			vatomic32_dec_rlx((vatomic32_t *)(uintptr_t)&(uring->shared_info->cq_futex));
		}
	} while (sz == 0U);
	return E_HM_OK;
}

int io_uring_complete_sqe(struct io_uring *uring, const struct uring_sqe *sqe, int err)
{
	size_t sz;
	int ret;
	unsigned int continue_cnt = 0;
	struct uring_cqe cqe;

	if ((uring == NULL) || (sqe == NULL)) {
		return E_HM_INVAL;
	}
	if ((uring->flags & IO_URING_WITHOUT_CQ) != 0U) {
		return E_HM_NOSYS;
	}

	cqe.result = err;
	cqe.user_data = sqe->user_data;
	sz = rb_write_entry(&uring->cq.que, &cqe, sizeof(cqe), POLLING_WRITE_TRY_TIMES);
	if (sz == 0U) {
		hm_error("submit cqe failed\n");
		return E_HM_NOMEM;
	}
	/*
	 * 1. the `rb_write_entry` and `__cq_queue_is_sleep` is not atomic.
	 * so `__cq_queue_is_sleep` may be misjudge.
	 * 2. operation of futex with `FUTEX_TYPE_SHARED` is slow.
	 * so here we need deferred notification.
	 * 3. misjudge and deferred notification may case that opposite sleep forever.
	 * so add one warning-level to slow down the write and send SOS.
	 */
	if (__cq_queue_is_sleep(uring)) {
		 continue_cnt = vatomic32_inc_get_rlx((vatomic32_t *)(uintptr_t)&uring->cq.cont_sqe);
		if (continue_cnt % WAKE_CQ_THREAD_THRESHOLD != 0U) {
			/* flush when sq sleep or reach warning level */
			return E_HM_OK;
		}
		ret = hm_futex_wake(&(uring->shared_info->cq_futex), 1, __uring_queue_futex_type(uring->shared_info));
		if (ret < 0) {
			hm_error("notify producer\n");
			return ret;
		}
	} else {
		if ((unsigned int)uring->shared_info->cq_futex > uring->obj_num / CQ_WARN_FLUSH_RATIO) {
			(void)hm_futex_wake(&(uring->shared_info->cq_futex), 1, __uring_queue_futex_type(uring->shared_info));
		}
	}
	return E_HM_OK;
}
