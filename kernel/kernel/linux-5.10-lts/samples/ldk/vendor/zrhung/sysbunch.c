// SPDX-License-Identifier: GPL-2.0
#include <hmkernel/errno.h>
#include <libbunch/bunch.h>
#include <hmsysmgr/dumpstack.h>
#include <libbunch/bunch_ipc.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/errno.h>

#include "sysbunch.h"

static int read_buffer_from_bunch(struct bunch *bunch, char *dest, int dest_size)
{
	int rc = E_HM_OK;
	char *cur_idx = dest;
	const char *src = NULL;
	unsigned int src_size = 0;
	unsigned int total_count = 0;
	unsigned int left_size = dest_size;
	unsigned int copy_size;

	total_count = (unsigned int)bunch_read_int32(bunch);
	if (total_count == 0 || dest_size == 0) {
		return E_HM_INVAL;
	}

	for (unsigned int count = 0; count < total_count; count++) {
		src = bunch_read_string(bunch);
		src_size = strlen(src);
		/* size of dest buffer must at least 1 greater than copy size */
		copy_size = (src_size > (left_size - 1)) ? (left_size - 1) : src_size;
		rc = strncpy_s(cur_idx, left_size, src, copy_size);
		if (rc != 0) {
			hm_error("strcpy buffer failed, left_size=%u, src_size=%u, errno:%d\n",
					left_size, src_size, rc);
			rc = E_HM_FAULT;
			break;
		}
		left_size -= copy_size;
		cur_idx += copy_size;
		if (left_size == 0U) {
			break;
		}
	}

	if (left_size == 0) {
		dest[dest_size - 1] = '\0';
	} else {
		dest[dest_size - left_size] = '\0';
	}

	return rc;
}

int save_cur_stack_trace_tsk(int tid, char *buff, int size)
{
	int ret = 0;
	struct bunch *bunch = NULL;
	unsigned int flags;

	flags = (0x1U << (DUMPSTACK_FLAGS_STACK_BACKTRACE))
		| (0x1U << (DUMPSTACK_FLAGS_SYMBOL_NO_LOAD))
		| (0x1U << (DUMPSTACK_FLAGS_SYMBOL_RELATIVE_ADDR))
		| (0x1U << (DUMPSTACK_FLAGS_SYMBOL_NO_FRAMEPOINTER));
	bunch = bunch_ipc_create(size);

	ret = hm_dump_thread_to_bunch(tid, flags, bunch);
	if (ret != 0) {
		hm_error("bunch dump thread failed, tid=%d, err=%s\n", tid, hmstrerror(ret));
		goto err_val;
	}

	ret = read_buffer_from_bunch(bunch, buff, size);
	if (ret != 0) {
		hm_error("bunch read thread failed, tid=%d, err=%s\n", tid, hmstrerror(ret));
		goto err_val;
	}

err_val:
	bunch_ipc_destroy(bunch);
	return -hmerrno2posix(ret);
}
