/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Header of hmprobemgr core
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 13 14:45:40 2023
 */
#ifndef H_SYSMGR_INCLUDE_HMPROBEMGR_H
#define H_SYSMGR_INCLUDE_HMPROBEMGR_H

#include <lib/hmprobe/utils.h>
#include <intapi/process/process.h>

struct hmprobemgr_event_s {
	int vspaceid;
	unsigned int event_fd;
	unsigned int event_id;
	struct dlist_node	list;
};

struct hmprobemgr_module_s {
	int vspaceid;
	unsigned int module_id;
	atomic64_t		refcnt;
	struct dlist_node	list;
};

struct hmprobemgr_prog_s {
	int vspaceid;
	unsigned int prog_fd;
	unsigned int prog_id;
	struct hmprobe_module_attach_attr_s attr;
	struct dlist_node	list;
};

void process_clean_hmprobemgr(struct process_s *process);

#endif
