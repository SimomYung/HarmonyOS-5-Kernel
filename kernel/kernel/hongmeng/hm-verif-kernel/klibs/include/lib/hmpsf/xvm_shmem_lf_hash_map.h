/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: headers for xvm shmem lf hash maps
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 26 19:23:40 2024
 */
#ifndef KLIBS_HMPSF_XVM_SHMEM_LF_HASH_MAP_H
#define KLIBS_HMPSF_XVM_SHMEM_LF_HASH_MAP_H

#include <hmasm/types.h>

/*
 * Because klibs headers do no depend on xvm headers, we use opaque pointers for xvm map pointers.
 */

void xvm_shmem_lf_hash_map_init(__u8 prio);
int xvm_shmem_lf_bytes_hash_map_check(void *map, __u32 *size);
int xvm_shmem_lf_bytes_hash_map_init(void *map, void *buf, __u32 buf_size, void *lock_buf, __u8 buf_init);

int xvm_shmem_lf_bytes_hash_map_get(void *map, const void *key, __u32 key_size, void *val, __u32 *val_size);
int xvm_shmem_lf_bytes_hash_map_set(void *map, const void *key, __u32 key_size, const void *val, __u32 val_size);
int xvm_shmem_lf_bytes_hash_map_iter_init(void *map, void *iter_handle);
int xvm_shmem_lf_bytes_hash_map_next_entry(void *map, void *iter_handle,
					   void *key, __u32 *key_size, void *val, __u32 *val_size);
int xvm_shmem_lf_bytes_hash_map_clear(void *map);

int xvm_shmem_lf_int_hash_map_check(void *map, __u32 *size);
int xvm_shmem_lf_int_hash_map_init(void *map, void *buf, __u32 buf_size, void *lock_buf, __u8 buf_init);
int xvm_shmem_lf_int_hash_map_load(void *map, const void *key, __u32 key_size, __u64 *val);
int xvm_shmem_lf_int_hash_map_store(void *map, const void *key, __u32 key_size, __u64 val);
int xvm_shmem_lf_int_hash_map_add_fetch(void *map, const void *key, __u32 key_size, __u64 *val);
int xvm_shmem_lf_int_hash_map_sub_fetch(void *map, const void *key, __u32 key_size, __u64 *val);
int xvm_shmem_lf_int_hash_map_exchange(void *map, const void *key, __u32 key_size, __u64 val, __u64 *old);
int xvm_shmem_lf_int_hash_map_compare_exchange(void *map, const void *key, __u32 key_size,
					       __u64 *expected, __u64 desired);
int xvm_shmem_lf_int_hash_map_iter_init(void *map, void *iter_handle);
int xvm_shmem_lf_int_hash_map_next_entry(void *map, void *iter_handle, void *key, __u32 *key_size, __u64 *val);
int xvm_shmem_lf_int_hash_map_clear(void *map);

#ifdef CONFIG_DEBUG_BUILD
void xvm_shmem_lf_hash_map_fuzz(void *map, void *hint_key, __u32 hint_key_size);
#endif

#endif
