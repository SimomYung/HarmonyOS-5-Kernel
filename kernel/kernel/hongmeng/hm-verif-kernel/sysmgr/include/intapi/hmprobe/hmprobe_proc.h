/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Header of hmprobemgr core
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 10 14:45:40 2023
 */
#ifndef H_SYSMGR_INCLUDE_HMPROBE_PROC_H
#define H_SYSMGR_INCLUDE_HMPROBE_PROC_H

#include <libhmsync/raw_mutex.h>
#include <lib/dlist.h>

struct hmprobe_proc_s {
	struct dlist_node	module_list_head; /* the list of module */
	struct dlist_node	event_list_head; /* the list of event */
	struct dlist_node	prog_list_head; /* the list of prog */
	struct raw_mutex	list_mutex; /* mutex for dlist */
};

static inline void hmprobe_proc_init(struct hmprobe_proc_s *proc)
{
	raw_mutex_init(&proc->list_mutex);
	dlist_init(&proc->module_list_head);
	dlist_init(&proc->event_list_head);
	dlist_init(&proc->prog_list_head);
}
#endif
