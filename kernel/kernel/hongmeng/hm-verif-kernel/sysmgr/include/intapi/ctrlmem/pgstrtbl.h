/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Ctrlmem API for pgstrtbl
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 04 09:25:14 2022
 */

#ifndef SYSMGR_INCLUDE_INTAPI_CTRLMEM_PGSTRTBL_H
#define SYSMGR_INCLUDE_INTAPI_CTRLMEM_PGSTRTBL_H

#include <hmasm/memory.h>
#include <stdbool.h>

struct pgstr_s;

struct pgstrtbl_segment_s {
	__paddr_t st;
	__paddr_t ed;

	unsigned long entry_sz;
	unsigned long entry_nr;
	unsigned long entry_pa_sz;

	void *ctrlmem_start;
	void *ctrlmem_end;
};

__paddr_t pgstrtbl_segment_pa_from_pgstr(const struct pgstrtbl_segment_s *seg,
					 const struct pgstr_s *pgstr);

#define PGSTRTBL_NR_SEGMENTS	64

struct pgstrtbl_segments_s {
	struct pgstrtbl_segment_s segments[PGSTRTBL_NR_SEGMENTS];

	unsigned int nr_segs;
};

struct pgstrtbl_segments_s *pgstrtbl_segments(void);

static inline __paddr_t pgstrtbl_segments_first_pa(void)
{
	return pgstrtbl_segments()->segments[0].st;
}

static inline __paddr_t pgstrtbl_segments_last_pa(void)
{
	struct pgstrtbl_segments_s *s = pgstrtbl_segments();
	unsigned int nr = s->nr_segs;

	return (nr > 0) ? s->segments[nr - 1u].ed : (__paddr_t)(0ULL);
}

void pgstrtbl_init(void);

void pgstrtbl_segments_dump(struct pgstrtbl_segments_s *segs);

struct pgstr_s *pgstrtbl_pa_to_pgstr(__paddr_t pa);

struct pgstr_s *pgstrtbl_seg_pa_to_pgstr(__paddr_t pa, const struct pgstrtbl_segment_s **lastseg);

__paddr_t pgstrtbl_pgstr_to_pa(struct pgstr_s *pgstr);

/* iterator */
struct pgstrtbl_iter_ctx_s {
	const struct pgstrtbl_segment_s *seg;
	struct pgstr_s *pgstr;
};

struct pgstr_s *pgstrtbl_iter_first_pgstr(struct pgstrtbl_iter_ctx_s *ctx);
struct pgstr_s *pgstrtbl_iter_start(struct pgstrtbl_iter_ctx_s *ctx, __paddr_t pa_st);
struct pgstr_s *pgstrtbl_iter_next_pgstr(struct pgstrtbl_iter_ctx_s *ctx);
struct pgstr_s *pgstrtbl_iter_next_pgstr_same_seg(struct pgstrtbl_iter_ctx_s *ctx);

struct pgstr_s *pgstrtbl_iter_first_pgstr_bound(struct pgstrtbl_iter_ctx_s *ctx, __paddr_t pa_ed);
struct pgstr_s *pgstrtbl_iter_start_bound(struct pgstrtbl_iter_ctx_s *ctx, __paddr_t pa_st, __paddr_t pa_ed);
struct pgstr_s *pgstrtbl_iter_next_pgstr_bound(struct pgstrtbl_iter_ctx_s *ctx, __paddr_t pa_ed);
struct pgstr_s *pgstrtbl_iter_next_pgstr_same_seg_bound(struct pgstrtbl_iter_ctx_s *ctx, __paddr_t pa_ed);

__paddr_t pgstrtbl_iter_pa_from_ctx(struct pgstrtbl_iter_ctx_s *ctx);

struct pgstrtbl_scan_ctx_s {
	struct pgstrtbl_iter_ctx_s iter_ctx;

	struct pgstr_s *first;
	struct pgstr_s *last;
	const struct pgstrtbl_segment_s *seg;
};

#define __PGSTRTBL_SCAN_ARGS_LIST(...)	, ##__VA_ARGS__
#define __PGSTRTBL_SCAN_PARAMS_LIST(...)	, ##__VA_ARGS__
/*
 * Do not process ',' at first level: when args/params are not null, use
 * ', ##__VA_ARGS__' too early makes following expanding generate an extra ','.
 */
#define PGSTRTBL_SCAN_ARGS_LIST(args...)	args
#define PGSTRTBL_SCAN_PARAMS_LIST(params...)	params

#define __DEFINE_PGSTRTBL_SCAN(func, args, params)	\
static inline bool __pgstrtbl_scan_##func##_find_first(struct pgstrtbl_scan_ctx_s *ctx, \
						       __paddr_t pa_st, __paddr_t pa_ed args) \
{ \
	struct pgstr_s *pgstr = pgstrtbl_iter_start_bound(&ctx->iter_ctx, pa_st, pa_ed); \
	bool found = false; \
 \
	ctx->first	= NULL; \
	ctx->last	= NULL; \
	ctx->seg	= NULL; \
	for(; pgstr != NULL; pgstr = pgstrtbl_iter_next_pgstr_bound(&ctx->iter_ctx, pa_ed)) { \
		if (func(pgstr params)) { \
			ctx->first	= pgstr; \
			ctx->seg	= ctx->iter_ctx.seg; \
			found = true;\
			break; \
		} \
	} \
	return found; \
} \
static inline void __pgstrtbl_scan_##func##_find_last(struct pgstrtbl_scan_ctx_s *ctx, \
						      __paddr_t pa_ed args) \
{ \
	struct pgstr_s *prev	= ctx->iter_ctx.pgstr; \
	struct pgstr_s *pgstr	= pgstrtbl_iter_next_pgstr_same_seg_bound(&ctx->iter_ctx, pa_ed); \
 \
	for(; pgstr != NULL; prev = pgstr, pgstr = pgstrtbl_iter_next_pgstr_same_seg_bound(&ctx->iter_ctx, pa_ed)) { \
		if (!func(pgstr params)) { \
			break; \
		} \
	} \
	ctx->last = prev; \
}

#define DEFINE_PGSTRTBL_SCAN_PGSTR(func, args, params)	\
	__DEFINE_PGSTRTBL_SCAN(func, __PGSTRTBL_SCAN_ARGS_LIST(args), __PGSTRTBL_SCAN_PARAMS_LIST(params)) \
static inline bool __pgstrtbl_scan_##func##_find_block_pgstr(struct pgstr_s **p_first, struct pgstr_s **p_last, \
							     __paddr_t pa_st, __paddr_t pa_ed \
							     __PGSTRTBL_SCAN_ARGS_LIST(args)) \
{ \
	struct pgstrtbl_scan_ctx_s ctx; \
	bool found = false; \
 \
	found = __pgstrtbl_scan_##func##_find_first(&ctx, pa_st, pa_ed \
						    __PGSTRTBL_SCAN_PARAMS_LIST(params)); \
	if (found && (p_first != NULL)) { \
		*p_first = ctx.first; \
	} \
	if (found && (p_last != NULL)) { \
		__pgstrtbl_scan_##func##_find_last(&ctx, pa_ed \
						   __PGSTRTBL_SCAN_PARAMS_LIST(params)); \
		*p_last = ctx.last; \
	} \
	return found; \
}

#define DEFINE_PGSTRTBL_SCAN(func, args, params)	\
	__DEFINE_PGSTRTBL_SCAN(func, __PGSTRTBL_SCAN_ARGS_LIST(args), __PGSTRTBL_SCAN_PARAMS_LIST(params)) \
static inline bool __pgstrtbl_scan_##func##_find_block_pa(__paddr_t *p_blk_pa_st, __paddr_t *p_blk_pa_ed, \
							  __paddr_t pa_st, __paddr_t pa_ed \
							  __PGSTRTBL_SCAN_ARGS_LIST(args)) \
{ \
	struct pgstrtbl_scan_ctx_s ctx; \
	bool found = false; \
 \
	found = __pgstrtbl_scan_##func##_find_first(&ctx, pa_st, pa_ed __PGSTRTBL_SCAN_PARAMS_LIST(params)); \
	if (found && (p_blk_pa_st != NULL)) { \
		*p_blk_pa_st = pgstrtbl_segment_pa_from_pgstr(ctx.seg, ctx.first); \
	} \
	if (found && (p_blk_pa_ed != NULL)) { \
		__pgstrtbl_scan_##func##_find_last(&ctx, pa_ed __PGSTRTBL_SCAN_PARAMS_LIST(params)); \
		*p_blk_pa_ed = pgstrtbl_segment_pa_from_pgstr(ctx.seg, ctx.last) + \
				ctx.seg->entry_pa_sz; \
	} \
	return found; \
}

extern __paddr_t pgstrtbl_pa_max(void);

struct pgstrtbl_coseg_iter_ctx_s {
	const struct pgstrtbl_segment_s *next_seg;
	__paddr_t pa;
};

extern void pgstrtbl_coseg_iter_start(struct pgstrtbl_coseg_iter_ctx_s *ctx,
				      __paddr_t pa_st);

extern bool pgstrtbl_coseg_iter_next(struct pgstrtbl_coseg_iter_ctx_s *ctx);

static inline __paddr_t pgstrtbl_coseg_curr_pa_st(struct pgstrtbl_coseg_iter_ctx_s *ctx)
{
	return ctx->pa;
}

static inline __paddr_t pgstrtbl_coseg_curr_pa_ed(const struct pgstrtbl_coseg_iter_ctx_s *ctx)
{
	__paddr_t pa;

	/*
	 * According to G.CTL.01: there is an exception that when `if' condition
	 * is a pointer, we are allowed to directly use the pointer itself as
	 * a condition expression, don't have to rewrite it to `if (p != NULL)'.
	 * We use this exception so don't have to introduce `NULL', which is
	 * provided by another header.
	 */
	if (ctx->next_seg) {
		pa = ctx->next_seg->st;
	} else {
		pa = pgstrtbl_pa_max();
	}
	return pa;
}

struct pgstrtblhier_iomem_iter_ctx_s {
	struct pgstrtbl_coseg_iter_ctx_s coseg_iter_ctx;
	__paddr_t pa;
};

extern void pgstrtblhier_iomem_iter_start(struct pgstrtblhier_iomem_iter_ctx_s *ctx, __paddr_t pa_st);
extern bool pgstrtblhier_iomem_iter_next(struct pgstrtblhier_iomem_iter_ctx_s *ctx, __paddr_t pa_ed,
					 __paddr_t *p_pa_blk_st, __paddr_t *p_pa_blk_ed);

#endif
