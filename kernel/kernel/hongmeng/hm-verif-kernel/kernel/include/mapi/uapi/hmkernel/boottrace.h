/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: boottrace stages.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Dec 9 16:20:11 2023
 */

#ifndef UAPI_HMKERNEL_BOOTTRACE_H
#define UAPI_HMKERNEL_BOOTTRACE_H

#define __BOOTTRACE_ELFLOADER_START	0
#define __BOOTTRACE_ELFLOADER_FINISH	1
#define __BOOTTRACE_KERNEL_START	2
#define __BOOTTRACE_KERNEL_POST_START	3
#define __BOOTTRACE_KERNEL_FINISH	4
#define __BOOTTRACE_KERNEL_NR		5

#define __BOOTTRACE_SYSMGR_START		10
#define __BOOTTRACE_SYSMGR_PRE_INIT		12
#define __BOOTTRACE_SYSMGR_EARLY_BOOT_DONE	14
#define __BOOTTRACE_SYSMGR_MEM_INIT		16
#define __BOOTTRACE_SYSMGR_PROCESS_INIT		18
#define __BOOTTRACE_SYSMGR_BOOT_DONE		20

#endif
