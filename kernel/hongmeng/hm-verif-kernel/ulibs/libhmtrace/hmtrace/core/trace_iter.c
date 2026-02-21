/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: IPC buffer iterator for hmtrace
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 27 16:57:44 2020
 */
#include "trace_iter.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_mutex_guard.h>

#define MAX_ITERS	8
#define CLIENT_BUF_SZ	(1UL << 18)

/*
 * iter_t management. A iter_t handle shall be allocated, when user calls
 * ipc_buf_open.
 * If iter[handle] is NULL , it means the slot/handle is free, we could
 * allocate the slot/handle to the user.
 */
typedef struct {
	struct raw_mutex lock; /* mutex lock of this struct */
	iter_t *iter[MAX_ITERS]; /* iter_t array */
	int nr_iters; /* elements number of iter_t */
} ipc_buf_iters_t;

static ipc_buf_iters_t buffer_iters = {
	.nr_iters 			= 0,
	.lock				= RAW_MUTEX_INITIALIZER,
	.iter[0 ... (MAX_ITERS - 1)]	= 0,
};

static match_operations_fn_t	match_operations;

static bool is_handle_valid(int handle)
{
	return (handle >= 0 && handle < MAX_ITERS);
}

/*
 * Find iter_t according to handle
 *
 * @param	handle	[I]	the handle o f iter_t
 *
 * @return	pointer of iter_t if success, or NULL if fail
 * @note	Accessing ipc_buf_iters_t structure without mutex.
 */
static iter_t *_find_iter(int handle)
{
	if (!is_handle_valid(handle)) {
		return NULL;
	}

	return buffer_iters.iter[handle];
}

static iter_t *iter_get(int handle)
{
	ipc_buf_iters_t  *iters = &buffer_iters;
	iter_t *iter = NULL;

	RAW_MUTEX_GUARD(_, &iters->lock);
	iter = _find_iter(handle);
	if (iter == NULL) {
		hm_warn("iter is NULL\n");
		return NULL;
	}
	if (iter->refcnt == 0) {
		iter->refcnt++;
	} else {
		hm_warn("iter refcnt is 0\n");
		return NULL;
	}

	return iter;
}

static void iter_put(iter_t *iter)
{
	ipc_buf_iters_t  *iters = &buffer_iters;

	RAW_MUTEX_GUARD(_, &iters->lock)
	if (iter->refcnt == 1) {
		iter->refcnt--;
	} else {
		hm_warn("iter->refcnt should be 1, but it's %d\n", iter->refcnt);
	}
}

/*
 * Allocate free slot for iterator
 *
 * @param	op	[I]	pointer of iter_operations to be registered
 *
 * @return	iterator handle if success, or error number if fail
 */
static int allocate_free_iter(const struct iter_operations *op,
			      const char *param)
{
	ipc_buf_iters_t  *iters = &buffer_iters;

	RAW_MUTEX_GUARD(_, &iters->lock);
	if (iters->nr_iters >= MAX_ITERS) {
		return E_HM_NOMEM;
	}

	for (int i = 0; i < MAX_ITERS; i++) {
		iter_t *iter = _find_iter(i);

		/* NULL means this slot is free */
		if (iter != NULL) {
			continue;
		}

		iter = (iter_t *)malloc(sizeof(iter_t));
		if (iter == NULL) {
			return E_HM_NOMEM;
		}

		/* handle is the index of iter in iters */
		iter->op = op;
		iter->pos = NULL;
		iter->finished = false;
		iter->refcnt = 0;

		iter->param = strdup(param);
		if (iter->param == NULL) {
			free(iter);
			return E_HM_NOMEM;
		}

		iters->iter[i] = iter;
		iters->nr_iters++;
		return i;
	}

	return E_HM_NOMEM;
}

int ipc_buf_srv_open(const char *name)
{
	const struct iter_operations *op = NULL;

	if (name == NULL) {
		hm_warn("invalid iter name\n");
		return E_HM_INVAL;
	}

	if (match_operations == NULL) {
		hm_warn("match_operations is NULL\n");
		return E_HM_POSIX_FAULT;
	}

	op = match_operations(name);
	if (op == NULL) {
		hm_warn("%s isn't registered\n", name);
		return E_HM_NOOBJ;
	}

	return allocate_free_iter(op, name);
}

int ipc_buf_srv_write(int handle, void *buffer, size_t size)
{
	iter_t *iter = NULL;
	const struct iter_operations *op = NULL;
	void *pos = NULL;
	int ret = 0;
	int data_size = 0;

	if (buffer == NULL) {
		return E_HM_INVAL;
	}

	/* Only one reader could read the same handle at same time */
	iter = iter_get(handle);
	if (iter == NULL) {
		hm_warn("get iter for handle %d failed\n", handle);
		return E_HM_POSIX_FAULT;
	}
	op = iter->op;

	if (iter->finished) {
		iter_put(iter);
		return data_size;
	}

	/* access the node after last ipc */
	pos = iter->pos;
	if (pos == NULL) {
		/* access the node from begin */
		pos = op->begin(iter->param);
	}
	while (pos != NULL) {
		ret = op->encode_write((void *)((uintptr_t)buffer + (uint32_t)data_size),
				       size - (size_t)(uint32_t)data_size, pos);
		if (ret <=  0) {
			if (ret < 0) {
				data_size = E_HM_INVAL;
			}
			/* store curr node to iter->pos */
			iter->pos = pos;
			hm_warn("encode failed\n");
			break;
		}

		/* updata data_size */
		data_size += ret;

		/* acquire the addr of next node */
		pos = op->next(iter->param, pos);
		if (pos == NULL) {
			/* all nodes have been accessed */
			iter->pos = NULL;
			iter->finished = true;
			op->end(iter->param);
		}
	}
	iter_put(iter);

	return data_size;
}

static int _ipc_buf_close(int handle)
{
	ipc_buf_iters_t  *iters = &buffer_iters;
	iter_t *iter = NULL;

	RAW_MUTEX_GUARD(_, &iters->lock);
	iter = _find_iter(handle);
	if (iter == NULL) {
		hm_warn("can't find iter\n");
		return E_HM_INVAL;
	}
	/* fail if reader's reading the IPC buffer */
	if (iter->refcnt != 0) {
		hm_warn("refcnt is %d, not 0\n", iter->refcnt);
		return E_HM_POSIX_FAULT;
	}
	iters->nr_iters--;

	if (is_handle_valid(handle)) {
		free(iters->iter[handle]->param);
		free(iters->iter[handle]);
		iters->iter[handle] = NULL;
	}

	return E_HM_OK;
}

int ipc_buf_srv_close(int handle)
{
	return _ipc_buf_close(handle);
}

int ipc_buf_client_open(const char *name, int _handle)
{
	const struct iter_operations *op = NULL;
	int handle = _handle;

	iter_t *iter = NULL;
	ipc_buf_iters_t  *iters = &buffer_iters;

	if (name == NULL || match_operations == NULL) {
		return E_HM_INVAL;
	}

	op = match_operations(name);
	if (op == NULL) {
		hm_warn("%s isn't registered\n", name);
		return E_HM_NOOBJ;
	}

	iter = (iter_t *)malloc(sizeof(iter_t));
	if (iter == NULL) {
		return E_HM_NOMEM;
	}

	mem_zero_s(*iter);
	/* handle is the index of iter in iters */
	iter->buf = calloc(CLIENT_BUF_SZ, sizeof(char));
	if (iter->buf == NULL) {
		free(iter);
		return E_HM_NOMEM;
	}
	iter->buf_size = CLIENT_BUF_SZ;
	iter->op = op;
	iter->data_size = 0;
	iter->nr_rw = 0;
	iter->refcnt = 0;
	RAW_MUTEX_GUARD(_, &iters->lock);
	handle = handle_index_bits(handle);
	if (!is_handle_valid(handle)) {
		free(iter->buf);
		free(iter);
		return E_HM_INVAL;
	}

	iters->iter[handle] = iter;
	iters->nr_iters++;

	return handle;
}

int ipc_buf_client_read_item(rref_t rref, int handle, void **item, ipc_buf_read_fn buf_read)
{
	int ret = 0;
	iter_t *iter = NULL;
	const struct iter_operations *op = NULL;
	int handle_idx = handle_index_bits(handle);

	if (item == NULL || buf_read == NULL) {
		return E_HM_INVAL;
	}

	iter = iter_get(handle_idx);
	if (iter == NULL) {
		hm_warn("get iter for handle %d failed\n", handle_idx);
		return E_HM_POSIX_FAULT;
	}
	op = iter->op;

	while (ret == 0) {
		/* 0 means there is no data in iter->buf */
		if (iter->nr_rw == 0 || iter->nr_rw >= iter->data_size) {
			iter->nr_rw = 0;
			ret = buf_read(rref, handle, iter->buf, iter->buf_size);
			if (ret < 0) {
				hm_warn("read data from server failed\n");
				break;
			} else if (ret == 0) {
				ret = E_HM_NODATA;
				break;
			}
			iter->data_size = (size_t)(uint32_t)ret;
		}

		ret = op->decode_read((iter->buf + iter->nr_rw), (iter->data_size - iter->nr_rw),
				      item);
		if (ret < 0) {
			hm_warn("decode failed\n");
			break;
		} else if (ret == 0) {
			/* 0 means there is no data in ipc buf */
			iter->nr_rw = 0;
		} else {
			iter->nr_rw += (size_t)(uint32_t)ret;
			ret = E_HM_OK;
			break;
		}
	}

	iter_put(iter);
	return ret;
}

int ipc_buf_client_close(int _handle)
{
	int handle = _handle;
	handle = handle_index_bits(handle);
	return _ipc_buf_close(handle);
}

void trace_iter_register(match_operations_fn_t func)
{
	if (match_operations != NULL) {
		hm_warn("match_operations has been registered\n");
	}
	match_operations = func;
}
