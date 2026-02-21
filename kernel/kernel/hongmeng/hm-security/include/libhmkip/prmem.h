/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Header of HM KIP pmalloc
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 4 02:21:13 2023
 */

#ifndef ULIBS_LIBHMKIP_PRMEM_H
#define ULIBS_LIBHMKIP_PRMEM_H

#include <stdbool.h>
#include <stddef.h>
#include <hmkernel/errno.h>
#include <hmkernel/compiler.h>
#include <libhmsync/raw_mutex_guard.h>
#include <stdint.h>
#include <string.h>
#include <libmem/utils.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_hkip.h>

#ifndef WARN
#define	WARN(condition, ...) ({			\
	bool __ret_warn_on = (condition);	\
	if (unlikely(__ret_warn_on))		\
		hm_warn(__VA_ARGS__);		\
	unlikely(__ret_warn_on);		\
})
#endif


/* ---------------------------- Data Types ---------------------------- */

/**
 * enum prmem_states - operating modes of prmem
 * @PRMEM_NO_HHEE: hhee disabled at boot
 * @PRMEM_BYPASS: prmem is compiled in bypass mode
 * @PRMEM_ENABLED: prmem is fully active
 */
enum prmem_states {
	PRMEM_DISABLED,
	PRMEM_ENABLED
};

/**
 * enum prmem_protection_states - possible protections states for a pool
 * @PMALLOC_UNPROTECTED: pmalloc list has at least some unprotected momory
 * @PMALLOC_NO_EL2: pmalloc list not protected, because hhee is off
 * @PMALLOC_PRMEM_DISABLED: pmalloc list not protected, because prmem disabled
 * @PMALLOC_PROTECTED: pmalloc list is protected
 */
enum prmem_protection_states {
	PMALLOC_UNPROTECTED,
	PMALLOC_NO_EL2,
	PMALLOC_PRMEM_DISABLED,
	PMALLOC_PROTECTED,
};

/**
 * enum prmem_type
 * @PRMEM_RO_NO_RECL: R/O allocations, non reclaimable
 * @PRMEM_WR_NO_RECL: W/R allocations, non reclaimable
 * @PRMEM_START_WR_NO_RECL: pre-protected W/R allocations, non reclaimable
 * @PRMEM_START_WR_RECL: pre-protected W/R allocations, reclaimable
 * @PRMEM_WR_RECL: W/R allocations, reclaimable
 * @PRMEM_RO_RECL: R/O allocations, reclaimable
 * @PRMEM_NO_RECL: combined range of non reclaimable ranges
 * @PRMEM_RECL: combined range of reclaimable ranges
 * @PRMEM_WR: combined range of W/R ranges
 * @PRMEM_START_WR: combined range of start W/R ranges
 * @PRMEM_FULL: combined range of protected ranges
 */
enum prmem_type {
	PRMEM_RO_NO_RECL,
	PRMEM_WR_NO_RECL,
	PRMEM_START_WR_NO_RECL,
	PRMEM_START_WR_RECL,
	PRMEM_WR_RECL,
	PRMEM_RO_RECL,
	/* The following indexes refer to unions of the above. */
	PRMEM_NO_RECL,
	PRMEM_RECL,
	PRMEM_WR,
	PRMEM_START_WR,
	PRMEM_FULL,
};

enum prmem_pool_type {
	PRMEM_POOL_RO_NO_RECL = PRMEM_RO_NO_RECL,
	PRMEM_POOL_WR_NO_RECL = PRMEM_WR_NO_RECL,
	PRMEM_POOL_START_WR_NO_RECL = PRMEM_START_WR_NO_RECL,
	PRMEM_POOL_START_WR_RECL = PRMEM_START_WR_RECL,
	PRMEM_POOL_WR_RECL = PRMEM_WR_RECL,
	PRMEM_POOL_RO_RECL = PRMEM_RO_RECL,
};

/**
 * struct prmem_node - node for tracking contiguous phys memory allocations
 * @pool: pointer to the pool owning the node
 * @next: pointer to the next node
 * @start: address of the beginning of the area
 * @size: size of the area
 * @locked : records whether the memory is protected via prmem_protect_range
 */
struct prmem_node {
	struct prmem_pool *pool;
	struct prmem_node *next;
	size_t start;
	size_t size;
	unsigned long locked;
};

/**
 * struct prmem_pool
 * @name: pointer to a string with the name of the pool
 * @pmalloc_list: pointer to the first node of a list of va allocations
 * @recl_list: list of reclaimable nodes
 * @no_recl_list: list of non-reclaimable nodes
 * @offset: amount of free memory available from the current node
 * @alignment: the minimum alignment required for allocations from this pool
 * @refill: the minimum amount of memory to allocate in a new node
 * @cap: maximum amount of memory that can be allocated - 0 disables it
 * @allocated: memory currently associated with the pool
 * @mutex: pointer to mutex lock - lock before manipulate pool data
 * @protection_status: protection status of the pmalloc memory
 * @type: type of the pool
 */

#define PRMEM_ALIGN (sizeof(void *))

struct prmem_pool {
	const char *name;
	struct prmem_node *pmalloc_list;
	struct prmem_node *recl_list;
	struct prmem_node *no_recl_list;
	size_t offset;
	size_t alignment;
	size_t refill;
	size_t cap;
	size_t allocated;
	size_t peak_allocated;
	struct raw_mutex* mutex;
	enum prmem_protection_states protection_status;
	enum prmem_pool_type type;
} __aligned(PRMEM_ALIGN);

/**
 * enum prmem_flags - set of options to use when allocating from prmem
 * @PRMEM_NO_FLAGS
 * @PRMEM_ZERO: memory must be zero-initialized
 * @PRMEM_NODE: always use a new node, rather than memory in current node
 * @PRMEM_FREEABLE_NODE: always create a freeable node
 */
enum prmem_flags {
	PRMEM_NO_FLAGS = 1,
	PRMEM_ZERO = (1 << 1),
	PRMEM_NODE = (1 << 2),
	PRMEM_FREEABLE_NODE = (1 << 3),
};

struct stack_node {
	struct stack_node *node;
};

/**
 * struct prmem_cache: hold nodes for pools
 * preallocate cache nodes so they can be protected
 * @name: the name of the object cache
 * @node: pointer to the first object in the stack implementing the cache
 * @count: number of objects currently in the cache
 * @pool: used to indicate the type of cache
 * @size: the size of one of the objects in the cache
 * @alignment: the minimum alignemnt order required
 * @mutex: lock
 */
struct prmem_cache {
	const char *name;
	struct stack_node *node __aligned(sizeof(void *));
	size_t count;
	size_t peak_count;
	struct prmem_pool *pool __aligned(sizeof(void *));
	uint16_t size;
	uint16_t alignment;
	/* lock before manipulate pool data. */
	struct raw_mutex* mutex;
} __aligned(PRMEM_ALIGN);


#define __stringify_1(x...)	#x
#define __stringify(x...)	__stringify_1(x)

#define __PRMEM_OBJECT_CACHE                                            \
	__attribute__((__section__(".data..prmem_object_caches")))

#define prmem_validate(val, cond) _Static_assert(cond(val), "prmem param invalid!")

/* true if the parameter is a power of 2 */
#define is_power_of_two(n) ((n) && (((n) & ((n) - 1)) == 0))

/* Checks if the alignment is a power of 2 */
#define verify_pool_alignment(alignment) is_power_of_two(alignment)

#define validate_pool_alignment(alignment)				\
	prmem_validate(alignment, verify_pool_alignment)

#define PRMEM_UNDEFINED_REFILL 0
#define PRMEM_DEFAULT_ALIGN_ORD 8 // Can be further optimized

#define PRMEM_NO_CAP 0

#define __round_mask(x, power2) ((__typeof__(x))((power2)-1U))

#define round_up(x, power2) ((((x)-1U) | __round_mask(x, power2))+1U)

#define round_down(x, power2) ((x) & ~__round_mask(x, power2))

#define str(x) #x

#define pool_data(_name) (_name##_data)

#define prmem_pool_declare(_name, _type, _al, _refill, _cap)		\
	static struct raw_mutex	_name##_pool_lock = RAW_MUTEX_INITIALIZER; \
	static const char _name##_pool_name[] = __stringify(_name);	\
	static struct prmem_pool* (_name) __ro_after_init;		\
	validate_pool_alignment(_al);					\
	static const struct prmem_pool __attribute__((__aligned__(PAGE_SIZE))) pool_data(_name) = { \
		.name = _name##_pool_name,				\
		.pmalloc_list = NULL,					\
		.recl_list = NULL,					\
		.no_recl_list = NULL,					\
		.offset = 0,						\
		.alignment = (_al) <= 0 ? PRMEM_DEFAULT_ALIGN_ORD :  (_al), \
		.refill = (!(_refill)) ? PRMEM_UNDEFINED_REFILL : round_up((_refill), PAGE_SIZE),	\
		.cap = round_up(_cap, PAGE_SIZE),			\
		.allocated = 0,						\
		.peak_allocated = 0,					\
		.mutex = &(_name##_pool_lock),				\
		.protection_status = PMALLOC_UNPROTECTED,		\
		.type =	(_type)						\
	}

/**
 * prmem_is_static_wr() - is the buffer within static wr memory?
 * @p: beginning of the buffer
 * @n: size of the buffer
 * Returns true if the buffer belongs to static wr memory, false otherwise
 */
static inline bool prmem_is_static_wr(const void *p, size_t n)
{
	UNUSED(p, n);
	return false;
}

/**
 * prmem_is_static_wr_after_init() - is the buffer within static wr after init?
 * @p: beginning of the buffer
 * @n: size of the buffer
 * Returns true if the buffer is static wr after init, false otherwise
 */
static inline bool prmem_is_static_wr_after_init(const void *p, size_t n)
{
	UNUSED(p, n);
	return false;
}

/* is START within the space of prmem? based on address in linking script */
static inline bool prmem_is_pmalloc(const void *start, size_t size)
{
	UNUSED(start, size);
	return true;
}

/**
 * prmem_pool_pmalloc_protected() - are the pmalloc allocations protected?
 * @pool: the pool to test
 *
 * Returns true if all the pmalloc memory is protected, false otherwise
 *
 */
static inline bool prmem_pool_pmalloc_protected(const struct prmem_pool *pool)
{
	return pool->protection_status != PMALLOC_UNPROTECTED;
}

/*
 * protected assignment
 * memory registered to rowm can only be write via prmem_assign
 */
#define prmem_assign(dst, val, pool)					\
do {									\
	typeof((dst)) *dst_ptr = &(dst);				\
	typeof(*dst_ptr) tmp = (typeof(tmp))(val);			\
	prmem_memcpy(dst_ptr, &tmp, sizeof(tmp), pool);			\
} while (0)

/**
 * prmem_pool_to_prmem_type() - give prmem_type of a pool based on its pool type
 * @pool
 */
static enum prmem_type prmem_pool_to_prmem_type(struct prmem_pool *pool)
{
	if (pool->type >= PRMEM_POOL_RO_NO_RECL &&
		pool->type <= PRMEM_POOL_RO_RECL) {
		return (enum prmem_type)(pool->type);
	} else {
		return E_HM_INVAL;
	}
}

/**
 * prmem_pool_type_is_wr_recl() - is memory from the pool both rw and reclaimable
 * @type: the pool type
 * Returns true if the memory type is both wr and reclaimable, else true
 */
static inline bool prmem_pool_type_is_wr_recl(enum prmem_pool_type type)
{
	return (type == PRMEM_POOL_START_WR_RECL || type == PRMEM_POOL_WR_RECL);
}

/**
 * prmem_pool_is_recl() - is the memory associated to the pool reclaimable?
 * @pool: the pool to analyse
 * Returns true if the memory associated to the pool can be released.
 */
static inline bool prmem_pool_is_recl(const struct prmem_pool *pool)
{
	enum prmem_pool_type type = prmem_pool_to_prmem_type(pool);

	return (type == PRMEM_POOL_RO_RECL || type == PRMEM_POOL_WR_RECL ||
		type == PRMEM_POOL_START_WR_RECL);
}

/**
 * prmem_pool_type_is_start_wr() - is memory from the pool pre protected
 * @type: the pool type
 * Returns true if the memory type is pre protected
 */
static inline bool prmem_pool_type_is_start_wr(enum prmem_pool_type type)
{
	return (type == PRMEM_POOL_START_WR_NO_RECL ||
		type == PRMEM_POOL_START_WR_RECL);
}


static inline bool pmalloc_is_start_wr(struct prmem_pool *pool)
{
	return prmem_pool_type_is_start_wr(pool->type);
}

/**
 * prmem_type_is_recl - true if the memory type is reclaimable
 * @type - one of the possible memory types from enum prmem_types
 * Returns true if the type provided is reclaimable
 */
static inline bool prmem_type_is_recl(enum prmem_type type)
{
	return (type == PRMEM_START_WR_RECL || type == PRMEM_WR_RECL ||
		type == PRMEM_RO_RECL || type == PRMEM_RECL);
}

/**
 * prmem_type_is_start_wr - true if the memory type starts as W/R
 * @type - one of the possible memory types from enum prmem_types
 * Returns true if the type provided is start_wr
 */
static inline bool prmem_type_is_start_wr(enum prmem_type type)
{
	return (type == PRMEM_START_WR_NO_RECL ||
		type == PRMEM_START_WR_RECL || type == PRMEM_START_WR);
}

static inline bool pmalloc_is_wr(struct prmem_pool *pool)
{
	enum prmem_type type = prmem_pool_to_prmem_type(pool);
	return (type == PRMEM_WR_NO_RECL ||
		type == PRMEM_START_WR_NO_RECL ||
		type == PRMEM_START_WR_RECL ||
		type == PRMEM_WR_RECL);
}

static inline bool prmem_is_wr(const void *p, size_t n, struct prmem_pool *pool)
{
	return (pmalloc_is_wr(pool) || prmem_is_static_wr(p, n) ||
		prmem_is_static_wr_after_init(p, n));
}

#define sub_1(x) ((x)-1)
#define EIGHT    8
#define FOUR     4
#define TWO      2

#ifdef __HOST_LLT__
#define ALWAYS_INLINE
#else
#define ALWAYS_INLINE always_inline
#endif

/**
 * prmem_memset() - sets n bytes of the destination p to the c value
 * @p: beginning of the memory to write to
 * @c: byte to replicate
 * @n: amount of bytes to copy
 * Returns pointer to the destination
 */
static inline __attribute__((ALWAYS_INLINE)) void prmem_memset(void *p, uint8_t c, size_t n, struct prmem_pool *pool)
{
	if ((WARN(p == NULL, "Invalid wr memset param")) ||
	    (WARN(!prmem_is_wr(p, n, pool), "Unsupported memory write"))) {
		return;
	}
	hm_hkip_rowm_set(p, c, n);
}

void prmem_memcpy(void *p, const void *q, size_t n, struct prmem_pool *pool);

#define test_object_size(size) ((size) >= sizeof(void *))

/* Validates size and alignemnt for a cached object */
#define test_cache_alignment(alignment)					\
	(is_power_of_two(alignment) && test_object_size(alignment))
#define validate_cache_alignment(alignment)				\
	prmem_validate(alignment, test_cache_alignment)

#define PRMEM_CACHE_ALIGN_AS_POOL (-1)

#define PRMEM_CACHE_NO_POOL NULL

#define cache_data(_name) (_name##_data)

#define prmem_cache_declare(_name, _pool, _size, _al)			\
	;								\
	static struct raw_mutex _name##_cache_lock = RAW_MUTEX_INITIALIZER; \
	static struct prmem_cache* (_name) __ro_after_init;		\
	static const char _name##_cache_name[] = __stringify(_name);	\
	validate_cache_alignment(_al);					\
	static const struct prmem_cache cache_data(_name) __PRMEM_OBJECT_CACHE = { \
		.name = _name##_cache_name,				\
		.node = NULL,						\
		.count = 0,						\
		.peak_count = 0,					\
		.pool = (_pool),					\
		.size = (_size),					\
		.alignment = ((_al) <= 0) ? -1 : (_al),			\
		.mutex = &(_name##_cache_lock),				\
	}

void *pmalloc_aligned(struct prmem_pool *pool, size_t size, int alignment, enum prmem_flags flags);

void *pmalloc(struct prmem_pool *pool, size_t size, enum prmem_flags flags);

void *pcalloc(struct prmem_pool *pool, size_t n, size_t size, enum prmem_flags flags);

char *pstrdup(struct prmem_pool *pool, const char *s, enum prmem_flags flags);

void prmem_cache_free(struct prmem_cache *cache, struct prmem_node *obj);

void prmem_free_pool(struct prmem_pool *pool);

int prmem_pfree(struct prmem_pool *pool, const void *addr, const char *filename, const char *func);

#define prmem_free_addr(pool, addr)  prmem_pfree(pool, addr, __FILE__, __func__)

int prmem_protect_addr(void *addr, struct prmem_pool *pool);

void prmem_protect_pool(struct prmem_pool *pool);

void prmem_aux_init(void);

void prmem_pool(struct prmem_pool **, const struct prmem_pool *);

int prmem_init_nodes_cache(void);

void debug_print_pool(struct prmem_pool *pool);

const char *get_prmem_status(void);

void set_prmem_status(enum prmem_states status);

/*
 * prmem_active() - tells if data will be really write protected
 *
 * Returns true when HW-backed write protection is available
 */
bool prmem_active(void);
#ifdef CONFIG_DEBUG_BUILD
void prmem_nodes_free(void);
void prmem_aux_init_revocable(void);
void prmem_aux_free(void);
int prmem_init_nodes_cache_revocable(void);
#endif

#endif
