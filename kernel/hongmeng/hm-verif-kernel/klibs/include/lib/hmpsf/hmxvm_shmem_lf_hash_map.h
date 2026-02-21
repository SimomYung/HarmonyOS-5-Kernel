/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: headers for hm xvm shmem lf hash maps
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 26 19:22:02 2024
 */
#ifndef KLIBS_HMPSF_HMXVM_SHMEM_LF_HASH_MAP_H
#define KLIBS_HMPSF_HMXVM_SHMEM_LF_HASH_MAP_H

#include <lib/hmpsf/xvm_shmem_lf_hash_map.h>
#include <xvm/hm_xvm_errno.h>

static inline int hmxvm_shmem_lf_bytes_hash_map_check(void *map, __u32 *size)
{
	return xvm2hmerrno(xvm_shmem_lf_bytes_hash_map_check(map, size));
}

static inline int hmxvm_shmem_lf_bytes_hash_map_init(void *map, void *buf, __u32 buf_size,
						     void *lock_buf, bool buf_init)
{
	return xvm2hmerrno(xvm_shmem_lf_bytes_hash_map_init(map, buf, buf_size, lock_buf, buf_init));
}

static inline int hmxvm_shmem_lf_bytes_hash_map_get(void *map, const void *key, __u32 key_size,
						    void *val, __u32 *val_size)
{
	return xvm2hmerrno(xvm_shmem_lf_bytes_hash_map_get(map, key, key_size, val, val_size));
}

static inline int hmxvm_shmem_lf_bytes_hash_map_set(void *map, const void *key, __u32 key_size,
						    const void *val, __u32 val_size)
{
	return xvm2hmerrno(xvm_shmem_lf_bytes_hash_map_set(map, key, key_size, val, val_size));
}

static inline int hmxvm_shmem_lf_bytes_hash_map_iter_init(void *map, void *iter_handle)
{
	return xvm2hmerrno(xvm_shmem_lf_bytes_hash_map_iter_init(map, iter_handle));
}

static inline int hmxvm_shmem_lf_bytes_hash_map_next_entry(void *map, void *iter_handle,
							   void *key, __u32 *key_size,
							   void *val, __u32 *val_size)
{
	return xvm2hmerrno(xvm_shmem_lf_bytes_hash_map_next_entry(map, iter_handle, key, key_size, val, val_size));
}

static inline int hmxvm_shmem_lf_bytes_hash_map_clear(void *map)
{
	return xvm2hmerrno(xvm_shmem_lf_bytes_hash_map_clear(map));
}

static inline int hmxvm_shmem_lf_int_hash_map_load(void *map, const void *key, __u32 key_size, __u64 *val)
{
	return xvm2hmerrno(xvm_shmem_lf_int_hash_map_load(map, key, key_size, val));
}

static inline int hmxvm_shmem_lf_int_hash_map_store(void *map, const void *key, __u32 key_size, __u64 val)
{
	return xvm2hmerrno(xvm_shmem_lf_int_hash_map_store(map, key, key_size, val));
}

static inline int hmxvm_shmem_lf_int_hash_map_add_fetch(void *map, const void *key, __u32 key_size, __u64 *val)
{
	return xvm2hmerrno(xvm_shmem_lf_int_hash_map_add_fetch(map, key, key_size, val));
}

static inline int hmxvm_shmem_lf_int_hash_map_sub_fetch(void *map, const void *key, __u32 key_size, __u64 *val)
{
	return xvm2hmerrno(xvm_shmem_lf_int_hash_map_sub_fetch(map, key, key_size, val));
}

static inline int hmxvm_shmem_lf_int_hash_map_exchange(void *map, const void *key, __u32 key_size,
						       __u64 val, __u64 *old)
{
	return xvm2hmerrno(xvm_shmem_lf_int_hash_map_exchange(map, key, key_size, val, old));
}

static inline int hmxvm_shmem_lf_int_hash_map_compare_exchange(void *map, const void *key, __u32 key_size,
							       __u64 *expected, __u64 desired)
{
	return xvm2hmerrno(xvm_shmem_lf_int_hash_map_compare_exchange(map, key, key_size, expected, desired));
}

static inline int hmxvm_shmem_lf_int_hash_map_clear(void *map)
{
	return xvm2hmerrno(xvm_shmem_lf_int_hash_map_clear(map));
}

static inline int hmxvm_shmem_lf_int_hash_map_iter_init(void *map, void *iter_handle)
{
	return xvm2hmerrno(xvm_shmem_lf_int_hash_map_iter_init(map, iter_handle));
}

static inline int hmxvm_shmem_lf_int_hash_map_next_entry(void *map, void *iter_handle,
							 void *key, __u32 *key_size, __u64 *val)
{
	return xvm2hmerrno(xvm_shmem_lf_int_hash_map_next_entry(map, iter_handle, key, key_size, val));
}

#endif
