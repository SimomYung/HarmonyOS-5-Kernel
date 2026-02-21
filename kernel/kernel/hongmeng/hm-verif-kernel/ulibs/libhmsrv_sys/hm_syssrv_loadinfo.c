/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Implementation of saving loadinfo for system services
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 13 16:24:46 2024
 */

#include <hmkernel/strict.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmsrv_sys/hm_syssrv_loadinfo.h>

#ifdef CONFIG_SAVE_LOAD_INFO
void hm_syssrv_loadinfo_save(void)
{
	(void)actvcall_hmcall_proc_syssrv_loadinfo_save(hm_getpid());
}

void hm_syssrv_loadinfo_save_pid(pid_t pid)
{
	(void)actvcall_hmcall_proc_syssrv_loadinfo_save(pid);
}
#else
void hm_syssrv_loadinfo_save(void) { }

void hm_syssrv_loadinfo_save_pid(pid_t pid)
{
	UNUSED(pid);
}
#endif
