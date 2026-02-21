/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sun Oct 6 10:29:09 2019
 */
#ifndef UAPI_ASM_PRIVILEGE_LAYOUT
#define UAPI_ASM_PRIVILEGE_LAYOUT

#include <hmkernel/const.h>

#define __PRIVILEGE_NORMAL		0
#define __PRIVILEGE_SYSMGR		1
#define __PRIVILEGE_TMPFS 		2
#define __PRIVILEGE_IDLE		3
#define __PRIVILEGE_DEVHOST		4
#define __NR_PRIVILEGE_SERVICE		5

#ifdef CONFIG_PRIVILEGED_SERVICE
/* VA Gap between different privilged services, default 50MB */
#define __PRIVILEGE_SERVICE_VA_GAP	(__UL(50) << 20)
#else
#define __PRIVILEGE_SERVICE_VA_GAP	__UL(0)
#endif

/* vs_size, vs_size_sysmgr */
#define __PRI_VS_SIZE			0
#define __PRI_VS_SIZE_SYSMGR		1
#define __NR_PRIV_VS_SIZE		2

#ifndef __ASSEMBLY__
/* calculates privileged service start address and vlimit */
static inline unsigned long
__privilege_vstart_addr(unsigned long va_start, unsigned long vs_size,
			unsigned long vs_size_sysmgr, unsigned int privilege_num)
{
	unsigned long vstart = 0UL;

	switch (privilege_num) {
	case __PRIVILEGE_NORMAL:
		break;
	case __PRIVILEGE_SYSMGR:
		vstart = va_start;
		break;
	case __PRIVILEGE_TMPFS:
	case __PRIVILEGE_IDLE:
	case __PRIVILEGE_DEVHOST:
	default:
		vstart = va_start + vs_size_sysmgr + __PRIVILEGE_SERVICE_VA_GAP +
			(vs_size + __PRIVILEGE_SERVICE_VA_GAP) * (privilege_num - __UL(2));
		break;
	}

	return vstart;
}

static inline unsigned long
__privilege_vlimit_addr(unsigned long va_start, unsigned long vs_size,
			unsigned long vs_size_sysmgr, unsigned int privilege_num)
{
	unsigned long vlimit = 0UL;

	switch (privilege_num) {
	case __PRIVILEGE_NORMAL:
		break;
	case __PRIVILEGE_SYSMGR:
		vlimit = va_start + vs_size_sysmgr;
		break;
	case __PRIVILEGE_TMPFS:
	case __PRIVILEGE_IDLE:
	case __PRIVILEGE_DEVHOST:
	default:
		vlimit = va_start + vs_size_sysmgr +
			(vs_size + __PRIVILEGE_SERVICE_VA_GAP) * (privilege_num - __UL(1));
		break;
	}

	return vlimit;
}
#endif

#endif
