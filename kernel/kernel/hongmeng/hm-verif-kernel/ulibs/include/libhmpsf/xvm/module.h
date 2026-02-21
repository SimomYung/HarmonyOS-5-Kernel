/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: libhmpsf xvm module in ulibs
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 14 10:56:27 2023
 */

#ifndef ULIBS_LIBHMPSF_XVM_MODULE_H
#define ULIBS_LIBHMPSF_XVM_MODULE_H

#include <libhmpsf/xvm/std/map/map.h>
#include <lib/hmpsf/hmpsf_def.h>

extern int g_xvm_exec_type;

int xvm_module_load(enum hmpsf_vspaceid vspaceid, enum hmpsf_module_type module_type,
		    unsigned char *code, unsigned long code_size,
		    xvm_map_handle_t *maps, unsigned int map_cnt);
int xvm_module_release(unsigned int id);

#endif
