/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Implementation of iaware notify policy interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 23 16:35:56 2023
 */
#ifndef __DEVHOST_PLUGIN_IAWARE_NOTIFY_POLICY_H__
#define __DEVHOST_PLUGIN_IAWARE_NOTIFY_POLICY_H__

#include <hmkernel/types.h>
#include <libsysif/devhost/server.h>
#include <hmsysmgr/eventmgr/event_def.h>
#include <hmkernel/capability.h>

#define CPU_NETLINK_DATA_LEN 2
/* default group id */
#define VIP_GROUP 3
#define DEFAULT_THREAD_GROUP_NO 1
/* count of static whitelist threads */
#define DEFAULT_THREAD_NAME_NO 7
/* default whitelist thread names */
#define BINDER_THREAD_NAME "Binder:"
#define MALI_THREAD_NAME "mali-cmar-backe"
#define MALI_UTIL_NAME "mali-utility-wo"
#define MALI_MEM_PURGE_NAME "mali-mem-purge"
#define GPU_WK_CLIENT_NAME "gpu-work-client"
#define GPU_WK_SRV_NAME "gpu-work-server"
#define GPU_MEM_POOL_NAME "gpu-mem-pool"

enum sock_no {
	CPU_HIGH_LOAD = 1,
	PROC_FORK = 2,
	PROC_COMM = 3,
	PROC_AUX_COMM = 4,
	PROC_LOAD = 5,
	PROC_AUX_COMM_FORK = 6,
	PROC_AUX_COMM_REMOVE = 7,
	PROC_CPUMASK_BIG = 8,
	PROC_MALI_COMM = 9
};

int thread_event_handler(struct evtcb_thread_msg_s *msg);
extern struct evtmgr_thread_name_wl_s name_whitelist;
extern struct evtmgr_thread_group_wl_s group_whitelist;
int set_thread_name_whitelist(char *buffer);
#endif /* __DEVHOST_PLUGIN_IAWARE_NOTIFY_POLICY_H__ */
