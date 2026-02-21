/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implementation of debug_chck
 * Author: RTOS
 * Create: Fri Jun 18 14:28:57 2021
 */
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>

int __poll_chk(struct pollfd *fds, nfds_t n, int timeout, size_t fds_len)
{
	if (fds_len / sizeof(*fds) < n) {
		printf("buffer overflow detected\n");
		abort();
	}

	return poll(fds, n, timeout);
}
