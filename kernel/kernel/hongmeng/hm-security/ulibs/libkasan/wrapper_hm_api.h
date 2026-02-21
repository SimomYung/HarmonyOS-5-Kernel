/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Wrapped hongmeng api
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 29 11:11:11 2019
 */
#include <hongmeng/types.h>
#include <libsysif/sysmgr/api.h>
#include <libmem/mem_pool.h>
#include <libkbox/kbox.h>

KASAN_WRAPPER(long, hm_mem_brk,
	      ARGS(const void *,
		   brk))

KASAN_WRAPPER(void *, hm_mem_mmap,
	      ARGS(void *, addr,
		   size_t, len,
		   uint32_t, prot,
		   uint32_t, flags,
		   int32_t, fd,
		   int64_t, offset))

KASAN_WRAPPER(void *, hm_mem_mmap_ex,
	      ARGS(void *, addr,
		   size_t, len,
		   uint32_t, prot,
		   uint32_t, flags,
		   int32_t, fd,
		   int64_t, offset,
		   size_t, guard_size))

KASAN_WRAPPER(void *, hm_mem_mmap_fixed,
	      ARGS(void *,
		   addr,
		   size_t,
		   len,
		   uint32_t,
		   prot,
		   uint32_t,
		   flags,
		   int32_t,
		   fd,
		   int64_t,
		   offset,
		   bool *,
		   unmapped,
		   int *,
		   ret_err))

KASAN_WRAPPER(void *, hm_mem_mmap_posix,
	      ARGS(void *, addr,
		   size_t, len,
		   uint32_t, prot,
		   uint32_t, flags,
		   int32_t, fd,
		   int64_t, offset,
		   int *, ret_err))

KASAN_WRAPPER(int, hm_mem_munmap,
	      ARGS(void *,
		   addr,
		   size_t,
		   len))

KASAN_WRAPPER(long, hm_mem_mremap,
	      ARGS(void *,
		   old_addr,
		   size_t,
		   old_len,
		   size_t,
		   new_len,
		   uint32_t,
		   flags,
		   void *,
		   new_addr))

KASAN_WRAPPER(void *, hm_server_mem_mmap_posix,
	      ARGS(rref_t, server,
		   const void *, addr,
		   size_t, len,
		   uint32_t, prot,
		   uint32_t, flags,
		   int32_t, fd,
		   int64_t, offset,
		   int *, ret_err))

KASAN_WRAPPER(int, hm_server_mem_munmap,
	      ARGS(rref_t,
		   server,
		   const void *,
		   addr,
		   size_t,
		   len))

KASAN_WRAPPER(int, actv_open_region,
	      ARGS(const char *,
		   region_name,
		   uintptr_t *,
		   pvaddr,
		   size_t *,
		   psize))

KASAN_WRAPPER(int, actv_close_region,
	      ARGS(const char *,
		   region_name,
		   uintptr_t,
		   vaddr,
		   size_t,
		   size))

KASAN_WRAPPER(int, hm_vm_map_to_process,
	      ARGS(struct vm_map_to_process_args *,
		   args))

KASAN_WRAPPER(int, hm_vm_map_iomem,
	      ARGS(struct vm_iomem_args *,
		   args))

KASAN_WRAPPER(uintptr_t, mem_pool_alloc,
	      ARGS(struct mem_pool_s *,
		   pool,
		   size_t,
		   len))

KASAN_WRAPPER(int, mem_pool_free,
	      ARGS(struct mem_pool_s *,
		   pool,
		   uintptr_t,
		   va,
		   size_t,
		   len))

KASAN_WRAPPER(int, hm_mem_mprotect,
	      ARGS(const void *,
		   addr,
		   size_t,
		   len,
		   uint32_t,
		   prot))

KASAN_WRAPPER(void *, hm_ipc_shmat,
	      ARGS(int,
		   shmid,
		   const void *,
		   addr,
		   int,
		   flags,
		   int *,
		   errn))

KASAN_WRAPPER(int, hm_ipc_shmdt,
	      ARGS(const void *,
		   addr))

#ifndef __HOST_LLT__
KASAN_WRAPPER(int, safe_copy,
	      ARGS(void * ,
		   dst,
		   const void *,
		   src,
		   size_t,
		   bytes))

KASAN_WRAPPER(int, safe_copy_nopf,
	      ARGS(void * ,
		   dst,
		   const void *,
		   src,
		   size_t,
		   bytes))
#endif
