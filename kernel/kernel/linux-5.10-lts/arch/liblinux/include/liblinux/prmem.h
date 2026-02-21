/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_PRMEM_H__
#define __LIBLINUX_PRMEM_H__

#ifdef CONFIG_HM_HKIP_PRMEM
#include <liblinux/pal.h>
#include <linux/kmemleak.h>
struct prmem_pool {
	void *hm_prmem_pool;
};

enum prmem_flags {
	PRMEM_NO_FLAGS = (1 << 0),
	PRMEM_ZERO = (1 << 1),
	PRMEM_NODE = (1 << 2),
	PRMEM_FREEABLE_NODE = (1 << 3),
};

enum prmem_type {
	PRMEM_RO_NO_RECL,
	PRMEM_WR_NO_RECL,
	PRMEM_START_WR_NO_RECL,
	PRMEM_START_WR_RECL,
	PRMEM_WR_RECL,
	PRMEM_RO_RECL,
	PRMEM_LAST_PROTECTED_REGION = PRMEM_RO_RECL,
	PRMEM_RW_RECL,
	PRMEM_LAST_REGION = PRMEM_RW_RECL,
	PRMEM_NO_RECL,
	PRMEM_RECL,
	PRMEM_WR,
	PRMEM_START_WR,
	PRMEM_FULL,
	PRMEM_INDEX_NUM,
};

#define prmem_pool(_name, _type, _al, _refill, _cap)	\
	struct prmem_pool _name = {							\
		.hm_prmem_pool = NULL,							\
	}

static inline void *pmalloc(struct prmem_pool *pool, size_t size, enum prmem_flags flags)
{
	void *ret = liblinux_pal_prmem_malloc(pool->hm_prmem_pool, size, (int)flags);
	kmemleak_alloc(ret, size, 0, GFP_KERNEL);
	return ret;
}

static inline void *pzalloc(struct prmem_pool *pool, size_t size, enum prmem_flags flags)
{
	void *ret = liblinux_pal_prmem_malloc(pool->hm_prmem_pool, size, (int)flags);
	kmemleak_alloc(ret, size, 0, GFP_KERNEL);
	return ret;
}

static inline void *pcalloc(struct prmem_pool *pool, size_t n, size_t size, enum prmem_flags flags)
{
	return liblinux_pal_prmem_calloc(pool->hm_prmem_pool, n, size, (int)flags);
}

static inline char *pstrdup(struct prmem_pool *pool, const char *s, enum prmem_flags flags)
{
	return liblinux_pal_prmem_strdup(pool->hm_prmem_pool, s, (int)flags);
}

static bool prmem_in_range(const void *start, unsigned long size, enum prmem_type type)
{
	return true;
}

static inline void prmem_protect_pool(struct prmem_pool *pool)
{
	liblinux_pal_prmem_protect_pool(pool->hm_prmem_pool);
}

static inline void prmem_sec_region_protect(void)
{
	liblinux_pal_prmem_sec_region_protect();
}

/* prmem's free func not supported */
#define pfree(addr)

#endif /* CONFIG_HM_HKIP_PRMEM */

#endif /* __LIBLINUX_PRMEM_H__ */
