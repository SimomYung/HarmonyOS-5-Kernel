/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Output tsan log and handle backtrace
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 01 20:17:18 2022
 */

#include "macro.h"
#include "tsan_report.h"
#include "tsan_algo.h"

#ifdef TSAN_SHARED
#include <execinfo.h>
#endif
#include <libtsan/tsan.h>
#include <libhmlog/hmlog.h>

#ifdef TSAN_SHARED
#define DEFAULT_SYM_LEN 256
#define DEFAULT_CALL_STACK_LEVEL 128
#endif

static tsan_backtrace_callback_type tsan_backtrace_callback = NULL;
void tsan_set_backtrace_callback(tsan_backtrace_callback_type back)
{
	tsan_backtrace_callback = back;
}

static void tsan_backtrace(void)
{
#ifdef TSAN_SHARED
	void *frame_buf[DEFAULT_CALL_STACK_LEVEL] = {0};
	char sym_buf[DEFAULT_SYM_LEN] = {0};

	int nptrs = backtrace(frame_buf, DEFAULT_CALL_STACK_LEVEL);
	char **res = backtrace_symbols(frame_buf, nptrs);
	if (res == NULL) {
		uext_error("failed to get function symbol !\n");
		return;
	}

	for (int i = 0; res != NULL && i < nptrs; i++) {
		int ret = snprintf_s(sym_buf, DEFAULT_SYM_LEN, DEFAULT_SYM_LEN - 1, "%s\n", res[i]);
		if (ret < 0) {
			uext_error("failed to generate symbol buffer !\n");
		} else {
			uext_log("%s", sym_buf);
		}
	}
	free(res);
#endif
}

void tsan_handle_race(void)
{
	tsan_backtrace();
	/*
	 * for statc link, libc backtrace function can't be used
	 * so we allow one to register self-defined backtrace function to libtsan
	 * which is called here when data race detected
	 * */
	if (tsan_backtrace_callback != NULL) {
		tsan_backtrace_callback((unsigned int)(HMLOG_INFO));
	}
}
