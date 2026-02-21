/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar  2 16:08:49 2020
 */

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <heapmgr.h>

#include "internal.h"

#define BUF_SIZE 256

void heapmgr_set_errno(int err, const char *func, int line)
{
	(void)func;
	(void)line;
	errno = err;
}

void heapmgr_log(const char *fmt, ...)
{
	char buf[BUF_SIZE];
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vsnprintf(buf, BUF_SIZE, fmt, ap);
	va_end(ap);
	if (ret > 0) {
		(void)write(STDOUT_FILENO, buf, ret);
	} else {
		(void)write(STDOUT_FILENO, "vsnprintf failed\n", 17u);
	}
}

void heapmgr_fatal(const char *msg, const char *func, int line)
{
	heapmgr_log("[function %s line %d] %s\n", func, line, msg);
	abort();
}

/*
 * Heap manager may require that memory region has larger than 4K alignment.
 * This function fulfills request by mmap'ing extra virtual memory, keeping
 * the aligned part, and munmap'ing unneeded front and/or rear parts.
 */
void *heapmgr_memory_map(size_t size, size_t alignment)
{
	void *ret = NULL;
	int r;
	void *p = mmap(NULL, size + alignment, PROT_NONE,
		       (int)((unsigned)MAP_ANONYMOUS | (unsigned)MAP_PRIVATE),
		       -1, 0);
	if (p != MAP_FAILED) {
		if (alignment == 0UL) {
			r = mprotect(p, size, (int)((unsigned)PROT_READ | (unsigned)PROT_WRITE));
			if (r != 0) {
				heapmgr_fprintf(stderr, "unexpected mprotect failure: err=%s\n",
						strerror(errno));
				heapmgr_memory_unmap(p, size);
			} else {
				ret = p;
			}
		} else {
			uintptr_t ptr = (uintptr_t)p;
			uintptr_t aligned_ptr = align_up(ptr, alignment);
			uintptr_t end = aligned_ptr + size;
			if (aligned_ptr > ptr) {
				heapmgr_memory_unmap((void *)ptr, aligned_ptr - ptr);
			}
			if (ptr + size + alignment > end) {
				heapmgr_memory_unmap((void *)end, ptr + alignment - aligned_ptr);
			}
			r = mprotect((void *)aligned_ptr, size, (int)((unsigned)PROT_READ | (unsigned)PROT_WRITE));
			if (r != 0) {
				heapmgr_fprintf(stderr, "unexpected mprotect failure: err=%s\n",
						strerror(errno));
				heapmgr_memory_unmap((void *)aligned_ptr, size);
			} else {
				ret = (void *)aligned_ptr;
			}
		}
	}

	return ret;
}

void heapmgr_memory_unmap(void *p, size_t size)
{
	int rc;

	rc = munmap(p, size);
	if (rc != 0) {
		heapmgr_fprintf(stderr, "unexpected munmap failure: err=%s\n",
				strerror(errno));
	}
}

void heapmgr_memory_trim(void *p, size_t size, const size_t alignment)
{
	uintptr_t aligned_start = align_up((uintptr_t)p, alignment);
	uintptr_t aligned_end = align_down((uintptr_t)p + size, alignment);
	int r;

	if (aligned_end > aligned_start) {
		r = madvise((void *)aligned_start, aligned_end - aligned_start,
			    MADV_DONTNEED);
		if (r != 0) {
			heapmgr_fprintf(stderr, "unexpected madvise failure: err=%s\n",
					strerror(errno));
		}
	}
}

void heapmgr_fprintf(FILE *f, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	(void)vfprintf(f, fmt, ap);
	va_end(ap);
}
