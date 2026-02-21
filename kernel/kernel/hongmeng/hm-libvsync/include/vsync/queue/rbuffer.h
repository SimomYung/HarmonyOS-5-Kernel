/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2024. All rights reserved.
 * Description: efficent non-blocking ring buffer
 * Author: Huawei Dresden Research Center
 * Create: Tue Sep 15 10:13:59 2020
 */
#ifndef VQUEUE_RBUFFER_H
#define VQUEUE_RBUFFER_H
/*******************************************************************************
 * @file rbuffer.h
 * @brief A formally verified and efficient ring buffer.
 *
 * rbuffer provides almost non-blocking multi-producer and speculative
 * single-consumer. The C code has been formally verified and optimized with
 * vsyncer on weak memory models
 *
 * @example
 * @include eg_rbuffer.c
 ******************************************************************************/
#include <vsync/atomic.h>
#include <vsync/common/compiler.h>
#include <vsync/utils/string.h>

#ifdef VSYNC_VERIFICATION
void _rb_cpy_buff(void *dst, vsize_t dstsz, const void *src, vsize_t n)
{
	for (int i = 0; i < n / sizeof(int); i++) {
		int tmp = vatomic32_read_rlx(
			(vatomic32_t *)(((const int *)src) + i));
		vatomic32_write_rlx((vatomic32_t *)(((int *)dst) + i), tmp);
	}
}
#endif

#define ALIGNMENT_MASK 0x7U
#define RB_ALIGN_DOWN(a) ((a) & (~ALIGNMENT_MASK))
#define MIN_BLOCK_NUM 2
#define BACKWARD_STEP 1
#define ENTRY_ALIGN_BIT 2
#define ENTRY_ALIGN_MASK ((1U << ENTRY_ALIGN_BIT) - 1)
#define MIN_ALLOC_SIZE 8
#define MAX_ALLOC_SIZE(blksz) \
	(((blksz) - sizeof(struct block_head_s) - sizeof(struct entry_s)))
#define MIN_BLOCK_SIZE \
	(sizeof(struct block_head_s) + sizeof(struct entry_s) + MIN_ALLOC_SIZE)
#define BLOCK_DATA_SIZE(blksz) ((blksz) - sizeof(struct block_head_s))
#define ENTRY_INDEX_BITS 16
#define ENTRY_INDEX_MASK ((1U << ENTRY_INDEX_BITS) - 1)
#define WRITE_INDEX_SHIFT 1
#define WRITE_INDEX_MASK ((1U << WRITE_INDEX_SHIFT) - 1U)
#define RB_TRUE 1
#define RB_FALSE 0
#define RB_RDWR 0
#define RB_RDONLY 1
#define RB_MODE(flags) ((flags)&RB_RDONLY)
#define RB_DROP_NEW 2
#define RB_DROP(flags) ((flags)&RB_DROP_NEW)
#define ALIGN_DOWN_WITH_POWER_2(a) \
	(1U << (32 - __builtin_clz((vuint32_t)(a)) - 1))

typedef struct entry_s {
	vatomic32_t blk_idx;
#ifdef VSYNC_VERIFICATION
	vatomic32_t next;
	vatomic32_t size;
#else
	vuint16_t next;
	vuint16_t size;
#endif
} entry_t;

typedef struct read_info_s {
	vuint64_t index;
	vuint32_t total;
	vuint32_t blk_read_cnt;
} read_info_t;

/* block index is 16 bit, so the block size is limited to 64k */
typedef struct block_head_s {
	vatomic32_t alloc_idx;
	vatomic32_t commit_num;
} block_head_t;

typedef struct buffer_head_s {
	vatomic64_t cur_wblk;
	vatomic64_t cur_rblk;
	vatomic32_t blk_read_cnt;
	vatomic32_t blk_read_idx;
} buffer_head_t;

typedef struct rbuffer_s {
	struct buffer_head_s *bh;
	vuint8_t *g_blk;
	vsize_t total_size;
	vuint16_t total_blks;
	vuint16_t blk_size;
	vuint16_t move_step;
	vuint16_t flags;
} rbuffer_t;

/******* Auxiliary functions ******/

static inline vuint16_t size_align(vuint16_t size)
{
	vuint16_t entry_size = size + sizeof(struct entry_s);
	return (entry_size + ENTRY_ALIGN_MASK) & (~ENTRY_ALIGN_MASK);
}

static inline vuint64_t get_write_idx(vuint64_t widx)
{
	return (widx >> WRITE_INDEX_SHIFT);
}

static inline vuint64_t get_busy_bit(vuint64_t widx)
{
	return (widx & WRITE_INDEX_MASK);
}

static inline vuint64_t set_busy_bit(vuint64_t widx)
{
	return (widx | WRITE_INDEX_MASK);
}

static inline int is_busy(vuint64_t widx)
{
	return get_busy_bit(widx);
}

static inline vuint64_t get_next_idx(vuint64_t widx)
{
	return get_write_idx(widx) + 1;
}

static inline vuint64_t get_next_write_idx(vuint64_t widx)
{
	vuint64_t idx = get_next_idx(widx);
	/* ready bit always 0 when get next */
	return idx << WRITE_INDEX_SHIFT;
}

static inline vuint16_t allocidx_of(vuint32_t idx)
{
	return (vuint16_t)(idx & ENTRY_INDEX_MASK);
}

static inline vuint16_t allocnum_of(vuint32_t idx)
{
	return (vuint16_t)(idx >> ENTRY_INDEX_BITS);
}

static inline vuint32_t get_alloc_idx(vuint16_t alloc, vuint16_t idx)
{
	return (vuint32_t)(((vuint32_t)alloc << ENTRY_INDEX_BITS) | idx);
}

static inline vuint16_t convert_idx(struct rbuffer_s *rb, vuint64_t idx)
{
	return (vuint16_t)(idx & (rb->total_blks - 1));
}

static inline struct block_head_s *idx_to_block(struct rbuffer_s *rb,
						vuint16_t idx)
{
	return (struct block_head_s *)(rb->g_blk + (idx * rb->blk_size));
}

static inline struct block_head_s *global_idx_to_block(struct rbuffer_s *rb,
						       vuint64_t index)
{
	vuint16_t idx = convert_idx(rb, index);
	return idx_to_block(rb, idx);
}

/*** ENTRY functions *****/
static inline vuint16_t entry_get_blk_idx(struct entry_s *entry)
{
	return vatomic32_read_rlx(&entry->blk_idx);
}

static inline void entry_set_blk_idx(struct entry_s *entry, vuint16_t idx)
{
	vatomic32_write_rlx(&entry->blk_idx, idx);
}

static inline vuint16_t entry_get_next(struct entry_s *entry)
{
#ifdef VSYNC_VERIFICATION
	return (vuint16_t)vatomic32_read_rlx(&entry->next);
#else
	return entry->next;
#endif
}

static inline void entry_set_next(struct entry_s *entry, vuint16_t next)
{
#ifdef VSYNC_VERIFICATION
	vatomic32_write_rlx(&entry->next, next);
#else
	entry->next = next;
#endif
}

static inline vuint16_t entry_get_size(struct entry_s *entry)
{
#ifdef VSYNC_VERIFICATION
	return (vuint16_t)vatomic32_read_rlx(&entry->size);
#else
	return entry->size;
#endif
}

static inline void entry_set_size(struct entry_s *entry, vuint16_t size)
{
#ifdef VSYNC_VERIFICATION
	vatomic32_write_rlx(&entry->size, size);
#else
	entry->size = size;
#endif
}

static inline void *entry_get_data(struct entry_s *entry)
{
	return (void *)(entry + 1);
}

static inline void entry_alloc(struct entry_s *entry, vuint16_t bidx,
			       vuint16_t next, vuint16_t size)
{
	entry_set_size(entry, size);
	entry_set_next(entry, next);
	entry_set_blk_idx(entry, bidx);
}

static inline void entry_commit(struct rbuffer_s *rb, struct entry_s *entry)
{
	vuint16_t bidx = entry_get_blk_idx(entry);
	struct block_head_s *blk = idx_to_block(rb, bidx);
	vatomic32_inc_rel(&blk->commit_num);
}

static inline int entry_is_invalid(struct block_head_s *blk,
				   struct entry_s *entry, vuint16_t blk_size)
{
	if (unlikely(entry == NULL)) {
		return RB_TRUE;
	}
	vuint16_t size = entry_get_size(entry);
	if (size < MIN_ALLOC_SIZE || size > MAX_ALLOC_SIZE(blk_size)) {
		return RB_TRUE;
	}
	vuint8_t *cur = (vuint8_t *)entry;
	if (cur > (vuint8_t *)blk + blk_size) {
		return RB_TRUE;
	}
	vuint8_t *next = cur + entry_get_next(entry);
	if (next > (vuint8_t *)blk + blk_size) {
		return RB_TRUE;
	}
	return RB_FALSE;
}

/*** BLOCK functions *****/
static inline vuint16_t
block_atomic_alloc_index(vatomic32_t *idx, vuint16_t offset, vuint16_t bound)
{
	vuint32_t cur, next;
	vuint32_t newidx = 0;
	int done = RB_FALSE;

	do {
		cur = vatomic32_read_rlx(idx);
		if (allocidx_of(cur) >= bound) {
			/* Because the block header is read with a relaxed access, it is
             * possible to read an out-of-date alloc-index here. If the
             * out-of-date alloc-index is small, with enough space to allocate
             * the entry/dummy entry, then the subsequent CAS must read a more
             * up-to-date alloc-index (otherwise, the behavior becomes an
             * out-of-thin-air (OOTA) behavior), and thus the out-of-date value
             * has no effect (either the CAS succeeds, which means the most
             * up-to-date value is exactly the same as the out-of-date
             * alloc-index and we can pretend we read that value, or the CAS
             * fails and we can pretend the out-of-date read never happened).
             *
             * However, if the alloc-index is already in FULL state, then the
             * out-of-date value is taken at face value, leading to closing the
             * block even if there actually is space. We fix this by using an
             * acquire-fence to synchronize with the update to the block-header
             * that we observed, and then reloading the value. This results in
             * us having at least as up-to-date as the block header, which
             * prevents us from spuriously moving the block forward.
             *
             * The acquire barrier could also be put in other locations, such as
             * the initial read from the block header. Putting the acquire fence
             * in this place means that the fence is only executed in the rare
             * case that the block is full. Therefore, the performance impact of
             * this fence is minimized compared to the other options. */
			vatomic_fence_acq();
			cur = vatomic32_read_rlx(idx);
			if (allocidx_of(cur) >= bound) {
				return ENTRY_INDEX_MASK;
			}
		}

		newidx = allocidx_of(cur) + offset;
		if (newidx > bound) {
			next = get_alloc_idx(allocnum_of(cur), bound);
			if (vatomic32_cmpxchg_rlx(idx, cur, next) == cur) {
				return ENTRY_INDEX_MASK;
			}
		} else {
			next = get_alloc_idx(allocnum_of(cur) + 1,
					     (vuint16_t)newidx);
			done = (vatomic32_cmpxchg_rlx(idx, cur, next) == cur);
		}
	} while (!done);

	return allocidx_of(cur);
}

static inline vuint16_t block_get_allocnum(struct block_head_s *blk)
{
	vuint32_t alloc_idx_num = vatomic32_read_rlx(&blk->alloc_idx);
	return allocnum_of(alloc_idx_num);
}

static inline vuint8_t *block_get_buff(struct block_head_s *blk)
{
	return (vuint8_t *)(blk + 1);
}

static inline int block_is_ready(struct block_head_s *blk)
{
	vuint16_t commit_num = (vuint16_t)vatomic32_read_acq(&blk->commit_num);
	/* Read commit_num must happen before reading alloc_num, because the writer
     * write alloc_num first, then commit the entry */
	vuint16_t alloc_num = block_get_allocnum(blk);
	if (alloc_num == commit_num) {
		return (int)alloc_num;
	}
	return -1;
}

static inline void block_reset_metadata(struct block_head_s *blk)
{
	vatomic32_write_rlx(&blk->commit_num, 0);
	/* reset commit_num must happen before reseting alloc_idx, because once
     * alloc_idx is set to 0, the pending threads can start to alloc */
	vatomic32_write_rel(&blk->alloc_idx, 0);
}

static inline void block_init(struct block_head_s *blk)
{
	block_reset_metadata(blk);
}

static inline struct entry_s *block_alloc_entry(struct rbuffer_s *rb,
						vuint64_t bidx, vuint16_t size)
{
	vuint16_t offset = size_align(size);

	struct block_head_s *blk = global_idx_to_block(rb, bidx);
	vuint16_t idx = block_atomic_alloc_index(&blk->alloc_idx, offset,
						 BLOCK_DATA_SIZE(rb->blk_size));
	if (idx <= BLOCK_DATA_SIZE(rb->blk_size) - sizeof(struct entry_s) -
			   MIN_ALLOC_SIZE) {
		struct entry_s *entry =
			(struct entry_s *)&(block_get_buff(blk)[idx]);
		vatomic_fence();
		entry_alloc(entry, convert_idx(rb, bidx), offset, size);
		return entry;
	} else {
		return NULL;
	}
}

static inline struct entry_s *block_get_first_entry(struct block_head_s *blk)
{
	return (struct entry_s *)(block_get_buff(blk));
}

static inline struct entry_s *block_get_next_entry(struct entry_s *entry)
{
	vuint8_t *cur = (vuint8_t *)entry;
	return (struct entry_s *)(cur + entry_get_next(entry));
}

static inline struct entry_s *block_fetch_entry(struct rbuffer_s *rb,
						struct block_head_s *rblk,
						vuint32_t idx)
{
	struct entry_s *entry = (struct entry_s *)&(block_get_buff(rblk)[idx]);
	if (entry_is_invalid(rblk, entry, rb->blk_size)) {
		return NULL;
	}
	return entry;
}

/**** INTERNAL FUNCTIONS ****/

static inline int rb_is_read_conflict(struct rbuffer_s *rb, vuint64_t ridx)
{
	vuint64_t widxl = vatomic64_read_rlx(&rb->bh->cur_wblk);
	vuint64_t widx = get_write_idx(widxl);
	vuint16_t blk_num = rb->total_blks;

#ifdef EAGER_MOVE_BLOCK
	if (widx - ridx < blk_num) {
		return RB_FALSE;
	}
#else
	if (widx - ridx + get_busy_bit(widxl) < blk_num) {
		return RB_FALSE;
	}
#endif
	return RB_TRUE;
}

static inline void rb_update_reader(struct rbuffer_s *rb, vuint32_t idx,
				    vuint32_t cnt)
{
	vatomic32_write_rlx(&rb->bh->blk_read_idx, idx);
	vatomic32_write_rlx(&rb->bh->blk_read_cnt, cnt);
}

static inline void rb_reset_reader(struct rbuffer_s *rb)
{
	rb_update_reader(rb, 0, 0);
}

static inline void rb_advance_rblk_conflict(struct rbuffer_s *rb)
{
	vuint64_t widx = get_write_idx(vatomic64_read_rlx(&rb->bh->cur_wblk));
	rb_reset_reader(rb);
	vatomic64_write_rlx(&rb->bh->cur_rblk, widx - rb->move_step);
}

static inline void rb_advance_rblk_normal(struct rbuffer_s *rb, vuint64_t ridx,
					  vuint64_t widx)
{
	if (likely(widx > ridx)) {
		rb_reset_reader(rb);
		vatomic64_write_rlx(&rb->bh->cur_rblk, ++ridx);
	}
}

static inline int rb_is_read_unsafe(struct rbuffer_s *rb, vuint64_t ridx)
{
	vuint64_t widxl = vatomic64_read_acq(&rb->bh->cur_wblk);
	vuint64_t widx = get_write_idx(widxl);
	if (likely(widx != ridx)) {
		return RB_FALSE;
	} else {
		if (!is_busy(widxl)) {
			return RB_FALSE;
		}
	}
	return RB_TRUE;
}

static inline void rb_setup_readinfo(struct read_info_s *info, vuint64_t idx,
				     vuint32_t total, vuint32_t cnt)
{
	if (info == NULL) {
		return;
	}
	info->index = idx;
	info->total = total;
	info->blk_read_cnt = cnt;
}

/***********ring buffer API for users*******/

static int rb_init(struct rbuffer_s *rb, void *buf, vsize_t size,
		   vuint16_t blk_num, vuint32_t mode) __attribute__((unused));

/**
 * Inits ring buffer.
 *
 * @param rb pointer to struct rbuffer_s object.
 * @param buf  address of memory that used to store data.
 *             it must be align with 8.
 * @param size size of buf.
 * @param blk_num block number of ring buffer. blk_num should be
 *             power of 2. If not, rb_init will align down with
 *             power of 2, for example, if blk_num is 9, the
 *             actual blk_num is 8. User should be carefully with
 *             the blk_num to make block size greater than the
 *             maximum entry size.
 * @param mode mode of ring buffer. Only RB_RDONLY and RB_RDWR are
 *             supported. Each read thread should create a ringbuffer
 *             with RB_RDONLY mode if want to multiple readers.
 * @return int 0 on success, -1 on failure.
 */
static int rb_init(struct rbuffer_s *rb, void *buf, vsize_t size,
		   vuint16_t blk_num, vuint32_t mode)
{
	if (rb == NULL || buf == NULL) {
		return -1;
	}
	/* The buf address should be aligned with 8, because
     * we have vatomic64 at the head of buf */
	if (((vuintptr_t)buf & (vuintptr_t)ALIGNMENT_MASK) != 0) {
		return -1;
	}
	if (size < sizeof(struct buffer_head_s)) {
		return -1;
	}
	struct buffer_head_s *bh = (struct buffer_head_s *)buf;
	vuint8_t *g_blk = (vuint8_t *)(bh + 1);
	rb->flags = mode;
	rb->g_blk = g_blk;
	rb->bh = bh;

	vsize_t blks_total_size = size - sizeof(struct buffer_head_s);
	rb->total_size = blks_total_size;
	vuint16_t total_blks = blk_num < MIN_BLOCK_NUM ? MIN_BLOCK_NUM :
							 blk_num;

	/* make total blocks align down with power of 2, so we can have a
     * quick computation of the block index by using ">>" instead of "%".
     * For example, if the blks is 9, then the actual block number is 8 */
	total_blks = ALIGN_DOWN_WITH_POWER_2(total_blks);
	if (blks_total_size < total_blks * MIN_BLOCK_SIZE) {
		return -1;
	}
	vuint32_t blk_size = blks_total_size / total_blks;
	if (blk_size >= (1U << ENTRY_INDEX_BITS)) {
		return -1;
	}

	/* The block size should be aligned with 8 */
	blk_size = RB_ALIGN_DOWN(blk_size);

	if (blk_size < MIN_BLOCK_SIZE) {
		return -1;
	}

	rb->total_blks = total_blks;
	rb->blk_size = (vuint16_t)blk_size;
	rb->move_step = total_blks - BACKWARD_STEP;
	if (RB_MODE(mode) == RB_RDONLY) {
		return 0;
	}

	/* only creaters need reset read index */
	vatomic32_write_rlx(&rb->bh->blk_read_cnt, 0);
	vatomic32_write_rlx(&rb->bh->blk_read_idx, 0);
	vatomic64_write_rlx(&rb->bh->cur_rblk, 0);

	for (vuint32_t i = 0; i < total_blks; i++) {
		block_init((struct block_head_s *)(g_blk + i * blk_size));
	}

	vatomic64_init(&bh->cur_wblk, 0);
	return 0;
}

static struct entry_s *rb_try_alloc_entry(struct rbuffer_s *rb, vsize_t size)
	__attribute__((unused));
/**
 * Tries to alloc an entry from ring buffer
 *
 * @param rb pointer to ring buffer object.
 * @param size the desired size of the entry.
 * @return struct entry_s* pointer of the entry on success. NULL, If there is no
 * enough space in the block, it will try to move the wblk to next block and
 * return NULL. And when other thread is trying to move wblk to next block and
 * hold the ready bit, it also return NULL. Also return NULL when trying to move
 * to next block and next block is not ready(there is still some thread writing
 * on the block). Once return NULL, the caller should try again.
 */
static struct entry_s *rb_try_alloc_entry(struct rbuffer_s *rb, vsize_t size)
{
	if (unlikely(rb == NULL || rb->bh == NULL || size < MIN_ALLOC_SIZE ||
		     size > MAX_ALLOC_SIZE(rb->blk_size))) {
		return NULL;
	}
	struct buffer_head_s *bh = rb->bh;

	vuint64_t wblk = vatomic64_read_rlx(&bh->cur_wblk);
	if (!is_busy(wblk)) {
		struct entry_s *entry = block_alloc_entry(
			rb, get_write_idx(wblk), (vuint16_t)size);
		if (entry != NULL) {
			return entry;
		}

		vuint64_t newblk = get_next_write_idx(wblk);
		if (RB_DROP(rb->flags) &&
		    get_write_idx(newblk) - vatomic64_read_acq(&bh->cur_rblk) >=
			    rb->total_blks) {
			return NULL;
		}
		struct block_head_s *nblk =
			global_idx_to_block(rb, get_write_idx(newblk));
		int rc = block_is_ready(nblk);
		if (unlikely(rc < 0)) {
			return NULL;
		}

		if (wblk != vatomic64_read_rlx(&bh->cur_wblk)) {
			return NULL;
		}
#ifdef EAGER_MOVE_BLOCK
		vuint64_t tmp_wblk = set_busy_bit(newblk);
#else
		vuint64_t tmp_wblk = set_busy_bit(wblk);
#endif
		if (vatomic64_cmpxchg_rlx(&bh->cur_wblk, wblk, tmp_wblk) ==
		    wblk) {
			if (likely(rc > 0)) {
				block_reset_metadata(nblk);
			}
			/* block_reset_metadata must happen before write wblk */
			vatomic64_write_rel(&bh->cur_wblk, newblk);
		}
	}
	return NULL;
}

static void rb_commit_entry(struct rbuffer_s *rb, struct entry_s *entry)
	__attribute__((unused));

/**
 * Commits an entry.
 *
 * @param rb pointer to ring buffer object.
 * @param entry the entry to be commited.
 *
 * @note This API may not be used by user directly, so has unused attribute.
 */
static void rb_commit_entry(struct rbuffer_s *rb, struct entry_s *entry)
{
	if (unlikely(rb == NULL || rb->bh == NULL || entry == NULL)) {
		return;
	}
	entry_commit(rb, entry);
}

static vsize_t rb_write_entry(struct rbuffer_s *rb, void *buf, vsize_t size,
			      vuint32_t try_time) __attribute__((unused));
/**
 * Writes an entry to ring buffer.
 *
 * @param rb pointer to ring buffer object.
 * @param buf the data buf.
 * @param size the size of data.
 * @param try_time try times on trying allocate entry.
 * @return vsize_t entry size on success, 0 on fail.
 *
 * @note This API may not be used by user directly, so has unused attribute.
 */
static vsize_t rb_write_entry(struct rbuffer_s *rb, void *buf, vsize_t size,
			      vuint32_t try_time)
{
	int ret = 0;
	if (unlikely(rb == NULL || buf == NULL)) {
		return 0;
	}

	vuint32_t cnt = 0;
	struct entry_s *entry = NULL;

	do {
		entry = rb_try_alloc_entry(rb, size);
		cnt++;
	} while (entry == NULL && cnt < try_time);

	if (entry == NULL) {
		return 0;
	}

	void *dst = entry_get_data(entry);
#if defined(VSYNC_VERIFICATION)
	_rb_cpy_buff(dst, size, buf, size);
#else
#ifdef __KERNEL__
	ret = 0;
	if (v_memcpy_s_params_ok(dst, size, buf, size)) {
		(void)memcpy(dst, buf, size);
	} else {
		ret = 1;
	}
#else
	ret = memcpy_s(dst, size, buf, size);
#endif
	if (ret != 0) {
		/* Mark entry as invalid when memcpy failed. */
		entry->size = 0;
	}
#endif
	entry_commit(rb, entry);
	return ret == 0 ? size : 0;
}

static vsize_t rb_try_read_entry(struct rbuffer_s *rb, void *user_buf,
				 vsize_t buff_size, struct read_info_s *info)
	__attribute__((unused));

/**
 * Tries to read an entry from ring buffer.
 *
 * @param rb  pointer to ring buffer object.
 * @param user_buf buf to store entry data, provided by user.
 * @param buff_size the size of user_buff, the buf should be big enough to
 * store.
 * @param info address to read_info_s object. The reader info contains
 * the total entries of current block, and the index of current entry.
 * @return vsize_t size of the entry on success, 0 when fails to read a valid
 * entry, and the content in buf is invalid.
 */
static vsize_t rb_try_read_entry(struct rbuffer_s *rb, void *user_buf,
				 vsize_t buff_size, struct read_info_s *info)
{
	if (unlikely(rb == NULL || user_buf == NULL || rb->bh == NULL)) {
		return 0;
	}

	vuint64_t ridx = vatomic64_read_rlx(&rb->bh->cur_rblk);
	struct block_head_s *rblk = global_idx_to_block(rb, ridx);
	vuint32_t read_cnt, read_idx;

	/* Check whether current rblk is overwritten */
	if (rb_is_read_conflict(rb, ridx)) {
		rb_advance_rblk_conflict(rb);
		return 0;
	}

	if (rb_is_read_unsafe(rb, ridx)) {
		return 0;
	}

	/* reading widx must before reading alloc */
	vuint64_t widx = get_write_idx(vatomic64_read_acq(&rb->bh->cur_wblk));

	/* Check all writers are done */
	int alloc_num = block_is_ready(rblk);
	if (alloc_num < 0) {
		return 0;
	}
	/* Check if there are entries remain in the block */
	read_cnt = vatomic32_read_rlx(&rb->bh->blk_read_cnt);
	if (read_cnt == (vuint32_t)alloc_num) {
		rb_advance_rblk_normal(rb, ridx, widx);
		return 0;
	}

	read_idx = vatomic32_read_rlx(&rb->bh->blk_read_idx);
	struct entry_s *entry = block_fetch_entry(rb, rblk, read_idx);
	if (entry != NULL) {
		vsize_t sz = (vsize_t)entry_get_size(entry);
		if (unlikely(buff_size < sz)) {
			return 0;
		}

#if defined(VSYNC_VERIFICATION)
		_rb_cpy_buff(user_buf, buff_size, entry_get_data(entry), sz);
#else
#ifdef __KERNEL__
		int ret = 0;
		if (v_memcpy_s_params_ok(user_buf, buff_size,
					 entry_get_data(entry), sz)) {
			(void)memcpy(user_buf, entry_get_data(entry), sz);
		} else {
			ret = 1;
		}
#else
		int ret = memcpy_s(user_buf, buff_size, entry_get_data(entry),
				   sz);
#endif
		if (ret != 0) {
			return 0;
		}
#endif

		vuint16_t next_offset = entry_get_next(entry);

		vatomic_fence_acq();

		if (!rb_is_read_conflict(rb, ridx)) {
			rb_update_reader(rb, read_idx + next_offset,
					 ++read_cnt);
			rb_setup_readinfo(info, ridx, (vuint32_t)alloc_num,
					  read_cnt);
			if (read_cnt == (vuint32_t)alloc_num) {
				rb_advance_rblk_normal(rb, ridx, widx);
			}
			return sz;
		} else {
			rb_advance_rblk_conflict(rb);
		}
	}
	return 0;
}

static vsize_t rb_read_entry(struct rbuffer_s *rb, void *buf, vsize_t buff_size,
			     struct read_info_s *info) __attribute__((unused));
/**
 * Reads an entry from ring buffer.
 *
 * @param rb pointer to ring buffer object.
 * @param buf  buf to store entry data, provided by user.
 * @param buff_size the size of user_buff, the buf should be big enough to
 * store.
 * @param info address to read_info_s object. The reader info contains
 * the total entries of current block, and the index of current entry.
 * @return vsize_t size of the entry on success. 0 input parameter is invalid.
 *
 * @note This API will keep trying to read a valid entry. It returns only
 * when read out a valid entry from ring buffer. Be careful when using this API,
 * make sure there is some writer writing the ring buffer.
 */
static vsize_t rb_read_entry(struct rbuffer_s *rb, void *buf, vsize_t buff_size,
			     struct read_info_s *info)
{
	if (unlikely(rb == NULL || buf == NULL || rb->bh == NULL)) {
		return 0;
	}

	vsize_t size = 0;
	do {
		size = rb_try_read_entry(rb, buf, buff_size, info);
	} while (size == 0);

	return size;
}

static void rb_rewind_reader(struct rbuffer_s *rb) __attribute__((unused));

/**
 * Moves reader pointer to the oldest block.
 *
 * @param rb pointer to ring buffer object.
 *
 * @note This API may not be used, so have a unused attribute.
 */
static void rb_rewind_reader(struct rbuffer_s *rb)
{
	if (unlikely(rb == NULL || rb->bh == NULL)) {
		return;
	}
	vuint64_t widx = get_write_idx(vatomic64_read_rlx(&rb->bh->cur_wblk));
	rb_reset_reader(rb);
	if (likely(widx >= rb->total_blks)) {
		vatomic64_write_rlx(&rb->bh->cur_rblk,
				    widx - rb->total_blks + 1);
	} else {
		vatomic64_write_rlx(&rb->bh->cur_rblk, 0);
	}
}

static int rb_get_write_offset(struct rbuffer_s *rb, struct entry_s *entry,
			       vsize_t entry_size) __attribute__((unused));
/**
 * Gets write offset from entry.
 *
 * @param rb pointer to ring buffer object.
 * @param entry pointer to entry.
 * @param entry_size entry size.
 * @return int the write offset of current entry on success, -1 on failure.
 */
static int rb_get_write_offset(struct rbuffer_s *rb, struct entry_s *entry,
			       vsize_t entry_size)
{
	if (unlikely(rb == NULL || entry == NULL)) {
		return -1;
	}
	vuint64_t ridx = vatomic64_read_rlx(&rb->bh->cur_rblk);
	if (rb_is_read_conflict(rb, ridx)) {
		return rb->total_size;
	} else {
		vuint8_t *cur_read = (vuint8_t *)global_idx_to_block(rb, ridx) +
				     sizeof(struct block_head_s) +
				     vatomic32_read_rlx(&rb->bh->blk_read_idx);
		vuint8_t *cur_write =
			(vuint8_t *)entry + entry_size + sizeof(struct entry_s);
		if (cur_write >= cur_read) {
			return cur_write - cur_read;
		} else {
			vuint8_t diff = cur_read - cur_write;
			return (int)(rb->total_size - diff);
		}
	}
}
#endif
