/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: libhmpsf xvm std bytes map
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 14 10:56:27 2023
 */

#ifndef ULIBS_LIBHMPSF_XVM_STD_BYTES_MAP_H
#define ULIBS_LIBHMPSF_XVM_STD_BYTES_MAP_H

#include <xvm/xvm_types/std/map/map.h>
#include <stdbool.h>
#include <stdint.h>
#include <hmkernel/errno.h>

extern unsigned char g_xvm_map_alg;

/*
 * Deprecated.
 * The metadata pointed to by map handle is freed by xvm_map_release_global_shared.
 */
void xvm_map_free_handle(xvm_map_handle_t map);

/*
 * Release the reference to a globally-shared map.
 * Note that the creation of globally-shared map is type-specific.
 */
int xvm_map_release_global_shared(xvm_map_handle_t map);

/*
 * Find a map of a module by the map name.
 * If found, handle of allocated metadata of the map will be returned through `map`.
 * The map can be of any map type and any map scope.
 */
int xvm_map_find_map(xvm_map_handle_t map, unsigned int module_id, const char *name);

/*
 * Find the next map of a given map.
 * This function can be used to iterate all maps of a module, including private maps
 * and shared maps.
 */
int xvm_module_next_map(unsigned int module_id, const xvm_map_handle_t map, xvm_map_handle_t *next);

#endif
