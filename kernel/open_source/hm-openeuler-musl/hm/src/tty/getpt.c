/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: The implement of getpt
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 26 15:50:16 2022
 */
#include <stdlib.h>
#include <fcntl.h>

int getpt(void)
{
	int fd;

	fd = open("/dev/ptmx", (int)((unsigned int)O_RDWR | (unsigned int)O_NOCTTY));
	return (fd < 0) ? -1 : fd;
}
