/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: Definition of multiple special reserved nsids
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 17 22:03:36 2025
 */
#ifndef SYSMGR_AAPI_NSMGR_NSID_H
#define SYSMGR_AAPI_NSMGR_NSID_H
 
/* Reservations for root ns id. */

enum {
	ID_ROOT_NET	= 8189,
	ID_ROOT_PID	= 8190,
	ID_ROOT_UTS	= 8191,
	ID_ROOT_IPC	= 8192,
	ID_ROOT_USR	= 8193,
	ROOT_NS_NUM	= 5,
};
 
#endif