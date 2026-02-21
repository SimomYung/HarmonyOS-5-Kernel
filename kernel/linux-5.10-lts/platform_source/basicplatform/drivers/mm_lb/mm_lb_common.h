/*
 * Copyright (c) 2022-2022 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __MM_LB_COMMON_H
#define __MM_LB_COMMON_H

#include <linux/types.h>
#include <linux/mutex.h>

#define LB_ERROR  1
#define LB_WARN   3
#define LB_INFO   5

static int lb_d_lvl = LB_WARN;

#define lb_print(level, x...)    \
	do {                         \
		if (lb_d_lvl >= (level)) \
			pr_err(x);           \
	} while (0)

 typedef struct {
	struct list_head list;
	struct mutex mutex;
	struct gen_pool *pool;
} vm_info;

typedef struct {
	struct list_head next;
	struct vm_struct *area;
} lb_area;

int add_gid_vm(vm_info *vm, size_t sz);
void *vm_map(vm_info *vm, struct page **pages, size_t cnt, pgprot_t prot);
void vm_unmap(vm_info *vm, const void *va, size_t sz);

#endif