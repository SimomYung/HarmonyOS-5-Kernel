/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: DAMON API
 * Author: Huawei CBG OS Lab
 * Create: Wed Mar 20 2024
 */

#ifndef SYSMGR_DAMON_H
#define SYSMGR_DAMON_H

#ifdef CONFIG_DAMON

#include <stdbool.h>
#include <hongmeng/errno.h>

#include <intapi/mem/damon-types.h>

#include <hmasm/page.h>
#include <lib/utils.h>
#include <hmasm/types.h>

#ifndef MAX
# define MAX(a, b) ({			\
	__typeof__(a) __a = (a);	\
	__typeof__(b) __b = (b);	\
	__a > __b ? __a : __b;		\
})
#endif

#ifndef MIN
# define MIN(a, b) ({			\
	__typeof__(a) __a = (a);	\
	__typeof__(b) __b = (b);	\
	__a < __b ? __a : __b;		\
})
#endif

#ifndef ABS
# define ABS(a) ({			\
	__typeof__(a) __a = (a);	\
	__a < 0 ? -__a : __a;		\
})
#endif

#ifndef SWAP_T
# define SWAP_T(x, y)			\
	do {				\
		typeof(x) __zzz = (x);	\
		(x) = (y);		\
		(y) = __zzz;		\
	} while (0)
#endif

extern struct damon_operations vaddr_ops;

/* Since we operate on page level here is a minimum */
#define DAMON_MIN_REGION_SIZE	(PAGE_SIZE)

unsigned long damon_rand_between(unsigned long l, unsigned long r);
void *damon_zalloc(size_t size);

static inline struct damon_region *damon_next_region(struct damon_region *r)
{
	return container_of(r->list.next, struct damon_region, list);
}

static inline struct damon_region *damon_prev_region(struct damon_region *r)
{
	return container_of(r->list.prev, struct damon_region, list);
}

static inline struct damon_region *damon_last_region(struct damon_target *t)
{
	return dlist_last_entry(&t->regions_list, struct damon_region, list);
}

static inline struct damon_region *damon_first_region(struct damon_target *t)
{
	return dlist_first_entry(&t->regions_list, struct damon_region, list);
}

static unsigned long damon_areg_size(struct damon_addr_region *areg)
{
	return areg->end - areg->start;
}

static inline unsigned long damon_region_size(struct damon_region *r)
{
	return damon_areg_size(&r->addr_region);
}

#define damon_for_each_region(r, t)						\
	dlist_for_each_entry(r, &t->regions_list, struct damon_region, list)

#define damon_for_each_region_from(r, t)					\
	dlist_for_each_entry_from(r, &t->regions_list, struct damon_region, list)

#define damon_for_each_region_safe(r, next, t)					\
	dlist_for_each_entry_safe(r, next, &t->regions_list, struct damon_region, list)

#define damon_for_each_region_continue_safe(r, next, t)				\
	dlist_for_each_entry_continue_safe(r, next, &t->regions_list, struct damon_region, list)

#define damon_for_each_target(t, ctx)						\
	dlist_for_each_entry(t, &(ctx)->targets_list, struct damon_target, list)

#define damon_for_each_target_safe(t, next, ctx)				\
	dlist_for_each_entry_safe(t, next, &(ctx)->targets_list, struct damon_target, list)

struct damon_region *damon_region_alloc(unsigned long start, unsigned long end);
void damon_insert_region(struct damon_region *r, struct damon_region *pos,
			struct damon_target *t);
void damon_add_region(struct damon_region *r, struct damon_target *t);
void damon_region_free(struct damon_region *r, struct damon_target *t);
int damon_set_regions(struct damon_target *t, struct damon_addr_region *ranges,
		unsigned int nr_ranges);
void damon_update_access_rate(struct damon_region *r, bool is_accessed,
		struct damon_params *params);

struct damon_target *damon_target_alloc(void);
struct damon_target *damon_target_alloc_linked(struct damon_context *ctx);
void damon_target_free(struct damon_target *t);
void damon_kill_tgts(struct damon_context *ctx);

struct damon_context *damon_ctx_alloc(void);
void damon_ctx_free(struct damon_context *ctx);

void damon_set_params(struct damon_context *ctx, struct damon_params *params);

static inline bool damon_target_uses_pid(const struct damon_context *ctx)
{
	return true;
}

/* Access ratio for base points conversion */
#define DAMON_BP_RATIO		(10000)
/* Conver number of accesses to base points */
#define DAMON_NR_BP(nr)		((nr) * DAMON_BP_RATIO)
/* Convert base points to number of accesses */
#define DAMON_BP_NR(bp)		((bp) / DAMON_BP_RATIO)

/* For API compatibility we should accepts probe_us = 0 from userspace */
static inline unsigned long damon_probe_us(const struct damon_params *params)
{
	return params->timing.probe_us ? params->timing.probe_us : 1;
}

/* Maximal number of accesses fitting `int` storage */
static inline unsigned int damon_max_nr_accesses(const struct damon_params *params)
{
	/*
	 * probing parameters are `unsigned long` while access counters
	 * are `unsigned int` thus make sure won't overflow happen.
	 */
	return MIN(params->timing.collect_us / damon_probe_us(params),
		(unsigned long)UINT_MAX);
}

int damon_start(struct damon_context *ctx);
int damon_stop(struct damon_context *ctx);

struct sysmgr_module_ctx;
int damon_init(struct sysmgr_module_ctx *module_ctx);
#endif /* CONFIG_DAMON */

#endif /* SYSMGR_DAMON_H */
