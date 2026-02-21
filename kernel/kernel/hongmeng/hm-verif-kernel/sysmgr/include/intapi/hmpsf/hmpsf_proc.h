/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description:
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 10 14:45:40 2023
 */
#ifndef H_SYSMGR_INCLUDE_HMPSF_PROC_H
#define H_SYSMGR_INCLUDE_HMPSF_PROC_H

#include <unistd.h>
#include <lib/dlist.h>

struct hmpsf_proc_s {
	struct dlist_node	module_use_list; /* the list of module use */
	struct raw_mutex	module_use_list_mutex; /* mutex for module use dlist */
#ifdef CONFIG_HMXVM
	struct dlist_node	map_use_list; /* the list of map use */
	struct raw_mutex	map_use_list_mutex; /* mutex for map use dlist */
#endif
};

static inline void hmpsf_proc_init(struct hmpsf_proc_s *proc)
{
	raw_mutex_init(&proc->module_use_list_mutex);
	dlist_init(&proc->module_use_list);
#ifdef CONFIG_HMXVM
	raw_mutex_init(&proc->map_use_list_mutex);
	dlist_init(&proc->map_use_list);
#endif
}
#endif
