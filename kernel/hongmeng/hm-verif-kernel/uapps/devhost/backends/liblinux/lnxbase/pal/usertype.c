/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: used to distinguish the user version
 * Author: Huawei OS Kernel Lab
 * Create: Tues Aug 20 17:13:35 2024
 */

#include <libsysif/utils_common.h>
#include <libsysif/sysmgr/api.h>
#include <libhmactv/actv.h>

unsigned int liblinux_pal_get_log_usertype(void)
{
	return actvcall_hmcall_procfs_get_log_usertype();
}
