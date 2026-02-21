/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 13 12:02:17 2019
 */
#include <assert.h>
#include <execinfo.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "libc.h"

#if UINTPTR_MAX > 0xffffffff
#define WORD_WIDTH 16
#else
#define WORD_WIDTH 8
#endif

/* 1 for sign, 2 for 0x */
#define OFFSET_SIZE (1 + WORD_WIDTH + 2)
#define ADDR_SIZE (WORD_WIDTH + 2)

/* Compute the length of the string, the format is
* "<file-name>(<sym-name>+offset) [address]."
*/
static inline unsigned int format_len(Dl_info *info, int status)
{
	unsigned int len = 0;

	/* If we found filename */
	if (status != 0 && info->dli_fname != NULL && info->dli_fname[0] != '\0') {
		/* filename */
		len += strlen(info->dli_fname);
		/* parentheses, symbol name, offset and blank space. */
		len += 2 + strlen(info->dli_sname ? : "") + OFFSET_SIZE + 1;
	}
	/* brackets, address and '\0' */
	len += (unsigned int)(2 + ADDR_SIZE + 1);
	return len;
}

static int put_string(char *dst, const void *addr, Dl_info *info, int status)
{
	/* If we found filename */
	if (status != 0 && info->dli_fname != NULL && info->dli_fname[0] != '\0') {
		if (info->dli_sname == NULL)
			/* If we found no symbol name to use,
			 * replace with the relative to the file.
			 */
			info->dli_saddr = info->dli_fbase;
		if (info->dli_sname == NULL && info->dli_saddr == 0) {
			return sprintf(dst, "%s(%s) [%p]", info->dli_fname, "", addr);
		} else {
			char sign;
			uintptr_t offset;

			if (addr >= info->dli_saddr) {
				sign = '+';
				offset = (uintptr_t)addr - (uintptr_t)info->dli_saddr;
			} else {
				sign = '-';
				offset = (uintptr_t)info->dli_saddr - (uintptr_t)addr;
			}
			return sprintf(dst, "%s(%s%c%#tx) [%p]",
					 info->dli_fname,
					 info->dli_sname ? : "",
					 sign, offset, addr);
		}
	}
	return sprintf(dst, "[%p]", addr);
}

static char **do_put_string(void *const *array, Dl_info *info, const int *status,
			    unsigned int msize, unsigned int size)
{
	int cnt;
	unsigned int i;
	char **result = NULL;

	if (msize == 0) {
		return NULL;
	}
	/* Allocate memory for the result. */
	result = (char **)malloc(msize);
	if (result != NULL) {
		char *last = (char *)(result + size);
		for (i = 0; i < size; i++) {
			result[i] = last;
			cnt = put_string(last, array[i], &info[i], status[i]);
			if (cnt < 0) {
				free(result);
				result = NULL;
				break;
			}
			last += 1 + cnt;
		}
	}
	return result;
}

static char **do_backtrace_symbols(void *const *array, unsigned int size)
{
	Dl_info *info = NULL;
	int *status = NULL;
	char **result = NULL;
	unsigned int msize, i;

	if (size == 0) {
		return NULL;
	}
	status = (int *)calloc(size, sizeof(int));
	if (status != NULL) {
		info = (Dl_info *)calloc(size, sizeof(Dl_info));
		if (info != NULL) {
			msize = size * sizeof(char *);
			/* Get info of address and the output length of string. */
			for (i = 0; i < size; i++) {
				status[i] = dladdr(array[i], &info[i]);
				msize += format_len(&info[i], status[i]);
			}
			/* Allocate memory for the result. */
			result = do_put_string(array, info, status, msize, size);
			free(info);
		}
		free(status);
	}
	return result;
}

char **backtrace_symbols(void *const *array, int size)
{
	char **ret = NULL;

	if (array != NULL && size > 0) {
		ret = do_backtrace_symbols(array, (unsigned int)size);
	}
	return ret;
}
