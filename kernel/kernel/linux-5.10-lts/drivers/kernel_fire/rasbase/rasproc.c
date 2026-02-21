/*
 * Copyright(C) 2023 Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include "../rasbase/rasbase.h"
#include "rasproc.h"

#include <linux/version.h>

#if KERNEL_VERSION(5, 10, 0) > LINUX_VERSION_CODE
int proc_init(char *name, const struct file_operations *proc_ops, void *data)
#else
int proc_init(char *name, const struct proc_ops *proc_ops, void *data)
#endif
{
	struct proc_dir_entry *pde = 0;

	if (name == 0)
		ras_retn(-ENOMEM);
	pde = proc_create_data(name, S_IFREG | 0666, 0, proc_ops, data); /* 0666 read-write */
	if (unlikely(pde == 0))
		ras_retn(-ENOMEM);
	return 0;
}

void proc_exit(const char *name)
{
	remove_proc_entry(name, 0);
}
