/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Declare interface of wakeup
 * Author: Huawei OS Kernel Lab
 * Create: Sat Mar 11 11:10:58 UTC 2023
 */

#ifndef __DEVHOST_WAKEUP_H__
#define __DEVHOST_WAKEUP_H__

#ifdef CONFIG_PM_SLEEP

int platform_wakelock_stat(void *args);

#else

static inline int platform_wakelock_stat(void *args)
{
	return 0;
}

#endif

#endif /* __DEVHOST_WAKEUP_H__ */
