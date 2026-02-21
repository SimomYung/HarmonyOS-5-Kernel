/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Mem slab function implementation
 * Author: Huawei OS Kernel Lab
 * Create: Sun May 26 22:33:25 2019
 */
#include <libmem/memm_slab.h>

#include <stdbool.h>

#include <libmem/slab_utils.h>
#include <libmem/utils.h>
#include <libmem/debug.h>
#ifdef CONFIG_SLAB_DEBUG_USAGE
#include <libmem/slab_track.h>
#endif
#include <hongmeng/types.h>
#include <hongmeng/panic.h>
#include <libhmlog/hmlog.h>
#include <hmkernel/compiler.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsync/raw_mutex.h>

#include "slab.h"
#ifdef CONFIG_PER_CPU_SLAB
#include "slab_per_cpu.h"
#endif

static int g_memm_slab_initialized = 0;

static struct slab_pool g_memm_slab_pool;
static struct slab_ator *g_sized_slab_ator;

/*
 * slab_allocator query array
 * give a size:(1~4096), query this size which after aligning to SLOT_ALIGN,
 * and return a allocator index whose obj_size is belongs to this size
 * here, value of g_size_to_ator_table only from 0~27, so unsigned char
 * is enough.
 */
static unsigned char *g_size_to_ator_table;

#define SLOT_ALIGN			8U

/*
 * Most CPU L1 cache lines are 32 bytes or 64 bytes, L2 cache lines
 * are 64 bytes or 128 bytes, for size larger than 32 bytes, we should
 * make slab size align with 32 bytes to get better performance.
 *
 * Some slot size is not aligned with 32 bytes to optimize the memory usage.
 */
static const uint32_t g_sized_array[MEMM_ROOT_SLAB_ATOR_NUM] = {
	SLOT_ALIGN * 2U,   SLOT_ALIGN * 3U,   SLOT_ALIGN * 4U,   SLOT_ALIGN * 5U,
	SLOT_ALIGN * 8U,
	SLOT_ALIGN * 12U,  SLOT_ALIGN * 16U,  SLOT_ALIGN * 20U,  SLOT_ALIGN * 24U,
	SLOT_ALIGN * 28U,  SLOT_ALIGN * 32U,  SLOT_ALIGN * 40U,  SLOT_ALIGN * 48U,
	SLOT_ALIGN * 56U,  SLOT_ALIGN * 64U,  SLOT_ALIGN * 80U,  SLOT_ALIGN * 96U,
	SLOT_ALIGN * 112U, SLOT_ALIGN * 128U, SLOT_ALIGN * 160U, SLOT_ALIGN * 192U,
	SLOT_ALIGN * 224U, SLOT_ALIGN * 256U, SLOT_ALIGN * 320U, SLOT_ALIGN * 384U,
	SLOT_ALIGN * 448U, (uint32_t)SLAB_MAX_MALLOC_SIZE,
};

#ifdef CONFIG_PER_CPU_SLAB
/*
 * Conditioned the pre-defined cache volume for each slab allocator of one cpu, a
 * length of two pages is needed to store the objs (pointers) in cache. Therefore,
 * in per-cpu scenario, we need extra 2 * CONFIG_NR_CPUS spaces as our cache pool,
 * storing objs.
 * The first 6 pages are used to store all slab allocators and the table.
 */
#define MEMM_SLAB_META_DATA_POLICY(num_cpus) ((unsigned int)(6 + (num_cpus) * 2) << PAGE_SHIFT)
#else
#define MEMM_SLAB_META_DATA ((unsigned int)2 << PAGE_SHIFT)
#endif

/*
 * static char g_sys_memm_slab_pool[MEMM_SLAB_META_DATA] __attribute__((aligned(PAGE_SIZE)));
 * g_sys_memm_slab_pool memory layout:
 *
 * |<------------------------------ g_memm_slab_meta_data -------------------------------------------------------->|
 *
 * +---------------------------------------------------------------------------------------------------------------+
 * | slab_ator[0] | slab_ator[1] | ... | slab_ator[27] |  g_size_to_ator_table  |     cache_pool     |     nop     |
 * +---------------------------------------------------------------------------------------------------------------+
 *
 * |<--------------- g_sized_slab_ator --------------->|< g_size_to_ator_table >|<--- cache_pool --->|<--- nop --->|
 *
 * g_sized_slab_ator: global slab allocator
 * g_size_to_ator_table: malloc size to allocator size change table
 * nop: unused
 *
 */
static void memm_slab_setup_lookup_table(size_t ator_table_index)
{
	unsigned char j;
	size_t i;
	/*
	 * i is the index of lookup table
	 * j is the index of slab allocator index
	 */
	for (i = 0, j = 0; i < ator_table_index; i++) {
		if ((j < MEMM_ROOT_SLAB_ATOR_NUM) &&
		    ((i * SLOT_ALIGN) == g_sized_array[j])) {
			j++;
		}
		g_size_to_ator_table[i] = j;
	}

	hm_debug("initial g_size_to_ator_table: i = %zu, j = %u\n", i, (unsigned int)j);
	if (j >= MEMM_ROOT_SLAB_ATOR_NUM ||
	    (i * SLOT_ALIGN) > g_sized_array[MEMM_ROOT_SLAB_ATOR_NUM - 1U]) {
		hm_panic("g_size_to_ator_table initial failed\n");
	}

	if (g_sized_array[j] != i * SLOT_ALIGN) {
		hm_error("i = %zu, j = %u, g_sized_array[j] = %"PRIu32"\n",
			 i, j, g_sized_array[j]);
		hm_panic("unexpected error in initial sized allocator\n");
	}
}

static int memm_slab_ator_create(const char *name, size_t slab_ator_num, uintptr_t *cache_pool)
{
	struct slab_cache_info_s info;

#ifdef CONFIG_PER_CPU_SLAB
	info.cache_pool = cache_pool;
	info.limit = slab_ator_cache_limit(g_sized_array[slab_ator_num]);
#else
	UNUSED(cache_pool);
#endif
	return slab_ator_init(&g_memm_slab_pool, &g_sized_slab_ator[slab_ator_num],
			      g_sized_array[slab_ator_num], name, &info, 0);
}

static void memm_slab_init_each_allocator(uintptr_t *cache_pool)
{
	size_t i;

	for (i = 0; i < MEMM_ROOT_SLAB_ATOR_NUM; i++) {
		char name[SLAB_NAME_LEN];
		int ret;
		if (snprintf_s(name, SLAB_NAME_LEN, SLAB_NAME_LEN - 1U, "slab-[%"PRIu32"]", g_sized_array[i]) < 0) {
			hm_panic("snprintf_s error\n");
		}
		ret = memm_slab_ator_create(name, i, cache_pool);
		BUG_ON(ret != E_HM_OK);
	}
}

void memm_slab_extend_pool_lock(void)
{
	raw_mutex_lock(&g_memm_slab_pool.extend_lock);
}

void memm_slab_extend_pool_unlock(void)
{
	raw_mutex_unlock(&g_memm_slab_pool.extend_lock);
}

void memm_slab_shrink_pool_lock(void)
{
	raw_mutex_lock(&g_memm_slab_pool.shrink_lock);
}

int memm_slab_shrink_pool_trylock(void)
{
	return raw_mutex_trylock(&g_memm_slab_pool.shrink_lock);
}

void memm_slab_shrink_pool_unlock(void)
{
	raw_mutex_unlock(&g_memm_slab_pool.shrink_lock);
}

static slab_cb_s slab_error_cb = NULL;
void memm_slab_error_cb(uintptr_t slab_desc)
{
	if (slab_error_cb) {
		slab_error_cb(slab_desc);
	}
}

void memm_slab_init(uintptr_t start, size_t size, size_t num_cpus, slab_cb_s cb)
{
	size_t all_ator_space, slab_table_space;
	unsigned char *meta_start = NULL;
	size_t ator_table_index;
	size_t need_space;
	uintptr_t cache_pool;
#ifdef CONFIG_PER_CPU_SLAB
	size_t cache_space;
#endif

	slab_error_cb = cb;
	if (g_memm_slab_initialized != 0) {
		hm_panic("memm_slab initial twice !\n");
	}

#ifdef CONFIG_SLAB_DEBUG
	slab_parse_cmdline_config();
#endif
	/*
	 * space for lookup table
	 * since the slab system only supports a maximum size of
	 * SLOT_ALIGN * 256 , for any size,
	 * first align to SLOT_ALIGN(16byte), then the largest allocator
	 * can support 256 number of 16bytes (4096 / 16 = 256).
	 * Therefore, the index of the index table is up to 256
	 */
	ator_table_index = (size_t)(g_sized_array[MEMM_ROOT_SLAB_ATOR_NUM - 1U]) / (size_t)SLOT_ALIGN;
	/* each value of index is char */
	slab_table_space = ator_table_index * sizeof(*g_size_to_ator_table);
	/* space for all slab allocator */
	all_ator_space = MEMM_ROOT_SLAB_ATOR_NUM * sizeof(struct slab_ator);
#ifdef CONFIG_PER_CPU_SLAB
	/* setup the number of cpu */
	slab_cpu_num_setup((uint32_t)num_cpus);
	/* space for all caches */
	cache_space = slab_cache_pool_size(g_sized_array, MEMM_ROOT_SLAB_ATOR_NUM);
	/* need space is a meta data of slab and look up table */
	need_space = all_ator_space + slab_table_space + cache_space;
	if (need_space > MEMM_SLAB_META_DATA_POLICY(num_cpus)) {
		hm_panic("need_space should (need_space < %u)\n", MEMM_SLAB_META_DATA_POLICY(num_cpus));
	}
#else
	UNUSED(num_cpus);
	need_space = all_ator_space + slab_table_space;
	if (need_space > MEMM_SLAB_META_DATA) {
		hm_panic("need_space should (need_space < %u)\n", MEMM_SLAB_META_DATA);
	}
#endif

	hm_debug("slab hold meta size is %zu, max malloc size %zu\n",
		 need_space, slab_max_malloc());

	if (size < need_space) {
		hm_panic("memm_slab given size:(%zu) is smaller than min size:(%zu)!\n",
			 size, need_space);
	}

	slab_pool_init(&g_memm_slab_pool);

	/* here should add in total size */
	(void)raw_atomic_ullong_add_relaxed(&g_memm_slab_pool.total_size, (uint64_t)size);
	(void)raw_atomic_ullong_add_relaxed(&g_memm_slab_pool.real_total_size, size);

	meta_start = (unsigned char *)start;

	g_sized_slab_ator = (struct slab_ator *)start;
	g_size_to_ator_table = meta_start + all_ator_space;

	/* setup lookup table, size(1~4096) -> index(0~27) */
	memm_slab_setup_lookup_table(ator_table_index);

	cache_pool = (uintptr_t)g_size_to_ator_table + ator_table_index * sizeof(*g_size_to_ator_table);

	/* initial each allocator according to given size */
	memm_slab_init_each_allocator(&cache_pool);

	g_memm_slab_initialized = 1;
}

#ifdef CONFIG_SLAB_FREELIST_RANDOM
void memm_slab_update_random(unsigned long random_value)
{
	slab_update_random_seed(random_value);
}
#endif

static int check_and_find_index(size_t size)
{
	int index = 0;
	/* check args and status */
	if (unlikely(g_memm_slab_initialized == 0)) {
		hm_panic("memm_slab has not been initialized yet!\n");
	}

	if (unlikely((g_size_to_ator_table == NULL) || (g_sized_slab_ator == NULL))) {
		hm_warn("g_size_to_ator_table or g_sized_slab_ator is NULL\n");
		hm_panic("unexpected error happened in memm_slab\n");
	}

	if (unlikely((size == 0UL) || (size > g_sized_array[MEMM_ROOT_SLAB_ATOR_NUM - 1U]))) {
		hm_warn("alloc for slab size %zu is out of range:(1~%ubytes)\n",
			size, g_sized_array[MEMM_ROOT_SLAB_ATOR_NUM - 1U]);
		index = E_HM_INVAL;
	} else {
		/* choose allocator based on lookup table */
		index = g_size_to_ator_table[(size - 1UL) / SLOT_ALIGN];
	}
	return index;
}

bool memm_slab_report(size_t size)
{
	bool ret_bool = false;
	int index;

	index = check_and_find_index(size);
	if (index >= 0) {
		/* report information allocation from corresponding allocator */
		ret_bool = slab_report(&(g_sized_slab_ator[index]));
	}
	return ret_bool;
}

void *memm_slab_alloc(size_t size)
{
	void *ptr = NULL;
	int index;

	index = check_and_find_index(size);
	if (index >= 0) {
		/* do allocation from corresponding allocator */
		ptr = slab_alloc(&(g_sized_slab_ator[index]));
	}

	return ptr;
}

void memm_slab_free(const void *obj)
{
	struct slab_ator *s_ator = slab_obj_to_ator(obj);

	if (unlikely(g_memm_slab_initialized == 0)) {
		hm_panic("memm_slab has not been initialized yet!\n");
	}

	if (unlikely(s_ator == NULL)) {
		hm_panic("search allocator from obj failed, maybe use error address:(0x%lx)\n",
			 ptr_to_ulong(obj));
	} else {
		slab_free(s_ator, (void *)obj);
	}
}

size_t memm_slab_alloc_usable_size(const void *obj)
{
	size_t usable_size = 0;
	struct slab_ator *s_ator = slab_obj_to_ator(obj);
	if (s_ator != NULL) {
		usable_size = slab_slot_size(s_ator);
	}

	return usable_size;
}

int memm_slab_extend(uintptr_t start, size_t size)
{
	if (unlikely(g_memm_slab_initialized == 0)) {
		hm_panic("memm_slab has not initial yet!\n");
	}

	return slab_pool_push_memory(&g_memm_slab_pool, start, size);
}

uintptr_t memm_slab_shrink(void)
{
	if (unlikely(g_memm_slab_initialized == 0)) {
		hm_panic("memm_slab has not initial yet!\n");
	}

	return slab_pool_pop_memory(&g_memm_slab_pool);
}

static void dump_slab_all_sized_ator(struct memm_slab_ator_info info[],
				     size_t size,
				     struct slab_slot_info_s *slot_info)
{
	size_t i;
	struct slab_ator *ator = NULL;
	struct slab_ator_info_s *ator_info = NULL;

	for (i = 0; i < size; i++) {
		ator = &g_sized_slab_ator[i];
		ator_info = &(info[i].ator_info);
		slab_dump_ator(ator, ator_info, slot_info);
	}
}

/* caller must holden g_memm_slab_pool.lock */
static inline void memm_slab_pool_dump(void)
{
	slab_dump_pool_desc(&g_memm_slab_pool);
}

size_t memm_slab_size_type_nolock(enum slab_size_type type)
{
	size_t ret;

	if (likely((g_memm_slab_initialized != 0) &&
		   (type >= SLAB_TOTAL_SIZE) &&
		   (type < NR_SLAB_SIZE_TYPE))) {
		ret = slab_mem_size(&g_memm_slab_pool, type);
	} else {
		ret = 0UL;
	}

	return ret;
}

size_t memm_slab_size_type(enum slab_size_type type)
{
	if (unlikely(g_memm_slab_initialized == 0)) {
		hm_panic("memm_slab has not initial yet!\n");
	}

	RAW_MUTEX_GUARD(_, &g_memm_slab_pool.lock);
	return memm_slab_size_type_nolock(type);
}

/*
 * memm_slab_dump_pool_nolock - dump all slab page in slab pool
 *
 * Note: caller must ensure the validity of @info pointer
 */
void memm_slab_dump_pool_nolock(struct memm_slab_info *info)
{
	if (unlikely(g_memm_slab_initialized == 0)) {
		hm_panic("memm_slab has not initial yet!\n");
	}

	slab_dump_nolock(&info->slab_info, &g_memm_slab_pool);
}

void memm_slab_dump_pool(struct memm_slab_info *info)
{
	if (unlikely(info == NULL)) {
		hm_debug("invalid parameters\n");
	} else {
		if (unlikely(g_memm_slab_initialized == 0)) {
			hm_panic("memm_slab has not initial yet!\n");
		}

		raw_mutex_lock(&g_memm_slab_pool.lock);
		slab_dump_nolock(&info->slab_info, &g_memm_slab_pool);
		raw_mutex_unlock(&g_memm_slab_pool.lock);
	}
}

void memm_slab_dump(struct memm_slab_info *info)
{
	if (unlikely(info == NULL)) {
		hm_debug("invalid parameters\n");
	} else {
		raw_mutex_lock(&g_memm_slab_pool.lock);
		memm_slab_dump_pool_nolock(info);
		memm_slab_pool_dump();
		raw_mutex_unlock(&g_memm_slab_pool.lock);
		/* dump allocator first */
		/* every sized allocator info */
		hmmem_debug("======== SIZED ALLOCATOR INFOMATION ========\n");
		dump_slab_all_sized_ator(info->sized_ator_info, MEMM_ROOT_SLAB_ATOR_NUM, &info->slot_info);
	}
}

size_t memm_slab_get_ator_free_size(void)
{
	size_t i;
	size_t free_size = 0;

	for (i = 0; i < MEMM_ROOT_SLAB_ATOR_NUM; i++) {
		free_size += slab_get_ator_free_size(&g_sized_slab_ator[i]);
	}

	return free_size;
}

#ifdef CONFIG_SLAB_DEBUG_USAGE
static struct slab_ator *memm_slab_ator_via_name(const char *name)
{
	struct slab_ator *ator = NULL;

	for (size_t i = 0; i < MEMM_SLAB_ALL_ATOR_INFO_NUM; i++) {
		if (strcmp(name, g_sized_slab_ator[i].name) == 0) {
			ator = &g_sized_slab_ator[i];
			break;
		}
	}

	return ator;
}

int memm_slab_dump_usage(struct track_head *th, uint32_t s_flags, const char *name)
{
	int ret = 0;
	struct slab_ator *ator = NULL;

	if ((ator = memm_slab_ator_via_name(name)) != NULL) {
		slab_dump_ator_usage(th, ator, s_flags);
		ret = 1;
	}

	return ret;
}
#endif
