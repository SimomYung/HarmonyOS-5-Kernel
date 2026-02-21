/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Memory mapping routines of UDK
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 03 10:27:05 2019
 */
#ifndef __UDK_MM_H__
#define __UDK_MM_H__

#include <udk/errno.h>
#include <stdint.h>
#include <stddef.h>

#define __user
#define __iomem

#define UDK_IOMEM_NORMAL	1U
#define UDK_IOMEM_NORMAL_NC	(1U << 1)
#define UDK_IOMEM_DEVICE	(1U << 2)

#define UDK_PAF_NO_FAIL		 (uint32_t)0x00020000
#define UDK_PAF_NO_IO		 (uint32_t)0x00008000
  
struct mem_raw_ranges;
int udk_iomem_mmap(unsigned long vaddr,
		   unsigned long long paddr,
		   unsigned long size,
		   unsigned long prot,
		   unsigned int flags);
int udk_iomem_mmap_batch(unsigned long vaddr,
			const struct mem_raw_ranges *pa_array,
			unsigned int pa_array_size,
			unsigned int prot,
			unsigned int flags);
void udk_iomem_munmap(unsigned long vaddr, unsigned long size);
void* udk_ioremap(unsigned long long paddr, unsigned long size);
void* udk_ioremap_cache(unsigned long long paddr, unsigned long size);
void* udk_ioremap_nocache(unsigned long long paddr, unsigned long size);
#define udk_iounmap(va, size) udk_iounmap2(va)
void udk_iounmap2(const void *vaddr);
int udk_mmap(unsigned long addr, unsigned long length,
	     unsigned long prot, unsigned int flags,
	     int fd, unsigned long offset, uintptr_t *rvaddr);
int udk_copy_from_user(void *dst, const void *src, unsigned long n);
int udk_copy_to_user(void *dst, const void *src, unsigned long n);
int udk_copy_from_caller(void *dst, const void *src, unsigned long n);
int udk_copy_to_caller(void *dst, const void *src, unsigned long n);

static inline int udk_put_user_int(void *dst, int val)
{
	return udk_copy_to_user(dst, (void *)&val, sizeof(val));
}

static inline int udk_put_user_ulong(void *dst, unsigned long val)
{
	return udk_copy_to_user(dst, (void *)&val, sizeof(val));
}

#define DMA_ALLOC_COHERENT 0x1U

void *udk_dma_alloc_aligned(unsigned long size, uint32_t flag,
			    unsigned long long *paddr, unsigned long align);
void *udk_dma_alloc(unsigned long size, uint32_t flag, unsigned long long *paddr);
int udk_dma_free(const void *vaddr);
unsigned long long udk_dma_va_to_pa(const void *vaddr);
void *udk_dma_pa_to_va(unsigned long long paddr);

struct udk_shm_info {
	int32_t id;  /* shmid */
	size_t len;  /* length of shared memory */
	void *vaddr; /* mapped address */
};

#define UDK_SHM_ALLOC_POPULATE	0x1 /* compatible with SHM_ALLOC_POPULATE */

/* udk shared memory */
int udk_alloc_shm_anon(size_t len, uint64_t flags,
		       struct udk_shm_info *ret_info);
int udk_free_shm_anon(struct udk_shm_info *shm_info);
int udk_grant_shm_to_caller(int32_t shmid, uint32_t prot, uint64_t *key);

/* udk vma */
struct udk_vma;

struct udk_vma_ops {
	int (*munmap)(struct udk_vma *vma);
};

struct udk_vma {
	unsigned long len;
	unsigned long start;
	unsigned long prot;
	unsigned long offset;
	unsigned int flags;
	unsigned int vma_id;
	struct udk_vma_ops *ops;
	/* private data for refcnt */
	void *fctx;
};

uint64_t udk_mem_size_total(void);
void udk_pal_change_secpage_range(unsigned long long phys_addr, unsigned long vaddr,
				  unsigned long size, unsigned int flags);

#endif /* __UDK_MM_H__ */
