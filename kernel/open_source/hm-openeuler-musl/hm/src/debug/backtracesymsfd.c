/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implementation of backtrace_symbols_fd
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 22 09:08:22 2021
 */
#include <dlfcn.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/uio.h>
#include <execinfo.h>

#include "libc.h"

#if UINTPTR_MAX > 0xffffffff
#define WORD_WIDTH 16U
#else
#define WORD_WIDTH 8U
#endif

/*
 * foramt:
 * "<filename>" + "(" + "<symbolname>" + "+/-0x" + "<offset>) " + "[0x<addr>]"
 */
#define IOV_TOKEN_MAX		6U
/* extra characters used in addr portion: "[0x]\n\0" */
#define BUF_SIZE		(WORD_WIDTH + 6U)

#define ptr_to_void(ptr)	((void *)(uintptr_t)(ptr))

static char base1[] = "(";
static char base2[] = "+0x";
static char base3[] = "-0x";

static void put_string_to_fd(void *addr, int fd)
{
	struct iovec iov[IOV_TOKEN_MAX];
	char offset_buf[BUF_SIZE] = { 0 };
	char addr_buf[BUF_SIZE] = { 0 };
	uintptr_t offset;
	Dl_info info;
	int status;
	int index = 0;

	memset(&info, 0, sizeof(info));
	status = dladdr(addr, &info);

	if (status != 0 && info.dli_fname != NULL && info.dli_fname[0] != '\0') {
		/* add filename */
		iov[index].iov_base = (void *)info.dli_fname;
		iov[index].iov_len = strlen(info.dli_fname);
		index++;

		iov[index].iov_base = ptr_to_void(base1);
		iov[index].iov_len = 1;
		index++;

		if (info.dli_sname != NULL) {
			/* if symbol found, add symbol name */
			iov[index].iov_base = (void *)info.dli_sname;
			iov[index].iov_len = strlen(info.dli_sname);
			index++;
		} else {
			/* else use file offset */
			info.dli_saddr = info.dli_fbase;
		}

		/* add sign +/-0x */
		if (addr >= info.dli_saddr) {
			iov[index].iov_base = ptr_to_void(base2);
			offset = (uintptr_t)addr - (uintptr_t)info.dli_saddr;
		} else {
			iov[index].iov_base = ptr_to_void(base3);
			offset = (uintptr_t)info.dli_saddr - (uintptr_t)addr;
		}
		iov[index].iov_len = strlen(iov[index].iov_base);
		index++;

		/* add offset */
		(void)snprintf(offset_buf, BUF_SIZE, "%tx) ", offset);
		iov[index].iov_base = offset_buf;
		iov[index].iov_len = strlen(offset_buf);
		index++;
	}

	/* add addr */
	(void)snprintf(addr_buf, BUF_SIZE, "[%p]\n", addr);
	iov[index].iov_base = addr_buf;
	iov[index].iov_len = strlen(addr_buf);
	index++;

	(void)writev(fd, iov, index);
}

void backtrace_symbols_fd(void *const *array, int size, int fd)
{
	int i;

	if (array != NULL && size > 0 && fd >= 0) {
		for (i = 0; i < size; i++) {
			put_string_to_fd(array[i], fd);
		}
	}
}
