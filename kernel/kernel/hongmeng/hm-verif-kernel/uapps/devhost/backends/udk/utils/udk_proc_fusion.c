/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: implements procmgr interfaces for UDK drivers under fusion mode
 * Author: Huawei OS Kernel Lab
 * Create: Tues Apr 1 15:27:05 2025
 */

#include <intapi/udkmgr/process.h>

#include <udk/proc_fusion.h>

int udk_process_name_of_current(char *name, size_t len)
{
	return udkmgr_process_name_of_current(name, len);
}

