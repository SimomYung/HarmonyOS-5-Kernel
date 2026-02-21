/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Interfaces for xvm lsyscall bytes_hash_map
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 16 15:56:53 2024
 */
#ifndef __ULIBS_LIBHMPSF_XVM_STD_BYTES_HASH_MAP_LSYSCALL_H
#define __ULIBS_LIBHMPSF_XVM_STD_BYTES_HASH_MAP_LSYSCALL_H

#include <hongmeng/types.h>
#include <libhmpsf/xvm/std/map/map.h>
#include <libhmpsf/xvm/lsyscall.h>
#include "sc_map_base.h"
#include "../../../adapter/lsyscall.h"

static inline int xvm_lsyscall_bytes_hash_map_create_global_shared(xvm_bytes_hash_map_t *bytes_hash_map)
{
	return xvm_lsyscall(XVM_SC_CMD_BYTES_HASH_MAP_CREATE_GS, ptr_to_ulong(bytes_hash_map));
}

static inline int xvm_lsyscall_bytes_hash_map_set(xvm_bytes_hash_map_t *bytes_hash_map, const void *key,
						  uint32_t key_size, const void *val, uint32_t val_size)
{
	struct xvm_sc_map_base_args_s base;
	xvm_sc_map_base_init(&base, &bytes_hash_map->hash_map.map, key, key_size);
	struct xvm_sc_bytes_hash_map_set_args_s args = {
		.base = base,
		.val = val,
		.val_size = val_size,
	};
	return xvm_lsyscall(XVM_SC_CMD_BYTES_HASH_MAP_SET, (unsigned long)&args);
}

static inline int xvm_lsyscall_bytes_hash_map_get(xvm_bytes_hash_map_t *bytes_hash_map, const void *key,
						  uint32_t key_size, void *val, uint32_t *val_size)
{
	struct xvm_sc_map_base_args_s base;
	xvm_sc_map_base_init(&base, &bytes_hash_map->hash_map.map, key, key_size);
	struct xvm_sc_bytes_hash_map_get_args_s args = {
		.base = base,
		.val = val,
		.val_size = val_size,
	};
	return xvm_lsyscall(XVM_SC_CMD_BYTES_HASH_MAP_GET, (unsigned long)&args);
}

static inline int xvm_lsyscall_bytes_hash_map_next_key(xvm_bytes_hash_map_t *bytes_hash_map, const void *key,
						       uint32_t key_size, void *next_key, uint32_t *next_key_size)
{
	struct xvm_sc_map_base_args_s base;
	xvm_sc_map_base_init(&base, &bytes_hash_map->hash_map.map, key, key_size);
	struct xvm_sc_bytes_hash_map_next_key_args_s args = {
		.base = base,
		.next_key = next_key,
		.next_key_size = next_key_size,
	};
	return xvm_lsyscall(XVM_SC_CMD_BYTES_HASH_MAP_NEXT_KEY, (unsigned long)&args);
}

static inline int xvm_lsyscall_bytes_hash_map_clear(xvm_bytes_hash_map_t *bytes_hash_map)
{
	struct xvm_sc_bytes_hash_map_clear_args_s args = {
		.module_id = XVM_MAP_INVALID_MODULE_ID,
		.scope = bytes_hash_map->hash_map.map.scope,
		.map_id = bytes_hash_map->hash_map.map.id,
	};
	return xvm_lsyscall(XVM_SC_CMD_BYTES_HASH_MAP_CLEAR, (unsigned long)&args);
}
#endif
