/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: libhmpsf xvm module
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 14 10:56:27 2023
 */
#include <libhmpsf/xvm/module.h>
#include <lib/hmpsf/hmpsf_def.h>
#include <xvm/common/xvm_module_defs.h>
#include "../adapter/module.h"

int g_xvm_exec_type = XVM_EXEC_COMPILER;

int xvm_module_load(enum hmpsf_vspaceid vspaceid, enum hmpsf_module_type module_type,
		    unsigned char *code, unsigned long code_size,
		    xvm_map_handle_t *maps, unsigned int map_cnt)
{
	int ret = E_HM_OK;
	struct hmpsf_module_attr_s attr = {
		.vm_type = HMPSF_T_HMXVM,
		.vspaceid = vspaceid,
		.module_type = module_type,
		.opt_level = 0,
		.exec_type = g_xvm_exec_type,
	};

	if (map_cnt > HMPSF_MODULE_MAX_MAPS) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		for (unsigned int i = 0; i < map_cnt; i++) {
			xvm_map_t *map = (xvm_map_t *)maps[i];
			attr.map_ids[i] = map->id;
		}
		attr.map_cnt = map_cnt;
		ret = xvm_syscall_module_load(&attr, code, code_size);
	}

	return ret;
}

int xvm_module_release(unsigned int id)
{
	return xvm_syscall_module_release(id);
}
