/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Declare interface of mcache may be used outside memmgr
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 16 09:57:41 UTC 2023
 */

#ifndef __SYSMGR_INCLUDE_INTAPI_MEM_MCACHE_H__
#define __SYSMGR_INCLUDE_INTAPI_MEM_MCACHE_H__

#include <malloc.h>
#include <hongmeng/types.h>
#include <hongmeng/compiler.h>
#include <libmem/slab_utils.h>
#include <libhmsync/raw_mutex.h>

struct mcache_s;

struct mcache_info_s {
	struct slab_info_s slab_info;
	struct slab_slot_info_s slot_info;
	struct slab_ator_info_s ator_info;
};

struct mcache_stat_s {
	uint64_t size;
	enum slab_size_type type;
};

/*
 * Create a mcache. A mcache is used for allocating objects whose size is obj_size.
 *
 * @param name		[I] The name of the mcache, better be prefixed with "mcache-",
 *			    this is used when printing statistics info.
 * @param obj_size	[I] The size of objects allocated from the created mcache,
 *			    should be less than SLAB_MAX_MALLOC_SIZE
 * @param cache_count	[I] The number of slot in per-cpu cache.
 *
 * @param cache_pool	[I] The address of the per-cpu cache pool. If NULL, the cache
 *			    pool is malloced inside mcache_create.
 * @param m_flags	[I] The flags used to configure slab
 *
 * @return		The created mcache if success, NULL otherwise.
 */
struct mcache_s *mcache_create(const char *name, size_t obj_size, size_t cache_count,
			       uintptr_t *cache_pool, uint32_t m_flags);

/*
 * Destroy the mcache.
 *
 * @param mcache	[I] The mcache to be destroyed.
 *
 * @param cache_pool	[O] The address of the per-cpu cache pool if it is given when creating the mcache.
 *			    NULL otherwise.
 * @note		The mcache cannot be destroyed if there's still slab slot in use.
 * 			Also there should be no memory allocation from the mcache in the
 * 			meanwhile, otherwise there may be memory leak due to memory remained
 * 			in the slab pool and can't be freed.
 * @return		E_HM_OK if success, E_HM_INVAL if len not equal to __NR_LRU_PAGE_TYPES
 */
int mcache_destroy(struct mcache_s *mcache, uintptr_t *cache_pool);

/*
 * Return the size of specifc-type memory in mcache.
 *
 * @param mcache	[I] The mcache to be checked out.
 *
 * @param type		[I] type of the slab size
 *
 * @return		The size of free memory in mcache.
 */
size_t mcache_size_type(struct mcache_s *mcache, enum slab_size_type type);

/*
 * Allocate an object from a given mcache with paf(see `paf.h`).
 *
 * @param mcache	[I] The mcache to be allocated from.
 *
 * @param paf		[I] page allocation flags.
 *
 * @return		The allocated object.
 */
void *mcache_paf_alloc(struct mcache_s *mcache, uint32_t paf);

/*
 * Allocate an object from a given mcache.
 *
 * @param mcache	[I] The mcache to be allocated from.
 *
 * @return		The allocated object.
 */
void *mcache_alloc(struct mcache_s *mcache);

/*
 * Free an object allocated from a mcache.
 *
 * @param ptr	[I] The object to be freed.
 */
void mcache_free(void *ptr);

/*
 * Iterate all mcaches and for each mcache execute fn.
 *
 * @param fn	[I] The function to be executed for each mcache
 *
 * @param args	[I] The arguments to be passed to the function fn.
 */
void mcache_for_each(void (*fn)(struct mcache_s *mcache, void *args), void *args);

/*
 * Dump detail of the mcache.
 *
 * @param mcache	[I] The mcache whose detail to be dumped.
 *
 * @param info		[O] The detail to be dumped is stored in info.
 */
void mcache_dump(const struct mcache_s *mcache, struct mcache_info_s *info);

/*
 * Query the used slot num of the ator.
 *
 * @param mcache	[I] The mcache whose detail to be dumped.
 */
uint32_t mcache_get_ator_used_slot_num(struct mcache_s *mcache);

/*
 * Query the used slot num of the ator with SLAB_STORE_USER.
 *
 * @param mcache	[I] The mcache whose detail to be dumped.
 */
uint32_t mcache_get_ator_actv_used_slot_num(struct mcache_s *mcache);

/*
 * Query the total size of all unused slots of the mcache allocator.
 *
 * @param mcache	[I] The mcache which to be queried.
 */
size_t mcache_get_ator_free_size(struct mcache_s *mcache);

/*
 * Query size info of the mcache.
 *
 * @param mcache	[I] The mcache whose size info to be queried.
 *
 * @param args		[O] The size info to be queried is stored in args.
 */
void mcache_size_query(struct mcache_s *mcache, void *args);

/*
 * Query the number of mcache in sysmgr
 */
uint32_t mcache_num(void);

/*
 * Query the name of mcache.
 */
const char *mcache_name_of(struct mcache_s *mcache);

/*
 * Check the ptr is a valid mcache obj ptr
 */
bool mcache_ptr_is_valid(struct mcache_s *mcache, uintptr_t ptr);

typedef void *(*mcache_alloc_ops)(void);
typedef void *(*mcache_paf_alloc_ops)(uint32_t paf);
typedef void (*mcache_free_ops)(void *ptr);

/*
 * Define a mcache allocator with mcache name, struct name,
 * cache count and object size.
 */
#define DECLARE_MCACHE(mcachename)						\
void *mcache_##mcachename##_malloc(void);					\
void *mcache_##mcachename##_zmalloc(void);					\
void *mcache_##mcachename##_pafmalloc(uint32_t paf);				\
void *mcache_##mcachename##_pafzmalloc(uint32_t paf);				\
void mcache_##mcachename##_free(void *ptr);					\
size_t mcache_##mcachename##_reclaimable(void);					\

#define MCACHE_RETRY_TIMES		10U

#define __DEFINE_MCACHE(mcachename, cache_count, obj_size, flags)			\
static struct raw_mutex g_##mcachename##_mcache_lock = RAW_MUTEX_INITIALIZER;	\
static struct mcache_s *g_##mcachename##_mcache = NULL;				\
static void *mcachename##_alloc_mcache(void);					\
static void *mcachename##_alloc_no_mcache(void);				\
static void *mcachename##_paf_alloc_mcache(uint32_t paf);			\
static void *mcachename##_paf_alloc_no_mcache(uint32_t paf);			\
static void mcachename##_free_mcache(void *ptr);				\
static mcache_alloc_ops g_##mcachename##_alloc = mcachename##_alloc_no_mcache;		\
static mcache_paf_alloc_ops g_##mcachename##_paf_alloc = mcachename##_paf_alloc_no_mcache;	\
static mcache_free_ops g_##mcachename##_free = mcachename##_free_mcache;		\
static inline void init_##mcachename##_ator(void)				\
{										\
	size_t retry = 0;							\
	raw_mutex_lock(&g_##mcachename##_mcache_lock);				\
	while ((g_##mcachename##_mcache == NULL) && (retry < MCACHE_RETRY_TIMES)) {	\
		g_##mcachename##_mcache = mcache_create(#mcachename, obj_size,	\
				cache_count, NULL, flags);			\
		if (g_##mcachename##_mcache != NULL) {				\
			g_##mcachename##_alloc = mcachename##_alloc_mcache;	\
			g_##mcachename##_paf_alloc = mcachename##_paf_alloc_mcache;	\
			break;							\
		}								\
		retry ++;							\
	}									\
	BUG_ON(g_##mcachename##_mcache == NULL);				\
	raw_mutex_unlock(&g_##mcachename##_mcache_lock);			\
}										\
static void *mcachename##_alloc_mcache(void)					\
{										\
	return mcache_alloc(g_##mcachename##_mcache);				\
}										\
static void *mcachename##_alloc_no_mcache(void)					\
{										\
	init_##mcachename##_ator();						\
	return mcache_alloc(g_##mcachename##_mcache);				\
}										\
static void *mcachename##_paf_alloc_mcache(uint32_t paf)			\
{										\
	return mcache_paf_alloc(g_##mcachename##_mcache, paf);			\
}										\
static void *mcachename##_paf_alloc_no_mcache(uint32_t paf)			\
{										\
	init_##mcachename##_ator();						\
	return mcache_paf_alloc(g_##mcachename##_mcache, paf);			\
}										\
static void mcachename##_free_mcache(void *ptr)					\
{										\
	mcache_free(ptr);							\
}										\

#define DEFINE_MCACHE_FUNC(mcachename, cache_count, obj_size)			\
void *mcache_##mcachename##_malloc(void)					\
{										\
	return g_##mcachename##_alloc();					\
}										\
void *mcache_##mcachename##_zmalloc(void)					\
{										\
	void *ret = g_##mcachename##_alloc();					\
	if (likely(ret != NULL)) {						\
		NOFAIL(memset_s(ret, obj_size, 0, obj_size));			\
	}									\
	return ret;								\
}										\
void *mcache_##mcachename##_pafmalloc(uint32_t paf)				\
{										\
	return g_##mcachename##_paf_alloc(paf);					\
}										\
void *mcache_##mcachename##_pafzmalloc(uint32_t paf)				\
{										\
	void *ret = g_##mcachename##_paf_alloc(paf);				\
	if (likely(ret != NULL)) {						\
		NOFAIL(memset_s(ret, obj_size, 0, obj_size));			\
	}									\
	return ret;								\
}										\
void mcache_##mcachename##_free(void *ptr)					\
{										\
	g_##mcachename##_free(ptr);						\
}										\
size_t mcache_##mcachename##_reclaimable(void)					\
{										\
	size_t re = 0UL;							\
	if (likely(g_##mcachename##_mcache != NULL)) {				\
		re = mcache_size_type(g_##mcachename##_mcache, SLAB_USED_SIZE);	\
	}									\
	return re;								\
}										\

#define DEFINE_MCACHE_STATIC_FUNC(mcachename, cache_count, obj_size)			\
static void *mcache_##mcachename##_malloc(void)					\
{										\
	return g_##mcachename##_alloc();					\
}										\
static void *mcache_##mcachename##_zmalloc(void)				\
{										\
	void *ret = g_##mcachename##_alloc();					\
	if (likely(ret != NULL)) {						\
		NOFAIL(memset_s(ret, obj_size, 0, obj_size));			\
	}									\
	return ret;								\
}										\
static void mcache_##mcachename##_free(void *ptr)				\
{										\
	g_##mcachename##_free(ptr);						\
}										\

#define DEFINE_RECLAIMABLE_MCACHE_STATIC_FUNC(mcachename, cache_count, obj_size)			\
DEFINE_MCACHE_STATIC_FUNC(mcachename, cache_count, obj_size)					\
static size_t mcache_##mcachename##_reclaimable(void)				\
{										\
	size_t re = 0UL;							\
	if (likely(g_##mcachename##_mcache != NULL)) {				\
		re = mcache_size_type(g_##mcachename##_mcache, SLAB_USED_SIZE);	\
	}									\
	return re;								\
}										\

#define DEFINE_PAF_MCACHE_STATIC_FUNC(mcachename, cache_count, obj_size)			\
static void *mcache_##mcachename##_pafmalloc(uint32_t paf)				\
{										\
	return g_##mcachename##_paf_alloc(paf);					\
}										\
static void mcache_##mcachename##_free(void *ptr)				\
{										\
	g_##mcachename##_free(ptr);						\
}										\

#define DEFINE_MCACHE_STATIC_ZERO_FUNC(mcachename, cache_count, obj_size)		\
static void *mcache_##mcachename##_zmalloc(void)				\
{										\
	void *ret = g_##mcachename##_alloc();					\
	if (likely(ret != NULL)) {						\
		NOFAIL(memset_s(ret, obj_size, 0, obj_size));			\
	}									\
	return ret;								\
}										\
static void mcache_##mcachename##_free(void *ptr)				\
{										\
	g_##mcachename##_free(ptr);						\
}										\

#define DEFINE_RECLAIMABLE_MCACHE_STATIC_ZERO_FUNC(mcachename, cache_count, obj_size)		\
DEFINE_MCACHE_STATIC_ZERO_FUNC(mcachename, cache_count, obj_size)					\
static size_t mcache_##mcachename##_reclaimable(void)				\
{										\
	size_t re = 0UL;							\
	if (likely(g_##mcachename##_mcache != NULL)) {				\
		re = mcache_size_type(g_##mcachename##_mcache, SLAB_USED_SIZE);	\
	}									\
	return re;								\
}											\

#define DEFINE_MCACHE(mcachename, cache_count, obj_size)			\
__DEFINE_MCACHE(mcachename, cache_count, obj_size, 0)				\
DEFINE_MCACHE_FUNC(mcachename, cache_count, obj_size)					\

#define DEFINE_RECLAIMABLE_MCACHE(mcachename, cache_count, obj_size)			\
__DEFINE_MCACHE(mcachename, cache_count, obj_size, SLAB_RECLAIMABLE)				\
DEFINE_MCACHE_FUNC(mcachename, cache_count, obj_size)					\

#define DEFINE_MCACHE_STATIC(mcachename, cache_count, obj_size)			\
__DEFINE_MCACHE(mcachename, cache_count, obj_size, 0)				\
DEFINE_MCACHE_STATIC_FUNC(mcachename, cache_count, obj_size)			\

#define DEFINE_RECLAIMABLE_MCACHE_STATIC(mcachename, cache_count, obj_size)			\
__DEFINE_MCACHE(mcachename, cache_count, obj_size, SLAB_RECLAIMABLE)				\
DEFINE_RECLAIMABLE_MCACHE_STATIC_FUNC(mcachename, cache_count, obj_size)					\

#define DEFINE_MCACHE_STATIC_PAF(mcachename, cache_count, obj_size)			\
__DEFINE_MCACHE(mcachename, cache_count, obj_size, 0)				\
DEFINE_PAF_MCACHE_STATIC_FUNC(mcachename, cache_count, obj_size)					\

#define DEFINE_MCACHE_STATIC_ZERO(mcachename, cache_count, obj_size)			\
__DEFINE_MCACHE(mcachename, cache_count, obj_size, 0)				\
DEFINE_MCACHE_STATIC_ZERO_FUNC(mcachename, cache_count, obj_size)					\

#define DEFINE_RECLAIMABLE_MCACHE_STATIC_ZERO(mcachename, cache_count, obj_size)			\
__DEFINE_MCACHE(mcachename, cache_count, obj_size, SLAB_RECLAIMABLE)				\
DEFINE_RECLAIMABLE_MCACHE_STATIC_ZERO_FUNC(mcachename, cache_count, obj_size)					\

#ifdef CONFIG_VFS_FUSION
#define DEFINE_MCACHE_FS(mcachename, cache_count, obj_size)			\
DEFINE_MCACHE(mcachename, cache_count, obj_size)				\

#define DEFINE_MCACHE_FS_STATIC(mcachename, cache_count, obj_size)		\
DEFINE_MCACHE_STATIC(mcachename, cache_count, obj_size)				\

#define DEFINE_MCACHE_FS_STATIC_ZERO(mcachename, cache_count, obj_size)		\
DEFINE_MCACHE_STATIC_ZERO(mcachename, cache_count, obj_size)			\

#define DEFINE_RECLAIMABLE_MCACHE_FS_STATIC_ZERO(mcachename, cache_count, obj_size)		\
DEFINE_RECLAIMABLE_MCACHE_STATIC_ZERO(mcachename, cache_count, obj_size)			\

#define DEFINE_RECLAIMABLE_MCACHE_FS(mcachename, cache_count, obj_size)			\
DEFINE_RECLAIMABLE_MCACHE(mcachename, cache_count, obj_size)				\

#define DEFINE_RECLAIMABLE_MCACHE_FS_STATIC(mcachename, cache_count, obj_size)		\
DEFINE_RECLAIMABLE_MCACHE_STATIC(mcachename, cache_count, obj_size)				\

#else /* !CONFIG_VFS_FUSION */
#define DEFINE_MCACHE_FS(mcachename, cache_count, obj_size)			\
void *mcache_##mcachename##_malloc(void)					\
{										\
	return malloc(obj_size);						\
}										\
void *mcache_##mcachename##_zmalloc(void)					\
{										\
	void *ret = malloc(obj_size);						\
	if (likely(ret != NULL)) {						\
		NOFAIL(memset_s(ret, obj_size, 0, obj_size));			\
	}									\
	return ret;								\
}										\
void *mcache_##mcachename##_pafmalloc(uint32_t paf)				\
{										\
	return malloc(obj_size);						\
}										\
void *mcache_##mcachename##_pafzmalloc(uint32_t paf)				\
{										\
	void *ret = malloc(obj_size);						\
	if (likely(ret != NULL)) {						\
		NOFAIL(memset_s(ret, obj_size, 0, obj_size));			\
	}									\
	return ret;								\
}										\
void mcache_##mcachename##_free(void *ptr)					\
{										\
	free(ptr);						\
}										\
size_t mcache_##mcachename##_reclaimable(void)					\
{										\
	return 0UL;								\
}										\

#define DEFINE_MCACHE_FS_STATIC(mcachename, cache_count, obj_size)		\
static void *mcache_##mcachename##_malloc(void)					\
{										\
	return malloc(obj_size);					\
}										\
static void mcache_##mcachename##_free(void *ptr)				\
{										\
	free(ptr);						\
}										\

#define DEFINE_MCACHE_FS_STATIC_ZERO(mcachename, cache_count, obj_size)		\
static void *mcache_##mcachename##_zmalloc(void)				\
{										\
	void *__ret = malloc(obj_size);						\
	if (likely(__ret != NULL)) {						\
		NOFAIL(memset_s(__ret, obj_size, 0, obj_size));			\
	}									\
	return __ret;								\
}										\
static void mcache_##mcachename##_free(void *ptr)				\
{										\
	free(ptr);								\
}										\

#define DEFINE_RECLAIMABLE_MCACHE_FS(mcachename, cache_count, obj_size)			\
DEFINE_MCACHE_FS(mcachename, cache_count, obj_size)			\

#define DEFINE_RECLAIMABLE_MCACHE_FS_STATIC(mcachename, cache_count, obj_size)		\
DEFINE_MCACHE_FS_STATIC(mcachename, cache_count, obj_size)				\

#define DEFINE_RECLAIMABLE_MCACHE_FS_STATIC_ZERO(mcachename, cache_count, obj_size)		\
DEFINE_MCACHE_FS_STATIC_ZERO(mcachename, cache_count, obj_size) 		\

#endif /* CONFIG_VFS_FUSION */

/*
 * The mcache tools for strdup and variant length malloc
 */

#define F0(s, f, a, v)
#define F1(s, f, a, v, ...) f(a, v)
#define F2(s, f, a, v, ...) F1(s, f, a, v) s F1(s, f, a, __VA_ARGS__)
#define F3(s, f, a, v, ...) F1(s, f, a, v) s F2(s, f, a, __VA_ARGS__)
#define F4(s, f, a, v, ...) F1(s, f, a, v) s F3(s, f, a, __VA_ARGS__)
#define F5(s, f, a, v, ...) F1(s, f, a, v) s F4(s, f, a, __VA_ARGS__)
#define F6(s, f, a, v, ...) F1(s, f, a, v) s F5(s, f, a, __VA_ARGS__)
#define F7(s, f, a, v, ...) F1(s, f, a, v) s F6(s, f, a, __VA_ARGS__)
#define F8(s, f, a, v, ...) F1(s, f, a, v) s F7(s, f, a, __VA_ARGS__)
#define F9(s, f, a, v, ...) F1(s, f, a, v) s F8(s, f, a, __VA_ARGS__)
#define F10(s, f, a, v, ...) F1(s, f, a, v) s F9(s, f, a, __VA_ARGS__)
#define F11(s, f, a, v, ...) F1(s, f, a, v) s F10(s, f, a, __VA_ARGS__)
#define F12(s, f, a, v, ...) F1(s, f, a, v) s F11(s, f, a, __VA_ARGS__)

#define FOREACH(sep, func, num, args, ...)					\
F##num(sep, func, args, __VA_ARGS__)						\

typedef size_t (*index_ops)(size_t len);

#define VARIANT_ATOR_LEN		12
#define VARIANT_SLOT_ALIGN		8
#ifdef CONFIG_KASAN_DUMPSTACK
#define VARIANT_MCACHE_MAX		3848
#else
#define VARIANT_MCACHE_MAX		3912
#endif
#define VARIANT_MCACHE_SIZE							\
16, 24, 32, 40, 64, 96, 128, 256, 512, 1024, 2048, VARIANT_MCACHE_MAX		\

raw_static_assert(VARIANT_MCACHE_MAX < SLAB_MAX_LOWER_BOUND, variant_mcache_max_too_large);

#define __DEFINE_MCACHE_SIZE_NAMED(mcachename, size)				\
__DEFINE_MCACHE(mcachename##_##size, 20U, size, 0)					\

#define __DEFINE_RECLAIMABLE_MCACHE_SIZE_NAMED(mcachename, size)				\
__DEFINE_MCACHE(mcachename##_##size, 20U, size, SLAB_RECLAIMABLE)					\

#define __REGISTER_ONE_MCACHE(mcachename, size)					\
{										\
	g_##mcachename##_alloc_list[index] = g_##mcachename##_##size##_alloc;	\
	g_##mcachename##_free_list[index] = g_##mcachename##_##size##_free;	\
	g_##mcachename##_mcache_list[index] = g_##mcachename##_##size##_mcache;	\
	index += 1;								\
}										\

#define __DEFINE_MCACHE_CORE(mcachename, num, ...)				\
static size_t mcachename##_index(size_t len);					\
static size_t mcachename##_index_no_table(size_t len);				\
index_ops g_##mcachename##_index = mcachename##_index_no_table;			\
static size_t *g_##mcachename##_index_table = NULL;				\
static size_t g_##mcachename##_size_list[VARIANT_ATOR_LEN] = { __VA_ARGS__};	\
static mcache_alloc_ops g_##mcachename##_alloc_list[VARIANT_ATOR_LEN];		\
static mcache_free_ops g_##mcachename##_free_list[VARIANT_ATOR_LEN];		\
static struct mcache_s *g_##mcachename##_mcache_list[VARIANT_ATOR_LEN];		\
static void mcachename##_prepare_alloc_free_list(void)				\
{										\
	size_t index = 0;							\
FOREACH(, __REGISTER_ONE_MCACHE, num, mcachename, __VA_ARGS__)			\
}										\
static void mcachename##_prepare_index_table(void)				\
{										\
	size_t table_size = VARIANT_MCACHE_MAX / VARIANT_SLOT_ALIGN;		\
	size_t i = 0;								\
	size_t j = 0;								\
	g_##mcachename##_index_table = (size_t *)malloc(table_size * sizeof(size_t));	\
	for (i = 0; i < table_size; ++i) {					\
		if ((i * VARIANT_SLOT_ALIGN) == g_##mcachename##_size_list[j]) {	\
			j++;							\
		}								\
		g_##mcachename##_index_table[i] = j;				\
	}									\
}										\
static size_t mcachename##_index(size_t len)					\
{										\
	size_t table_index = (len - 1) / VARIANT_SLOT_ALIGN;			\
	return g_##mcachename##_index_table[table_index];			\
}										\
static size_t mcachename##_index_no_table(size_t len)				\
{										\
	size_t table_index = (len - 1) / VARIANT_SLOT_ALIGN;			\
	mcachename##_prepare_alloc_free_list();					\
	mcachename##_prepare_index_table();					\
	g_##mcachename##_index = mcachename##_index;				\
	return g_##mcachename##_index_table[table_index];			\
}										\

#define __DEFINE_MCACHE_VARIANT(mcachename, num, ...)				\
FOREACH(, __DEFINE_MCACHE_SIZE_NAMED, num, mcachename, __VA_ARGS__)		\
__DEFINE_MCACHE_CORE(mcachename, num, __VA_ARGS__)	\

#define __DEFINE_RECLAIMABLE_MCACHE_VARIANT(mcachename, num, ...)				\
FOREACH(, __DEFINE_RECLAIMABLE_MCACHE_SIZE_NAMED, num, mcachename, __VA_ARGS__)		\
__DEFINE_MCACHE_CORE(mcachename, num, __VA_ARGS__)	\

#define DECLARE_MCACHE_VARIANT(mcachename)					\
void *mcache_##mcachename##_malloc(size_t len);					\
void *mcache_##mcachename##_zmalloc(size_t len);				\
void mcache_##mcachename##_free(void *ptr);					\
size_t mcache_##mcachename##_reclaimable(void);					\

#define DECLARE_MCACHE_STRDUP(mcachename)					\
DECLARE_MCACHE_VARIANT(mcachename##_str)					\
char *mcache_##mcachename##_strdup(const char *str);				\

#define __DEFINE_MCACHE_VARIANT_API(mcachename)					\
void *mcache_##mcachename##_malloc(size_t len)					\
{										\
	size_t index = 0;							\
	void *ret = NULL;							\
	if (likely((len > 0) && (len < VARIANT_MCACHE_MAX))) {			\
		index = g_##mcachename##_index(len);				\
		ret = g_##mcachename##_alloc_list[index]();			\
	} else {								\
		ret = malloc(len);						\
	}									\
	return ret;								\
}										\
void *mcache_##mcachename##_zmalloc(size_t len)					\
{										\
	size_t index = 0;							\
	void *ret = NULL;							\
	if (likely((len > 0) && (len < VARIANT_MCACHE_MAX))) {			\
		index = g_##mcachename##_index(len);				\
		ret = g_##mcachename##_alloc_list[index]();			\
	} else {								\
		ret = malloc(len);						\
	}									\
	if (likely(ret != NULL)) {						\
		NOFAIL(memset_s(ret, len, 0, len));				\
	}									\
	return ret;								\
}										\
void mcache_##mcachename##_free(void *ptr)					\
{										\
	size_t len = malloc_usable_size(ptr);					\
	size_t index = 0;							\
	if (ptr != NULL) {							\
		if (likely((len > 0) && (len < VARIANT_MCACHE_MAX))) {		\
			index = g_##mcachename##_index(len);			\
			g_##mcachename##_free_list[index]((void *)ptr);		\
		} else {							\
			free(ptr);						\
		}								\
	}									\
}										\
size_t mcache_##mcachename##_reclaimable(void)					\
{										\
	size_t ret = 0;								\
	mcachename##_prepare_alloc_free_list();		\
	for (size_t i = 0; i < VARIANT_ATOR_LEN; ++i) {				\
		if (g_##mcachename##_mcache_list[i] != NULL) {			\
			ret += mcache_size_type(g_##mcachename##_mcache_list[i], SLAB_USED_SIZE);	\
		}								\
	}									\
	return ret;								\
}										\

#define __DEFINE_MCACHE_VARIANT_API_NORMAL(mcachename)				\
void *mcache_##mcachename##_malloc(size_t len)					\
{										\
	return malloc(len);							\
}										\
void *mcache_##mcachename##_zmalloc(size_t len)					\
{										\
	void *ret = malloc(len);						\
	if (likely(ret != NULL)) {						\
		NOFAIL(memset_s(ret, len, 0, len));				\
	}									\
	return ret;								\
}										\
void mcache_##mcachename##_free(void *ptr)					\
{										\
	if (ptr != NULL) {							\
		free(ptr);							\
	}									\
}										\
size_t mcache_##mcachename##_reclaimable(void)					\
{										\
	return 0UL;								\
}										\

#if defined(CONFIG_SYSMGR_KASAN) || defined(CONFIG_FSRV_KASAN)
#define DEFINE_MCACHE_VARIANT(mcachename)					\
__DEFINE_MCACHE_VARIANT_API_NORMAL(mcachename)					\

#define DEFINE_MCACHE_STRDUP(mcachename)					\
DEFINE_MCACHE_VARIANT(mcachename##_str)						\
char *mcache_##mcachename##_strdup(const char *str)				\
{										\
	return strdup(str);							\
}										\

#define DEFINE_RECLAIMABLE_MCACHE_VARIANT(mcachename)					\
DEFINE_MCACHE_VARIANT(mcachename)					\

#define DEFINE_RECLAIMABLE_MCACHE_STRDUP(mcachename)					\
DEFINE_MCACHE_STRDUP(mcachename)					\

#else /* !CONFIG_SYSMGR_KASAN && !CONFIG_FSRV_KASAN */
#define DEFINE_MCACHE_VARIANT(mcachename)					\
__DEFINE_MCACHE_VARIANT(mcachename, VARIANT_ATOR_LEN, VARIANT_MCACHE_SIZE)	\
__DEFINE_MCACHE_VARIANT_API(mcachename)						\

#define DEFINE_RECLAIMABLE_MCACHE_VARIANT(mcachename)					\
__DEFINE_RECLAIMABLE_MCACHE_VARIANT(mcachename, VARIANT_ATOR_LEN, VARIANT_MCACHE_SIZE)	\
__DEFINE_MCACHE_VARIANT_API(mcachename)						\

#define DEFINE_MCACHE_STRDUP(mcachename)					\
DEFINE_MCACHE_VARIANT(mcachename##_str)						\
char *mcache_##mcachename##_strdup(const char *str)				\
{										\
	size_t len = strlen(str) + 1;						\
	char *ret = mcache_##mcachename##_str_malloc(len);			\
	if (likely(ret != NULL)) {						\
		NOFAIL(memcpy_s(ret, len, str, len));				\
	}									\
	return ret;								\
}										\

#define DEFINE_RECLAIMABLE_MCACHE_STRDUP(mcachename)					\
DEFINE_RECLAIMABLE_MCACHE_VARIANT(mcachename##_str)						\
char *mcache_##mcachename##_strdup(const char *str)				\
{										\
	size_t len = strlen(str) + 1;						\
	char *ret = mcache_##mcachename##_str_malloc(len);			\
	if (likely(ret != NULL)) {						\
		NOFAIL(memcpy_s(ret, len, str, len));				\
	}									\
	return ret;								\
}										\

#endif /* CONFIG_SYSMGR_KASAN || CONFIG_FSRV_KASAN */

#ifdef CONFIG_VFS_FUSION
#define DEFINE_MCACHE_FS_VARIANT(mcachename)					\
DEFINE_RECLAIMABLE_MCACHE_VARIANT(mcachename)						\

#define DEFINE_MCACHE_FS_STRDUP(mcachename)					\
DEFINE_RECLAIMABLE_MCACHE_STRDUP(mcachename)						\

#else /* !CONFIG_VFS_FUSION */
#define DEFINE_MCACHE_FS_VARIANT(mcachename)					\
__DEFINE_MCACHE_VARIANT_API_NORMAL(mcachename)					\

#define DEFINE_MCACHE_FS_STRDUP(mcachename)					\
DEFINE_MCACHE_FS_VARIANT(mcachename##_str)					\
char *mcache_##mcachename##_strdup(const char *str)				\
{										\
	return strdup(str);							\
}										\

#endif /* CONFIG_VFS_FUSION */
#endif /* __SYSMGR_INCLUDE_INTAPI_MEM_MCACHE_H__ */
