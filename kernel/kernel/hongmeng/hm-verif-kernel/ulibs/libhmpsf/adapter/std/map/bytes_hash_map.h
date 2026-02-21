/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Adapted interfaces for xvm bytes_hash_map
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 16 16:09:26 2024
 */
#ifndef __ULIBS_LIBHMPSF_XVM_ADAPTER_BYTES_HASH_MAP_H
#define __ULIBS_LIBHMPSF_XVM_ADAPTER_BYTES_HASH_MAP_H

#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <libsysif/sysmgr/api.h>
#include <libhmpsf/adapter/lsyscall.h>
#include "../../../xvm/std/map/bytes_hash_map_lsyscall.h"

static inline int xvm_syscall_bytes_hash_map_create_global_shared(xvm_bytes_hash_map_t *bytes_hash_map)
{
	int err = E_HM_OK;

	if (xvm_is_using_lsyscall()) {
		err = xvm_lsyscall_bytes_hash_map_create_global_shared(bytes_hash_map);
	} else {
		err = actvcall_hmcall_hmpsf_bytes_hash_map_create_global_shared(bytes_hash_map);
	}
	return err;
}

static inline int xvm_syscall_bytes_hash_map_set(xvm_bytes_hash_map_t *bytes_hash_map, const void *key,
						 uint32_t key_size, const void *val, uint32_t val_size)
{
	int err = E_HM_OK;

	if (xvm_is_using_lsyscall()) {
		err = xvm_lsyscall_bytes_hash_map_set(bytes_hash_map, key, key_size, val, val_size);
	} else {
		err = actvcall_hmcall_hmpsf_bytes_hash_map_set(XVM_MAP_INVALID_MODULE_ID,
							       bytes_hash_map->hash_map.map.scope,
							       bytes_hash_map->hash_map.map.id,
							       key, key_size, val, val_size);
	}
	return err;
}

static inline int xvm_syscall_bytes_hash_map_get(xvm_bytes_hash_map_t *bytes_hash_map, const void *key,
						 uint32_t key_size, void *val, uint32_t *val_size)
{
	int err = E_HM_OK;

	if (xvm_is_using_lsyscall()) {
		err = xvm_lsyscall_bytes_hash_map_get(bytes_hash_map, key, key_size, val, val_size);
	} else {
		err = actvcall_hmcall_hmpsf_bytes_hash_map_get(XVM_MAP_INVALID_MODULE_ID,
							       bytes_hash_map->hash_map.map.scope,
							       bytes_hash_map->hash_map.map.id,
							       key, key_size, val, val_size);
	}
	return err;
}

static inline int xvm_syscall_bytes_hash_map_next_key(xvm_bytes_hash_map_t *bytes_hash_map, const void *key,
						      uint32_t key_size, void *next_key, uint32_t *next_key_size)
{
	int err = E_HM_OK;

	if (xvm_is_using_lsyscall()) {
		err = xvm_lsyscall_bytes_hash_map_next_key(bytes_hash_map, key, key_size, next_key, next_key_size);
	} else {
		err = actvcall_hmcall_hmpsf_bytes_hash_map_next_key(XVM_MAP_INVALID_MODULE_ID,
								    bytes_hash_map->hash_map.map.scope,
								    bytes_hash_map->hash_map.map.id, key,
								    key_size, next_key, next_key_size);
	}
	return err;
}

static inline int xvm_syscall_bytes_hash_map_clear(xvm_bytes_hash_map_t *bytes_hash_map)
{
	int err = E_HM_OK;

	if (xvm_is_using_lsyscall()) {
		err = xvm_lsyscall_bytes_hash_map_clear(bytes_hash_map);
	} else {
		err = actvcall_hmcall_hmpsf_bytes_hash_map_clear(XVM_MAP_INVALID_MODULE_ID,
								 bytes_hash_map->hash_map.map.scope,
								 bytes_hash_map->hash_map.map.id);
	}
	return err;
}
#endif
