/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Internal API of iterator for hmtrace
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 27 19:43:17 2020
 */
#ifndef ULIBS_LIBHMTRACE_HMTRACE_TRACE_ITER_H
#define ULIBS_LIBHMTRACE_HMTRACE_TRACE_ITER_H
#include <unistd.h>
#include <stdbool.h>
#include <hmkernel/capability.h>
#include <libhmtrace/hmtrace_iter.h>

typedef int (*ipc_buf_read_fn)(rref_t rref, int handle, void *buf, size_t size);

#define IPC_BUF_MAX_SIZE	(1UL << 18) /* IPC buffer max size */

/*
 * IPC buffer handle consists of sender src cnode and handle index
 * returned by ipc_buf_open
 * handle bit assignments
 * 31      23  	        7
 * |sign  |  sender src  |handle index|
 */
#define HANDLE_INDEX_SHIFT	8U
#define HANDLE_INDEX_MASK	((1U << HANDLE_INDEX_SHIFT) - 1U)
#define HANDLE_SENDER_SHIFT	24U
#define HANDLE_SENDER_MASK	((1U << HANDLE_SENDER_SHIFT) - 1U)

#define src_sender_bits(src)		((src) & HANDLE_SENDER_MASK)
#define handle_sender_bits(handle)	(((handle) >> HANDLE_INDEX_SHIFT) & HANDLE_SENDER_MASK)
#define handle_comb(sender, idx)	((idx) | (src_sender_bits(sender) << HANDLE_INDEX_SHIFT))

#define handle_index_bits(handle)	(int)((unsigned int)(handle) & HANDLE_INDEX_MASK)

/*
 * iterator management. When user open ipc buffer, we will allocate a iter_t to
 * manage the iterator for this user, and return the handle to user. Itertor could
 * use the handle to find iter_t when user pass the handle in read API.
 */
typedef struct {
	bool finished;
	char *buf; /* buffer of iterator */
	char *param; /* paramerters of iterator */
	size_t buf_size; /* buffer size */
	size_t nr_rw; /* the number of bytes write or read from IPC buffer this time */
	size_t data_size; /* the total data size in ipc buffer */
	void *pos; /* pos where iterator read from for this time ipc */
	const struct iter_operations *op; /* the opertaions of this iterator */
	int refcnt; /* reference count for this structrue */
} iter_t;

/*
 * Open IPC buffer handle, used by hmtrace server
 *
 * @param	name	[I]	IPC buffer name
 *
 * @return	handle of ipc buffer if success, or error number if fail
 */
int ipc_buf_srv_open(const char *name);

/*
 *  Write control plane related data into IPC buffer
 *
 * @param	handle	[I]	IPC buffer handle
 * @param	buffer	[O]	the buffer to store the data from IPC buffer
 * @param	size	[I]	the buffer size
 *
 * @return	the size of data read from IPC buffer if success, or error number if fail
 */
int ipc_buf_srv_write(int handle, void *buffer, size_t size);

/*
 * Close the IPC buffer handle, used by hmtrace server
 *
 * @param	handle	[I]	IPC buffer handle
 *
 * @return	E_HM_OK if success, or error number if fail
 */
int ipc_buf_srv_close(int handle);
/*
 * Open IPC buffer handle in client
 *
 * @param	name	[I]	IPC buffer name
 * @param	handle	[I]	the handle return by server
 * @return	handle of ipc buffer if success, or error number if fail
 */
int ipc_buf_client_open(const char *name, int _handle);

/*
 * Read the item of IPC buffer in client
 *
 * @param	handle		[I]	the handle return by server
 * @param	item		[O]	the item address, should be freed by caller
 * @param	buf_read	[I]	the hook to read from IPC buffer
 */
int ipc_buf_client_read_item(rref_t rref, int handle, void **item, ipc_buf_read_fn buf_read);

/*
 * Close the IPC buffer handle in client
 *
 * @param	handle	[I]	IPC buffer handle
 *
 * @return	E_HM_OK if success, or error number if fail
 */
int ipc_buf_client_close(int _handle);

/*
 * Register hook for iterator opertations. The hook return iter_operations pointer
 * according to the name passed by user
 *
 * @param	func	[I]	hook functions for match_operations
 * @return None
 */
void trace_iter_register(match_operations_fn_t func);
#endif
