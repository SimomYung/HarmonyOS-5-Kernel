/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: hmpsf ctrlmem syscall wrapper
 * Author: Huawei OS Kernel Lab
 * Create: Wen Dec 27 14:44:12 2023
 */

#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_CTRLMEM_PSF_SYSCALLS_LAYER_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_CTRLMEM_PSF_SYSCALLS_LAYER_H

#include <hmasm/memory.h>
#include <sys/types.h>
#include <hmkernel/types.h>
#include <hmkernel/ctrlmem/kobjs/hmpsfmod.h>
#include <hmkernel/ctrlmem/kobjs/hmpsfmap.h>
#include <hmkernel/ctrlmem/syscall.h>
#include <hmkernel/pgstr/pgstr.h>
#include <libhmsrv_sys/hm_ctrlmem_syscalls.h>
#include <hmasm/pgtbl/pgtblhdlr.h>

struct __ctrlmem_kobjctnr_page_freelist_s;

static inline int sysctrlmem_kobjctnr_spawn_hmpsfmod(struct __ctrlmem_kobjctnr_page_freelist_s *fl,
						     struct pgstr_s *pages_pgstr, __paddr_t pages_pa,
						     __u32 pages_num)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &fl->head, __CTRLMEM_KOBJCTNR_META_KOBJ_HMPSFMOD,
			      __sysctrlmem_arg_ptr(pages_pgstr),
			      __sysctrlmem_arg_paddr(pages_pa),
			      __sysctrlmem_arg_u32(pages_num));
}

static inline int sysctrlmem_kobjctnr_spawn_hmpsfmap(struct __ctrlmem_kobjctnr_page_freelist_s *fl,
						     struct pgstr_s *pages_pgstr, __paddr_t pages_pa,
						     __u32 pages_num, __u64 flags)
{
	struct __ctrlmem_kobj_hmpsfmap_spawn_args_s args = {
		.pages_pgstr = (void *)pages_pgstr,
		.pages_pa = pages_pa,
		.pages_num = pages_num,
		.flags = flags,
	};
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, &fl->head, __CTRLMEM_KOBJCTNR_META_KOBJ_HMPSFMAP,
			      __sysctrlmem_arg_ptr(&args));
}

struct __ctrlmem_kobjctnr_page_s;

static inline int sysctrlmem_kobj_hmpsfmod_load(struct __ctrlmem_kobj_hmpsfmod_s *kobj,
						struct hmpsf_module_kernel_attr_s *attr)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMOD_LOAD),
			      __sysctrlmem_arg_ptr(attr));
}

static inline int sysctrlmem_kobj_hmpsfmod_unload(struct __ctrlmem_kobj_hmpsfmod_s *kobj)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMOD_UNLOAD));
}

static inline int sysctrlmem_kobj_hmpsfmod_invalidate(struct __ctrlmem_kobj_hmpsfmod_s *kobj)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMOD_INVALIDATE));
}

static inline int sysctrlmem_kobj_hmpsfmap_map_delete(struct __ctrlmem_kobj_hmpsfmap_s *kobj)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMAP_MAP_DELETE));
}

static inline int sysctrlmem_kobj_hmpsfmap_map_create(struct __ctrlmem_kobj_hmpsfmap_s *kobj,
						      void *map, unsigned char map_type)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMAP_MAP_CREATE),
			      __sysctrlmem_arg_ptr(map), __sysctrlmem_arg_u32(map_type));
}

static inline int sysctrlmem_kobj_hmpsfmap_int_hash_map_load(struct __ctrlmem_kobj_hmpsfmap_s *kobj,
							     const void *key, __u32 key_size, __u64 *val)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMAP_INT_HASH_MAP_LOAD),
			      __sysctrlmem_arg_ptr(key), __sysctrlmem_arg_u32(key_size),
			      __sysctrlmem_arg_ptr(val));
}

static inline int sysctrlmem_kobj_hmpsfmap_int_hash_map_store(struct __ctrlmem_kobj_hmpsfmap_s *kobj,
							      const void *key, __u32 key_size, __u64 val)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMAP_INT_HASH_MAP_STORE),
			      __sysctrlmem_arg_ptr(key), __sysctrlmem_arg_u32(key_size),
			      __sysctrlmem_arg_u64(val));
}

static inline int sysctrlmem_kobj_hmpsfmap_int_hash_map_exchange(struct __ctrlmem_kobj_hmpsfmap_s *kobj,
								 const void *key,
								 __u32 key_size,
								 __u64 val,
								 __u64 *old)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMAP_INT_HASH_MAP_EXCHANGE),
			      __sysctrlmem_arg_ptr(key), __sysctrlmem_arg_u32(key_size),
			      __sysctrlmem_arg_u64(val), __sysctrlmem_arg_ptr(old));
}

static inline int sysctrlmem_kobj_hmpsfmap_int_hash_map_compare_exchange(struct __ctrlmem_kobj_hmpsfmap_s *kobj,
									 const void *key,
									 __u32 key_size,
									 __u64 *expected,
									 __u64 desired)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMAP_INT_HASH_MAP_COMPARE_EXCHANGE),
			      __sysctrlmem_arg_ptr(key), __sysctrlmem_arg_u32(key_size),
			      __sysctrlmem_arg_ptr(expected), __sysctrlmem_arg_u64(desired));
}

static inline int sysctrlmem_kobj_hmpsfmap_int_hash_map_add_fetch(struct __ctrlmem_kobj_hmpsfmap_s *kobj,
								  const void *key,
								  __u32 key_size,
								  __u64 *val)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMAP_INT_HASH_MAP_ADD_FETCH),
			      __sysctrlmem_arg_ptr(key), __sysctrlmem_arg_u32(key_size),
			      __sysctrlmem_arg_ptr(val));
}

static inline int sysctrlmem_kobj_hmpsfmap_int_hash_map_sub_fetch(struct __ctrlmem_kobj_hmpsfmap_s *kobj,
								  const void *key,
								  __u32 key_size,
								  __u64 *val)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMAP_INT_HASH_MAP_SUB_FETCH),
			      __sysctrlmem_arg_ptr(key), __sysctrlmem_arg_u32(key_size),
			      __sysctrlmem_arg_ptr(val));
}

static inline int sysctrlmem_kobj_hmpsfmap_int_hash_map_next_key(struct __ctrlmem_kobj_hmpsfmap_s *kobj,
								 const void *key,
								 __u32 key_size,
								 void *next_key,
								 __u32 *next_key_size)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMAP_INT_HASH_MAP_NEXT_KEY),
			      __sysctrlmem_arg_ptr(key), __sysctrlmem_arg_u32(key_size),
			      __sysctrlmem_arg_ptr(next_key), __sysctrlmem_arg_ptr(next_key_size));
}

static inline int sysctrlmem_kobj_hmpsfmap_int_hash_map_clear(struct __ctrlmem_kobj_hmpsfmap_s *kobj)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMAP_INT_HASH_MAP_CLEAR));
}

static inline int sysctrlmem_kobj_hmpsfmap_bytes_hash_map_get(struct __ctrlmem_kobj_hmpsfmap_s *kobj,
							      const void *key, __u32 key_size,
							      void *val, __u32 *val_size)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMAP_BYTES_HASH_MAP_GET),
			      __sysctrlmem_arg_ptr(key), __sysctrlmem_arg_u32(key_size),
			      __sysctrlmem_arg_ptr(val), __sysctrlmem_arg_ptr(val_size));
}

static inline int sysctrlmem_kobj_hmpsfmap_bytes_hash_map_set(struct __ctrlmem_kobj_hmpsfmap_s *kobj,
							      const void *key, __u32 key_size,
							      const void *val, __u32 val_size)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMAP_BYTES_HASH_MAP_SET),
			      __sysctrlmem_arg_ptr(key), __sysctrlmem_arg_u32(key_size),
			      __sysctrlmem_arg_ptr(val), __sysctrlmem_arg_u32(val_size));
}

static inline int sysctrlmem_kobj_hmpsfmap_bytes_hash_map_next_key(struct __ctrlmem_kobj_hmpsfmap_s *kobj,
								   const void *key,
								   __u32 key_size,
								   void *next_key,
								   __u32 *next_key_size)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMAP_BYTES_HASH_MAP_NEXT_KEY),
			      __sysctrlmem_arg_ptr(key), __sysctrlmem_arg_u32(key_size),
			      __sysctrlmem_arg_ptr(next_key), __sysctrlmem_arg_ptr(next_key_size));
}

static inline int sysctrlmem_kobj_hmpsfmap_bytes_hash_map_clear(struct __ctrlmem_kobj_hmpsfmap_s *kobj)
{
	return sysctrlmem_v32(__CTRLMEM_SYSCALL_UNKNOWN, kobj,
			      __sysctrlmem_arg_u32(__CTRLMEM_KOBJ_HMPSFMAP_BYTES_HASH_MAP_CLEAR));
}

#endif
