/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Header file of netstats for network unix
 * Author: Huawei OS Kernel Lab
 * Create: Mon Oct 19 10:21:00 2020
 */

#ifndef __UX_UNIX_STAT_H_
#define __UX_UNIX_STAT_H_

#include "ux_unix_util.h"

void delete_unixlist(ux_unix_sock_t *usk);
void insert_unixlist(ux_unix_sock_t *usk);

int get_unixlist_netstat(ux_unix_sock_t *usk, char *string, int *offset, int mlen);

#endif
