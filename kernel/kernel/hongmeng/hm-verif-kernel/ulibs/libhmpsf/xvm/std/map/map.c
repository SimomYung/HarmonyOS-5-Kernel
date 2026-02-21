/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Provide xvm map API for xvm clients
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 23 11:46:20 2023
 */

#include <libhmpsf/xvm/std/map/map.h>
#include <malloc.h>
#include <string.h>
#include "../../../adapter/std/map/map.h"

unsigned char g_xvm_map_alg = XVM_MAP_ALG_DEFAULT;

/*
 * Deprecated.
 */
void xvm_map_free_handle(xvm_map_handle_t map)
{
	(void)map;
}

/*
 * Release the reference to a globally-shared map.
 * Note that the creation of globally-shared map is type-specific.
 */
int xvm_map_release_global_shared(xvm_map_handle_t map)
{
	int err = xvm_syscall_map_release_global_shared(((xvm_map_t *)map)->id);
	if (err == E_HM_OK) {
		free(map);
	}
	return err;
}
