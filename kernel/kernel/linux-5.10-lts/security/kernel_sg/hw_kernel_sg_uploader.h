// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: the hw_kernel_sg_uploader.h for kernel data uploading through uevent
 * Create: 2022-07-23
 */

#ifndef _HW_KERNEL_SG_UPLOADER_H_
#define _HW_KERNEL_SG_UPLOADER_H_

#include <linux/kobject.h>
#include <linux/slab.h>
#include <security/security_guard_collect.h>
#include "hw_kernel_sg_utils.h"

#define KSG_UEVENT_LEN  2

#define KSG_REPLACE_WHITE 0
#define KSG_MAX_WHITE_SIZE 100

enum sg_item_fun_type {
	KERNEL_SG_UPLOAD = 0,
	KERNEL_SG_UPLOAD_MAX,
};

struct sg_item_fun {
	enum sg_item_fun_type type;
	int (*func)(const event_info *info, char *upload_info, int upload_size);
};

struct kernel_sg_white {
	struct list_head sg_white; /* kernel sg_white */
	unsigned long event_id; /* white event_id */
};

int kernel_sg_upload_parse(const event_info *info, char *upload_info, int upload_size);

int kernel_sg_uploader_init(void);
void kernel_sg_uploader_exit(void);
int kernel_sg_update_white(char *str);

#endif /* _HW_KERNEL_SG_UPLOADER_H_ */

