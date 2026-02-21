/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Interface of thread futex vsyscall
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 21 16:32:46 2018
 */
#include <sys/time.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/syscall.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <hongmeng/types.h>
#include <hmasm/lsyscall.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/atomic.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_futex.h>
#include <libhwsecurec/securec.h>
#include "../vsyscall.h"

long hm_sys_futex(va_list va)
{
	struct thread_futex_args args;

	args.uaddr1 = va_arg(va, int *);
	args.futex_op = (int)va_arg(va, long);
	args.val1 = (int)va_arg(va, long);
	args.ts_or_val2 = va_arg(va, unsigned long);
	args.uaddr2 = va_arg(va, int *);
	args.val3 = (int)va_arg(va, long);

	return lsyscall_syscall6(__NR_futex,
				ptr_to_long(args.uaddr1),
				args.futex_op, args.val1, (long)args.ts_or_val2,
				ptr_to_long(args.uaddr2), args.val3);
}
