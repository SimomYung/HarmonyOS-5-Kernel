/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Interfaces for xvm lsyscall int_hash_map
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 16 15:21:52 2024
 */

#ifndef __ULIBS_LIBHMPSF_XVM_STD_INT_HASH_MAP_LSYSCALL_H
#define __ULIBS_LIBHMPSF_XVM_STD_INT_HASH_MAP_LSYSCALL_H

#include <hongmeng/types.h>
#include <libhmpsf/xvm/std/map/map.h>
#include <libhmpsf/xvm/lsyscall.h>
#include "sc_map_base.h"
#include "../../../adapter/lsyscall.h"

static inline int xvm_lsyscall_int_hash_map_create_global_shared(xvm_int_hash_map_t *int_hash_map)
{
	return xvm_lsyscall(XVM_SC_CMD_INT_HASH_MAP_CREATE_GS, ptr_to_ulong(int_hash_map));
}

static inline int xvm_lsyscall_int_hash_map_load(xvm_int_hash_map_t *int_hash_map, const void *key,
						 uint32_t key_size, unsigned long long *val)
{
	struct xvm_sc_map_base_args_s base;
	xvm_sc_map_base_init(&base, &int_hash_map->hash_map.map, key, key_size);
	struct xvm_sc_int_hash_map_load_args_s args = {
		.base = base,
		.val = val,
	};
	return xvm_lsyscall(XVM_SC_CMD_INT_HASH_MAP_LOAD, (unsigned long)&args);
}

static inline int xvm_lsyscall_int_hash_map_store(xvm_int_hash_map_t *int_hash_map, const void *key,
						  uint32_t key_size, unsigned long long val)
{
	struct xvm_sc_map_base_args_s base;
	xvm_sc_map_base_init(&base, &int_hash_map->hash_map.map, key, key_size);
	struct xvm_sc_int_hash_map_store_args_s args = {
		.base = base,
		.val = val,
	};
	return xvm_lsyscall(XVM_SC_CMD_INT_HASH_MAP_STORE, (unsigned long)&args);
}

static inline int xvm_lsyscall_int_hash_map_next_key(xvm_int_hash_map_t *int_hash_map, const void *key,
						     uint32_t key_size, void *next_key, uint32_t *next_key_size)
{
	struct xvm_sc_map_base_args_s base;
	xvm_sc_map_base_init(&base, &int_hash_map->hash_map.map, key, key_size);
	struct xvm_sc_int_hash_map_next_key_args_s args = {
		.base = base,
		.next_key = next_key,
		.next_key_size = next_key_size,
	};
	return xvm_lsyscall(XVM_SC_CMD_INT_HASH_MAP_NEXT_KEY, (unsigned long)&args);
}

static inline int xvm_lsyscall_int_hash_map_exchange(xvm_int_hash_map_t *int_hash_map, const void *key,
						     uint32_t key_size, uint64_t val, unsigned long long *old)
{
	struct xvm_sc_map_base_args_s base;
	xvm_sc_map_base_init(&base, &int_hash_map->hash_map.map, key, key_size);
	struct xvm_sc_int_hash_map_exchange_args_s args = {
		.base = base,
		.val = val,
		.old = old,
	};
	return xvm_lsyscall(XVM_SC_CMD_INT_HASH_MAP_EXCHANGE, (unsigned long)&args);
}

static inline int xvm_lsyscall_int_hash_map_compare_exchange(xvm_int_hash_map_t *int_hash_map, const void *key,
							     uint32_t key_size, unsigned long long *expected,
							     const uint64_t desired)
{
	struct xvm_sc_map_base_args_s base;
	xvm_sc_map_base_init(&base, &int_hash_map->hash_map.map, key, key_size);
	struct xvm_sc_int_hash_map_compare_exchange_args_s args = {
		.base = base,
		.expected = expected,
		.desired = desired,
	};
	return xvm_lsyscall(XVM_SC_CMD_INT_HASH_MAP_COMPARE_EXCHANGE, (unsigned long)&args);
}

static inline int xvm_lsyscall_int_hash_map_add_fetch(xvm_int_hash_map_t *int_hash_map, const void *key,
						      uint32_t key_size, unsigned long long *val)
{
	struct xvm_sc_map_base_args_s base;
	xvm_sc_map_base_init(&base, &int_hash_map->hash_map.map, key, key_size);
	struct xvm_sc_int_hash_map_add_fetch_args_s args = {
		.base = base,
		.val = val,
	};
	return xvm_lsyscall(XVM_SC_CMD_INT_HASH_MAP_ADD_FETCH, (unsigned long)&args);
}

static inline int xvm_lsyscall_int_hash_map_sub_fetch(xvm_int_hash_map_t *int_hash_map, const void *key,
						      uint32_t key_size, unsigned long long *val)
{
	struct xvm_sc_map_base_args_s base;
	xvm_sc_map_base_init(&base, &int_hash_map->hash_map.map, key, key_size);
	struct xvm_sc_int_hash_map_sub_fetch_args_s args = {
		.base = base,
		.val = val,
	};
	return xvm_lsyscall(XVM_SC_CMD_INT_HASH_MAP_SUB_FETCH, (unsigned long)&args);
}

static inline int xvm_lsyscall_int_hash_map_clear(xvm_int_hash_map_t *int_hash_map)
{
	struct xvm_sc_int_hash_map_clear_args_s args = {
		.module_id = XVM_MAP_INVALID_MODULE_ID,
		.scope = int_hash_map->hash_map.map.scope,
		.map_id = int_hash_map->hash_map.map.id,
	};
	return xvm_lsyscall(XVM_SC_CMD_INT_HASH_MAP_CLEAR, (unsigned long)&args);
}
#endif
