/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 08 14:53:17 2019
 */
#define _GNU_SOURCE

#include <stddef.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <stdbool.h>

#include <string.h>
#include "syscall.h"
#include "atomic.h"

int safe_copy(void *dst, const void *src, size_t len)
{
	int r = 0;
	static int (*func)(void *, const void *, size_t) =
		(int (*)(void *, const void *, size_t))NULL;
	if (func == NULL) {
		void *p = __vdsosym(VDSO_SAFE_COPY_VERSION, VDSO_SAFE_COPY);
		if (p == NULL) {
			errno = ENOSYS;
			r = -ENOSYS;
		} else {
			(void)a_cas_p(&func, NULL, p);
		}
	}

	if (r == 0) {
		r = func(dst, src, len);
		if (r != 0) {
			errno = EFAULT;
		}
	}
	return r;
}

int safe_copy_align(void *dst, const void *src, size_t len)
{
	int r = 0;
	static int (*func_align)(void *, const void *, size_t) =
		(int (*)(void *, const void *, size_t))NULL;
	if (func_align == NULL) {
		void *p = __vdsosym(VDSO_SAFE_COPY_VERSION, VDSO_SAFE_COPY_ALIGN);
		if (p == NULL) {
			errno = ENOSYS;
			r = -ENOSYS;
		} else {
			(void)a_cas_p(&func_align, NULL, p);
		}
	}

	if (r == 0) {
		r = func_align(dst, src, len);
		if (r != 0) {
			errno = EFAULT;
		}
	}
	return r;
}

int safe_copy_nopf(void *dst, const void *src, size_t len)
{
	int r = 0;
	static int (*func_nopf)(void *, const void *, size_t) =
		(int (*)(void *, const void *, size_t))NULL;
	if (func_nopf == NULL) {
		void *p = __vdsosym(VDSO_SAFE_COPY_VERSION, VDSO_SAFE_COPY_NOPF);
		if (p == NULL) {
			errno = ENOSYS;
			r = -ENOSYS;
		} else {
			(void)a_cas_p(&func_nopf, NULL, p);
		}
	}

	if (r == 0) {
		r = func_nopf(dst, src, len);
		if (r != 0) {
			errno = EFAULT;
		}
	}
	return r;
}

static int __check_mem(void *addr, size_t size, char rw)
{
	int err;
	uintptr_t page_start, page_mask, next_page_start;
	size_t remain, page_size, page_mem;
	char byte_mem;
	remain = size;
	page_size = (size_t)getpagesize();
	page_mask = (~((uintptr_t)page_size - 1u));
	page_start = (uintptr_t)addr;
	next_page_start = (page_start + page_size) & page_mask;
	while (true) {
		page_mem = next_page_start - page_start;
		err = safe_copy(&byte_mem, (const void *)page_start, sizeof(char));
		if (err != 0) {
			err = -1;
			break;
		}
		if (rw == 'w') {
			err = safe_copy((void *)page_start, &byte_mem, sizeof(char));
			if (err != 0) {
				err = -1;
				break;
			}
		}
		page_start = next_page_start;
		next_page_start += page_size;
		if (remain > page_mem) {
			remain -= page_mem;
		} else {
			break;
		}
	}

	return err;
}

int safe_check_mem(void *addr, size_t size, char rw)
{
	int rc = 0;
	if (rw != 'r' && rw != 'w') {
		errno = EINVAL;
		rc = -1;
	}

	if (rc == 0) {
		rc = __check_mem(addr, size, rw);
	}
	return rc;
}
