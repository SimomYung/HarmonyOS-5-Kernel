// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: the hw_kernel_sg_proc.h for proc file create and destroy
 * Create: 2022-07-23
 */

#ifndef _HW_KERNEL_SG_PROC_H_
#define _HW_KERNEL_SG_PROC_H_

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/uidgid.h>
#include "hw_kernel_sg_utils.h"

#define KSG_PROC_MAX_LEN 1023
#define KSG_PROC_HEX_BASE 16

enum sg_proc_feature {
	KERNEL_SG_SCANNER_REPORT = 0,
	KERNEL_SG_SCANNER_WHITE = 1,
};

int kernel_sg_proc_init(void);
void kernel_sg_proc_exit(void);

#endif
