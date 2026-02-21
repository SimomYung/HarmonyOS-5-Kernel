/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: General purpose memory pool mechanism.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 27 14:56:15 2021
 */
#ifndef ULIBS_INCLUDE_LIBMEM_MEM_POOL_H
#define ULIBS_INCLUDE_LIBMEM_MEM_POOL_H

#include <libhmsync/raw_scopedptr.h>
#include <libhmsync/raw_mutex.h>
#include <hongmeng/panic.h>
#include <lib/utils.h>
#include <lib/dlist.h>
#include <stddef.h>

struct mem_pool_s;
struct mem_pool_range_s;

/*
 * mem_pool allocation algorithm type definition.
 *
 * @param pool	[I]	the pool to look for available memory
 *
 * @param bmap	[I]	bitmap on which perform allocation
 *
 * @param sz	[I]	the number of bits in the bitmap
 *
 * @param start	[I]	the position in the bitmap from where to search
 *
 * @param len	[I]	number of needed zero bits
 *
 * @return the appropriate bit position in the bitmap if succeeded,
 *	   or invalid bit position that is greater than bitmap size.
 */
typedef uint32_t (*mempool_algo_t)(const struct mem_pool_s *pool,
				   const unsigned long long *bmap,
				   uint32_t sz,
				   uint32_t start,
				   uint32_t len);

/*
 * mem_pool range insert behavior type definition.
 *
 * @param pool	[I]	the pool to be extended
 *
 * @param range	[I]	the range to be inserted to pool
 */
typedef void (*mempool_insert_t)(struct mem_pool_s *pool,
				 struct mem_pool_range_s *range);

struct mem_pool_s {
	struct raw_mutex mutex;            /* lock before manipulate pool data. */
	struct dlist_node dnode;           /* Record memory ranges added in pool. */
	mempool_insert_t insert;           /* Mem pool insert behavior. */
	mempool_algo_t algo;               /* Memory allocation function. */
	uint32_t order;                    /* Allocation granularity. */
};

/*
 * Memory range that represents contiguous physical memory.
 */
struct mem_pool_range_s {
	struct dlist_node node;     /* dnode used in mem pool range list. */
	size_t free;                /* Available memory in the range. */
	uintptr_t start;            /* Start virtual address of the range. */
	uintptr_t end;              /* End virtual address of the range. */
	uint64_t paddr;             /* Start physical address of the range */
	unsigned long long *bmap;   /* Bitmap to track status of memory in the range. */
};

/*
 * Create one mem pool with allocation order.
 *
 * @param order	[I]	granularity used when allocating mem from pool
 *
 * @return mem pool created with order.
 *
 * @note The caller should give allocation order used for mem_pool_alloc.
 */
struct mem_pool_s *mem_pool_create(uint32_t order);

/*
 * Free the memory pool.
 *
 * @param pool	[I]	pool to be destroyed
 *
 * @return E_HM_OK if succeeded to destroy the given pool, and errno if not.
 *
 * @note memory pool here do not consider using pool concurrently, so the user
 *	 should guarantee the availability of pool which can be manipulated
 *	 concurrently in multiple threads.
 */
int mem_pool_destroy(struct mem_pool_s *pool);

/*
 * Add one memory range into mem pool.
 *
 * @param pool	[I]	The pool to add memory to
 *
 * @param va	[I]	virtual start address of the range
 *
 * @param pa	[I]	physical start address of the range
 *
 * @param len	[I]	length of the range
 *
 * @return E_HM_OK if succeeded to add the given range into the mem pool.
 *	   Otherwise, errno would returns.
 */
int mem_pool_insert(struct mem_pool_s *pool,
		    uintptr_t va,
		    uint64_t pa,
		    size_t len);

/*
 * Add one memory range into mem pool and alloc at same time.
 *
 * @param pool		[I]	The pool to add memory to
 *
 * @param va		[I]	virtual start address of the range
 *
 * @param pa		[I]	physical start address of the range
 *
 * @param len		[I]	length of the range
 *
 * @param alloc_len	[I]	bytes needed to be allocated
 *
 * @return E_HM_OK if succeeded to add the given range into the mem pool.
 *	   Otherwise, errno would returns.
 */
int mem_pool_insert_alloc(struct mem_pool_s *pool,
			  uintptr_t va,
			  uint64_t pa,
			  size_t len,
			  size_t alloc_len);

/*
 * Remove one unused memory range from pool.
 *
 * @param pool	[I]	from which the given range can be removed
 *
 * @param va	[I]	start virtual address of the given range
 *
 * @param len	[I]	length of the range
 *
 * @return E_HM_OK in normal cases, and E_HM_INVAL if the range info is
 *	   invalid or memory of the range is still used.
 */
int mem_pool_remove(struct mem_pool_s *pool,
		    uintptr_t va,
		    size_t len);

/*
 * mem_pool_find_first - find the first available memory range.
 *
 * @param pool	[I]	where to find the usable memory range
 *
 * @param bmap	[I]	bitmap used to track memory using status of range
 *
 * @param sz	[I]	size of the bitmap in bits
 *
 * @param start	[I]	looking begins with this bit position
 *
 * @param len	[I]	the number of zero bits to looked up
 *
 * @return bit position in bitmap of the available memory range,
 *	   or invalid bit position that is greater than bitmap size.
 */
uint32_t mem_pool_find_first(const struct mem_pool_s *pool,
			     const unsigned long long *bmap,
			     uint32_t sz,
			     uint32_t start,
			     uint32_t len);

/*
 * mem_pool_find_best - find the best available memory range.
 *
 * @param pool	[I]	where to find the usable memory range
 *
 * @param bmap	[I]	bitmap used to track memory using status of range
 *
 * @param sz	[I]	size of the bitmap in bits
 *
 * @param start	[I]	looking begins with this bit position
 *
 * @param len	[I]	the number of zero bits to looked up
 *
 * @return bit position in bitmap of the available memory range,
 *	   or invalid bit position that is greater than bitmap size.
 */
uint32_t mem_pool_find_best(const struct mem_pool_s *pool,
			    const unsigned long long *bmap,
			    uint32_t sz,
			    uint32_t start,
			    uint32_t len);

/*
 * mem_pool_set_algo - set the memory allocation algorithm of the mem_pool.
 *
 * @param pool	[I]	the pool to set algo
 *
 * @param algo	[I]	the algo to be set
 */
void mem_pool_set_algo(struct mem_pool_s *pool, mempool_algo_t algo);

/*
 * Allocate special memory from pool, of which the size is given.
 *
 * @param pool	[I]	the mem pool where we can alloc memory
 *
 * @param len	[I]	the number of bytes needed to be allocated
 *
 * @return The allocated memory address if succeeded, and O if failed.
 */
uintptr_t mem_pool_alloc(struct mem_pool_s *pool,
			 size_t len);

/*
 * Free memory back to pool
 *
 * @param pool	[I]	the pool which maintains the to-be-freed memory
 *
 * @param va	[I]	start address of the allocated memory
 *
 * @param len	[I]	the number of bytes to be freed back to pool
 *
 * @return E_HM_OK if freeing succeeded, and errno if failed.
 */
int mem_pool_free(struct mem_pool_s *pool,
		  uintptr_t va,
		  size_t len);

/*
 * Transform the vaddr allocated from mem_pool into physical address.
 *
 * @param pool	[I]	mem pool to which the vaddr memory belongs
 *
 * @param va	[I]	start virtual address of the allocated memory
 *
 * @param pa	[O]	the corresponding physical address of vaddr
 *
 * @return E_HM_OK if the given vaddr is valid, and E_HM_INVAL if not.
 *
 * @note The validity of the calculated physical memory address
 *	 is guaranteed by the given paddr when inserting range
 *	 into the pool.
 */
int mem_pool_paddr_from_vaddr(struct mem_pool_s *pool,
			      uintptr_t va,
			      uint64_t *pa);

/*
 * Imem_pool_insert_asc_nolock - Insert mem_pool ranges by ascending sort.
 *
 * @param pool	[I]	the pool to be extended
 *
 * @param range	[I]	the range to be inserted to pool
 *
 * @note The function is advised to be used through mem_pool_set_insert_behavior
 */
void mem_pool_insert_asc_nolock(struct mem_pool_s *pool,
				struct mem_pool_range_s *range);

/*
 * mem_pool_set_insert_behavior - Set the mem_pool insert behavior.
 *
 * @param pool	[I]	the pool to set insert behavior
 *
 * @param func	[I]	the insert behavior to be set
 */
void mem_pool_set_insert_behavior(struct mem_pool_s *pool, mempool_insert_t func);

#endif /* ULIBS_INCLUDE_LIBMEM_MEM_POOL_H */
