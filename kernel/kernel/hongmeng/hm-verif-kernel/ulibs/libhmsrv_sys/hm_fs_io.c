/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation of fs io
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 13 15:08:52 2023
 */

#include <hmasm/ucontext.h>
#include <hmkernel/sched_module.h>
#include <libhmsync/thread.h>
#include <libhmsrv_sys/hm_fs_io.h>

int hm_fs_io_restore_uctx(void)
{
#ifdef CONFIG_ACTV_SCHED_FS
	return syscap_ModuleIoctl(thread_module_sched_ref(), __SCHED_RESTORE_UCTX,
				  NULL, 0, NULL, 0);
#else
	return 0;
#endif
}
