/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
#ifndef _IMPROPER_ALLOC_H
#define _IMPROPER_ALLOC_H

#include <linux/gfp.h>
#include <linux/seq_file.h>

void improper_alloc_show(struct seq_file *m);
void improper_alloc_hook(gfp_t gfp_mask, unsigned int order, size_t size);

#endif
