/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Container with uts namespace
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 13 10:01:00 2024
 */

#ifndef SYSMGR_INCLUDE_NSMGR_NS_CONTAINER_H
#define SYSMGR_INCLUDE_NSMGR_NS_CONTAINER_H

enum {
	NON_CONTAINER = 0, /* not in any container */
	IN_CONTAINER, /* indicate the only container for now */
};

int container_id_of_tid(unsigned int nsid, pid_t tid);
bool current_thread_in_DH(void);

#endif /* SYSMGR_INCLUDE_NSMGR_NS_CONTAINER_H! */
