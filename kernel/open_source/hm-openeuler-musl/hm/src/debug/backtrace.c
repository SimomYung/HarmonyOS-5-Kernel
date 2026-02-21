/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implementation of backtrace
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 05 14:15:57 2021
 */
#include <stdlib.h>
#include <unwind.h>
#include <execinfo.h>

struct bt_arg {
	void **buffer;
	int size;
	int count;
};

static _Unwind_Reason_Code unwind_trace_func(struct _Unwind_Context *ctx, void *data)
{
	_Unwind_Reason_Code ret = _URC_NO_REASON;
	struct bt_arg *arg = (struct bt_arg *)data;

	if (ctx == NULL) {
		ret = _URC_END_OF_STACK;
	} else {
		/* always skip current frame which is backtrace function */
		if (arg->count != -1) {
			arg->buffer[arg->count] = (void *)_Unwind_GetIP(ctx);
		}
		arg->count++;
		if (arg->count >= arg->size) {
			ret = _URC_END_OF_STACK;
		} else {
			ret =  _URC_NO_REASON;
		}
	}

	return ret;
}

int backtrace(void **buffer, int size)
{
	int ret = 0;
	struct bt_arg arg;

	if (buffer != NULL && size > 0) {
		arg.buffer = buffer;
		arg.size = size;
		arg.count = -1;
		(void)_Unwind_Backtrace(unwind_trace_func, &arg);

		/* _Unwind_Backtrace seems to put NULL address above start, skip it */
		if (arg.count > 1 && arg.buffer[arg.count - 1] == NULL) {
			arg.count--;
		}
		ret = arg.count != -1 ? arg.count : 0;
	}

	return ret;
}
