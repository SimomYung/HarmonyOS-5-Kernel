/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 18 11:42:49 2019
 */

#ifndef ULIBS_LIBHMPM_SUSPEND_H
#define ULIBS_LIBHMPM_SUSPEND_H

enum suspend_state {
	SUSPEND_STATE_INVALID = -1,
	SUSPEND_STATE_MEM,
#ifdef CONFIG_PM_SUSPEND_ULSR
	SUSPEND_STATE_ULSR,
#endif
#ifdef CONFIG_PM_SUSPEND_DISK
	SUSPEND_STATE_DISK,
#endif
	SUSPEND_STATE_MAX,
};

int hm_suspend2mem(void);
int hm_suspend_disk_restore(void);

#endif
