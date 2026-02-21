/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Procfs function headfile for network unix
 * Author: Huawei OS Kernel Lab
 * Create: Mon Oct 12 10:21:00 2020
 */

#ifndef __UX_UNIX_PROCFS_H__
#define __UX_UNIX_PROCFS_H__

int ux_max_dgram_qlen(void);

void ux_unix_procfs_init(void);

#endif
