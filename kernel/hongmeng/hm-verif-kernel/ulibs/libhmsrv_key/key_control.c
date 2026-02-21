/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 03 16:22:13 2021
 */

#include <libsysif/keymgr/api.h>

long hm_key_control(int operation, unsigned long arg2, unsigned long arg3,
		    unsigned long arg4, unsigned long arg5)
{
	return lsyscall_keymgr_keyctl(operation, arg2, arg3, arg4, arg5);
}
