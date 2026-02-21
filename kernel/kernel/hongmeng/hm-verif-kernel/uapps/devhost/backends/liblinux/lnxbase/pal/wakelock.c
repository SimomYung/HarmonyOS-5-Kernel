/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Adapt wakelock for liblinux
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 03 17:50:53 2020
 */
#include <liblinux/pal.h>

#include <devhost/pm.h>

int liblinux_pal_ws_activate(void)
{
	return devhost_wakelock_acquire();
}

int liblinux_pal_ws_deactivate(void)
{
	return devhost_wakelock_release();
}
