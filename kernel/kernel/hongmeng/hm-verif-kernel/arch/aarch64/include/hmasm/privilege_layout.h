/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sun Oct 6 10:29:09 2019
 */
#ifndef AARCH64_ASM_PRIVILEGE_LAYOUT_H
#define AARCH64_ASM_PRIVILEGE_LAYOUT_H

#include <hmasm/memory.h>
#include <uapi/hmasm/privilege_layout.h>

#ifdef CONFIG_PRIVILEGED_SERVICE
#define PRIVILEGE_VS_SIZE		CONFIG_AARCH64_PRIV_VS_SIZE
#define PRIVILEGE_VS_SIZE_SYSMGR	CONFIG_AARCH64_PRIV_VS_SIZE_SYSMGR
#else
#define PRIVILEGE_VS_SIZE		0
#define PRIVILEGE_VS_SIZE_SYSMGR	0
#endif

#define PRIVILEGE_NORMAL		__PRIVILEGE_NORMAL
#define PRIVILEGE_SYSMGR		__PRIVILEGE_SYSMGR
#define PRIVILEGE_IDLE			__PRIVILEGE_IDLE
#define PRIVILEGE_TMPFS			__PRIVILEGE_TMPFS
#define PRIVILEGE_DEVHOST		__PRIVILEGE_DEVHOST
#define NR_PRIVILEGE_SERVICE		__NR_PRIVILEGE_SERVICE

#define PRIVILEGE_SERVICE_VA_GAP	__PRIVILEGE_SERVICE_VA_GAP

#define privilege_vstart_addr(n)	__privilege_vstart_addr(VA_START, PRIVILEGE_VS_SIZE, PRIVILEGE_VS_SIZE_SYSMGR, n)
#define privilege_vlimit_addr(n)	__privilege_vlimit_addr(VA_START, PRIVILEGE_VS_SIZE, PRIVILEGE_VS_SIZE_SYSMGR, n)
#define PRIVILEGE_VA_LIMIT		privilege_vlimit_addr((NR_PRIVILEGE_SERVICE - 1U))

#endif
