/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  Header file for iov iter
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 5 16:20:14 2021
 */

#ifndef __IOV_ITER_H_
#define __IOV_ITER_H_

#include <stdlib.h>
#include <sys/uio.h>

struct iov_iter {
	const struct iovec *iov;
	unsigned long nr_iov;

	unsigned long curr_index;	 /* iov index current processing */
	size_t curr_offset;		 /* offset in current iov */

	size_t size_left;		 /* space or data left of all iovs */
};

int iov_iter_init(struct iov_iter *i, const struct iovec *base, unsigned long nr_iov);

/**
 * Get space or data left of current processing iov.
 */
size_t iov_iter_curr_left(const struct iov_iter *i);

/**
 * Get ptr of current processing iov, caller can read or write by this ptr,
 * and the size available is returned by @iov_iter_curr_left
 */
void* iov_iter_curr_ptr(const struct iov_iter *i);

/**
 * Forward the ptr by len bytes. When len is greater than current left, ptr will move to
 * the avaiable iov in order until to the last iov.
 */
void iov_iter_forward(struct iov_iter *i, size_t len);

/**
 * Get the space or data left of all iovs in @iov_iter.
 */
size_t iov_iter_left(const struct iov_iter *i);

/**
 * Copy data between @iov_iter and a user ptr, the @len parameter specifies how many bytes to copy.
 * These series function always return the bytes copied, when returns less than zero,
 * error is occured.
 */
ssize_t copy_data_to_iter(struct iov_iter *dst, const char *src, size_t len);
ssize_t copy_data_to_caller_iter(struct iov_iter *dst, const char *src, size_t len);
/**
 * Copy data to iterator by user customized copy function
 */
ssize_t copy_data_to_iter_customize(struct iov_iter *dst, const char *src, size_t len,
				   int (*copy_fun)(void *dst, const void *src, unsigned long n));
ssize_t copy_data_from_iter(struct iov_iter *src, char *dst, size_t len);

/**
 * @dst is data in our address space, @src is in the actv-caller address space
 */
ssize_t copy_caller_vm_to_iter(struct iov_iter *dst, const char *src, size_t len);

/**
 * @src is data in caller address space, @dst is in our address space
 */
ssize_t copy_data_from_caller_iter(struct iov_iter *src, char *dst, size_t len);
/**
 * Copy data from iterator by user customized copy function
 */
ssize_t copy_data_from_iter_customize(struct iov_iter *src, char *dst, size_t len,
				     int (*copy_fun)(void *dst, const void *src, unsigned long n));
#endif
