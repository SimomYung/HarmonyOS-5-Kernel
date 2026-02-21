/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Format and print data
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 11 16:57:54 2020
 */
#include <libhwsecurec/securec.h>
#include <stdio.h>
#include <sys/uio.h>
#include <stdarg.h>
#include <hmasm/lsyscall.h>
#include <hongmeng/syscall.h>
#include <errno.h>
#include <syscall.h>
#include <vfs.h>
#include <lib/printf.h>
#include <libhmsrv_sys/hm_log.h>

#define HM_STDOUT_FILENO	1
#define IOV_CNT			2
#define LOG_BUF_SIZE		272

static long hm_writev(int fd, struct iovec *iovs, int iovcnt)
{
	return lsyscall_syscall3(__NR_writev, (long)fd, ptr_to_long(iovs), (long)iovcnt);
}

__attribute__((format(printf, 1, 0)))
int hm_log(const char *fmt, ...)
{
	int ret;
	va_list ap;
	char log_buf[LOG_BUF_SIZE];
	struct iovec iovs[IOV_CNT];
	struct iovec *iov = iovs;

	va_start(ap, fmt);
	ret = vsnprintf_s(log_buf, LOG_BUF_SIZE, LOG_BUF_SIZE - 1, fmt, ap);
	va_end(ap);

	if (ret >= 0) {
		log_buf[LOG_BUF_SIZE - 1] = '\0';
		iovs[0].iov_base = (void *)log_buf;
		iovs[0].iov_len = (size_t)(long)ret;
		iovs[1].iov_base = NULL;
		iovs[1].iov_len = (size_t)0;
		ret = (int)hm_writev(HM_STDOUT_FILENO, iov, IOV_CNT);
	}
	return ret;
}
