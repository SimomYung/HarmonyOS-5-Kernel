/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of ulibs utils
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 28 23:51:08 2020
 */
#include <libmem/utils.h>
#include <stdlib.h>
#include <errno.h>

/*
 * NOTE: Just a work around to enable page demanding, and
 * we should remove this function when kernel copy_to_user
 * support page demanding.
 */
int __touch_buffer(void *touch_ptr, size_t touch_len, bool need_write)
{
	int rc = E_HM_OK;
	volatile char tmp;
	char touch_read = '\0';
	char *touch_start = (char *)touch_ptr;
	char *touch_end = touch_start + touch_len;
	if (touch_end < touch_start) {
		rc = E_HM_POSIX_FAULT;
	}

	if (rc == E_HM_OK) {
		while (touch_start < touch_end) {
			rc = safe_copy((char *)&touch_read, touch_start, 1);
			if (rc != E_HM_OK) {
				rc = posix2hmerrno(-rc);
				break;
			}
			tmp = touch_read;
			if (need_write) {
				(void)safe_copy(touch_start, (char *)&touch_read, 1);
			}
			touch_start = PAGE_ALIGN_DOWN(touch_start + PAGE_SIZE);
			(void)tmp;
		}
	}
	return rc;
}

static unsigned int __shift_len(char end_char)
{
	unsigned int shift_len = 0U;
	switch (end_char) {
	case 'P':
		shift_len = 50U;
		break;
	case 'p':
		shift_len = 50U;
		break;
	case 'T':
		shift_len = 40U;
		break;
	case 't':
		shift_len = 40U;
		break;
	case 'G':
		shift_len = 30U;
		break;
	case 'g':
		shift_len = 30U;
		break;
	case 'M':
		shift_len = 20U;
		break;
	case 'm':
		shift_len = 20U;
		break;
	case 'K':
		shift_len = 10U;
		break;
	case 'k':
		shift_len = 10U;
		break;
	default:
		shift_len = 0U;
		break;
	}
	return shift_len;
}

int memstr_to_size(const char *ptr, unsigned long long *r_size)
{
	unsigned long long size = 0UL;
	unsigned int shift_len = 0U;
	long long result = 0;
	char *endptr = NULL;
	int rc = E_HM_OK;
	int base = 10;

	if (ptr == NULL || r_size == NULL) {
		rc = E_HM_INVAL;
	} else {
		errno = 0;
		result = strtoll(ptr, &endptr, base);
		if (errno != 0 || result < 0 || endptr == ptr) {
			rc = E_HM_INVAL;
		} else {
			size = (unsigned long long)result;
		}
	}

	if (rc == E_HM_OK && endptr != NULL) {
		shift_len = __shift_len(*endptr);
		size <<= shift_len;
	}

	if (rc == E_HM_OK) {
		*r_size = size;
	}

	return rc;
}

/*
 * safe_strncpy - copy string from @src to @dst on a byte by byte basis.
 *
 *
 * return:
 * 	E_HM_OK: if bytes can be accessed and its length is valid.
 *	E_HM_POSIX_FAULT: if address is invalid (e.g, does not belong to the current vspace).
 *	E_HM_RANGE: if the string is not ended with '\0' within the max buffer size.
 */
int safe_strncpy(void *dst, const void *src, size_t size)
{
	int err;
	char *dst_ptr = (char *)dst;
	const char *src_ptr = (const char *)src;

	for (size_t i = 0; i < size; i++, dst_ptr++, src_ptr++) {
		/*
		 * Use safe_copy to process the data fault that might be
		 * occurred when accessing the user memory. Later, the
		 * safe_strncpy can be implemented like the safe_copy to
		 * optimize the performance of it if needed.
		 */
		err = safe_copy(ptr_to_void(dst_ptr), ptr_to_void(src_ptr), 1);
		if (err == 0) {
			if (*dst_ptr == '\0') {
				return E_HM_OK;
			}
		} else {
			return E_HM_POSIX_FAULT;
		}
	}
	return E_HM_RANGE;
}
