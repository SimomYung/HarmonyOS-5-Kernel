/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Netlink taskstats library
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb  6 15:47:16 UTC 2020
 */

#ifndef __UX_NETLINK_TASKSTATS_H_
#define __UX_NETLINK_TASKSTATS_H_

#define TASKSTATS_GENL_NAME		"TASKSTATS"
#define TASKSTATS_GENL_VERSION		0x1

enum {
	UX_TASKSTATS_CMD_UNSPEC = 0,
	UX_TASKSTATS_CMD_GET,
	UX_TASKSTATS_CMD_NEW,
	UX_TASKSTATS_CMD_MAX,
};

enum {
	UX_TASKSTATS_TYPE_UNSPEC = 0,
	UX_TASKSTATS_TYPE_PID,
	UX_TASKSTATS_TYPE_TGID,            /* Thread group id */
	UX_TASKSTATS_TYPE_STATS,           /* taskstats structure */
	UX_TASKSTATS_TYPE_AGGR_PID,        /* contains pid + stats */
	UX_TASKSTATS_TYPE_AGGR_TGID,       /* contains tgid + stats */
};

enum {
	UX_TASKSTATS_CMD_ATTR_UNSPEC = 0,
	UX_TASKSTATS_CMD_ATTR_PID,
	UX_TASKSTATS_CMD_ATTR_MAX,
};

int ux_netlink_taskstats_init(void);
void ux_netlink_taskstats_exit(void);

#endif
