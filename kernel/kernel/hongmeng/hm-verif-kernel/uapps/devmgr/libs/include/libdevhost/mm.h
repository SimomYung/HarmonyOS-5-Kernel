/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Header of libdh mm
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 23 19:12:35 2020
 */
#ifndef ULIBS_LIBDEVHOST_MM_H
#define ULIBS_LIBDEVHOST_MM_H

#include <stdbool.h>
#include <sys/mman.h>
#include <libalgo/rbtree.h>

#define LIBDH_REMAP_NONE	0U
#define LIBDH_REMAP_CACHE	1U
#define LIBDH_REMAP_NORMAL_NC	(1U << 1)
#define LIBDH_REMAP_DEVICE	(1U << 2)
#define LIBDH_REMAP_DMA_BUF	(1U << 3)
#define LIBDH_REMAP_PBHA_SHIFT	5
#define LIBDH_REMAP_PBHA	(0xFU << LIBDH_REMAP_PBHA_SHIFT) // bit[8:5]
#define LIBDH_REMAP_SH_SHIFT	9
#define LIBDH_REMAP_SH		(0x3U << LIBDH_REMAP_SH_SHIFT) // bit[10:9]
#define LIBDH_REMAP_FIXED	(1U << 30) /* udk used */

#define LIBDH_VM_TO_USER	(1U << 31)

static inline bool libdh_remap_flag_test(unsigned int flags, unsigned int flag)
{
	return (flags & flag) != 0U;
}

typedef unsigned long long paddr_t;

/*
 *  definition for vmap tree node
 */
struct libdh_vmap_node {
	struct rbt_node node; /* node in vmap tree */
	const void *vaddr;    /* key: aligned vaddr */
	unsigned long size;   /* size of virtual address range */
	int (*release)(struct libdh_vmap_node *node); /* release vmap node */
};

/*
 * helper for vmap tree
 */
#define libdh_init_vmap_release(pnode, func) ((pnode)->release = (func))

/*
 * insert node to vmap tree
 *
 * @param    vnode  [I] the node will be inserted in vmap tree
 *
 * @return   E_HM_OK for success, hmerrno for errors
 */
int libdh_vmap_insert(struct libdh_vmap_node *vnode);

/*
 *  reserve virtual address space
 *
 *  @param    vaddr  [I] start of expected virtual address range
 *  @param    size   [I] physical memory size
 *  @param    prot   [I] memory access permission
 *  @param    flags  [I] features of the mapping area
 * 		  - LIBDH_REMAP_CACHE        normal memory with cache
 * 		  - LIBDH_REMAP_NORMAL_NC    normal meory no cache
 * 		  - LIBDH_REMAP_DEVICE       device memory no cache
 * 		  - LIBDH_REMAP_FIXED        interpret addr exactly
 * 		  - LIBDH_VM_TO_USER         map to xact fwd vspace instead of self
 *
 *  @return   base virtual address
 *  @note     caller should ensure aligned up size
 */
void *libdh_vma_reserve(const void *vaddr, unsigned long size,
			unsigned int prot, unsigned int flags);
/*
 *  map physical memory to virtual address space
 *
 *  @param    vaddr  [I] base virtual address
 *  @param    size   [I] physical memory size
 *  @param    paddr  [I] base physical address
 *  @param    prot   [I] memory access permission
 *  @param    flags  [I] features of the mapping area
 * 		  - LIBDH_REMAP_CACHE        normal memory with cache
 * 		  - LIBDH_REMAP_NORMAL_NC    normal meory no cache
 * 		  - LIBDH_REMAP_DEVICE       device memory no cache
 * 		  - LIBDH_REMAP_FIXED        interpret addr exactly
 * 		  - LIBDH_VM_TO_USER         map to xact fwd vspace instead of self
 *
 *  @return   E_HM_OK for success, hmerrno for errors
 *  @note     1. caller should ensure page aligned vaddr and paddr, aligned up size
 *            2. paddr only support udrv memory defined in DTS
 */
int libdh_vma_map_page(void *vaddr, unsigned long size, paddr_t paddr,
		       unsigned int prot, unsigned int flags);

/*
 *  reverse virtual address space and map physical memory to it
 *
 *  @param    vaddr  [I] start of expected virtual address range
 *  @param    size   [I] physical memory size
 *  @param    paddr  [I] base physical address
 *  @param    prot   [I] memory access permission
 *  @param    flags  [I] features of the mapping area
 * 		  - LIBDH_REMAP_CACHE        normal memory with cache
 * 		  - LIBDH_REMAP_NORMAL_NC    normal meory no cache
 * 		  - LIBDH_REMAP_DEVICE       device memory no cache
 * 		  - LIBDH_REMAP_FIXED        interpret addr exactly
 * 		  - LIBDH_VM_TO_USER         map to xact fwd vspace instead of self
 *
 *  @return   base virtual address which is page aligned
 *  @note     1. caller should ensure page aligned vaddr and paddr, aligned up size
 *            2. paddr only support udrv memory defined in DTS
 */
void *libdh_vma_mmap(const void *vaddr, unsigned long size, paddr_t paddr,
		     unsigned int prot, unsigned int flags);
/*
 * unmap virtual address space
 *
 * @param    addr  [I] base virtual address will be unmap
 *
 * @return   E_HM_OK for success, hmerrno for errors
 * @note     caller should ensure page aligned vaddr
 */
int libdh_vma_unmap(const void *addr);

/*
 * map physical memory to system virtual address space.
 *
 * parameters:
 *  - paddr		base physical address
 *  - size		physical memory size
 *  - prot		memory access permission:
 * 			  - PROT_NONE	page can not be accessed
 * 			  - PROT_READ	page can be read
 * 			  - PROT_WRITE	page can be written
 * 			  - PROT_EXEC	page can be executed
 *  - flags		cache flags:
 * 			  - LIBDH_REMAP_NONE		like PROT_NONE, not support now
 * 			  - LIBDH_REMAP_CACHE		normal memory with cache
 * 			  - LIBDH_REMAP_NORMAL_NC	normal meory no cache
 * 			  - LIBDH_REMAP_DEVICE		device memory no cache
 *
 * return base virtual address.
*/
void *libdh_ioremap_ex(paddr_t paddr, size_t size,
		       unsigned int prot, unsigned int flags);
bool libdh_check_remap_flags_valid(unsigned int flags);
uint64_t libdh_iomem_map_hint(unsigned int prot, unsigned int flags);
uint64_t libdh_get_pbha_hint(unsigned int flags);

static inline void *libdh_ioremap(paddr_t paddr, size_t size)
{
	return libdh_ioremap_ex(paddr, size, (unsigned int)(PROT_READ) |
				(unsigned int)(PROT_WRITE), LIBDH_REMAP_DEVICE);
}

static inline void *libdh_ioremap_cache(paddr_t paddr, size_t size)
{
	return libdh_ioremap_ex(paddr, size, (unsigned int)(PROT_READ) |
				(unsigned int)(PROT_WRITE), LIBDH_REMAP_CACHE);
}

static inline void *libdh_ioremap_wc(paddr_t paddr, size_t size)
{
	return libdh_ioremap_ex(paddr, size, (unsigned int)(PROT_READ) |
				(unsigned int)(PROT_WRITE), LIBDH_REMAP_NORMAL_NC);
}

void libdh_iounmap(const void *vaddr, size_t size);

int libdh_iomem_map(void *vaddr, paddr_t paddr,
		    size_t size, unsigned int prot,
		    unsigned int flags);
struct mem_raw_ranges;
int libdh_iomem_map_batch(void *vaddr, const struct mem_raw_ranges *pa_array,
			  unsigned int pa_array_size, unsigned int prot,
			  unsigned int flags);
int libdh_iomem_map_batch_pid(void *vaddr, const struct mem_raw_ranges *pa_array,
			      unsigned int pa_array_size, unsigned int prot,
			      unsigned int flags, int pid, unsigned long long vr_key);
int libdh_iomem_map_batch_mm(void *vaddr, const struct mem_raw_ranges *pa_array,
			      unsigned int pa_array_size, unsigned int prot,
			      unsigned int flags, unsigned long long vs_key);
int libdh_vm_mmap_batch(const struct mem_raw_ranges *pa_array,
			unsigned int pa_array_size,
			void *vaddr,
			unsigned int prot, unsigned int flags);
int libdh_vmalloc_mmap_page(unsigned long long vs_key, unsigned long uaddr,
			    unsigned long kaddr, unsigned long size, unsigned int prot,
			    unsigned int flags);

void libdh_register_nosave_region(unsigned long pfn_start, unsigned long long pfn_end);

#endif /* ULIBS_LIBDEVHOST_MM_H */
