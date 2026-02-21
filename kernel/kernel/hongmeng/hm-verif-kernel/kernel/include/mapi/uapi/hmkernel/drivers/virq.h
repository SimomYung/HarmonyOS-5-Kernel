/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Audit kernel uapi interface header file
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 29 10:17:33 CST 2021
 */

#ifndef UAPI_HMKERNEL_VIRQ_H
#define UAPI_HMKERNEL_VIRQ_H

#include <hmkernel/types.h>
#include <hmkernel/capability.h>

#define __VIRQ_CMD_BIND 	1
#define __VIRQ_CMD_UNBIND 	2
#define __VIRQ_CMD_GRANT 	3
#define __VIRQ_CMD_TRANSLATE	4

struct __virq_bind_s {
	cref_t cref;
	__u32 virt_irq;
	int retval;
};

struct __virq_unbind_s {
	__u32 virt_irq;
};

struct __virq_grant_s {
	__u32 virt_irq;
	__u32 cnode_idx;
};

struct __virq_translate_s {
	__u32 hw_irq;
};

#endif
