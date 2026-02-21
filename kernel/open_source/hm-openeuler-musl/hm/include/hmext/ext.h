/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Unified hmext interface for rru
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 25 17:07:11 2022
 */

#ifndef HM_INCLUDE_HMEXT_EXT_H
#define HM_INCLUDE_HMEXT_EXT_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define HM_EXT_CMD_HVC_PASSTHROUGH		0x0001
#define HM_EXT_CMD_MM_VA2PA			0x0002
#define HM_EXT_CMD_FLUSH_CACHE			0x0003
#define HM_EXT_CMD_IRQ_OP			0x0004

#define __NR_EXT_CTRL 407
#define __NR_EXT_WAIT 406

#define __ext_syscall(x0, ...) do { \
	__asm__ __volatile__ ( "svc 0" \
	: "=r"(x0) : __VA_ARGS__ : "memory", "cc"); \
	} while (0)

static inline int hm_ext_syscall(uint32_t user_cmd,
				 void *cfg, uint32_t cfg_len,
				 void *rlt, uint32_t rlt_len)
{
	register long x8 __asm__("x8") = (long)__NR_EXT_CTRL;
	register long x0 __asm__("x0") = (long)user_cmd;
	register long x1 __asm__("x1") = (long)cfg;
	register long x2 __asm__("x2") = (long)cfg_len;
	register long x3 __asm__("x3") = (long)rlt;
	register long x4 __asm__("x4") = (long)rlt_len;
	__ext_syscall(x0, "r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4));

	return x0;
}

static inline int hm_ext_wait(unsigned long flags, const struct __hmsigmask *sigmask)
{
	register long x8 __asm__("x8") = (long)__NR_EXT_WAIT;
	register long x0 __asm__("x0") = (long)flags;
	register long x1 __asm__("x1") = (long)sigmask;
	__ext_syscall(x0, "r"(x8), "0"(x0), "r"(x1));

	return x0;
}

#endif
