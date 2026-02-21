/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: process event header file
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 01 11:13:29 2021
 */

#ifndef INTAPI_PROCESS_EVENT_H
#define INTAPI_PROCESS_EVENT_H

#include <lib/dlist.h>
#include <hmkernel/capability.h>
#include <libstrict/strict.h>
#include <hongmeng/errno.h>

struct process_s;
struct process_event_s {
	rref_t server;
	unsigned int type;
	struct dlist_node node;
};

#ifdef CONFIG_PROCESS_EVENT
int register_process_event_type(unsigned int type,
				int (*handler)(struct process_s *process,
					       struct process_event_s *event));
int process_register_event(struct process_s *process, unsigned int type, rref_t server);
void process_notify_event(struct process_s *process);
void register_uvmm_notify_event(void);

#else
static inline int
register_process_event_type(unsigned int type,
			    int (*handler)(struct process_s *process,
					   struct process_event_s *event))
{
	UNUSED(type, handler);
	return E_HM_OK;
}

static inline int
process_register_event(struct process_s *process, unsigned int type,
		       rref_t server)
{
	UNUSED(process, type, server);
	return E_HM_OK;
}

static inline void
process_notify_event(struct process_s *process)
{
	UNUSED(process);
}

static inline void
register_uvmm_notify_event(void)
{
}

#endif

#endif
