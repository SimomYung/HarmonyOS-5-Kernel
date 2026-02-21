/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: interface for iov iter
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun  2 14:21:46 2021
 */

#include <stdlib.h>
#include <limits.h>

#include <hongmeng/errno.h>
#include <libhwsecurec/securec.h>
#include <libhmactv/actv.h>
#include <libstrict/strict.h>

#include "libhmiov/iov_iter.h"

struct copy_args {
	struct iov_iter *iter;
	char *data_ptr;
	size_t len;

	void *iter_ptr;
	size_t iter_size;

	void *buff_ptr;
	size_t buff_size;
	int (*copy_fun)(void *dst, const void *src, unsigned long n);
	int to_caller;
};

int iov_iter_init(struct iov_iter *i, const struct iovec *base, unsigned long nr_iov)
{
	int err = E_HM_OK;
	unsigned int j;
	ssize_t total_size = 0;

	if (i == NULL || (base == NULL && nr_iov > 0)) {
		err = E_HM_INVAL;
	} else {
		i->iov = base;
		i->nr_iov = nr_iov;

		i->curr_index = 0;
		i->curr_offset = 0;
		i->size_left = 0;

		for (j = 0; j < nr_iov; ++j) {
			ssize_t len = (ssize_t)(base[j].iov_len);

			/* overflow check */
			if (len < 0 || len > (SSIZE_MAX - total_size)) {
				err = E_HM_OVERFLOW;
				break;
			}

			total_size += len;
		}
	}

	if (err == E_HM_OK) {
		i->size_left = (size_t)total_size;
	}

	return err;
}

size_t iov_iter_left(const struct iov_iter *i)
{
	return i->size_left;
}

size_t iov_iter_curr_left(const struct iov_iter *i)
{
	size_t left = 0;
	const struct iovec *iov = NULL;

	if (i->curr_index < i->nr_iov) {
		iov = &(i->iov[i->curr_index]);
		if (iov->iov_len >= i->curr_offset) {
			left = iov->iov_len - i->curr_offset;
		}
	}

	return left;
}

void* iov_iter_curr_ptr(const struct iov_iter *i)
{
	void *ptr = NULL;
	const struct iovec *iov = NULL;

	if (i->curr_index < i->nr_iov) {
		iov = &(i->iov[i->curr_index]);
		ptr = (void *)((char *)(iov->iov_base) + i->curr_offset);
	}

	return ptr;
}

void iov_iter_forward(struct iov_iter *i, size_t len)
{
	const struct iovec *iov = NULL;
	size_t curr_left;
	size_t size = len;

	while (size > 0 && i->curr_index < i->nr_iov) {
		iov = &(i->iov[i->curr_index]);

		curr_left = iov->iov_len - i->curr_offset;
		if (size >= curr_left) {
			i->curr_offset = 0;
			i->curr_index++;
			i->size_left -= curr_left;
			size -= curr_left;
		} else {
			i->curr_offset += size;
			i->size_left -= size;
			size = 0;
		}
	}
}

static ssize_t do_copy(struct copy_args *arg,
		       int (copy_func)(struct copy_args *arg))
{
	struct iov_iter *i = arg->iter;
	char *buf = arg->data_ptr;
	size_t len = arg->len;
	size_t copied = 0;
	int ret = E_HM_OK;

	while (len > 0 && iov_iter_left(i) > 0) {
		char *ptr = iov_iter_curr_ptr(i);
		size_t curr_left = iov_iter_curr_left(i);
		size_t curr_copy = len;

		if (curr_copy > curr_left) {
			curr_copy = curr_left;
		}

		/* skip zero length iov */
		if (curr_copy == 0) {
			if (i->curr_index < i->nr_iov) {
				i->curr_offset = 0;
				i->curr_index++;
				continue;
			}
			hm_warn("iov_iter in inconsistent state: "
				"size_left=%zu, curr_index=%lu, nr_iov=%lu",
				iov_iter_left(i), i->curr_index, i->nr_iov);
			break;
		}

		arg->iter_ptr = ptr;
		arg->iter_size = curr_copy;
		arg->buff_ptr = (void *)(buf + copied);
		arg->buff_size = curr_copy;
		ret = copy_func(arg);
		if (ret != E_HM_OK) {
			ret = E_HM_RESFAULT;
			break;
		}

		copied += curr_copy;
		len -= curr_copy;
		iov_iter_forward(i, curr_copy);
	}

	return copied > 0 ? (ssize_t)copied : (ssize_t)ret;
}

static int __copy_data_to_iter(struct copy_args *arg)
{
	int ret = memcpy_s(arg->iter_ptr, arg->iter_size,
			   arg->buff_ptr, arg->buff_size);

	return ret == EOK ? E_HM_OK: E_HM_RESFAULT;
}

ssize_t copy_data_to_iter(struct iov_iter *dst, const char *src, size_t len)
{
	ssize_t ret;

	if (src == NULL) {
		ret = E_HM_INVAL;
	} else {
		struct copy_args arg = {
			.len = len,
			.iter = dst,
			.data_ptr = (char *)src,
			.iter_size = 0,
			.iter_ptr = NULL,
			.buff_size = 0,
			.buff_ptr = NULL,
			.copy_fun = NULL,
			.to_caller = 0,
		};
		ret = do_copy(&arg, __copy_data_to_iter);
	}

	return ret;
}

static int __copy_data_to_caller_iter(struct copy_args *arg)
{
	return hm_actv_write_caller_vm(arg->iter_ptr, arg->buff_ptr, arg->buff_size);
}

ssize_t copy_data_to_caller_iter(struct iov_iter *dst, const char *src, size_t len)
{
	ssize_t ret;

	if (src == NULL) {
		ret = E_HM_INVAL;
	} else {
		struct copy_args arg = {
			.iter = dst,
			.len = len,
			.data_ptr = (char *)src,
			.iter_size = 0,
			.iter_ptr = NULL,
			.buff_ptr = NULL,
			.buff_size = 0,
			.copy_fun = NULL,
			.to_caller = 0,
		};
		ret = do_copy(&arg, __copy_data_to_caller_iter);
	}

	return ret;
}

static int __copy_by_customized(struct copy_args *arg)
{
	int ret = E_HM_OK;
	if (arg->copy_fun && arg->to_caller) {
		ret = arg->copy_fun(arg->iter_ptr, arg->buff_ptr, arg->buff_size);
	} else if (arg->copy_fun && !arg->to_caller) {
		ret = arg->copy_fun(arg->buff_ptr, arg->iter_ptr, arg->buff_size);
	}
	return ret;
}

ssize_t copy_data_to_iter_customize(struct iov_iter *dst, const char *src, size_t len,
				   int (*copy_fun)(void *dst, const void *src, unsigned long n))
{
	ssize_t ret;

	if (copy_fun == NULL) {
		return E_HM_INVAL;
	}
	if (src == NULL) {
		ret = E_HM_INVAL;
	} else {
		struct copy_args arg = {
			.iter = dst,
			.len = len,
			.data_ptr = (char *)src,
			.iter_size = 0,
			.iter_ptr = NULL,
			.buff_ptr = NULL,
			.buff_size = 0,
			.copy_fun = copy_fun,
			.to_caller = 1,
		};
		ret = do_copy(&arg, __copy_by_customized);
	}

	return ret;
}

static int __copy_iter_to_data(struct copy_args *arg)
{
	int ret = memcpy_s(arg->buff_ptr, arg->buff_size,
			   arg->iter_ptr, arg->iter_size);

	return ret == EOK ? E_HM_OK: E_HM_RESFAULT;
}

ssize_t copy_data_from_iter(struct iov_iter *src, char *dst, size_t len)
{
	ssize_t ret;

	if (dst == NULL) {
		ret = E_HM_INVAL;
	} else {
		struct copy_args arg = {
			.iter = src,
			.data_ptr = dst,
			.iter_size = 0,
			.len = len,
			.iter_ptr = NULL,
			.buff_ptr = NULL,
			.buff_size = 0,
			.copy_fun = NULL,
			.to_caller = 0,
		};
		ret = do_copy(&arg, __copy_iter_to_data);
	}

	return ret;
}

static int __copy_caller_vm_to_iter(struct copy_args *arg)
{
	return hm_actv_read_caller_vm(arg->iter_ptr, arg->buff_ptr, arg->iter_size);
}

ssize_t copy_caller_vm_to_iter(struct iov_iter *dst, const char *src, size_t len)
{
	ssize_t ret;

	if (src == NULL) {
		ret = E_HM_INVAL;
	} else {
		struct copy_args arg = {
			.iter = dst,
			.data_ptr = (char *)src,
			.iter_ptr = NULL,
			.iter_size = 0,
			.buff_ptr = NULL,
			.len = len,
			.buff_size = 0,
			.copy_fun = NULL,
			.to_caller = 0,
		};
		ret = do_copy(&arg, __copy_caller_vm_to_iter);
	}

	return ret;
}

static int __copy_data_from_caller_iter(struct copy_args *arg)
{
	return hm_actv_read_caller_vm(arg->buff_ptr, arg->iter_ptr, arg->buff_size);
}

ssize_t copy_data_from_caller_iter(struct iov_iter *src, char *dst, size_t len)
{
	ssize_t ret;

	if (dst == NULL) {
		ret = E_HM_INVAL;
	} else {
		struct copy_args arg = {
			.iter_ptr = NULL,
			.iter_size = 0,
			.iter = src,
			.data_ptr = dst,
			.buff_ptr = NULL,
			.buff_size = 0,
			.len = len,
			.copy_fun = NULL,
			.to_caller = 0,
		};
		ret = do_copy(&arg, __copy_data_from_caller_iter);
	}

	return ret;
}

ssize_t copy_data_from_iter_customize(struct iov_iter *src, char *dst, size_t len,
				     int (*copy_fun)(void *dst, const void *src, unsigned long n))
{
	ssize_t ret;

	if (copy_fun == NULL) {
		return E_HM_INVAL;
	}
	if (dst == NULL) {
		ret = E_HM_INVAL;
	} else {
		struct copy_args arg = {
			.iter_ptr = NULL,
			.iter_size = 0,
			.iter = src,
			.data_ptr = dst,
			.buff_ptr = NULL,
			.buff_size = 0,
			.len = len,
			.copy_fun = copy_fun,
			.to_caller = 0,
		};
		ret = do_copy(&arg, __copy_by_customized);
	}

	return ret;
}
