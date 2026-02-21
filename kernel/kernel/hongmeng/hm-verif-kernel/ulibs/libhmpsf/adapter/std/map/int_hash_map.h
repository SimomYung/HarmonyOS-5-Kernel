/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Adapted interfaces for xvm int_hash_map
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 16 16:09:26 2024
 */
#ifndef __ULIBS_LIBHMPSF_XVM_ADAPTER_INT_HASH_MAP_H
#define __ULIBS_LIBHMPSF_XVM_ADAPTER_INT_HASH_MAP_H

#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <libsysif/sysmgr/api.h>
#include <libhmpsf/adapter/lsyscall.h>
#include "../../../xvm/std/map/int_hash_map_lsyscall.h"

static inline int xvm_syscall_int_hash_map_create_global_shared(xvm_int_hash_map_t *int_hash_map)
{
	int err = E_HM_OK;

	if (xvm_is_using_lsyscall()) {
		err = xvm_lsyscall_int_hash_map_create_global_shared(int_hash_map);
	} else {
		err = actvcall_hmcall_hmpsf_int_hash_map_create_global_shared(int_hash_map);
	}
	return err;
}

static inline int xvm_syscall_int_hash_map_load(xvm_int_hash_map_t *int_hash_map, const void *key,
					        uint32_t key_size, unsigned long long *val)
{
	int err = E_HM_OK;

	if (xvm_is_using_lsyscall()) {
		err = xvm_lsyscall_int_hash_map_load(int_hash_map, key, key_size, val);
	} else {
		err = actvcall_hmcall_hmpsf_int_hash_map_load(XVM_MAP_INVALID_MODULE_ID,
							      int_hash_map->hash_map.map.scope,
							      int_hash_map->hash_map.map.id, key, key_size,
							      val);
	}
	return err;
}

static inline int xvm_syscall_int_hash_map_store(xvm_int_hash_map_t *int_hash_map, const void *key,
						 uint32_t key_size, uint64_t val)
{
	int err = E_HM_OK;

	if (xvm_is_using_lsyscall()) {
		err = xvm_lsyscall_int_hash_map_store(int_hash_map, key, key_size, val);
	} else {
		err = actvcall_hmcall_hmpsf_int_hash_map_store(XVM_MAP_INVALID_MODULE_ID,
							       int_hash_map->hash_map.map.scope,
							       int_hash_map->hash_map.map.id, key,
							       key_size, val);
	}
	return err;
}

static inline int xvm_syscall_int_hash_map_next_key(xvm_int_hash_map_t *int_hash_map, const void *key,
						    uint32_t key_size, void *next_key, uint32_t *next_key_size)
{
	int err = E_HM_OK;

	if (xvm_is_using_lsyscall()) {
		err = xvm_lsyscall_int_hash_map_next_key(int_hash_map, key, key_size, next_key, next_key_size);
	} else {
		err = actvcall_hmcall_hmpsf_int_hash_map_next_key(XVM_MAP_INVALID_MODULE_ID,
								  int_hash_map->hash_map.map.scope,
								  int_hash_map->hash_map.map.id,
								  key, key_size, next_key, next_key_size);
	}
	return err;
}

static inline int xvm_syscall_int_hash_map_exchange(xvm_int_hash_map_t *int_hash_map, const void *key,
						    uint32_t key_size, uint64_t val, unsigned long long *old)
{
	int err = E_HM_OK;

	if (xvm_is_using_lsyscall()) {
		err = xvm_lsyscall_int_hash_map_exchange(int_hash_map, key, key_size, val, old);
	} else {
		err = actvcall_hmcall_hmpsf_int_hash_map_exchange(XVM_MAP_INVALID_MODULE_ID,
								  int_hash_map->hash_map.map.scope,
								  int_hash_map->hash_map.map.id,
								  key, key_size, val, old);
	}
	return err;
}

static inline int xvm_syscall_int_hash_map_compare_exchange(xvm_int_hash_map_t *int_hash_map, const void *key,
							    uint32_t key_size, unsigned long long *expected,
							    const uint64_t desired)
{
	int ret = E_HM_OK;

	if (xvm_is_using_lsyscall()) {
		ret = xvm_lsyscall_int_hash_map_compare_exchange(int_hash_map, key, key_size, expected, desired);
	} else {
		ret = actvcall_hmcall_hmpsf_int_hash_map_compare_exchange(XVM_MAP_INVALID_MODULE_ID,
									  int_hash_map->hash_map.map.scope,
									  int_hash_map->hash_map.map.id,
									  key, key_size, expected, desired);
	}
	return ret;
}

static inline int xvm_syscall_int_hash_map_add_fetch(xvm_int_hash_map_t *int_hash_map, const void *key,
						     uint32_t key_size, unsigned long long *val)
{
	int err = E_HM_OK;

	if (xvm_is_using_lsyscall()) {
		err = xvm_lsyscall_int_hash_map_add_fetch(int_hash_map, key, key_size, val);
	} else {
		err = actvcall_hmcall_hmpsf_int_hash_map_add_fetch(XVM_MAP_INVALID_MODULE_ID,
								   int_hash_map->hash_map.map.scope,
								   int_hash_map->hash_map.map.id,
								   key, key_size, val);
	}
	return err;
}

static inline int xvm_syscall_int_hash_map_sub_fetch(xvm_int_hash_map_t *int_hash_map, const void *key,
						     uint32_t key_size, unsigned long long *val)
{
	int err = E_HM_OK;

	if (xvm_is_using_lsyscall()) {
		err = xvm_lsyscall_int_hash_map_sub_fetch(int_hash_map, key, key_size, val);
	} else {
		err = actvcall_hmcall_hmpsf_int_hash_map_sub_fetch(XVM_MAP_INVALID_MODULE_ID,
								   int_hash_map->hash_map.map.scope,
								   int_hash_map->hash_map.map.id,
								   key, key_size, (unsigned long long *)val);
	}
	return err;
}

static inline int xvm_syscall_int_hash_map_clear(xvm_int_hash_map_t *int_hash_map)
{
	int err = E_HM_OK;
	if (xvm_is_using_lsyscall()) {
		err = xvm_lsyscall_int_hash_map_clear(int_hash_map);
	} else {
		err = actvcall_hmcall_hmpsf_int_hash_map_clear(XVM_MAP_INVALID_MODULE_ID,
							       int_hash_map->hash_map.map.scope,
							       int_hash_map->hash_map.map.id);
	}
	return err;
}
#endif
