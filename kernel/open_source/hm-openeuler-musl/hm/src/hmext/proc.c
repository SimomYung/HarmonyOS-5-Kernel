/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2022. All rights reserved.
 * Description: Implementation of hm_proc_check_regs
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 25 09:53:02 2020
 */

#include <hmext/proc.h>
#include <unistd.h>
#include <syscall.h>

int hm_proc_check_regs(unsigned int which, unsigned long addr, size_t size)
{
	int ret = (int)syscall(SYS_proc_check_regs, which, addr, size);
	return ret;
}
