/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Devicetree
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 28 23:17:23 2018
 */
#ifndef KLIBS_ELFLOADER_DEVICETREE
#define KLIBS_ELFLOADER_DEVICETREE

#include <hmkernel/memory.h>

extern void dt_init(void *dt_buffer);

extern void
dt_edit_memory_area(const char *nodename,
		    void *start, void *end);

extern void
dt_edit_u64(const char *nodename, const char *propname,
	    u64 val);

extern void
dt_edit_u32(const char *nodename, const char *propname,
	    u32 val);

extern u64
dt_get_memory(const char *nodename, unsigned int idx, u64 *_size);

extern u32
dt_get_prop32(const char *nodename, const char *propname);

extern u64
dt_get_prop64(const char *nodename, const char *propname);

extern const void *
dt_read_prop(const char *nodename, const char *propname, int *psize);

extern bool
dt_detect_node(const char *nodename);
#endif
