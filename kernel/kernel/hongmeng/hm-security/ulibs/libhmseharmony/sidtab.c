/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony source file for sidtable
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#include <sys/mman.h>
#include <fcntl.h>

#include <libhwsecurec/securec.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmseharmony/flask.h>
#include "sidtab.h"
#include "policydb.h"
#include "ocontext.h"
#include "debug.h"

#ifndef __HOST_LLT__
struct map_info {
	unsigned map_flags;
	int prot;
};
#endif

#define SEL_INVALID_SHM_ID (-1)

static int map_actually(struct hmsel_buf *buf, size_t size)
{
#ifdef __HOST_LLT__
	UNUSED(buf);
	if (size == 0u) {
		return E_HM_INVAL;
	}
	return E_HM_OK;
#else
	void *addr = NULL;
	void *fixed_addr = NULL;
	bool unmapped = false;
	int ret = E_HM_OK;
	size_t aligned_size;
	int rc;
	struct map_info minfo;

	minfo.map_flags = (____IS(buf->flags & HMSEL_SID_CTX_TBL_CFG_FILE) ? 0 : MAP_ANON) |
		(____IS(buf->flags & HMSEL_SID_CTX_TBL_CFG_REMOTE) ? MAP_SHARED : MAP_PRIVATE);
	minfo.prot = ____IS(buf->flags & HMSEL_SID_CTX_TBL_CFG_RDONLY) ?
		(PROT_READ) : (PROT_READ | PROT_WRITE);

	if (size == 0u) {
		ret = E_HM_INVAL;
	}
	aligned_size = PAGE_ALIGN_UP(size);
	if (ret == E_HM_OK && aligned_size > buf->size - buf->mapped_size) {
		ret = E_HM_NOMEM;
	}
	if (ret == E_HM_OK) {
		fixed_addr = (void*)((uintptr_t)(buf->data) + buf->mapped_size);
		addr = hm_mem_mmap_fixed(fixed_addr, aligned_size, minfo.prot,
					 minfo.map_flags, buf->shm_id, buf->mapped_size,
					 &unmapped, &ret);
		if (ret != E_HM_OK && unmapped) { /* rollback: remap */
			addr = hm_mem_mmap_fixed(fixed_addr, aligned_size, PROT_NONE,
					       MAP_PRIVATE | MAP_ANON, SEL_INVALID_SHM_ID, 0LL,
					       &unmapped, &rc);
			if (rc != E_HM_OK) {
				/* we may overwrite some mappings owned by others */
				hm_warn("failed to rollback reserved mapping, rc=%s, raw_rc=%s\n",
					hmstrerror(rc), hmstrerror(ret));
			}
		}
	}
	if (ret == E_HM_OK) {
		BUG_ON(fixed_addr != addr);
		buf->mapped_size += aligned_size;
	}
	return ret;
#endif
}

static int hmsel_buf_init(struct hmsel_buf *buf, int shm_id, unsigned flags, size_t size)
{
#ifdef __HOST_LLT__
	void *addr = malloc(size);
	if (addr == NULL) {
		return E_HM_NOMEM;
	}
	buf->shm_id = shm_id;
	buf->flags = flags;
	buf->data = addr;
	buf->size = size;
	buf->mapped_size = size;
	return E_HM_OK;
#else
	int ret = E_HM_OK;
	size_t aligned_size = PAGE_ALIGN_UP(size);
	void *addr = hm_mem_mmap(NULL, aligned_size, PROT_NONE, MAP_PRIVATE | MAP_ANON, SEL_INVALID_SHM_ID, 0);
	if (addr == MAP_FAILED) {
		ret = E_HM_NOMEM;
	}
	if (ret == E_HM_OK) {
		buf->shm_id = shm_id;
		buf->flags = flags;
		buf->data = addr;
		buf->size = aligned_size;
		buf->mapped_size = 0u;
	}
	return ret;
#endif
}

static void hmsel_buf_fini(struct hmsel_buf *buf)
{
#ifdef __HOST_LLT__
	free(buf->data);
	buf->data = NULL;
#else
	(void)hm_mem_munmap(buf->data, buf->size);
	buf->data = NULL;
	if (____IS(buf->flags & HMSEL_BUF_CFG_REMOTE)) {
		(void)hm_mem_shm_close(buf->shm_id);
	}
#endif
}

static int hmsel_sid_ctx_tbl_init(struct hmsel_sid_ctx_tbl *tbl,
		      const struct hmsel_sid_ctx_tbl_cfg *cfg)
{
	int ret;
	unsigned flags = 0;
	size_t size = cfg->table_length * ENT_SIZE;
	size_t prealloc_size = cfg->preallocated_sid_num * ENT_SIZE;

	flags |= ____IS(cfg->flags & HMSEL_SID_CTX_TBL_CFG_REMOTE) ? HMSEL_BUF_CFG_REMOTE : 0;
	flags |= ____IS(cfg->flags & HMSEL_SID_CTX_TBL_CFG_RDONLY) ? HMSEL_BUF_CFG_RDONLY : 0;
	flags |= ____IS(cfg->flags & HMSEL_SID_CTX_TBL_CFG_FILE) ? HMSEL_BUF_CFG_FILE : 0;

	ret = hmsel_buf_init(&tbl->buf, cfg->shm_id, flags, size);
	if (ret == E_HM_OK && prealloc_size != 0u) {
		ret = map_actually(&tbl->buf, prealloc_size);
		if (ret != E_HM_OK) {
			hmsel_buf_fini(&tbl->buf);
		}
	}
	if (ret == E_HM_OK) {
		tbl->sid_base = cfg->sid_base;
		tbl->sid = cfg->sid_base;
		tbl->sid_end = cfg->sid_base + cfg->table_length;
		tbl->offset_end = 0u;
		raw_mutex_init(&tbl->lock);
	}
	return ret;
}

static inline
struct hmsel_context *sid_ctx_tbl_index(const struct hmsel_sid_ctx_tbl *tbl, hmsel_sid_t sid)
{
	return (struct hmsel_context *)((uintptr_t)tbl->buf.data + tbl->sid_to_offset[sid - tbl->sid_base]);
}

static void hmsel_sid_ctx_tbl_fini(struct hmsel_sid_ctx_tbl *tbl)
{
	struct hmsel_context *ctx = NULL;
	if (!____IS(tbl->buf.flags & HMSEL_BUF_CFG_RDONLY)) {
		for (hmsel_sid_t s = tbl->sid_base; s < tbl->sid; s++) {
			if (tbl->sid_to_offset[s - tbl->sid_base] == OFFSET_INVALID) {
				continue;
			}
			ctx = sid_ctx_tbl_index(tbl, s);
			if (ctx->len != 0u) {
				free(ctx->str);
			}
		}
	}
	raw_mutex_destroy(&tbl->lock);
	hmsel_buf_fini(&tbl->buf);
}

static inline
hmsel_sid_t first_invalid_sid(const struct hmsel_sid_ctx_tbl *tbl)
{
	return (hmsel_sid_t)(tbl->buf.mapped_size / sizeof(struct hmsel_context) + tbl->sid_base);
}

static struct hmsel_context *get_remote_context(struct hmsel_sid_ctx_tbl *tbl,
							   hmsel_sid_t sid)
{
	struct hmsel_context *res = NULL;
	int ret = E_HM_OK;
	size_t size;

	if (sid < tbl->sid_base || sid >= tbl->sid_end ||
	    (!____IS(tbl->buf.flags & HMSEL_BUF_CFG_RDONLY) && tbl->sid_to_offset[sid - tbl->sid_base] >= tbl->offset_end)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK && tbl->sid_to_offset[sid - tbl->sid_base] >= tbl->offset_end) {
		raw_mutex_lock(&tbl->lock);
		if (tbl->sid_to_offset[sid - tbl->sid_base] >= tbl->offset_end) {
			/* contain sid self */
			size = tbl->sid_to_offset[SIDTAB_GLOBAL_MAX - 1] - tbl->offset_end;
			ret = map_actually(&tbl->buf, size);
			tbl->offset_end = tbl->sid_to_offset[SIDTAB_GLOBAL_MAX - 1];
		}
		raw_mutex_unlock(&tbl->lock);
	}
	if (ret == E_HM_OK) {
		res = sid_ctx_tbl_index(tbl, sid);
	}
	return res;
}

static struct hmsel_context *get_local_context(struct hmsel_sid_ctx_tbl *tbl,
							  hmsel_sid_t sid)
{
	struct hmsel_context *res = NULL;
	if (sid >= tbl->sid_base && sid < tbl->sid) {
		res = sid_ctx_tbl_index(tbl, sid);
	}
	return res;
}

static inline
struct hmsel_context *hmsel_sid_ctx_tbl_search_force(struct hmsel_sid_ctx_tbl *tbl,
						      hmsel_sid_t sid)
{
	return ____IS(tbl->buf.flags & HMSEL_BUF_CFG_REMOTE) ?
		get_remote_context(tbl, sid) :
		get_local_context(tbl, sid);
}

static void context_to_store(struct hmsel_sid_ctx_tbl *tbl,
			size_t offset, struct hmsel_context *ctx)
{
	struct hmsel_context *tmp = (struct hmsel_context *)(((uintptr_t)tbl->buf.data + offset));
	if (ctx->len !=0) {
		tmp->len = ctx->len;
		tmp->str = ctx->str;
		return;
	}
	tmp->shadow_len = 0;
	tmp->user = ctx->user;
	tmp->role = ctx->role;
	tmp->type = ctx->type;
	tmp->flag = 0;
	struct hmmls_level *dst_level = tmp->level;
	struct hmmls_level *src_level = ctx->levels;
	mls_level_copy(dst_level, src_level);
	dst_level = get_high_level(dst_level);
	src_level = get_high_level(src_level);
	mls_level_copy(dst_level, src_level);
}

static int hmsel_sid_ctx_tbl_alloc_sid(struct hmsel_sid_ctx_tbl *tbl,
			   struct hmsel_context *ctx,
			   hmsel_sid_t *new_sid, size_t *offset)
{
	int ret = E_HM_OK;
	size_t size;

	if (tbl == NULL || ctx == NULL) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK &&
	    ____IS(tbl->buf.flags & HMSEL_BUF_CFG_RDONLY)) {
		/* ensure only sysmgr can alloc global sidtable */
		hm_error("failed to alloc sid, try to alloc readonly sid\n");
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK && tbl->sid >= tbl->sid_end) {
		hm_error("no free sidtable entry left\n");
		ret = E_HM_NOMEM;
	}
	if (tbl != NULL) {
		raw_mutex_lock(&tbl->lock);
	}
	if (ret == E_HM_OK) {
		size = (size_t)(tbl->offset_end + hmsel_context_size(ctx));
		if (size > tbl->buf.mapped_size) {
			ret = map_actually(&tbl->buf, size - tbl->buf.mapped_size);
		}
	}
	if (ret == E_HM_OK) {
		context_to_store(tbl, tbl->offset_end, ctx); // store hmsel_context
		if (new_sid == NULL) {
			tbl->sid++;
		}
		if (new_sid != NULL) {
			if (*new_sid == 0) {
				*new_sid = tbl->sid;
				tbl->sid++;
			} else if (*new_sid >= tbl->sid) {
				tbl->sid = *new_sid;
				tbl->sid++;
			}
			*offset = tbl->offset_end;
		}
		tbl->offset_end += hmsel_context_size(ctx);
		tbl->sid_to_offset[SIDTAB_GLOBAL_MAX - 1] = tbl->offset_end;
	}
	if (tbl != NULL) {
		raw_mutex_unlock(&tbl->lock);
	}
	return ret;
}

static uint32_t sidtab_rs_last(const struct hmsel_sidtab *s)
{
	return s->rsnum - 1;
}

STATIC uint32_t sidtab_rs_last_free(const struct hmsel_sidtab *s)
{
	return s->rsnum;
}

static void sidtab_rsnum_add(struct hmsel_sidtab *s)
{
	s->rsnum++;
}

static struct hmsel_context *stable_context_of(struct hmsel_sid_ctx_tbl *tab, hmsel_sid_t sid)
{
	return hmsel_sid_ctx_tbl_search_force(tab, sid);
}

static struct hmsel_context *sidtab_context_of_local(struct hmsel_sidtab *s, hmsel_sid_t sid)
{
	return stable_context_of(&s->local_tab, sid);
}

static struct hmsel_context *sidtab_context_of_global(struct hmsel_sidtab *s, hmsel_sid_t sid)
{
	return stable_context_of(&s->global_tab, sid);
}

struct hmsel_context *sidtab_context_of(struct hmsel_sidtab *s, hmsel_sid_t sid)
{
	if (sid < SIDTAB_GLOBAL_MAX && sid > 0) {
		return hmsel_sid_ctx_tbl_search_force(&s->global_tab, sid);
	} else {
		return hmsel_sid_ctx_tbl_search_force(&s->local_tab, sid);
	}
}

static int mls_cat_cmp(const struct hm_catmap_node *s, const struct hm_catmap_node *t, uint16_t s_cat_num)
{
	uint16_t cat_num = s_cat_num;
	while (cat_num > 0) {
		cat_num--;
		if (s[cat_num].startbit == t[cat_num].startbit) {
			if (s[cat_num].map == t[cat_num].map) {
				continue;
			} else {
				return s[cat_num].map > t[cat_num].map ? 1 : -1;
			}
		} else {
			return s[cat_num].startbit > t[cat_num].startbit ? 1 : -1;
		}
	}
	return 0;
}

#define HIGHBIT_BITS 11
#define CATNUM_BITS 5
static int mls_level_cmp(const struct hmmls_level *s, const struct hmmls_level *t)
{
	uint32_t scmp = 0, tcmp = 0;
	/* max sens is 32 --- 7 bits, max cat_num is 16 --- 5 bits, max highbit is 1024 --- 11 bits */
	scmp = s->sens << (CATNUM_BITS + HIGHBIT_BITS) | (uint32_t)s->cat_num << HIGHBIT_BITS | (uint32_t)s->highbit;
	tcmp = t->sens << (CATNUM_BITS + HIGHBIT_BITS) | (uint32_t)t->cat_num << HIGHBIT_BITS | (uint32_t)t->highbit;
	return scmp != tcmp ? (scmp > tcmp ? 1 : -1) : mls_cat_cmp(s->node, t->node, s->cat_num);
}


static int mls_range_cmp(const struct hmmls_level *s, const struct hmmls_level *t)
{
	int res = 0;
	res = mls_level_cmp(s, t);
	if (res == 0) {
		const struct hmmls_level *s_high = get_high_level(s);
		const struct hmmls_level *t_high = get_high_level(t);
		return mls_level_cmp(s_high, t_high);
	} else {
		return res;
	}
}

/* most context have same user and role, so compare type first */
static int context_cmp_valid(const struct hmsel_context *s, const struct hmsel_context *t)
{
	if (s->type != t->type) {
		return (s->type > t->type) ? 1 : -1;
	}
	if (s->role != t->role) {
		return (s->role > t->role) ? 1 : -1;
	}
	if (s->user != t->user) {
		return (s->user > t->user) ? 1 : -1;
	}
	if (s->flag != 0U) {
		return mls_range_cmp(s->levels, t->level);
	} else {
		return mls_range_cmp(s->level, t->level);
	}
}

static int context_cmp_invalid(const struct hmsel_context *s, const struct hmsel_context *t)
{
	return strcmp(s->str, t->str);
}

int hmsel_context_cmp(const struct hmsel_context *s, const struct hmsel_context *t)
{
	int ret = 0;
	if (s->len == 0 && t->len == 0) {
		ret = context_cmp_valid(s, t);
	} else if (s->len == 0 && t->len != 0) {
		ret = -1;
	} else if (s->len != 0 && t->len != 0) {
		ret = context_cmp_invalid(s, t);
	} else if (s->len != 0 && t->len == 0) {
		ret = 1;
	}
	return ret;
}

static struct raw_rwlock *sidtab_rsearch_lock(struct hmsel_sidtab *s)
{
	return &s->rsearch_lock;
}

#define DIVISOR 2
static hmsel_sid_t rstable_sid_of(struct hmsel_sidtab *s, uint32_t idx)
{
	return s->rstable[idx].sid;
}

STATIC hmsel_sid_t context_to_sid(struct hmsel_sidtab *s, const struct hmsel_context *con)
{
	hmsel_sid_t sid = (hmsel_sid_t)(0u);
	uint32_t low = 1;
	uint32_t high;
	uint32_t mid;
	struct hmsel_context *tmp_con;
	int res;
	raw_rwlock_rdlock(sidtab_rsearch_lock(s));
	high = sidtab_rs_last(s);
	/* binary search */
	while (low <= high) {
		mid = (low + high) / DIVISOR;
		sid = rstable_sid_of(s, mid);
		tmp_con = sidtab_context_of(s, sid);
		res = hmsel_context_cmp(con, tmp_con);
		if (res > 0) {
			low = mid + 1;
		} else if (res < 0) {
			high = mid - 1;
		} else {
			raw_rwlock_unlock(sidtab_rsearch_lock(s));
			SEL_DEBUG("rsearch context find to sid %"PRIu32", at rs idx %"PRIu32"\n", sid, mid);
			return sid;
		}
	}
	raw_rwlock_unlock(sidtab_rsearch_lock(s));
	return (hmsel_sid_t)(0u);
}

STATIC hmsel_sid_t context_to_sid_new(struct hmsel_sidtab *s, const struct hmsel_context *con, uint32_t *ret_idx)
{
	hmsel_sid_t sid = (hmsel_sid_t)(0u);
	uint32_t low = 1;
	uint32_t high;
	uint32_t mid;
	struct hmsel_context *tmp_con;
	int res;
	/* called in wr lock, no lock need here */
	high = sidtab_rs_last(s);
	/* binary search */
	while (low <= high) {
		mid = (low + high) / DIVISOR;
		sid = rstable_sid_of(s, mid);
		tmp_con = sidtab_context_of(s, sid);
		res = hmsel_context_cmp(con, tmp_con);
		if (res < 0) {
			high = mid - 1;
		} else if (res > 0) {
			low = mid + 1;
		} else {
			*ret_idx = mid;
			return sid;
		}
	}
	/* context not found, return the last search position */
	*ret_idx = low;
	return (hmsel_sid_t)(0u);
}

static void rsearch_entry_fill(struct sid_rsearch_entry *e, hmsel_sid_t sid)
{
	e->sid = sid;
}

static void rsearch_entry_copy(struct sid_rsearch_entry *t, struct sid_rsearch_entry *s)
{
	*t = *s;
}

static void rsearch_entry_move(struct sid_rsearch_entry *s, struct sid_rsearch_entry *e)
{
	struct sid_rsearch_entry *tmp = e;
	while (tmp != s) {
		rsearch_entry_copy(tmp, tmp - 1);
		tmp--;
	}
	return;
}

static void rstable_insert_fast_nolock(struct hmsel_sidtab *s, hmsel_sid_t sid, uint32_t insert_idx)
{
	uint32_t end_idx = sidtab_rs_last_free(s);
	struct sid_rsearch_entry *cur = s->rstable + insert_idx;
	struct sid_rsearch_entry *end = s->rstable + end_idx;
	if (cur == end) {
		rsearch_entry_fill(cur, sid);
	} else {
		rsearch_entry_move(cur, end);
		rsearch_entry_fill(cur, sid);
	}
	return;
}

/* is_isid is true */
static int do_icontext_to_isid(struct hmsel_sidtab *s, struct hmsel_context *context,
				hmsel_sid_t *sidp)
{
	hmsel_sid_t sid;
	int rc;
	uint32_t insert_idx;
	size_t offset;
	sid = context_to_sid_new(s, context, &insert_idx);
	if (sid != 0) {
		offset = s->global_tab.sid_to_offset[sid - s->global_tab.sid_base];
		s->global_tab.sid_to_offset[*sidp - s->global_tab.sid_base] = offset;
		SEL_DEBUG("global rsearch new context find to sid %"PRIu32", at rs idx %"PRIu32"\n", sid, insert_idx);
		return E_HM_OK;
	}
	rc = hmsel_sid_ctx_tbl_alloc_sid(&s->global_tab, context, sidp, &offset);
	if (rc != E_HM_OK) {
		hm_error("failed to alloc global sid\n");
		return rc;
	}
	rstable_insert_fast_nolock(s, *sidp, insert_idx);
	s->global_tab.sid_to_offset[*sidp - s->global_tab.sid_base] = offset;
	SEL_DEBUG("global insert sid %"PRIu32" to rsindex %"PRIu32"\n", *sidp, insert_idx);
	PRINT_CON(context);
	sidtab_rsnum_add(s);
	return rc;
}

static int do_context_to_sid_global(struct hmsel_sidtab *s, struct hmsel_context *context,
				hmsel_sid_t *sidp)
{
	hmsel_sid_t tmp, sid;
	tmp = *sidp;
	*sidp = 0;
	int rc;
	uint32_t insert_idx;
	struct raw_rwlock *rsearch_lock = sidtab_rsearch_lock(s);
	sid = context_to_sid(s, context);
	size_t offset;
	if (sid != 0) {
		*sidp = sid;
		SEL_DEBUG("global rsearch context find to sid %"PRIu32"\n", sid);
		return (context->len != 0u) ? E_HM_EXIST : E_HM_OK;
	}
	raw_rwlock_wrlock(rsearch_lock);
	sid = context_to_sid_new(s, context, &insert_idx);
	if (sid != 0) {
		*sidp = sid;
		raw_rwlock_unlock(rsearch_lock);
		SEL_DEBUG("global rsearch new context find to sid %"PRIu32", at rs idx %"PRIu32"\n", sid, insert_idx);
		return (context->len != 0u) ? E_HM_EXIST : E_HM_OK;
	}
	rc = hmsel_sid_ctx_tbl_alloc_sid(&s->global_tab, context, sidp, &offset);
	if (rc != E_HM_OK) {
		hm_error("failed to alloc global sid\n");
		*sidp = tmp;
		raw_rwlock_unlock(rsearch_lock);
		return rc;
	}
	rstable_insert_fast_nolock(s, *sidp, insert_idx);
	s->global_tab.sid_to_offset[*sidp - s->global_tab.sid_base] = offset;
	SEL_DEBUG("global insert sid %"PRIu32" to rsindex %"PRIu32"\n", *sidp, insert_idx);
	PRINT_CON(context);
	sidtab_rsnum_add(s);
	raw_rwlock_unlock(rsearch_lock);
	return rc;
}

static int do_context_to_sid_local(struct hmsel_sidtab *s, struct hmsel_context *context, hmsel_sid_t *sidp)
{
	int rc;
	uint32_t insert_idx;
	struct raw_rwlock *rsearch_lock = sidtab_rsearch_lock(s);
	hmsel_sid_t sid = context_to_sid(s, context);
	size_t offset;
	if (sid != 0) {
		*sidp = sid;
		SEL_DEBUG("local rsearch context find to sid %"PRIu32"\n", sid);
		return (context->len != 0u) ? E_HM_EXIST : E_HM_OK;
	}
	raw_rwlock_wrlock(rsearch_lock);
	sid = context_to_sid_new(s, context, &insert_idx);
	if (sid != 0) {
		*sidp = sid;
		raw_rwlock_unlock(rsearch_lock);
		SEL_DEBUG("local rsearch new context find to sid %"PRIu32", at rs idx %"PRIu32"\n", sid, insert_idx);
		return (context->len != 0u) ? E_HM_EXIST : E_HM_OK;
	}
	rc = hmsel_sid_ctx_tbl_alloc_sid(&s->local_tab, context, &sid, &offset);
	if (rc != E_HM_OK) {
		hm_error("failed to alloc local sid\n");
		raw_rwlock_unlock(rsearch_lock);
		return rc;
	}
	rstable_insert_fast_nolock(s, sid, insert_idx);
	s->local_tab.sid_to_offset[sid - s->local_tab.sid_base] = offset;
	*sidp = sid;
	SEL_DEBUG("local insert sid %"PRIu32" to rsindex %"PRIu32"\n", sid, insert_idx);
	PRINT_CON(context);
	sidtab_rsnum_add(s);
	raw_rwlock_unlock(rsearch_lock);
	return rc;
}

int hmsel_sidtab_context_to_sid(struct hmsel_sidtab *s, struct hmsel_context *context, hmsel_sid_t *sidp)
{
	int rc;
	if (s == NULL || context == NULL || sidp == NULL) {
		rc = E_HM_INVAL;
	} else {
		rc = do_context_to_sid_local(s, context, sidp);
	}
	if (rc != E_HM_OK) {
		if (context != NULL && context->len != 0) {
			/* telling caller to free context str */
			context->len = 0;
		}
	}
	return rc;
}

int hmsel_sidtab_global_context_to_sid(struct hmsel_sidtab *s, struct hmsel_context *context, hmsel_sid_t *sidp)
{
	int rc;
	if (s == NULL || context == NULL || sidp == NULL) {
		rc = E_HM_INVAL;
	} else {
		rc = do_context_to_sid_global(s, context, sidp);
	}
	if (rc != E_HM_OK) {
		if (context != NULL && context->len != 0) {
			/* telling caller to free context str */
			context->len = 0;
		}
	}
	return rc;
}

static void sidtab_global_destroy(struct hmsel_sidtab *s)
{
	struct hmsel_sid_ctx_tbl *tab = &s->global_tab;
	hmsel_sid_ctx_tbl_fini(tab);
}

static void sidtab_rstable_destroy(struct hmsel_sidtab *s)
{
	if (s->rstable != NULL) {
#ifdef __HOST_LLT__
		free(s->rstable);
		s->rstable = NULL;
#else
		size_t rentrysize = sizeof(struct sid_rsearch_entry);
		(void)hm_mem_munmap(s->rstable, SIDTAB_MAX * rentrysize);
		s->rstable = NULL;
#endif
	}
}

static void sidtab_local_destroy(struct hmsel_sidtab *s)
{
	struct hmsel_sid_ctx_tbl *tab = &s->local_tab;
	hmsel_sid_ctx_tbl_fini(tab);
}

static void sidtab_loffset_destroy(struct hmsel_sidtab *s)
{
	if (s->local_tab.sid_to_offset != NULL) {
#ifdef __HOST_LLT__
		free(s->local_tab.sid_to_offset);
		s->local_tab.sid_to_offset = NULL;
#else
		size_t offset_size = sizeof(size_t);
		(void)hm_mem_munmap(s->local_tab.sid_to_offset, SIDTAB_LOCAL_MAX * offset_size);
		s->local_tab.sid_to_offset = NULL;
#endif
	}
}

static void sidtab_goffset_destroy(struct hmsel_sidtab *s)
{
	if (s->global_tab.sid_to_offset != NULL) {
#ifdef __HOST_LLT__
		free(s->global_tab.sid_to_offset);
		s->global_tab.sid_to_offset = NULL;
#else
		size_t offset_size = sizeof(size_t);
		(void)hm_mem_munmap(s->global_tab.sid_to_offset, SIDTAB_GLOBAL_MAX * offset_size);
		s->global_tab.sid_to_offset = NULL;
		(void)hm_mem_shm_close(s->gsid_shm_fd);
#endif
	}
}

static int sidtab_rstable_init(struct hmsel_sidtab *s)
{
	struct sid_rsearch_entry *rstable;
	size_t rentrysize = sizeof(struct sid_rsearch_entry);
#ifdef __HOST_LLT__
	rstable = (struct sid_rsearch_entry *)malloc(SIDTAB_MAX * rentrysize);
	if (rstable == NULL) {
		hm_error("failed to alloc sid rstable\n");
		return E_HM_NOMEM;
	}
	memset_s(rstable, SIDTAB_MAX * rentrysize, 0, SIDTAB_MAX * rentrysize);
#else
	rstable = hm_mem_mmap(0, SIDTAB_MAX * rentrysize, PROT_READ | PROT_WRITE,
			      MAP_ANON | MAP_PRIVATE, SEL_INVALID_SHM_ID, 0);
	if (rstable == MAP_FAILED) {
		hm_error("failed to alloc sid rstable\n");
		return E_HM_NOMEM;
	}
#endif
	s->rstable = rstable;
	s->rsnum = SIDTAB_RSTBL_START;
	return E_HM_OK;
}

static int sidtab_local_init(struct hmsel_sidtab *s)
{
	struct hmsel_sid_ctx_tbl *tab = &s->local_tab;
	struct hmsel_sid_ctx_tbl_cfg ltc;

	ltc.shm_id = SEL_INVALID_SHM_ID;
	ltc.flags = 0u;
	ltc.sid_base = SIDTAB_LOCAL_START;
	ltc.table_length = SIDTAB_LOCAL_MAX;
	ltc.preallocated_sid_num = 0u;
	int ret = hmsel_sid_ctx_tbl_init(tab, &ltc);
	if (ret != E_HM_OK) {
		hm_error("failed to init local sid tbl, err=%d\n", ret);
		return ret;
	}
	return E_HM_OK;
}

void hmsel_sidtab_destroy(struct hmsel_sidtab *s)
{
	if (s != NULL) {
		sidtab_local_destroy(s);
		sidtab_global_destroy(s);
		sidtab_rstable_destroy(s);
		sidtab_loffset_destroy(s);
		sidtab_goffset_destroy(s);
	}
}

static const char * const isid_to_str[] = {
	NULL,
	"kernel",
	"security",
	"unlabeled",
	NULL,
	"file",
	NULL,
	NULL,
	"any_socket",
	"port",
	"netif",
	"netmsg",
	"node",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	"devnull",
};

const char *isid_to_context_str(hmsel_sid_t sid)
{
	if (sid > HM_SECINITSID_NUM)
		return NULL;
	return isid_to_str[sid];
}

raw_static_assert(sizeof(isid_to_str) / sizeof(isid_to_str[0]) == HM_SECINITSID_NUM + 1, isid_num_valid);
raw_static_assert(SIDTAB_GLOBAL_MAX > HM_SECINITSID_NUM, isid_range_valid);

static int sidtab_isid_set(struct hmsel_sidtab *s,
			   hmsel_sid_t sid, struct hmsel_context *conp)
{
	int rc = E_HM_OK;
	if (sid > HM_SECINITSID_NUM) {
		hm_error("illegal sid %"PRIu32" beyond secinitsid num\n", sid);
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		rc = do_icontext_to_isid(s, conp, &sid);
		if (rc != E_HM_OK) {
			hm_error("failed to insert isid to stable\n");
		}
	}
	return rc;
}

static int sidtab_isids_init(struct hmsel_policydb *p, struct hmsel_sidtab *s)
{
	int ret = E_HM_OK;
	struct hmsel_context con = {0};
	struct hmsel_ocontext *oconp = hmsel_ocon_isids(p);
	struct raw_rwlock *rsearch_lock = sidtab_rsearch_lock(s);
	struct hmmls_level levels = {0};
	con.levels = &levels;

	raw_rwlock_wrlock(rsearch_lock);

	for (; oconp != NULL; oconp = hmsel_ocon_next(oconp)) {
		hmsel_sid_t sid = hmsel_ocon_sid(oconp, 0);
		const char *name = isid_to_context_str(sid);
		if (sid == 0) {
			hm_error("failed to init isids, has illegal sid value 0\n");
			raw_rwlock_unlock(rsearch_lock);
			return E_HM_INVAL;
		}
		if (name == NULL) {
			continue;
		}
		ret = hmsel_ocon_context(oconp, &con, 0);
		if (ret == E_HM_OK) {
			ret = sidtab_isid_set(s, sid, &con);
		}
		if (unlikely(con.levels != &levels)) {
			hmsel_context_free(&con);
		}
		if (ret != E_HM_OK) {
			break;
		}
	}
	raw_rwlock_unlock(rsearch_lock);
	return ret;
}

static int sidtab_global_prepare(struct hmsel_sid_ctx_tbl_cfg *gtc, unsigned int flags)
{
#ifndef __HOST_LLT__
/* need to add version number in sidtab shm name for policy update */
#define HMSEL_SIDTAB_SHM "hmselsidtab"

	size_t size = gtc->table_length * ENT_SIZE;
	size = PAGE_ALIGN_UP(size);
	size_t len = 0;
	uint64_t oflags;
	int fd = SEL_INVALID_SHM_ID;
	if ((flags & HMSEL_LOAD_FLAGS_GUEST) == 0u) {
		oflags = O_CREAT | O_EXCL | O_RDONLY;
#ifdef CONFIG_DEBUG_BUILD
		mode_t mode = S_IRUSR | S_IRGRP;
#else
		mode_t mode = S_ISYS_SRVC | S_IRUSR | S_IRGRP;
#endif
		fd = hm_mem_shm_open_with_mode(HMSEL_SIDTAB_SHM, size, oflags,
					       mode, &len);
		if (fd < 0) {
			hm_error("failed to open master sel sidtable shm, err=%s\n", hmstrerror(fd));
			return fd;
		}
		gtc->flags = HMSEL_SID_CTX_TBL_CFG_REMOTE;
	} else {
		oflags = O_RDONLY;
		fd = hm_mem_shm_open(HMSEL_SIDTAB_SHM, 0, oflags, &len);
		if (fd < 0) {
			hm_error("failed to open sel sidtable shm, err=%s\n", hmstrerror(fd));
			return fd;
		}
		gtc->flags = HMSEL_SID_CTX_TBL_CFG_REMOTE | HMSEL_SID_CTX_TBL_CFG_RDONLY;
	}
	gtc->shm_id = fd;
	return E_HM_OK;
#else
	UNUSED(flags);
	gtc->shm_id = SEL_INVALID_SHM_ID;
	gtc->flags = 0u;
	return E_HM_OK;
#endif
}

static int sidtab_global_init(struct hmsel_sidtab *s, unsigned int flags)
{
	struct hmsel_sid_ctx_tbl *tab = &s->global_tab;
	struct hmsel_sid_ctx_tbl_cfg gtc;

	gtc.sid_base = SIDTAB_GLOBAL_START;
	gtc.table_length = SIDTAB_GLOBAL_MAX;
	gtc.preallocated_sid_num = HM_SECINITSID_NUM;
	int ret = sidtab_global_prepare(&gtc, flags);
	if (ret != E_HM_OK) {
		hm_error("failed to prepare global sid cfg\n");
		return ret;
	}
	ret = hmsel_sid_ctx_tbl_init(tab, &gtc);
	if (ret != E_HM_OK) {
		hm_error("failed to init global sid tbl, err=%d\n", ret);
#ifndef __HOST_LLT__
		hm_mem_shm_close(gtc.shm_id);
#endif
		return ret;
	}
	return E_HM_OK;
}

static int sidtab_lsid_offset_init(struct hmsel_sidtab *s)
{
	size_t *lsid_to_offset;
	size_t offset_size = sizeof(size_t);
#ifdef __HOST_LLT__
	lsid_to_offset = (size_t *)malloc(SIDTAB_LOCAL_MAX * offset_size);
	if (lsid_to_offset == NULL) {
		hm_error("failed to alloc sid lsid_to_offset\n");
		return E_HM_NOMEM;
	}
	memset_s(lsid_to_offset, SIDTAB_LOCAL_MAX * offset_size, 0, SIDTAB_LOCAL_MAX * offset_size);
#else
	lsid_to_offset = hm_mem_mmap(0, SIDTAB_LOCAL_MAX * offset_size, PROT_READ | PROT_WRITE,
			      MAP_ANON | MAP_PRIVATE, SEL_INVALID_SHM_ID, 0);
	if (lsid_to_offset == MAP_FAILED) {
		hm_error("failed to alloc sid lsid_to_offset\n");
		return E_HM_NOMEM;
	}
#endif
	s->local_tab.sid_to_offset = lsid_to_offset;
	return E_HM_OK;
}

static int sidtab_gsid_offset_init(struct hmsel_sidtab *s, unsigned int flags)
{
	size_t *gsid_to_offset;
	size_t offset_size = sizeof(size_t);
#ifndef __HOST_LLT__
/* need to add version number in sidtab shm name for policy update */
#define HMSEL_GSID2OFFSET_SHM "hmsel_gsid_to_offset"

	size_t size = SIDTAB_LOCAL_MAX * offset_size, len = 0;
	size = PAGE_ALIGN_UP(size);
	uint64_t oflags;
	int fd = SEL_INVALID_SHM_ID;
	if ((flags & HMSEL_LOAD_FLAGS_GUEST) == 0u) {
		oflags = O_CREAT | O_EXCL | O_RDONLY;
#ifdef CONFIG_DEBUG_BUILD
		mode_t mode = S_IRUSR | S_IRGRP;
#else
		mode_t mode = S_ISYS_SRVC | S_IRUSR | S_IRGRP;
#endif
		fd = hm_mem_shm_open_with_mode(HMSEL_GSID2OFFSET_SHM, size, oflags,
					       mode, &len);
		if (fd < 0) {
			hm_error("failed to open master sel gsid_to_offset shm, err=%s\n", hmstrerror(fd));
			return fd;
		}
		s->gsid_shm_fd = fd;
		gsid_to_offset = hm_mem_mmap(0, SIDTAB_GLOBAL_MAX * offset_size, PROT_READ | PROT_WRITE,
			      MAP_ANON | MAP_SHARED, fd, 0);
		if (gsid_to_offset == MAP_FAILED) {
			hm_error("failed to alloc sid gsid_to_offset\n");
			return E_HM_NOMEM;
		}
		memset_s(gsid_to_offset, SIDTAB_LOCAL_MAX * offset_size, OFFSET_INVALID, SIDTAB_LOCAL_MAX * offset_size);
	} else {
		oflags = O_RDONLY;
		fd = hm_mem_shm_open(HMSEL_GSID2OFFSET_SHM, 0, oflags, &len);
		if (fd < 0) {
			hm_error("failed to open sel gsid_to_offset shm, err=%s\n", hmstrerror(fd));
			return fd;
		}
		s->gsid_shm_fd = fd;
		gsid_to_offset = hm_mem_mmap(0, SIDTAB_GLOBAL_MAX * offset_size, PROT_READ,
			      MAP_ANON | MAP_SHARED, fd, 0);
	}
#else
	gsid_to_offset = (size_t *)malloc(SIDTAB_LOCAL_MAX * offset_size);
	if (gsid_to_offset == NULL) {
		hm_error("failed to alloc sid gsid_to_offset\n");
		return E_HM_NOMEM;
	}
	memset_s(gsid_to_offset, SIDTAB_LOCAL_MAX * offset_size, 0, SIDTAB_LOCAL_MAX * offset_size);
#endif
	s->global_tab.sid_to_offset = gsid_to_offset;
	return E_HM_OK;
}

static int do_sidtab_init(struct hmsel_policydb *db, struct hmsel_sidtab *s, unsigned int flags)
{
	raw_rwlock_init(sidtab_rsearch_lock(s), NULL);

	int rc = sidtab_local_init(s);
	if (rc != E_HM_OK) {
		return rc;
	}
	rc = sidtab_global_init(s, flags);
	if (rc != E_HM_OK) {
		sidtab_local_destroy(s);
		return rc;
	}
	rc = sidtab_rstable_init(s);
	if (rc != E_HM_OK) {
		sidtab_global_destroy(s);
		sidtab_local_destroy(s);
		return rc;
	}
	rc = sidtab_lsid_offset_init(s);
	if (rc != E_HM_OK) {
		sidtab_rstable_destroy(s);
		sidtab_global_destroy(s);
		sidtab_local_destroy(s);
		return rc;
	}
	rc = sidtab_gsid_offset_init(s, flags);
	if (rc != E_HM_OK) {
		sidtab_loffset_destroy(s);
		sidtab_rstable_destroy(s);
		sidtab_global_destroy(s);
		sidtab_local_destroy(s);
		return rc;
	}

	if ((flags & HMSEL_LOAD_FLAGS_GUEST) == 0u) {
		rc = sidtab_isids_init(db, s);
		if (rc != E_HM_OK) {
			hmsel_sidtab_destroy(s);
		}
	}
	return rc;
}

int hmsel_sidtab_init(struct hmsel_policydb *db, struct hmsel_sidtab *s, unsigned int flags)
{
	int rc;
	if (db == NULL || s == NULL) {
		rc = E_HM_INVAL;
	} else {
		rc = do_sidtab_init(db, s, flags);
	}
	return rc;
}

static struct hmsel_context *
sidtab_context_of_initial(struct hmsel_sidtab *s, hmsel_sid_t sid)
{
	struct hmsel_context *con = NULL;
	const char *name = isid_to_context_str(sid);
	if (name != NULL) {
		con = sidtab_context_of_global(s, sid);
	}
	return con;
}

static struct hmsel_context *
hmsel_sidtab_search_core(struct hmsel_sidtab *s, hmsel_sid_t sid, int force)
{
	if (unlikely(s == NULL)) {
		return NULL;
	}
	struct hmsel_context *conp = NULL;
	if (likely(sid != 0)) {
		if (sid <= HM_SECINITSID_NUM) {
			conp = sidtab_context_of_initial(s, sid);
			SEL_DEBUG("initial search for sid %"PRIu32"\n", sid);
		} else {
			if (sid < SIDTAB_GLOBAL_MAX) {
				conp = sidtab_context_of_global(s, sid);
				SEL_DEBUG("global search for sid %"PRIu32", force %d\n", sid, force);
			} else if (sid >= SIDTAB_LOCAL_START) {
				conp = sidtab_context_of_local(s, sid);
				SEL_DEBUG("local search for sid %"PRIu32", force %d\n", sid, force);
			}
		}
		if ((conp != NULL) && (conp->len == 0 || force != 0)) {
			SEL_DEBUG("sid search success find a not unlabeled context, force %d\n", force);
			PRINT_CON(conp);
			return conp;
		}
	}
	conp = sidtab_context_of_initial(s, HM_SECINITSID_UNLABELED);
	SEL_DEBUG("sid search failed, return unlabeled, force %d\n", force);
	return conp;
}

struct hmsel_context *hmsel_sidtab_search(struct hmsel_sidtab *s, hmsel_sid_t sid)
{
	return hmsel_sidtab_search_core(s, sid, 0);
}

struct hmsel_context *hmsel_sidtab_search_force(struct hmsel_sidtab *s, hmsel_sid_t sid)
{
	return hmsel_sidtab_search_core(s, sid, 1);
}
