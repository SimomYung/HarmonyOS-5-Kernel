/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: interface for netstats
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 29 09:56:30 2021
 */

#ifndef __UX_VISP_NETSTAT_H__
#define __UX_VISP_NETSTAT_H__

int visp_procfs_get_netstat(char **msg, int *msg_len, int type);
void visp_procfs_release_netstat(char *buf, int type);

#endif
