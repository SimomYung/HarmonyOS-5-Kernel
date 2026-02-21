/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: libhmpsf xvm std int hash map
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 14 10:56:27 2023
 */

#ifndef ULIBS_LIBHMPSF_XVM_STD_INT_HASH_MAP_H
#define ULIBS_LIBHMPSF_XVM_STD_INT_HASH_MAP_H

#include <libhmpsf/xvm/std/map/map.h>
#include <xvm/xvm_types/std/map/int_hash_map.h>

/*
 * Create globally-shared integer hash map. If an existent one is found, it will be reused.
 * If successful, handle of allocated metadata of the map will be returned through `map`.
 * By default, int hash map is created with shared-memory lock-free (shmem lf) algorithm. The basic
 * algorithm is obsolete.
 *
 * @map: map handle to be returned.
 * @name: map name.
 * @participants: participants flags.
 * @max_key_size: maximum key size of the map.
 * @max_entries: maximum entries of the map.
 * @return: negative errno if any error happens, else module id.
 */
int xvm_int_hash_map_create_global_shared(xvm_map_handle_t *map, const char *name, unsigned short participants,
					  uint32_t max_key_size, uint32_t max_entries);

/*
 * Find a map of a module by the map name.
 * If found, handle of allocated metadata of the map will be returned through `map`.
 * The map must be a private integer hash map.
 */
int xvm_int_hash_map_find_private(xvm_map_handle_t *map, uint32_t module_id, const char *name);

/*
 * Accessors.
 */

/*
 * Clear all entries in the int hash map.
 *
 * @map: xvm map handle.
 * @return: negative errno if any error happens.
 */
int xvm_int_hash_map_clear(xvm_map_handle_t map);

/*
 * Get next key of specified key in int hash map.
 * Supported by basic int hash map. Not supported by shmem lf int hash map.
 *
 * @map: xvm map handle.
 * @key: specified key.
 * @key_size: size of the specified key.
 * @next_key: next key of the specified key.
 * @next_key_size: size of the next key.
 * @return: negative errno if any error happens.
 */
int xvm_int_hash_map_next_key(xvm_map_handle_t map, const void *key, uint32_t key_size,
			      void *next_key, uint32_t *next_key_size);

/*
 * Initialize an iterator.
 * Supported by shmem lf int hash map. Not supported by basic int hash map.
 *
 * @map: xvm map handle.
 * @iter_handle: iterator handle to be returned.
 * @return: negative errno if any error happens.
 */
int xvm_int_hash_map_iter_init(xvm_map_handle_t map, xvm_map_iter_handle_t *iter_handle);

/*
 * Use an iterator to get next entry.
 * Supported by shmem lf int hash map. Not supported by basic int hash map.
 *
 * @map: xvm map handle.
 * @iter_handle: iterator handle.
 * @key: key of the next entry.
 * @key_size: key size of the next entry.
 * @val: value of the next entry.
 * @return: negative errno if any error happens; 0 if next entry is successfully found and returned;
 *          XVM_MAP_ITER_DONE if iteration is finished and next entry is not returned.
 */
int xvm_int_hash_map_next_entry(xvm_map_handle_t map, xvm_map_iter_handle_t *iter_handle,
				void *key, uint32_t *key_size, uint64_t *val);

/*
 * Load value from int hash map with specified key.
 *
 * @map: xvm map handle.
 * @key: key.
 * @key_size: key size.
 * @val: value to be returned.
 * @return: negative errno if any error happens.
 */
int xvm_int_hash_map_load(xvm_map_handle_t map, const void *key, uint32_t key_size, uint64_t *val);

/*
 * Store value to int hash map with specified key.
 *
 * @map: xvm map handle.
 * @key: key.
 * @key_size: key size.
 * @val: value.
 * @return: negative errno if any error happens.
 */
int xvm_int_hash_map_store(xvm_map_handle_t map, const void *key, uint32_t key_size, uint64_t val);

/*
 * Atomically exchange a new value with the old value int the hash map.
 *
 * @map: xvm map handle.
 * @key: key.
 * @key_size: key size.
 * @val: new value.
 * @old: old value to be returned.
 * @return: negative errno if any error happens.
 */
int xvm_int_hash_map_exchange(xvm_map_handle_t map, const void *key, uint32_t key_size,
			      uint64_t val, uint64_t *old);

/*
 * Compare the old value int the hash map with an expected value; if equal, then write the desired
 * value into the map; if not equal, then update the expected value to the current value.
 * This is an atomic operation.
 *
 * @map: xvm map handle.
 * @key: key.
 * @key_size: key size.
 * @expected: expected value passed in, and possibly current value returned.
 * @desired: desired new value.
 * @return: negative errno if any error happens; XVM_INT_HASH_MAP_CMPXCHG_FAILURE if no error
 *          happens but compare-exchange fails; XVM_INT_HASH_MAP_CMPXCHG_SUCCESS if no error
 *          happens and compare-exchange succeeds.
 */
int xvm_int_hash_map_compare_exchange(xvm_map_handle_t map, const void *key, uint32_t key_size,
				      uint64_t *expected, uint64_t desired);

/*
 * Atomically add a delta value to the current value of a specified key, write the new value into
 * the map, and return the new value. This is an atomic operation.
 *
 * @map: xvm map handle.
 * @key: key.
 * @key_size: key size.
 * @val: delta value passed in, and new value returned.
 * @return: negative errno if any error happens.
 */
int xvm_int_hash_map_add_fetch(xvm_map_handle_t map, const void *key, uint32_t key_size, uint64_t *val);

/*
 * Atomically subtract a delta value from the current value of a specified key, write the new value
 * into the map, and return the new value.
 *
 * @map: xvm map handle.
 * @key: key.
 * @key_size: key size.
 * @val: delta value passed in, and new value returned.
 * @return: negative errno if any error happens.
 */
int xvm_int_hash_map_sub_fetch(xvm_map_handle_t map, const void *key, uint32_t key_size, uint64_t *val);

/*
 * Atomically clear all entries in the int hash map.
 *
 * @map: xvm map handle.
 * @return: negative errno if any error happens.
 */
int xvm_int_hash_map_clear(xvm_map_handle_t map);

#endif
