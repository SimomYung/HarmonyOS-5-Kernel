/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Structures and interfaces for ipc namespace
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 20 16:58:25 2023
 */

#ifndef SYSMGR_INCLUDE_NSMGR_NS_IPC_H
#define SYSMGR_INCLUDE_NSMGR_NS_IPC_H

#include <sched.h>
#include <intapi/nsmgr/ns_common.h>
#include <libhmsync/raw_mutex.h>
#include <mqueuemgr/mqueuemgr_core.h>

#ifndef CLONE_NEWIPC
#define CLONE_NEWIPC    0x08000000
#endif

struct ipc_ns {
	struct ns_common ns_common;
	struct mqueue_data *mq_data;
};

static inline int
ipc_ns_get(struct ipc_ns *ns)
{
	return ns_common_get(&ns->ns_common);
}

static inline void
ipc_ns_put(struct ipc_ns *ns)
{
	return ns_common_put(&ns->ns_common);
}

static inline struct ipc_ns *
common_to_ipc_ns(struct ns_common *ns)
{
	return container_of(ns, struct ipc_ns, ns_common);
}

struct ns_operation *fetch_ipc_ns_op(void);

#ifdef CONFIG_IPC_NS
int copy_ipc_ns(struct ipc_ns *ns_old, unsigned int flags, struct ipc_ns **ns_new);
struct ipc_ns *ipc_ns_of_process(struct process_s *process);
#else
static inline int copy_ipc_ns(struct ipc_ns *ns_old, unsigned int flags, struct ipc_ns **ns_new)
{
	int err = E_HM_OK;

	if ((flags & (unsigned int)CLONE_NEWIPC) != 0U) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		*ns_new = ns_old;
		int rc = ipc_ns_get(ns_old);
		BUG_ON(rc <= 0);
	}

	return err;
}

static inline struct ipc_ns *ipc_ns_of_process(struct process_s *process)
{
	UNUSED(process);
	return NULL;
}
#endif

#endif
