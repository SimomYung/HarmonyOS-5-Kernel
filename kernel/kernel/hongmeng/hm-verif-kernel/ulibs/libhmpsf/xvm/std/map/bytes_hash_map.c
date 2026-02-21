/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Provide xvm map API for xvm clients
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 23 11:41:30 2023
 */

#include <hongmeng/errno.h>
#include <libhmpsf/xvm/std/map/bytes_hash_map.h>
#include <libhwsecurec/securec.h>
#include <lib/hmpsf/hmxvm_shmem_lf_hash_map.h>
#include <malloc.h>
#include <string.h>
#include <xvm/hmxvm.h>
#include "../../../adapter/std/map/bytes_hash_map.h"

int xvm_bytes_hash_map_create_global_shared(xvm_map_handle_t *map, const char *name, unsigned short participants,
					    uint32_t max_key_size, uint32_t max_val_size, uint32_t max_entries)
{
	int err = E_HM_OK;
	xvm_bytes_hash_map_t *bytes_hash_map = malloc(sizeof(*bytes_hash_map));

	if (bytes_hash_map == NULL) {
		err = E_HM_NOMEM;
	}

	if (err == E_HM_OK) {
		NOFAIL(memset_s(bytes_hash_map, sizeof(*bytes_hash_map), 0, sizeof(*bytes_hash_map)));
		bytes_hash_map->hash_map.map.type = XVM_MAP_TYPE_BYTES_HASH_MAP;
		bytes_hash_map->hash_map.map.scope = XVM_MAP_SCOPE_GLOBAL_SHARED;
		bytes_hash_map->hash_map.map.participants = participants;
		bytes_hash_map->hash_map.map.algorithm = g_xvm_map_alg;

		if (memcpy_s(bytes_hash_map->hash_map.map.name, XVM_MAP_MAX_NAME_LEN, name, strlen(name)) != 0) {
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		bytes_hash_map->hash_map.max_key_size = max_key_size;
		bytes_hash_map->hash_map.max_val_size = max_val_size;
		bytes_hash_map->hash_map.max_entries = max_entries;
		err = xvm_syscall_bytes_hash_map_create_global_shared(bytes_hash_map);
	}

	if (err == E_HM_OK) {
		if (bytes_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_SHMEM_LF) {
			void *lock_buf = NULL;
			lock_buf = calloc(1, XVM_HASH_MAP_LOCK_BUF_SIZE);
			/* lock_buf is optional. So, it is ok even if the allocation failed.  */
			bytes_hash_map->hash_map.lock_buf = (uintptr_t)lock_buf;
			/* Initialize lock buffer. */
			NOFAIL(xvm_shmem_lf_bytes_hash_map_init(bytes_hash_map,
								(void *)bytes_hash_map->hash_map.buf,
								bytes_hash_map->hash_map.buf_size,
								(void *)bytes_hash_map->hash_map.lock_buf, false));
		}

		*map = bytes_hash_map;
	}

	if (err != 0 && bytes_hash_map != NULL) {
		free(bytes_hash_map);
	}

	return err;
}

int xvm_bytes_hash_map_set(xvm_map_handle_t map, const void *key, uint32_t key_size, const void *val,
			   uint32_t val_size)
{
	int err;
	xvm_bytes_hash_map_t *bytes_hash_map = (xvm_bytes_hash_map_t *)map;

	if (bytes_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_BASIC) {
		err = xvm_syscall_bytes_hash_map_set(bytes_hash_map, key, key_size, val, val_size);
	} else {
		/* XVM_MAP_ALG_SHMEM_LF */
		err = hmxvm_shmem_lf_bytes_hash_map_set(bytes_hash_map, key, key_size, val, val_size);
	}

	return err;
}

int xvm_bytes_hash_map_get(xvm_map_handle_t map, const void *key, uint32_t key_size, void *val,
			   uint32_t *val_size)
{
	int err;
	xvm_bytes_hash_map_t *bytes_hash_map = (xvm_bytes_hash_map_t *)map;

	if (bytes_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_BASIC) {
		err = xvm_syscall_bytes_hash_map_get(bytes_hash_map, key, key_size, val, val_size);
	} else {
		/* XVM_MAP_ALG_SHMEM_LF */
		err = hmxvm_shmem_lf_bytes_hash_map_get(bytes_hash_map, key, key_size, val, val_size);
	}

	return err;
}

int xvm_bytes_hash_map_next_key(xvm_map_handle_t map, const void *key, uint32_t key_size,
				void *next_key, uint32_t *next_key_size)
{
	int err;
	xvm_bytes_hash_map_t *bytes_hash_map = (xvm_bytes_hash_map_t *)map;

	if (bytes_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_BASIC) {
		err = xvm_syscall_bytes_hash_map_next_key(bytes_hash_map, key, key_size,
							  next_key, next_key_size);
	} else {
		err = E_HM_NOSYS;
	}

	return err;
}

int xvm_bytes_hash_map_iter_init(xvm_map_handle_t map, xvm_map_iter_handle_t *iter_handle)
{
	int err = E_HM_OK;
	xvm_bytes_hash_map_t *bytes_hash_map = (xvm_bytes_hash_map_t *)map;
	if (bytes_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_BASIC) {
		err = E_HM_INVAL;
	} else {
		/* XVM_MAP_ALG_SHMEM_LF */
		err = hmxvm_shmem_lf_bytes_hash_map_iter_init(bytes_hash_map, iter_handle);
	}
	return err;
}

int xvm_bytes_hash_map_next_entry(xvm_map_handle_t map, xvm_map_iter_handle_t *iter_handle,
				  void *key, uint32_t *key_size, void *val, uint32_t *val_size)
{
	int err = E_HM_OK;
	xvm_bytes_hash_map_t *bytes_hash_map = (xvm_bytes_hash_map_t *)map;
	if (bytes_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_BASIC) {
		err = E_HM_INVAL;
	} else {
		/* XVM_MAP_ALG_SHMEM_LF */
		err = hmxvm_shmem_lf_bytes_hash_map_next_entry(bytes_hash_map, iter_handle,
							     key, key_size, val, val_size);
	}
	return err;
}

int xvm_bytes_hash_map_clear(xvm_map_handle_t map)
{
	int err;
	xvm_bytes_hash_map_t *bytes_hash_map = (xvm_bytes_hash_map_t *)map;

	if (bytes_hash_map->hash_map.map.algorithm == XVM_MAP_ALG_BASIC) {
		err = xvm_syscall_bytes_hash_map_clear(bytes_hash_map);
	} else {
		/* XVM_MAP_ALG_SHMEM_LF */
		err = hmxvm_shmem_lf_bytes_hash_map_clear(bytes_hash_map);
	}

	return err;
}
