/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Declaration of schedctrl
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 07 20:24:21 2023
 */
#ifndef UAPI_HMKERNEL_SCHEDULER_H
#define UAPI_HMKERNEL_SCHEDULER_H

struct schedctrl_scheduler {
	cref_t scheduler_cref;
	cref_t thread_cref;
};

enum schedctrl_cmd {
	SCHEDCTRL_THREAD_ATTACH_SCHEDULER,
#ifdef CONFIG_DEBUG_BUILD
	SCHEDCTRL_THREAD_GET_SCHEDULER_CREF,
#endif
	SCHEDCTRL_NR,
};

#endif
