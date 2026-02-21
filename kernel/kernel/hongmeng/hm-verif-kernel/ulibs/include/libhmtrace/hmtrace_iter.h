/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Interator API for hmtrace
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 27 19:30:32 2020
 */
#ifndef ULIBS_LIBHMTRACE_HMTRACE_ITER_H
#define ULIBS_LIBHMTRACE_HMTRACE_ITER_H

#include <unistd.h>
#include <libhmtrace/efmt.h>
#include <libstrict/strict.h>

struct efmt_iter_data {
	int eid;
	struct event_format *efmt;
};

/*
 * iterator operations. Users pass opertion function pointers to iterator
 * iterator starts session with begin, and find next position when user pass the
 * handle in read APIion by next, and prensent user's data by show, and finish
 * the session by end
 */
struct iter_operations {
	char *name; /* iterator name */
	void *(*begin)(const char *param); /* function pointer to return start pointer of IPC */
	void (*end)(const char *param); /* function pointer to do last work */
	void *(*next)(const char *param, void *pos); /* function pointer to return next pointer of IPC */
	/* function pointer to encode and store data */
	int (*encode_write)(void *buffer, size_t buf_size, void *pos);
	/* function pointer to decode and read data */
	int (*decode_read)(void *buffer, size_t buf_size, void **out);
};

typedef const struct iter_operations *(*match_operations_fn_t)(const char *name);

/*
 * Register hook for iterator opertations. The hook return iter_operations pointer
 * according to the name passed by user
 *
 * @param	func	[I]	hook functions for match_operations
 * @return None
 */
void register_match_operations_fn(match_operations_fn_t func);

const struct iter_operations *match_hmtrace_ipc_iter_operations(const char *name);

#endif
