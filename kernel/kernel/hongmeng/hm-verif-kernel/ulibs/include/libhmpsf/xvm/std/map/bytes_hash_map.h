/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: libhmpsf xvm std bytes hash map
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 14 10:56:27 2023
 */

#ifndef ULIBS_LIBHMPSF_XVM_STD_BYTES_HASH_MAP_H
#define ULIBS_LIBHMPSF_XVM_STD_BYTES_HASH_MAP_H

#include <libhmpsf/xvm/std/map/map.h>
#include <xvm/xvm_types/std/map/bytes_hash_map.h>

/*
 * Create globally-shared bytes hash map. If an existent one is found, it will be reused.
 * If successful, handle of allocated metadata of the map will be returned through `map`.
 * By default, bytes hash map is created with shared-memory lock-free (shmem lf) algorithm. The
 * basic algorithm is obsolete.
 *
 * @map: map handle to be returned.
 * @name: map name.
 * @participants: participants flags.
 * @max_key_size: maximum key size of the map.
 * @max_val_size: maximum value size of the map.
 * @max_entries: maximum entries of the map.
 * @return: negative errno if any error happens, else module id.
 */
int xvm_bytes_hash_map_create_global_shared(xvm_map_handle_t *map, const char *name, unsigned short participants,
					    uint32_t max_key_size, uint32_t max_val_size,
					    uint32_t max_entries);

/*
 * Find a private map of a module by the map name.
 * If found, handle of allocated metadata of the map will be returned through `map`.
 * The map must be a private bytes hash map.
 */
int xvm_bytes_hash_map_find_private(xvm_map_handle_t *map, uint32_t module_id, const char *name);

/*
 * Accessors.
 */

/*
 * Clear all entries in the bytes hash map.
 *
 * @map: xvm map handle.
 * @return: negative errno if any error happens.
 */
int xvm_bytes_hash_map_clear(xvm_map_handle_t map);

/*
 * Get next key of specified key in bytes hash map.
 * Supported by basic bytes hash map. Not supported by shmem lf bytes hash map.
 *
 * @map: xvm map handle.
 * @key: specified key.
 * @key_size: size of the specified key.
 * @next_key: next key of the specified key.
 * @next_key_size: size of the next key.
 * @return: negative errno if any error happens.
 */
int xvm_bytes_hash_map_next_key(xvm_map_handle_t map, const void *key, uint32_t key_size,
				void *next_key, uint32_t *next_key_size);

/*
 * Initialize an iterator.
 * Supported by shmem lf bytes hash map. Not supported by basic bytes hash map.
 *
 * @map: xvm map handle.
 * @iter_handle: iterator handle to be returned.
 * @return: negative errno if any error happens.
 */
int xvm_bytes_hash_map_iter_init(xvm_map_handle_t map, xvm_map_iter_handle_t *iter_handle);

/*
 * Use an iterator to get next entry.
 * Supported by shmem lf bytes hash map. Not supported by basic bytes hash map.
 *
 * @map: xvm map handle.
 * @iter_handle: iterator handle.
 * @key: key of the next entry.
 * @key_size: key size of the next entry.
 * @val: value of the next entry.
 * @return: negative errno if any error happens; 0 if next entry is successfully found and returned;
 *          XVM_MAP_ITER_DONE if iteration is finished and next entry is not returned.
 */
int xvm_bytes_hash_map_next_entry(xvm_map_handle_t map, xvm_map_iter_handle_t *iter_handle,
				  void *key, uint32_t *key_size,
				  void *val, uint32_t *val_size);

/*
 * Get value from bytes hash map with specified key.
 *
 * @map: xvm map handle.
 * @key: key.
 * @key_size: key size.
 * @val: value to be returned.
 * @val_size: value size to be returned.
 * @return: negative errno if any error happens.
 */
int xvm_bytes_hash_map_get(xvm_map_handle_t map, const void *key, uint32_t key_size,
			   void *val, uint32_t *val_size);

/*
 * Set value to bytes hash map with specified key.
 *
 * @map: xvm map handle.
 * @key: key.
 * @key_size: key size.
 * @val: value.
 * @val_size: value size.
 * @return: negative errno if any error happens.
 */
int xvm_bytes_hash_map_set(xvm_map_handle_t map, const void *key, uint32_t key_size,
			   const void *val, uint32_t val_size);

/*
 * Atomically clear all entries in the bytes hash map.
 *
 * @map: xvm map handle.
 * @return: negative errno if any error happens.
 */
int xvm_bytes_hash_map_clear(xvm_map_handle_t map);

#endif
