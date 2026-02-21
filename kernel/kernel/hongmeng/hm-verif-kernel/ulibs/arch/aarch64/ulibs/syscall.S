/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 25 12:57:57 2018
 */

/*
 * svc number: from 0 to 0xffff
 * Encoding:
 *
 * |      1       |    1     | 111111111 | 11111 |
 * | proxy or not | 32 or 64 |  method   |  cap  |
 */

#ifndef __ASSEMBLY__
#define __ASSEMBLY__
#endif

#include "cap_numbers.h"

#define LOCAL_FUNCTION_LABEL(x) .type x,@function; x:
#define LOCAL_FUNCTION(x) LOCAL_FUNCTION_LABEL(x) .cfi_startproc
#define FUNCTION(x) .global x; LOCAL_FUNCTION(x)
#define END_FUNCTION(x) .cfi_endproc; .size x, . - x

#define SYSCALL(name, svcno)	\
FUNCTION(name);	\
	svc svcno;\
	ret;	\
END_FUNCTION(name) ;

#define __FAST_SYSCALL(name, no)	\
	SYSCALL(__syscall_fast_##name, __FAST_SYSCALL_SVCNO(no));
#include <hmasm/kern_syscall.h>

#define CAPERRNO(a, b, c, d)
#define CAPMETHOD(cap, method)	\
SYSCALL(__syscall_cap_##cap##method, __CAP_SYSCALL_SVCNO(CAP_NUM_##cap, \
	CAPMETHOD_NUM_##cap##method)) \
SYSCALL(__sysproxycall_cap_##cap##method, __CAP_SYSCALL_PROXY_SVCNO(CAP_NUM_##cap, \
	CAPMETHOD_NUM_##cap##method))

#define CAPTYPE(cap, num, skip, methods) \
	methods

#include <hmkernel/capability/captypes.h>
