/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes for devhost memory APIs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#ifndef __DEVHOST_API_MEMORY_H__
#define __DEVHOST_API_MEMORY_H__

#if defined(MODULE)
#include <linux/types.h>
#elif defined(UDK_MODULE) || defined(UDK_FRAMEWORK)
#include <udk/stdint.h>
#include <stdbool.h>
#include <stddef.h>
#else
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#endif

/*
 * CAUSION: This file will be used in linux source,
 *          **DO NOT** include any HMOS headers.
 */

unsigned int pool_va2offset(uintptr_t va);
uintptr_t pool_offset2va(unsigned int offset);
unsigned long long pool_offset2pa(unsigned int offset);

/* dma mem related */
void devhost_shm_release(void *shm_vaddr, size_t shm_len);
int __devhost_shm_mem_alloc(size_t size, uint64_t oflags, unsigned long long *paddr,
			    int *ret_id);
int __devhost_shm_mem_destroy(int shm_id);
void *devhost_backend_mem_alloc(size_t size, uint32_t mprot,
				unsigned long long *rpaddr,
				int *rshmid);
void devhost_backend_mem_free(int shmid, void *vaddr, size_t size);

/* for net service only */
void *devhost_dma_pool_alloc(unsigned long long *paddr,
			     unsigned int *offset);
void *devhost_dma_pool_alloc_ex(unsigned long long *paddr,
				unsigned int *offset, unsigned int size);
int devhost_dma_pool_free(unsigned int offset);
unsigned int devhost_dma_get_free_num(void);

int devhost_pal_copy_from_user(void *dst, const void *src, unsigned long n);
int devhost_pal_copy_to_user(void *dst, const void *src, unsigned long n);
int devhost_pal_copy_in_user(void *dst, const void *src, unsigned long n);
int devhost_pal_copy_from_caller(void *dst, const void *src, unsigned long n);
int devhost_pal_copy_to_caller(void *dst, const void *src, unsigned long n);
int devhost_pal_copy_msghdr_from_user(void *dst, void *src);
int devhost_pal_copy_msghdr_to_user(void *dst, void *src, void *uaddr, void *ucmsg, int ulen);

/*
 * struct devhost_vma_info - vma info in devhost for support user apps map and munmap
 * @len:	  lenght of the virtual address area from sysmgr
 * @start:	  start  of the virtual address area
 * @prot:	  protection mode of the memory
 * @offset:	  offset of the memory
 * @flags:	  flags of the memory
 * @vma_id:	  id of the devhost vma info
 * @priv:	  private data of the backend
 */
struct devhost_vma_info {
	unsigned long len;
	unsigned long start;
	unsigned long prot;
	unsigned long offset;
	unsigned int flags;
	unsigned int vma_id;
	void *priv;
	unsigned int seq;
};

int devhost_pal_umap_populate(const void *va, unsigned long long pa,
			      unsigned long len, unsigned int prot,
			      unsigned int flag);

int devhost_pal_usermap_prepare(unsigned long addr, unsigned long len,
				unsigned long prot, unsigned int flags,
				const bool iomap_prepared_ahead,
				struct devhost_vma_info **out);
void devhost_pal_usermap_finish(const struct devhost_vma_info *vma_info,
				const bool iomap_prepared_ahead, int success, int update);
int devhost_pal_usermap_munmap(unsigned long addr, unsigned long len);
int __do_devhost_usermap_munmap(unsigned int vma_id, unsigned long start, unsigned long len, int vs_is_releasing);
void *devhost_pal_find_vma(unsigned long addr, unsigned long long vs_key);

int devhost_pal_vm_zap(unsigned long addr, unsigned long len, unsigned int flags);
int __devhost_do_umap_page_fault(unsigned int vma_key, unsigned long long vr_key, unsigned long long addr,
				 unsigned int flags, unsigned long long offset);

int devhost_mem_resv_mem(const void *vaddr, unsigned long len);
#endif /* __DEVHOST_API_MEMORY_H__ */
