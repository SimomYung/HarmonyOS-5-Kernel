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

typedef struct {
	struct list_head next;
	struct vm_struct *area;
} lb_area;

int add_gid_vm(vm_info *vm, size_t sz);
#ifndef CONFIG_LIBLINUX
void vm_uninit(struct lb_device *lbd);
int vm_init(struct lb_device *lbd);
#endif
void *vm_map(vm_info *vm, struct page **pages, size_t cnt, pgprot_t prot);
void vm_unmap(vm_info *vm, const void *va, size_t sz);
int dsm_init(struct lb_device *lbd);
int soc_init(struct platform_device *pdev, struct lb_device *lbd);
int set_page(struct lb_plc_info *policy, struct page *page_alloc, size_t count);
void reset_page(struct lb_plc_info *policy, struct page *page, size_t count);
#endif