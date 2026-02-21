/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: headers for hmpsf map syscalls
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 02 15:54:32 2024
 */

#ifndef ULIBS_LIBHMPSF_XVM_SC_MAP_BASE_H
#define ULIBS_LIBHMPSF_XVM_SC_MAP_BASE_H

#include <stdint.h>
#include <libhmpsf/xvm/lsyscall.h>
#include <xvm/xvm_types/std/map/map.h>

static inline void xvm_sc_map_base_init(struct xvm_sc_map_base_args_s *base, xvm_map_t *map,
					const void *key, uint32_t key_size)
{
	base->module_id = XVM_MAP_INVALID_MODULE_ID;
	base->scope = map->scope;
	base->map_id = map->id;
	base->key = key;
	base->key_size = key_size;
}

#endif
