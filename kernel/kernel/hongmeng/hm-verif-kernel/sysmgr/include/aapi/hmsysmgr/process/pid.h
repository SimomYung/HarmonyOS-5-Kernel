/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of multiple special reserved pids
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 7 11:16:37 2019
 */
#ifndef SYSMGR_AAPI_PROCESS_PID_H
#define SYSMGR_AAPI_PROCESS_PID_H

enum {
	PID_NONE = -1,
	PID_RESERVED,
	PID_INIT_PROC,
	PID_SYSMGR_PROC,
	PID_VFS_PROC,
	PID_ALLOC_BASE,
};

#endif
