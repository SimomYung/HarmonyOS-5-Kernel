/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: hmlog switch stack
 * Author: Huawei OS Kernel Lab
 * Create: Sep 3 16:00:20 2024
 */

#ifndef __ASSEMBLY__
#define __ASSEMBLY__
#endif

#include <hmasm/pac.h>
#ifdef PAC_FWCFI_SYSSERVICE
#include <generated/pac.h>
#endif

.global arch_hmlog_vprintf_switch_stack
.type arch_hmlog_vprintf_switch_stack, %function

/*
 * int arch_hmlog_vprintf_switch_stack(enum hmlog_level level,
 *                                     const char *func,
 *                                     int line,
 *                                     const char *fmt,
 *                                     va_list args,
 *                                     void *stack,
 *                                     void *vrpintf_func)
 *
 * Note: struct va_list is on the orignal stack and its pointer args is passed by x4.
 */
arch_hmlog_vprintf_switch_stack:
#ifdef PAC_BWCFI
	pac_bwcfi_sign
#endif
	mov x7, sp
	mov sp, x5
	/* save fp and lr */
	stp x29, x30, [sp, #-32]!
	/* save original stack */
	str x7, [sp, #16]
	blr_pacfix x6, x7, PAC_MODIFIER_HMLOG_VPRINTF
	/* restore fp and lr */
	ldp x29, x30, [sp]
	/* restore original stack */
	ldr x7, [sp, #16]
	mov sp, x7
#ifdef PAC_BWCFI
	pac_bwcfi_retaut
#else
	ret
#endif
