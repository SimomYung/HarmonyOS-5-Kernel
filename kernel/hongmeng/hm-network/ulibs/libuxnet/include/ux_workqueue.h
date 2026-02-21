/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for workqueue
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 11 11:12:02 2019
 */

#ifndef __LIBUXNET_UX_WORKQUEUE_
#define __LIBUXNET_UX_WORKQUEUE_

#include <hongmeng/hongmeng.h>
#include <hongmeng/errno.h>

typedef void (*net_work_handler)(void *);

#ifdef CONFIG_WORKQUEUE
int net_add_async_work(net_work_handler func, void *data);
#else
static inline int net_add_async_work(net_work_handler func, void *data)
{
	UNUSED(func, data);
	return E_HM_OPNOTSUPP;
}
#endif

#endif
