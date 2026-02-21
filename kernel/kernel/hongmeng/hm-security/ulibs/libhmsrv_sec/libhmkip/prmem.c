/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation of HM KIP pmalloc
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 4 02:21:13 2023
 */

#include <libhmkip/prmem.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>

/* enum prmem_errno - error states that can be returned within prmem */
enum prmem_errno {
	PRMEM_OK = 0,
	PRMEM_VA_NOT_RECLAIMABLE,
	PRMEM_NODE_NOT_FOUND,
	PRMEM_NODES_BATCH_FAIL,
	PRMEM_POOL_UNKNOWN_TYPE,
	PRMEM_NO_SPACE,
	PRMEM_INVALID_INPUT_PARAMETERS,
	PRMEM_NO_NODE,
	PRMEM_NO_VA,
	PRMEM_NO_GROW,
	PRMEM_INVALID_ADDRESS,
	PRMEM_POOL_IS_START_WR,
	PRMEM_POOL_NOT_RECLAIMABLE,
	PRMEM_WRITE_ERR,
	PRMEM_MEMSET_ERR,
	PRMEM_CACHE_EXHAUSTED,
	PRMEM_ERR_NUMBERS,
};

static const char *error_messages[(int)PRMEM_ERR_NUMBERS] = {
	[(int)PRMEM_OK] = "PRMEM OK",
	[(int)PRMEM_VA_NOT_RECLAIMABLE] = "Not reclaimable vmap_area",
	[(int)PRMEM_NODE_NOT_FOUND] = "Not found prmem node",
	[(int)PRMEM_NODES_BATCH_FAIL] = "Failed allocating prmem_nodes batch",
	[(int)PRMEM_NO_SPACE] = "Could not allocate space",
	[(int)PRMEM_POOL_UNKNOWN_TYPE] = "Pool has unknown type",
	[(int)PRMEM_INVALID_INPUT_PARAMETERS] = "Prmem invalid input parameters",
	[(int)PRMEM_NO_NODE] = "Could not allocate node",
	[(int)PRMEM_NO_VA] = "Failed to allocate from mmap",
	[(int)PRMEM_NO_GROW] = "Failed to grow the memory pool",
	[(int)PRMEM_INVALID_ADDRESS] = "Not prmem memory",
	[(int)PRMEM_POOL_IS_START_WR] = "Trying to protect a start_wr pool",
	[(int)PRMEM_POOL_NOT_RECLAIMABLE] = "Pool is not reclaimable",
	[(int)PRMEM_WRITE_ERR] = "Write to memory failed",
	[(int)PRMEM_MEMSET_ERR] = "Memset failed",
	[(int)PRMEM_CACHE_EXHAUSTED] = "Cache has no node",
};

/**
 * prmem_memcpy() - copies n bytes from q to p
 * @p: beginning of the memory to write to
 * @q: beginning of the memory to read from
 * @n: amount of bytes to copy
 * Returns pointer to the destination
 */
void prmem_memcpy(void *p, const void *q, size_t n, struct prmem_pool *pool)
{
	if (p == NULL || q == NULL || !prmem_is_wr(p, n, pool)) {
		return;
	}
	hm_hkip_rowm_cpy(p, q, n);
}

static inline void __prmem_err(enum prmem_errno pr_errno,
			       const char *filename, const char *func,
			       unsigned int line_nr)
{
	if (likely((pr_errno >= PRMEM_ERR_NUMBERS) || (filename == NULL) || (func == NULL))) {
		return;
	}
	hm_error("[%s:%u %s] %s\n", filename, line_nr, func,
		 error_messages[pr_errno]);
}

#define prmem_err(pr_errno)  __prmem_err(pr_errno, __FILE_NAME__, __func__, __LINE__)

#if defined __HOST_LLT__ || !defined CONFIG_HKIP_PROTECT_PRMEM_META
#define prmem_assign_private(dst, val)					\
	do {								\
		typeof((dst)) *dst_ptr = &(dst);			\
		typeof(*dst_ptr) tmp = (typeof(tmp))(val);		\
		memcpy_s(dst_ptr, sizeof(tmp), &tmp, sizeof(tmp));	\
	} while (0)

#define prmem_add(dst, val)						\
	do {								\
		typeof((dst)) *dst_ptr = &(dst);			\
		typeof(*dst_ptr) tmp = (*dst_ptr) + (typeof(tmp))(val);	\
		memcpy_s(dst_ptr, sizeof(tmp), &tmp, sizeof(tmp));	\
	} while (0)

#define prmem_sub(dst, val)						\
	do {								\
		typeof((dst)) *dst_ptr = &(dst);			\
		typeof(*dst_ptr) tmp = (*dst_ptr) - (typeof(tmp))(val);	\
		memcpy_s(dst_ptr, sizeof(tmp), &tmp, sizeof(tmp));	\
	} while (0)
#else
#define prmem_assign_private(dst, val)				\
	do {							\
		typeof((dst)) *dst_ptr = &(dst);		\
		typeof(*dst_ptr) tmp = (typeof(tmp))(val);	\
		hm_hkip_rowm_cpy(dst_ptr, &tmp, sizeof(tmp));	\
	} while (0)

#define prmem_add(dst, val)						\
	do {								\
		typeof((dst)) *dst_ptr = &(dst);			\
		typeof(*dst_ptr) tmp = (*dst_ptr) + (typeof(tmp))(val);	\
		hm_hkip_rowm_cpy(dst_ptr, &tmp, sizeof(tmp));		\
	} while (0)

#define prmem_sub(dst, val)						\
	do {								\
		typeof((dst)) *dst_ptr = &(dst);			\
		typeof(*dst_ptr) tmp = (*dst_ptr) - (typeof(tmp))(val);	\
		hm_hkip_rowm_cpy(dst_ptr, &tmp, sizeof(tmp));		\
	} while (0)
#endif

#ifndef __HOST_LLT__
char *pstrdup(struct prmem_pool *pool, const char *s, enum prmem_flags flags)
{
	size_t len;
	char *buf = NULL;

	if (unlikely(!pool || !s)) {
		prmem_err(PRMEM_INVALID_INPUT_PARAMETERS);
		return NULL;
	}

	len = strlen(s) + 1;
	buf = pmalloc(pool, len, flags);
	if (unlikely(!buf))
		return NULL;
	if (pmalloc_is_start_wr(pool)) {
		hm_hkip_rowm_cpy(buf, s, len);
	} else {
		if (strncpy_s(buf, len, s, len) != 0) {
			prmem_err(PRMEM_WRITE_ERR);
			return NULL;
		}
	}
	return buf;
}
#endif

static uint32_t get_prot_from_type(enum prmem_pool_type type)
{
	switch (type) {
	case PRMEM_POOL_RO_NO_RECL:
		return MPROT_NONE;
	case PRMEM_POOL_START_WR_NO_RECL:
	case PRMEM_POOL_WR_NO_RECL:
		return MPROT_HKIP_ROWM;
	case PRMEM_POOL_START_WR_RECL:
	case PRMEM_POOL_WR_RECL:
		return MPROT_HKIP_ROWM | MPROT_HKIP_REVOCABLE;
	case PRMEM_POOL_RO_RECL:
		return MPROT_HKIP_REVOCABLE;
	default:
		prmem_err(PRMEM_POOL_UNKNOWN_TYPE);
		return MPROT_NONE;
	}
}

static void prmem_protect_range(struct prmem_node *node, const size_t start,
				const size_t size)
{
	int ret;

	if (unlikely(node->locked != 0)) {
		return;
	}

	ret = hm_mem_mprotect((void*)start, size, MPROT_READ);
	if (likely(ret == 0)) {
		prmem_assign_private(node->locked, 1);
	}
}

#define BITS_PER_BYTE		8U

static inline unsigned int __flsl(unsigned long v)
{
	return (v == 0UL) ? 0u
		: (unsigned int)(sizeof(v) * BITS_PER_BYTE - (unsigned int)__builtin_clzl(v));
}

static inline unsigned long roundup_pow_of_two(unsigned long n)
{
	return 1UL << __flsl(n - 1);
}

#define LT_MINUS_1		(-2)

static inline size_t get_alignment(struct prmem_pool *pool, size_t size, int alignment)
{
	if (alignment < -1) {
		return roundup_pow_of_two(size);
	} else if (alignment == -1) {
		return pool->alignment;
	} else {
		return (size_t)(unsigned)alignment;
	}
}

static void* prmem_cache_alloc(struct prmem_cache *cache,
			       enum prmem_flags flags)
{
	struct prmem_node *node = NULL;
	if (cache == NULL) {
		prmem_err(PRMEM_INVALID_INPUT_PARAMETERS);
		return NULL;
	}
	raw_mutex_lock(cache->mutex);
	node = (void*)cache->node;
	if(likely(node)) {
		prmem_assign_private(cache->node, cache->node->node);
		prmem_sub(cache->count, 1);
		if ((unsigned int)flags & PRMEM_ZERO) {
			prmem_memset(node, 0, cache->size, cache->pool);
		}
	} else {
		prmem_err(PRMEM_CACHE_EXHAUSTED);
	}
	raw_mutex_unlock(cache->mutex);
	return node;
}

/* --------------------   runtime allocator --------------------------- */

static enum prmem_protection_states protected_status __ro_after_init;
static enum prmem_states prmem_status __ro_after_init = PRMEM_DISABLED;

static const char *prmem_states_names[] = {
	[PRMEM_DISABLED] = "disabled",
	[PRMEM_ENABLED] = "enabled",
};

inline const char *get_prmem_status(void)
{
	if ((int)prmem_status > (int)PRMEM_ENABLED)
		return "error status";
	return prmem_states_names[prmem_status];
}

inline bool prmem_active(void)
{
#ifdef CONFIG_HKIP_PRMEM_SEHARMONY_DEFAULT_PROT
	return true;
#else
	return prmem_status == PRMEM_ENABLED;
#endif
}

inline void set_prmem_status(enum prmem_states status)
{
	prmem_status = status;
}

/**
 * prmem_declare_ptr_twin() - replica of a variable from its ptr
 * @ptr_pr_var: the pointer to the variable to replicate
 */
#define prmem_declare_ptr_twin(ptr_pr_var)					\
	typeof(*(ptr_pr_var)) twin_##ptr_pr_var

/**
 * prmem_twin() - references the twin of a variable or its pointer
 * @pr_var: the original variable or pointer to it
 */
#define prmem_twin(pr_var)	(twin_##pr_var)

/**
 * prmem_ptr_sync_from_pr() - sync the pointer to the original to its replica
 * @ptr_pr_var: the original protected variable
 */
#define prmem_ptr_sync_from_pr(ptr_pr_var) (twin_##ptr_pr_var = *(ptr_pr_var))

#ifndef __HOST_LLT__
/**
 * prmem_ptr_sync_to_pr() - sync the replica to the pointer to the original
 * @ptr_pr_var: the original protected variable
 */
#define prmem_ptr_sync_to_pr(ptr_pr_var)				\
	hm_hkip_rowm_cpy(ptr_pr_var, &(twin_##ptr_pr_var), sizeof(twin_##ptr_pr_var))
#endif

static void prmem_activate_node(struct prmem_node *node,
				void* rvaddr, size_t size,
				struct prmem_pool *pool,
				struct prmem_node **list)
{
	prmem_declare_ptr_twin(node);
	prmem_ptr_sync_from_pr(node);

	prmem_twin(node).pool = pool;
	prmem_twin(node).next = (*list);
	prmem_twin(node).start = (uintptr_t)rvaddr;
	prmem_twin(node).size = size;
	prmem_twin(node).locked = 0;
#if defined __HOST_LLT__ || !defined CONFIG_HKIP_PROTECT_PRMEM_META
	memcpy_s(node, sizeof(prmem_twin(node)), &(prmem_twin(node)), sizeof(prmem_twin(node)));
#else
	prmem_ptr_sync_to_pr(node);
#endif
	if (memset_s((void *)(uintptr_t)node->start, node->size, 0, node->size) != E_HM_OK) {
		prmem_err(PRMEM_MEMSET_ERR);
	}
	if (pmalloc_is_start_wr((void *)(uintptr_t)node->pool)) {
		prmem_protect_range(node, node->start, node->size);
	}
	prmem_assign_private((*list), node);
}

/* ------------------- prmem self managed memory ---------------------- */
/**
 *   /<  ...   PRMEM_AUX_MAX_SIZE   ...                               >/
 *      ^ PRMEM_AUX_HEAD
 *   /current offset (sizeof(size_t))  / data  .......                 /
 * allocate a memory chunk of size @PRMEM_AUX_MAX_SIZE
 * @PRMEM_AUX_HEAD points to the head of the memory
 * the offset is at the beginning of memory, i.e. (size_t *)@PRMEM_AUX_HEAD
 */


static void* prmem_aux_head __ro_after_init;

static struct raw_mutex prmem_aux_head_lock = RAW_MUTEX_INITIALIZER;

static const size_t prmem_aux_max_size = PAGE_SIZE;

/**
 * aux_alloc is used to allocate memory for cache (prmem_nodes) and pools,
 * once aux_protect is called, it can (and should) no longer be used
 */
static void* aux_alloc(size_t size)
{
	raw_mutex_lock(&prmem_aux_head_lock);
	size_t prmem_aux_offset = *(size_t*)prmem_aux_head;
	size_t new_offset = size + prmem_aux_offset;
	if (new_offset < prmem_aux_max_size && prmem_aux_offset < new_offset) {
		/* update offset, the memory is not protected at this moment */
		prmem_assign_private((*(size_t*)prmem_aux_head), new_offset);
		raw_mutex_unlock(&prmem_aux_head_lock);
	} else {
		raw_mutex_unlock(&prmem_aux_head_lock);
		hm_panic("Not enough space for prmem data structure!\n");
	}
	return (void*)((uintptr_t)prmem_aux_head + prmem_aux_offset);
}

void prmem_aux_init(void)
{
	static bool inited = false;
	static struct raw_mutex	mutex = RAW_MUTEX_INITIALIZER;
	if (inited) {
		return;
	}
	raw_mutex_lock(&mutex);
	if (inited) {
		raw_mutex_unlock(&mutex);
		return;
	}
#ifdef CONFIG_HKIP_PROTECT_PRMEM_META
	prmem_aux_head = hm_mem_mmap(0, prmem_aux_max_size, MPROT_READ | MPROT_HKIP | MPROT_HKIP_ROWM,
				     MAP_PRIVATE | MAP_ANONYMOUS | MFLAG_MAP_HKIP, -1, 0);
#else
	prmem_aux_head = hm_mem_mmap(0, prmem_aux_max_size, MPROT_READ | MPROT_WRITE,
				     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif

	if (prmem_aux_head == MAP_FAILED) {
		raw_mutex_unlock(&mutex);
		hm_panic("Prmem init failed!\n");
	}
	/* initially, sizeof(size_t) is already used to store offset */
	prmem_assign_private((*(size_t*)prmem_aux_head), sizeof(size_t));
	inited = true;
	raw_mutex_unlock(&mutex);
}

/* ------------------- Protected ranges management -------------------- */


#ifdef CONFIG_HKIP_PRMEM_ALIGN_ENFORCE
#define SZ_ALIGN (0x00200000UL)
#else
#define SZ_ALIGN (PAGE_SIZE)
#endif
static size_t node_size_align(void)
{
#ifdef __HOST_LLT__
	return (size_t)PAGE_SIZE;
#else
	/* For hkip, we need 2m align, otherwise el2 pagetable memory is not enough */
	if (prmem_status == PRMEM_ENABLED) {
		return ALIGN_UP((size_t)(PAGE_SIZE), SZ_ALIGN);
	}
	return (size_t)(PAGE_SIZE);
#endif
}

void prmem_pool(struct prmem_pool **pool_ptr, const struct prmem_pool *pool)
{
	*pool_ptr = aux_alloc(sizeof(struct prmem_pool));
	prmem_assign_private(**pool_ptr, *pool);
	if (pool->refill == PRMEM_UNDEFINED_REFILL) {
		size_t size_align = node_size_align();
		prmem_assign_private(((*pool_ptr)->refill), size_align);
	}
}

prmem_cache_declare(prmem_nodes, PRMEM_CACHE_NO_POOL, sizeof(struct prmem_node), sizeof(void *));


static inline void prmem_put_node(struct prmem_node *node)
{
	prmem_cache_free(prmem_nodes, node);
}

static inline bool prmem_node_is_req(enum prmem_flags flags)
{
	return !!((unsigned int)flags & ((int)PRMEM_NODE | (int)PRMEM_FREEABLE_NODE));
}

static inline bool prmem_node_is_freeable(enum prmem_flags flags)
{
	return !!((unsigned int)flags & ((int)PRMEM_FREEABLE_NODE));
}

static inline bool prmem_pool_can_grow(struct prmem_pool *pool, size_t size)
{
	return ((pool->cap == PRMEM_NO_CAP) ||
		((pool->allocated + size <= pool->cap) && (pool->allocated < pool->allocated + size)));
}

static struct prmem_node *pool_add_node(struct prmem_pool *pool, size_t size,
					enum prmem_flags flags)
{
	struct prmem_node **list = NULL;
	int type = (int)prmem_pool_to_prmem_type(pool);
	struct prmem_node *node = NULL;

	if (type < 0) {
		prmem_err(PRMEM_POOL_UNKNOWN_TYPE);
		return NULL;
	}
	if (prmem_node_is_req(flags)) {
		if (prmem_type_is_recl((enum prmem_type)type) && prmem_node_is_freeable(flags)) {
			list = &(pool->recl_list);
		} else {
			list = &(pool->no_recl_list);
		}
	} else {
		size = (size > pool->refill) ? size : pool->refill;
		list = &(pool->pmalloc_list);
	}

	size = PAGE_ALIGN_UP(size);
	if (unlikely(!prmem_pool_can_grow(pool, size))) {
		prmem_err(PRMEM_NO_GROW);
		return NULL;
	}

	node = prmem_cache_alloc(prmem_nodes, flags);
	if (unlikely(node == NULL)) {
		prmem_err(PRMEM_NO_NODE);
		return NULL;
	}

	/* need to tell hm_mem_mmap to allocate memory immediately
	 * e.g. adding a flag in mmap for KIP
	 */
#ifdef CONFIG_SEHARMONY_POLICY_SHARE
	void* rvaddr;
	if (pool->supervs_vaddr == NULL) {
		rvaddr = hm_mem_mmap(0, size, MPROT_READ | MPROT_WRITE | MPROT_HKIP | get_prot_from_type(pool->type),
				     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	} else {
		rvaddr = ulong_to_ptr(ptr_to_ulong(pool->supervs_vaddr) + (unsigned long)pool->allocated, void);
	}
#else
	void* rvaddr = hm_mem_mmap(0, size, MPROT_READ | MPROT_WRITE | MPROT_HKIP | get_prot_from_type(pool->type),
				   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
	if (rvaddr == MAP_FAILED) {
		hm_panic("Prmem add node failed: %s!\n", error_messages[PRMEM_NO_VA]);
	}
	prmem_activate_node(node, rvaddr, size, pool, list);
	if (*list == pool->pmalloc_list) {
		if (prmem_type_is_start_wr((enum prmem_type)type)) {
			if (!prmem_pool_pmalloc_protected(pool)) {
				prmem_assign_private(pool->protection_status, protected_status);
			}
		} else if (prmem_pool_pmalloc_protected(pool)) {
			prmem_assign_private(pool->protection_status, PMALLOC_UNPROTECTED);
		}
	}
	prmem_add(pool->allocated, size);
	if (pool->allocated > pool->peak_allocated) {
		prmem_assign_private(pool->peak_allocated, pool->allocated);
	}
	return node;
}

static void pool_free_node(struct prmem_node **n)
{
	struct prmem_node *node = *n;

	prmem_assign_private(*n, (*n)->next);
	prmem_sub(node->pool->allocated, node->size);
	(void)hm_mem_munmap((void*)node->start, node->size);
	prmem_assign_private(node->locked, 0);
	prmem_put_node(node);
}

static void debug_print_list(struct prmem_node *node)
{
	for (struct prmem_node *iter = node; iter; iter = iter->next) {
		hm_debug("node:%p, next:%p\t", iter, iter->next);
	}
}

void debug_print_pool(struct prmem_pool *pool)
{
	hm_debug("malloc list:");
	debug_print_list(pool->pmalloc_list);
	hm_debug("\nrecl list:");
	debug_print_list(pool->recl_list);
	hm_debug("\nno recl list:");
	debug_print_list(pool->no_recl_list);
}

static inline struct prmem_node**
prmem_find_node_in_list(struct prmem_node **list, const void *addr)
{
	for (struct prmem_node **iter = list; *iter; iter = &((*iter)->next)) {
		if ((*iter)->start <= (uintptr_t)addr && (*iter)->size >
		    (size_t)addr - (*iter)->start) {
			return iter;
		}
	}
	return NULL;
}

static struct prmem_node** prmem_find_node_in_pool(struct prmem_pool *pool,
						   const void *addr)
{
	struct prmem_node **ret = NULL;
	ret = prmem_find_node_in_list(&pool->pmalloc_list, addr);
	if (ret) {
		return ret;
	}
	ret = prmem_find_node_in_list(&pool->recl_list, addr);
	if (ret) {
		return ret;
	}
	return prmem_find_node_in_list(&pool->no_recl_list, addr);
}


int prmem_pfree(struct prmem_pool *pool, const void *addr, const char *filename,
		const char *func)
{
	bool found = false;

	if (unlikely(addr == NULL || filename == NULL || func == NULL)) {
		prmem_err(PRMEM_INVALID_INPUT_PARAMETERS);
		return -(int)PRMEM_INVALID_INPUT_PARAMETERS;
	}

	raw_mutex_lock(pool->mutex);
	if (unlikely(!prmem_is_pmalloc(addr, 1))) {
		raw_mutex_unlock(pool->mutex);
		prmem_err(PRMEM_INVALID_ADDRESS);
		return -(int)PRMEM_INVALID_ADDRESS;
	}
	if (unlikely(!prmem_pool_is_recl(pool))) {
		raw_mutex_unlock(pool->mutex);
		prmem_err(PRMEM_VA_NOT_RECLAIMABLE);
		return -(int)PRMEM_VA_NOT_RECLAIMABLE;
	}
	struct prmem_node** node_to_free =
		prmem_find_node_in_list(&pool->recl_list, addr);
	if (node_to_free != NULL) {
		pool_free_node(node_to_free);
		found = true;
	}
	raw_mutex_unlock(pool->mutex);
	if (unlikely(!found)) {
		prmem_err(PRMEM_NODE_NOT_FOUND);
		return -(int)PRMEM_NODE_NOT_FOUND;
	}
	return E_HM_OK;
}

static inline int prmem_pool_add_node(struct prmem_pool *pool, size_t size)
{
	struct prmem_node *node = NULL;
	node = pool_add_node(pool, size, PRMEM_NO_FLAGS);
	if (unlikely(node == NULL)) {
		return E_HM_FAULT;
	}
	prmem_assign_private(pool->offset, node->size);
	return E_HM_OK;
}

static inline bool pmalloc_is_empty(struct prmem_pool *pool)
{
	return unlikely(pool->pmalloc_list == NULL);
}

static inline bool pmalloc_is_exhausted(struct prmem_pool *pool, size_t size)
{
	/*
	 * Note: the test is done by offset vs size, without accounting for
	 * alignment, because the offset grows downward, in the direction
	 * of the beginning of the page(s) allocated, which is already page
	 * aligned. Page alignment is a superset of any reasonable
	 * alignment that might be required,up to PAGE_SIZE, included.
	 */
	return unlikely(pool->offset < size);
}

static inline bool pmalloc_space_needed(struct prmem_pool *pool, size_t size)
{
	return pmalloc_is_empty(pool) || pmalloc_is_exhausted(pool, size);
}

static inline int
prmem_pool_get_space(struct prmem_pool *pool, size_t size)
{
	if (pmalloc_space_needed(pool, size))
		return prmem_pool_add_node(pool, size);
	return (int)PRMEM_OK;
}


#define SZ_1M				(0x00100000)
#define DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))

#define __roundup(a, b) ((((a) + (b) - 1) / (b)) * (b))

#define PRMEM_PHYS_MEMORY_SIZE_MB 16
/* Worst case scenario: each page mapped individually to a node */
#define MAX_NODES_NR							\
	DIV_ROUND_UP(PRMEM_PHYS_MEMORY_SIZE_MB * SZ_1M, PAGE_SIZE)

#define PRMEM_NODE_SIZE							\
	__roundup(sizeof(struct prmem_node), sizeof(void *))

#define PRMEM_NODES_SIZE						\
	__roundup(MAX_NODES_NR * PRMEM_NODE_SIZE, PAGE_SIZE)

static size_t prmem_nodes_size = 0;

#ifdef CONFIG_DEBUG_BUILD
static void* prmem_nodes_block = NULL;

void prmem_nodes_free(void)
{
	(void) hm_mem_munmap(prmem_nodes_block, prmem_nodes_size);
}

void prmem_aux_init_revocable(void)
{
	prmem_aux_head = hm_mem_mmap(0, prmem_aux_max_size,
				     MPROT_READ | MPROT_WRITE | MPROT_HKIP | MPROT_HKIP_ROWM | MPROT_HKIP_REVOCABLE,
				     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (prmem_aux_head == MAP_FAILED) {
		hm_panic("Prmem init failed!\n");
	}
	/* initially, sizeof(size_t) is already used to store offset */
	*(size_t*)prmem_aux_head = sizeof(size_t);
}

void prmem_aux_free(void)
{
	(void) hm_mem_munmap(prmem_aux_head, prmem_aux_max_size);
}
#endif

static inline void prmem_cache_inc_obj_count(struct prmem_cache *cache)
{
	prmem_add(cache->count, 1);
	if (cache->count > cache->peak_count) {
		prmem_assign_private(cache->peak_count, cache->count);
	}
}

static inline void prmem_cache_push(struct prmem_cache *cache, struct prmem_node *obj)
{
	struct stack_node *node = (struct stack_node *)obj;

	if (cache == NULL || node == NULL) {
		prmem_err(PRMEM_INVALID_INPUT_PARAMETERS);
		return;
	}

	prmem_assign_private(node->node, cache->node);
	prmem_assign_private(cache->node, node);
	prmem_cache_inc_obj_count(cache);
}

/**
 * prmem_cache_free() - returns an object to the object cache
 * @cache: the cache into which the object must be pushed
 * @obj: the object to return
 */
void prmem_cache_free(struct prmem_cache *cache, struct prmem_node *obj)
{
	if (unlikely(cache == NULL || obj == NULL)) {
		prmem_err(PRMEM_INVALID_INPUT_PARAMETERS);
		return;
	}

	raw_mutex_lock(cache->mutex);
	prmem_cache_push(cache, obj);
	raw_mutex_unlock(cache->mutex);
}

static int prmem_init_nodes_cache_internal(bool revocable)
{
	unsigned int i;
	struct prmem_node *prmem_node = NULL;
	int ret = E_HM_OK;
	protected_status = PMALLOC_PROTECTED;
	prmem_nodes = aux_alloc(sizeof(struct prmem_cache));
	prmem_assign_private(*prmem_nodes, cache_data(prmem_nodes));

	if (ret == E_HM_OK) {
		prmem_nodes_size = PAGE_ALIGN_UP(PRMEM_NODES_SIZE);
		uint32_t prot = MPROT_READ | MPROT_WRITE;
#ifdef CONFIG_HKIP_PROTECT_PRMEM_META
		prot |= (MPROT_HKIP | MPROT_HKIP_ROWM)
#endif
		if (revocable) {
			prot |= MPROT_HKIP_REVOCABLE;
		}
		void* va = hm_mem_mmap(0, prmem_nodes_size, prot, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

		if (va == MAP_FAILED) {
			prmem_err(PRMEM_NODES_BATCH_FAIL);
			ret = E_HM_NOMEM;
		}
		prmem_node = va;
	}
	if (ret != E_HM_OK) {
		(void)hm_mem_munmap(prmem_aux_head, prmem_aux_max_size);
		return ret;
	}

#ifdef CONFIG_DEBUG_BUILD
	prmem_nodes_block = prmem_node;
#endif

#ifdef CONFIG_HKIP_PROTECT_PRMEM_META
	hm_mem_mprotect((void*)prmem_node, prmem_nodes_size, MPROT_READ);
#endif

	for (i = 0; i < MAX_NODES_NR; i++) {
		prmem_cache_push(prmem_nodes, prmem_node + i);
	}
	hm_debug("preloaded %d prmem_nodes\n", MAX_NODES_NR);
	return ret;
}

int prmem_init_nodes_cache(void)
{
	int ret = E_HM_OK;
	static bool inited = false;
	static struct raw_mutex	mutex = RAW_MUTEX_INITIALIZER;
	if (inited) {
		return ret;
	}
	raw_mutex_lock(&mutex);
	if (inited) {
		raw_mutex_unlock(&mutex);
		return ret;
	}
	ret = prmem_init_nodes_cache_internal(false);
	if(ret == E_HM_OK) {
		inited = true;
	}
	raw_mutex_unlock(&mutex);
	return ret;
}

#ifdef CONFIG_DEBUG_BUILD
int prmem_init_nodes_cache_revocable(void)
{
	return prmem_init_nodes_cache_internal(true);
}
#endif

/*
 * pmalloc_aligned() - allocate protectable memory from a pool
 * @pool: handle to the pool to be used for memory allocation
 * @size: amount of memory (in bytes) requested
 * @alignment: alignment required.
 * @flags: enum prmem_flags variable affecting the allocation
 *
 * Return pointer to memory on success, NULL otherwise
 */
void *pmalloc_aligned(struct prmem_pool *pool, size_t size,
		      int alignment, enum prmem_flags flags)
{
	size_t new_offset;
	void *ret = NULL;

	if (unlikely(pool == NULL || !size)) {
		prmem_err(PRMEM_INVALID_INPUT_PARAMETERS);
		return NULL;
	}

	alignment = (int)get_alignment(pool, size, alignment);

	raw_mutex_lock(pool->mutex);
	/* freeable node doesn't share pool->offset, but add new node */
	if ((unsigned int)flags & ((int)PRMEM_NODE | (int)PRMEM_FREEABLE_NODE)) {
		struct prmem_node *node = NULL;
		node = pool_add_node(pool, size, flags);
		if (likely(node)) {
			ret = (void *)(uintptr_t)node->start;
		} else {
			prmem_err(PRMEM_NO_NODE);
		}
		raw_mutex_unlock(pool->mutex);
		return ret;
	}
	if (prmem_pool_get_space(pool, size) != (int)PRMEM_OK) {
		prmem_err(PRMEM_NO_SPACE);
		raw_mutex_unlock(pool->mutex);
		return ret;
	}
	new_offset = (size_t)round_down(pool->offset - size, (size_t)(unsigned int)alignment);
	prmem_assign_private(pool->offset, new_offset);
	ret = (void *)(uintptr_t)(pool->pmalloc_list->start + new_offset);
	raw_mutex_unlock(pool->mutex);
	return ret;
}

void *pmalloc(struct prmem_pool *pool, size_t size, enum prmem_flags flags)
{
	return pmalloc_aligned(pool, size, LT_MINUS_1, flags);
}

void *pcalloc(struct prmem_pool *pool, size_t n, size_t size, enum prmem_flags flags)
{
	size_t element_size;
	element_size = round_up(size, get_alignment(pool, size, LT_MINUS_1));
	return pmalloc_aligned(pool, element_size * n, LT_MINUS_1, flags);
}

static inline void prmem_free_nodes_list(struct prmem_node **node)
{
	while (*node) {
		pool_free_node(node);
	}
}

/*
 * prmem_free_pool() - Releases all the memory associated to a pool
 * @pool: the pool whose memory must be reclaimed
 *
 * Only reclaimable pool can be freed!
 */
void prmem_free_pool(struct prmem_pool *pool)
{
	if (unlikely(pool == NULL)) {
		prmem_err(PRMEM_INVALID_INPUT_PARAMETERS);
		return;
	}

	if (unlikely(!prmem_pool_is_recl(pool))) {
		prmem_err(PRMEM_POOL_NOT_RECLAIMABLE);
		return;
	}
	raw_mutex_lock(pool->mutex);
	prmem_free_nodes_list(&(pool->pmalloc_list));
	prmem_free_nodes_list(&(pool->recl_list));
	prmem_free_nodes_list(&(pool->no_recl_list));
	prmem_assign_private(pool->offset, 0);
	if (!pmalloc_is_start_wr(pool)) {
		prmem_assign_private(pool->protection_status, PMALLOC_UNPROTECTED);
	}
	raw_mutex_unlock(pool->mutex);
}

int prmem_protect_addr(void *addr, struct prmem_pool *pool)
{
	struct prmem_node *n = NULL;
	struct prmem_node** n_ptr = NULL;

	if (unlikely(pool == NULL)) {
		prmem_err(PRMEM_INVALID_INPUT_PARAMETERS);
		return -(int)PRMEM_INVALID_INPUT_PARAMETERS;
	}

	raw_mutex_lock(pool->mutex);

	n_ptr = prmem_find_node_in_pool(pool, addr);
	if (unlikely(n_ptr == NULL || *n_ptr == NULL)) {
		raw_mutex_unlock(pool->mutex);
		prmem_err(PRMEM_NODE_NOT_FOUND);
		return -(int)PRMEM_NODE_NOT_FOUND;
	}

	n = *n_ptr;
	prmem_protect_range(n, n->start, n->size);
	raw_mutex_unlock(pool->mutex);
	return E_HM_OK;
}

/*
 * prmem_protect_pool() - write-protect the pmalloced memory in the pool
 * @pool: the pool with the pmalloced memory to write-protect
 *
 * protect POOL will freeze the current node, reset offset,
 * so next time pmalloc will add a new node
 */
void prmem_protect_pool(struct prmem_pool *pool)
{
	struct prmem_node *node = NULL;

	if (unlikely(pool == NULL)) {
		prmem_err(PRMEM_INVALID_INPUT_PARAMETERS);
		return;
	}

	if (unlikely(pmalloc_is_start_wr(pool))) {
		prmem_err(PRMEM_POOL_IS_START_WR);
		return;
	}
	raw_mutex_lock(pool->mutex);
	for (node = pool->pmalloc_list; node; node = node->next) {
		prmem_protect_range(node, node->start, node->size);
	}
	prmem_assign_private(pool->protection_status, protected_status);
	prmem_assign_private(pool->offset, 0);
	raw_mutex_unlock(pool->mutex);
}
