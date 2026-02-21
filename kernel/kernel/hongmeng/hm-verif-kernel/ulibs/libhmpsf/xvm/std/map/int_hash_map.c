/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Provide xvm map API for xvm clients
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 23 11:41:30 2023
 */
#include <hongmeng/errno.h>
#include <libhmpsf/xvm/std/map/int_hash_map.h>
#include <lib/hmpsf/hmxvm_shmem_lf_hash_map.h>
#include <libhwsecurec/securec.h>
#include <malloc.h>
#include <string.h>
#include <xvm/hmxvm.h>
#include "../../../adapter/std/map/int_hash_map.h"

/*
 * The shmem lf hash map backend defaults to use piority XVM_HASH_MAP_PRIORITY_MAX. So, client uapps
 * do not need to initialize the priority.
 */

int xvm_int_hash_map_create_global_shared(xvm_map_handle_t *map, const char *name, unsigned short participants,
					  uint32_t max_key_size, uint32_t max_entries)
{
	int err = E_HM_OK;
	xvm_int_hash_map_t *int_hash_map = malloc(sizeof(*int_hash_map));

	if (int_hash_map == NULL) {
		err = E_HM_NOMEM;
	}

	if (err == E_HM_OK) {
		NOFAIL(memset_s(int_hash_map, sizeof(*int_hash_map), 0, sizeof(*int_hash_map)));
		int_hash_map->hash_map.map.type = XVM_MAP_TYPE_INT_HASH_MAP;
		int_hash_map->hash_map.map.scope = XVM_MAP_SCOPE_GLOBAL_SHARED;
		int_hash_map->hash_map.map.participants = participants;
		int_hash_map->hash_map.map.algorithm = g_xvm_map_alg;

		if (memcpy_s(int_hash_map->hash_map.map.name, XVM_MAP_MAX_NAME_LEN, name, strlen(name)) != 0) {
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		int_hash_map->hash_map.max_key_size = max_key_size;
		int_hash_map->hash_map.max_val_size = sizeof(uint64_t);
		int_hash_map->hash_map.max_entries = max_entries;
		err = xvm_syscall_int_hash_map_create_global_shared(int_hash_map);
	}

	if (err == E_HM_OK) {
		if (int_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_SHMEM_LF) {
			void *lock_buf = NULL;
			lock_buf = calloc(1, XVM_HASH_MAP_LOCK_BUF_SIZE);
			/* lock_buf is optional. So, it is ok even if the allocation failed.  */
			int_hash_map->hash_map.lock_buf = (uintptr_t)lock_buf;
			/* Initialize lock buffer. */
			NOFAIL(xvm_shmem_lf_int_hash_map_init(int_hash_map,
							      (void *)int_hash_map->hash_map.buf,
							      int_hash_map->hash_map.buf_size,
							      (void *)int_hash_map->hash_map.lock_buf, false));
		}

		*map = int_hash_map;
	}

	if (err != 0 && int_hash_map != NULL) {
		free(int_hash_map);
	}

	return err;
}

int xvm_int_hash_map_load(xvm_map_handle_t map, const void *key, uint32_t key_size, uint64_t *val)
{
	int err = E_HM_OK;
	xvm_int_hash_map_t *int_hash_map = (xvm_int_hash_map_t *)map;

	if (int_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_BASIC) {
		err = xvm_syscall_int_hash_map_load(int_hash_map, key, key_size, (unsigned long long *)val);
	} else {
		/* XVM_MAP_ALG_SHMEM_LF */
		err = hmxvm_shmem_lf_int_hash_map_load(int_hash_map, key, key_size, (__u64 *)val);
	}

	return err;
}

int xvm_int_hash_map_store(xvm_map_handle_t map, const void *key, uint32_t key_size, uint64_t val)
{
	int err = E_HM_OK;
	xvm_int_hash_map_t *int_hash_map = (xvm_int_hash_map_t *)map;

	if (int_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_BASIC) {
		err = xvm_syscall_int_hash_map_store(int_hash_map, key, key_size, val);
	} else {
		/* XVM_MAP_ALG_SHMEM_LF */
		err = hmxvm_shmem_lf_int_hash_map_store(int_hash_map, key, key_size, val);
	}

	return err;
}

int xvm_int_hash_map_next_key(xvm_map_handle_t map, const void *key, uint32_t key_size,
			      void *next_key, uint32_t *next_key_size)
{
	int err;
	xvm_int_hash_map_t *int_hash_map = (xvm_int_hash_map_t *)map;

	if (int_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_BASIC) {
		err = xvm_syscall_int_hash_map_next_key(int_hash_map, key, key_size, next_key, next_key_size);
	} else {
		err = E_HM_NOSYS;
	}

	return err;
}

int xvm_int_hash_map_exchange(xvm_map_handle_t map, const void *key, uint32_t key_size, uint64_t val, uint64_t *old)
{
	int err;
	xvm_int_hash_map_t *int_hash_map = (xvm_int_hash_map_t *)map;

	if (int_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_BASIC) {
		err = xvm_syscall_int_hash_map_exchange(int_hash_map, key, key_size, val, (unsigned long long *)old);
	} else {
		/* XVM_MAP_ALG_SHMEM_LF */
		err = hmxvm_shmem_lf_int_hash_map_exchange(int_hash_map, key, key_size, val, (__u64 *)old);
	}

	return err;
}

int xvm_int_hash_map_compare_exchange(xvm_map_handle_t map, const void *key, uint32_t key_size,
				      uint64_t *expected, const uint64_t desired)
{
	int ret;
	xvm_int_hash_map_t *int_hash_map = (xvm_int_hash_map_t *)map;

	if (int_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_BASIC) {
		ret = xvm_syscall_int_hash_map_compare_exchange(int_hash_map, key, key_size,
							       (unsigned long long *)expected, desired);
	} else {
		/* XVM_MAP_ALG_SHMEM_LF */
		ret = hmxvm_shmem_lf_int_hash_map_compare_exchange(int_hash_map, key, key_size,
								   (__u64 *)expected, desired);
	}

	return ret;
}

int xvm_int_hash_map_add_fetch(xvm_map_handle_t map, const void *key, uint32_t key_size, uint64_t *val)
{
	int err;
	xvm_int_hash_map_t *int_hash_map = (xvm_int_hash_map_t *)map;

	if (int_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_BASIC) {
		err = xvm_syscall_int_hash_map_add_fetch(int_hash_map, key, key_size, (unsigned long long *)val);
	} else {
		/* XVM_MAP_ALG_SHMEM_LF */
		err = hmxvm_shmem_lf_int_hash_map_add_fetch(int_hash_map, key, key_size, (__u64 *)val);
	}

	return err;
}

int xvm_int_hash_map_sub_fetch(xvm_map_handle_t map, const void *key, uint32_t key_size, uint64_t *val)
{
	int err;
	xvm_int_hash_map_t *int_hash_map = (xvm_int_hash_map_t *)map;

	if (int_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_BASIC) {
		err = xvm_syscall_int_hash_map_sub_fetch(int_hash_map, key, key_size, (unsigned long long *)val);
	} else {
		/* XVM_MAP_ALG_SHMEM_LF */
		err = hmxvm_shmem_lf_int_hash_map_sub_fetch(int_hash_map, key, key_size, (__u64 *)val);
	}

	return err;
}

int xvm_int_hash_map_iter_init(xvm_map_handle_t map, xvm_map_iter_handle_t *iter_handle)
{
	int err = E_HM_OK;
	xvm_int_hash_map_t *int_hash_map = (xvm_int_hash_map_t *)map;
	if (int_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_BASIC) {
		err = E_HM_INVAL;
	} else {
		/* XVM_MAP_ALG_SHMEM_LF */
		err = hmxvm_shmem_lf_int_hash_map_iter_init(int_hash_map, iter_handle);
	}
	return err;
}

int xvm_int_hash_map_next_entry(xvm_map_handle_t map, xvm_map_iter_handle_t *iter_handle,
				void *key, uint32_t *key_size, uint64_t *val)
{
	int err = E_HM_OK;
	xvm_int_hash_map_t *int_hash_map = (xvm_int_hash_map_t *)map;

	if (int_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_BASIC) {
		err = E_HM_INVAL;
	} else {
		/* XVM_MAP_ALG_SHMEM_LF */
		err = hmxvm_shmem_lf_int_hash_map_next_entry(int_hash_map, iter_handle, key, key_size, (__u64 *)val);
	}
	return err;
}

int xvm_int_hash_map_clear(xvm_map_handle_t map)
{
	int err;
	xvm_int_hash_map_t *int_hash_map = (xvm_int_hash_map_t *)map;

	if (int_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_BASIC) {
		err = xvm_syscall_int_hash_map_clear(int_hash_map);
	} else {
		/* XVM_MAP_ALG_SHMEM_LF */
		err = hmxvm_shmem_lf_int_hash_map_clear(int_hash_map);
	}

	return err;
}
