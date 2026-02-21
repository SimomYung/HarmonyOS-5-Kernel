/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Implementation of wrapped hongmeng functon
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 29 11:11:11 2019
 */
/* direct header */
#include "macro.h"
#include "wrapper_hm.h"

/* libc */
#include <dlfcn.h>
#include <stdio.h>
#include <stdint.h>

/* hongmeng ulibs */
#include <libmem/utils.h>
#include <libmem/mem_pool.h>
#include <libkbox/kbox.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsrv_sys/hm_vm.h>
#include <libhmsrv_sys/hm_ipc_shm.h>

/* hm-security */
#include <libkasan/kasan.h>
#include <libkasan/kasan_init.h>
#include "shadow.h"
#include "wrapper_hm_define.h"

struct ipc_shm_node {
	struct dlist_node node;
	uintptr_t ipc_shm_addr;
	size_t aligned_len;
};

DLIST_HEAD(ipc_shm_list);

struct raw_mutex kasan_ipc_shm_mutex = RAW_MUTEX_INITIALIZER;

static kasan_mutex_t kasan_heap_mutex;

void kasan_heap_mutex_init(void)
{
	kasan_mutex_init(&kasan_heap_mutex);
}

#ifdef KASAN_SHARED
void hmcrt_posthook(void)
{
	int rc;
	rc = kasan_init();
	if (rc != E_HM_OK) {
		hm_error("failed to initialize kasan\n");
		return;
	}

	hmcrt_posthook_type ori_hook = dlsym(RTLD_NEXT, "hmcrt_posthook");
	if (ori_hook != NULL) {
		ori_hook();
	}

	return;
}
#else
void __wrap_hmcrt_posthook(void)
{
	int rc;
	rc = kasan_init();
	if (rc != E_HM_OK) {
		hm_error("failed to initialize kasan\n");
		return;
	}
	__real_hmcrt_posthook();
	return;
}
#endif

#ifdef KASAN_SHARED
#define KASAN_WRAPPER_DEFINE
#define KASAN_WRAPPER(ret_type, name, ARG) \
static ret_type (*_##name)(_PARA(PACK(ARG))) = NULL; \
void kasan_reg_##name(void) \
{ \
	_##name = dlsym(RTLD_NEXT, #name); \
} \
ret_type ori_##name(_PARA(PACK(ARG))) \
{ \
	if (_##name == NULL) { \
		kasan_reg_##name(); \
	} \
	BUG_ON(_##name == NULL); \
	return _##name(_ARGS(PACK(ARG))); \
}
#include "wrapper_hm_api.h"
#undef KASAN_WRAPPER
#undef KASAN_WRAPPER_DEFINE
#endif

#ifdef KASAN_SHARED
long hm_mem_brk(const void *brk)
#else
long __wrap_hm_mem_brk(const void *brk)
#endif
{
	long ret;
	uintptr_t raddr = 0;
	uintptr_t heap_start = kasan_get_heap_start();
	uintptr_t heap_end = kasan_get_heap_end();

	kasan_mutex_lock(&kasan_heap_mutex);
	ret = ori_hm_mem_brk(brk);
	if (ret > 0) {
		raddr = (uintptr_t)ret;
		ret = E_HM_OK;
	}

	if (ret == E_HM_OK) {
		/* libc use brk 0 to get *heap_start_p */
		if (((uintptr_t)brk == 0) && (heap_start == 0)) {
			heap_start = raddr;
			heap_end = heap_start;
			kasan_set_heap_start(heap_start);
			kasan_set_heap_end(heap_end);
		}

		if (raddr < heap_end) {
			kasan_unmap_shadow_range(raddr, heap_end);
		} else if (raddr > heap_end) {
			ret = kasan_map_shadow_range(heap_end, raddr);
		}

		kasan_set_heap_end((((uintptr_t)brk <= heap_start) ? heap_end : raddr));
	}
	kasan_mutex_unlock(&kasan_heap_mutex);

	return ret;
}

#ifdef KASAN_SHARED
void *hm_mem_mmap_posix(void *addr, size_t len, uint32_t prot, uint32_t flags,
			int32_t fd, int64_t offset, int *ret_err)
#else
void *__wrap_hm_mem_mmap_posix(void *addr, size_t len, uint32_t prot, uint32_t flags,
			       int32_t fd, int64_t offset, int *ret_err)
#endif
{
	int ret;
	void *mmap_posix_addr = NULL;

	mmap_posix_addr = ori_hm_mem_mmap_posix(addr, len, prot, flags, fd, offset, ret_err);
	if (mmap_posix_addr != MAP_FAILED) {
		if (prot != PROT_NONE) {
			ret = kasan_map_shadow_range((uintptr_t)mmap_posix_addr,
						(uintptr_t)mmap_posix_addr + ALIGN_UP(len, __PAGE_SIZE));
			if (ret < 0) {
				(void)ori_hm_mem_munmap(mmap_posix_addr, len);
				mmap_posix_addr = MAP_FAILED;
			}
			if (ret_err != NULL && ret < 0) {
				*ret_err = ret;
			}
		}
	}

	return mmap_posix_addr;
}

#ifdef KASAN_SHARED
void *hm_mem_mmap_fixed(void *addr, size_t len,
			uint32_t prot, uint32_t flags,
			int32_t fd, int64_t offset,
			bool *unmapped, int *ret_err)
#else
void *__wrap_hm_mem_mmap_fixed(void *addr, size_t len,
			       uint32_t prot, uint32_t flags,
			       int32_t fd, int64_t offset,
			       bool *unmapped, int *ret_err)
#endif
{
	int ret;
	void *mmap_addr = NULL;

	mmap_addr = ori_hm_mem_mmap_fixed(addr, len, prot, flags, fd, offset,
					  unmapped, ret_err);
	if (mmap_addr != MAP_FAILED) {
		if (prot != PROT_NONE) {
			ret = kasan_map_shadow_range((uintptr_t)mmap_addr,
						     (uintptr_t)mmap_addr + ALIGN_UP(len, __PAGE_SIZE));
			if (ret < 0) {
				(void)ori_hm_mem_munmap(mmap_addr, len);
				mmap_addr = MAP_FAILED;
			}
			if (ret_err != NULL && ret < 0) {
				*ret_err = ret;
			}
		}
	}
	return mmap_addr;
}

static void *do_hm_mem_mmap(uintptr_t mmap_addr, size_t len,
			    uint32_t prot)
{
	int ret;
	if (mmap_addr != (uintptr_t)MAP_FAILED) {
		if (prot != PROT_NONE) {
			ret = kasan_map_shadow_range(mmap_addr,
						     mmap_addr + ALIGN_UP(len, __PAGE_SIZE));
			if (ret < 0) {
				(void)ori_hm_mem_munmap((void *)mmap_addr, len);
				mmap_addr = (uintptr_t)MAP_FAILED;
			}
		}
	}

	return (void *)mmap_addr;
}

#ifdef KASAN_SHARED
void *hm_mem_mmap(void *addr, size_t len,
		  uint32_t prot, uint32_t flags,
		  int32_t fd, int64_t offset)
#else
void *__wrap_hm_mem_mmap(void *addr, size_t len,
			 uint32_t prot, uint32_t flags,
			 int32_t fd, int64_t offset)
#endif
{
	void *mmap_addr = NULL;

	mmap_addr = ori_hm_mem_mmap(addr, len, prot, flags, fd, offset);

	return do_hm_mem_mmap((uintptr_t)mmap_addr, len, prot);
}

#ifdef KASAN_SHARED
void *hm_mem_mmap_ex(void *addr, size_t len,
		  uint32_t prot, uint32_t flags,
		  int32_t fd, int64_t offset, size_t guard_size)
#else
void *__wrap_hm_mem_mmap_ex(void *addr, size_t len,
			 uint32_t prot, uint32_t flags,
			 int32_t fd, int64_t offset, size_t guard_size)
#endif
{
	void *mmap_addr = NULL;

	mmap_addr = ori_hm_mem_mmap_ex(addr, len, prot, flags, fd, offset, guard_size);

	return do_hm_mem_mmap((uintptr_t)mmap_addr, len, prot);
}

#ifdef KASAN_SHARED
int hm_mem_munmap(void *addr, size_t len)
#else
int __wrap_hm_mem_munmap(void *addr, size_t len)
#endif
{
	size_t aligned_len = ALIGN_UP(len, __PAGE_SIZE);
	kasan_unmap_shadow_range((uintptr_t)addr, (uintptr_t)addr + aligned_len);
	return ori_hm_mem_munmap(addr, len);
}

#ifdef KASAN_SHARED
void *hm_server_mem_mmap_posix(rref_t server,
			       const void *addr, size_t len,
			       uint32_t prot, uint32_t flags,
			       int32_t fd, int64_t offset, int *ret_err)
#else
void *__wrap_hm_server_mem_mmap_posix(rref_t server,
				      const void *addr, size_t len,
				      uint32_t prot, uint32_t flags,
				      int32_t fd, int64_t offset, int *ret_err)
#endif
{
	void *ret_addr = NULL;
	int ret;
	size_t aligned_len;

	ret_addr = ori_hm_server_mem_mmap_posix(server, addr, len, prot, flags, fd, offset, ret_err);
	if (ret_addr != MAP_FAILED) {
		aligned_len = ALIGN_UP(len, __PAGE_SIZE);
		ret = kasan_map_shadow_range((uintptr_t)ret_addr, (uintptr_t)ret_addr + aligned_len);
		if (ret < 0) {
			(void)ori_hm_server_mem_munmap(server, ret_addr, len);
			ret_addr = MAP_FAILED;
		}
		if (ret_err != NULL && ret < 0) {
			*ret_err = ret;
		}
	}

	return ret_addr;
}

#ifdef KASAN_SHARED
int hm_server_mem_munmap(rref_t server, const void *addr, size_t len)
#else
int __wrap_hm_server_mem_munmap(rref_t server, const void *addr, size_t len)
#endif
{
	size_t aligned_len = ALIGN_UP(len, __PAGE_SIZE);

	kasan_unmap_shadow_range((uintptr_t)addr, (uintptr_t)addr + aligned_len);
	return ori_hm_server_mem_munmap(server, addr, len);
}

#ifdef KASAN_SHARED
long hm_mem_mremap(void *old_addr, size_t old_len, size_t new_len,
		    uint32_t flags, void *new_addr)
#else
long __wrap_hm_mem_mremap(void *old_addr, size_t old_len, size_t new_len,
		    uint32_t flags, void *new_addr)
#endif
{
	int ret;
	long ret_addr;
	size_t aligned_len;

	ret_addr = ori_hm_mem_mremap(old_addr, old_len, new_len, flags, new_addr);
	if (ret_addr < 0) {
		return ret_addr;
	}

	if (old_len == new_len && old_addr == ulong_to_ptr(ret_addr, void)) {
		return ret_addr;
	}

	/*
	 * if kasan_map_shadow_range() failed, the old_addr has been unmapped.
	 * it is different from mremap(), need attention!
	 */
	aligned_len = ALIGN_UP(new_len, __PAGE_SIZE);
	ret = kasan_map_shadow_range((uintptr_t)ret_addr,
				     (uintptr_t)ret_addr + aligned_len);
	if (ret < 0) {
		(void)ori_hm_mem_munmap(ulong_to_ptr(ret_addr, void), new_len);
		ret_addr = (long)ptr_to_ulong(MAP_FAILED);
	}

	aligned_len = ALIGN_UP(old_len, __PAGE_SIZE);
	kasan_unmap_shadow_range((uintptr_t)old_addr,
				(uintptr_t)old_addr + aligned_len);

	return ret_addr;
}
#ifdef KASAN_SHARED
int actv_open_region(const char *region_name, uintptr_t *pvaddr, size_t *psize)
#else
int __wrap_actv_open_region(const char *region_name, uintptr_t *pvaddr, size_t *psize)
#endif
{
	int ret;

	ret = ori_actv_open_region(region_name, pvaddr, psize);
	if (ret == E_HM_OK) {
		size_t aligned_len = ALIGN_UP(*psize, __PAGE_SIZE);
		ret = kasan_map_shadow_range(*pvaddr, *pvaddr + aligned_len);
		if (ret < 0) {
			(void)ori_actv_close_region(region_name, *pvaddr, *psize);
			ret = E_HM_RESFAULT;
		}
	}

	return ret;
}

#ifdef KASAN_SHARED
int actv_close_region(const char *region_name, uintptr_t vaddr, size_t size)
#else
int __wrap_actv_close_region(const char *region_name, uintptr_t vaddr, size_t size)
#endif
{
	size_t aligned_len = ALIGN_UP(size, __PAGE_SIZE);
	kasan_unmap_shadow_range(vaddr, vaddr + aligned_len);
	return ori_actv_close_region(region_name, vaddr, size);
}

#ifdef KASAN_SHARED
int hm_vm_map_to_process(struct vm_map_to_process_args *args)
#else
int __wrap_hm_vm_map_to_process(struct vm_map_to_process_args *args)
#endif
{
	int ret;
	unsigned int i;
	size_t aligned_len;
	uintptr_t process_args_vaddr;

	ret = ori_hm_vm_map_to_process(args);
	if (ret == E_HM_OK) {
		for (i = 0; i < args->region_num; i++) {
			process_args_vaddr = (uintptr_t)args->regions[i].hva;
			if (process_args_vaddr != 0) {
				aligned_len = (size_t)ALIGN_UP(args->regions[i].size, __PAGE_SIZE);
				(void)kasan_map_shadow_range(process_args_vaddr,
							     process_args_vaddr + aligned_len);
			}
		}
	}

	return ret;
}

#ifdef KASAN_SHARED
int hm_vm_map_iomem(struct vm_iomem_args *args)
#else
int __wrap_hm_vm_map_iomem(struct vm_iomem_args *args)
#endif
{
	int ret;
	unsigned int i;
	size_t aligned_len;
	uintptr_t iomem_args_vaddr;

	ret = ori_hm_vm_map_iomem(args);
	if (ret == E_HM_OK) {
		for (i = 0; i < args->region_num; i++) {
			iomem_args_vaddr = (uintptr_t)args->regions[i].hva;
			if (iomem_args_vaddr != 0) {
				aligned_len = (size_t)ALIGN_UP(args->regions[i].size, __PAGE_SIZE);
				(void)kasan_map_shadow_range(iomem_args_vaddr,
							     iomem_args_vaddr + aligned_len);
			}
		}
	}

	return ret;
}

#ifdef KASAN_SHARED
uintptr_t mem_pool_alloc(struct mem_pool_s *pool, size_t len)
#else
uintptr_t __wrap_mem_pool_alloc(struct mem_pool_s *pool, size_t len)
#endif
{
	uintptr_t ret_addr;

	ret_addr = ori_mem_pool_alloc(pool, len);
	if (ret_addr != 0UL) {
		kasan_unpoison_shadow(ret_addr, len);
	}

	return ret_addr;
}

#ifdef KASAN_SHARED
int mem_pool_free(struct mem_pool_s *pool, uintptr_t va, size_t len)
#else
int __wrap_mem_pool_free(struct mem_pool_s *pool, uintptr_t va, size_t len)
#endif
{
	kasan_poison_shadow(va, len, MEMPOOL_POISON_VALUE);
	return ori_mem_pool_free(pool, va, len);
}

#ifdef KASAN_SHARED
int hm_mem_mprotect(const void *addr, size_t len, uint32_t prot)
#else
int __wrap_hm_mem_mprotect(const void *addr, size_t len, uint32_t prot)
#endif
{
	int ret;
	size_t aligned_len = (size_t)ALIGN_UP(len, __PAGE_SIZE);

	ret = ori_hm_mem_mprotect(addr, len, prot);
	if (ret == E_HM_OK) {
		if (prot != PROT_NONE) {
			(void)kasan_mprotect_shadow_range((uintptr_t)addr, (uintptr_t)addr + aligned_len);
		}
	}

	return ret;
}

static void kasan_ipc_shm_add_listnode(uintptr_t add_addr, size_t len)
{
	size_t aligned_len = ALIGN_UP(len, __PAGE_SIZE);
	struct ipc_shm_node *new_node = NULL;

	new_node = (struct ipc_shm_node *)malloc(sizeof(struct ipc_shm_node));
	if (new_node != NULL) {
		new_node->ipc_shm_addr = add_addr;
		new_node->aligned_len = aligned_len;
		(void)kasan_map_shadow_range((uintptr_t)add_addr,
					     (uintptr_t)add_addr + aligned_len);
		raw_mutex_lock(&kasan_ipc_shm_mutex);
		dlist_insert_tail(&ipc_shm_list, &new_node->node);
		raw_mutex_unlock(&kasan_ipc_shm_mutex);
	} else {
		hm_warn("failed to alloc kasan ipc shm node\n");
	}

	return;
}

static void kasan_ipc_shm_delete_listnode(uintptr_t del_addr)
{
	struct ipc_shm_node *pos = NULL;
	struct ipc_shm_node *next = NULL;
	struct ipc_shm_node *del_node = NULL;

	raw_mutex_lock(&kasan_ipc_shm_mutex);
	dlist_for_each_entry_safe(pos, next, &ipc_shm_list,
				  struct ipc_shm_node, node) {
		if (pos->ipc_shm_addr == del_addr) {
			del_node = pos;
			dlist_delete(&pos->node);
			break;
		}
	}
	raw_mutex_unlock(&kasan_ipc_shm_mutex);

	if (del_node != NULL) {
		kasan_unmap_shadow_range((uintptr_t)del_node->ipc_shm_addr,
					       (uintptr_t)del_node->ipc_shm_addr +
					       del_node->aligned_len);
		free(del_node);
	} else {
		hm_warn("failed to delete kasan ipc shm node\n");
	}

	return;
}

#ifdef KASAN_SHARED
void *hm_ipc_shmat(int shmid, const void *addr, int flags, int *errn)
#else
void *__wrap_hm_ipc_shmat(int shmid, const void *addr, int flags, int *errn)
#endif
{
	int ret;
	void *rvaddr = NULL;
	struct shmid_ds buff = {0};

	rvaddr = ori_hm_ipc_shmat(shmid, addr, flags, errn);
	if (rvaddr != NULL) {
		ret = hm_ipc_shmctl(shmid, IPC_STAT, &buff);
		if (ret == E_HM_OK) {
			kasan_ipc_shm_add_listnode((uintptr_t)rvaddr, buff.shm_segsz);
		} else {
			hm_warn("failed to get shmat info, shmid=%d\n", shmid);
		}
	}

	return rvaddr;
}

#ifdef KASAN_SHARED
int hm_ipc_shmdt(const void *addr)
#else
int __wrap_hm_ipc_shmdt(const void *addr)
#endif
{
	kasan_ipc_shm_delete_listnode((uintptr_t)addr);
	return ori_hm_ipc_shmdt(addr);
}

#ifndef __HOST_LLT__
#ifdef KASAN_SHARED
int safe_copy(void *dst, const void *src, size_t bytes)
#else
int __wrap_safe_copy(void *dst, const void *src, size_t bytes)
#endif
{
	__asan_storeN_noabort((unsigned long)(uintptr_t)dst, bytes);
	return ori_safe_copy(dst, src, bytes);
}

#ifdef KASAN_SHARED
int safe_copy_nopf(void *dst, const void *src, size_t bytes)
#else
int __wrap_safe_copy_nopf(void *dst, const void *src, size_t bytes)
#endif
{
	__asan_storeN_noabort((unsigned long)(uintptr_t)dst, bytes);
	return ori_safe_copy_nopf(dst, src, bytes);
}
#endif
