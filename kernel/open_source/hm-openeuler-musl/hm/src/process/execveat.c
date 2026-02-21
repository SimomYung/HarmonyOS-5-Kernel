/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 02 14:40:46 2020
 */
#include <unistd.h>
#include "syscall.h"

int execveat(int dirfd, const char *pathname, char *const argv[], char *const envp[], int flags)
{
	return (int)syscall(SYS_execveat, dirfd, pathname, argv, envp, flags);
}
