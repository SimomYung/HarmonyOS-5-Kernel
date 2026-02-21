// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: the hw_kernel_sg_scanner.h for kernel sg trigger scan
 * Create: 2022-07-23
 */

#ifndef _HW_KERNEL_SG_SCANNER_H_
#define _HW_KERNEL_SG_SCANNER_H_

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include "hw_kernel_sg_utils.h"
#include <security/security_guard_collect.h>

#define KSG_SCAN_LIST_MAX_SIZE 100

struct kernel_sg_scanner_node {
	struct list_head sg_scanner_list; /* kernel sg scanner list */
	sg_callback callbackfunc; /* detection modules callbackfunc */
};

struct kernel_sg_scan_work {
	sg_callback callbackfunc; /* detection modules callbackfunc */
	struct work_struct work;
};

int kernel_sg_scanner_init(void);

void kernel_sg_scanner(void);

void kernel_sg_scanner_exit(void);

#endif /* _HW_KERNEL_SG_SCANNER_H_ */

