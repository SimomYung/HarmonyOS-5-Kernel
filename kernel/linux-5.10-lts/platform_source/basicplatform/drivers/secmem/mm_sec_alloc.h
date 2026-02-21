/*
 * drivers/secmem/mm_sec_alloc.h
 *
 * Copyright (C) 2022 Hisilicon, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _MM_SEC_ALLOC_H
#define _MM_SEC_ALLOC_H

extern void sec_alloc_init(struct device_node *np);
extern struct cma *get_svc_cma(int id);
extern void set_svc_cma(u64 svc_id, struct cma *cma);
#endif
