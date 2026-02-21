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
#ifndef _PROC_COM_H_
#define _PROC_COM_H_

#include <linux/version.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/file.h>
#include <linux/uaccess.h>

#if KERNEL_VERSION(4, 1, 0) > LINUX_VERSION_CODE
#define FILE_NODE(file) file->f_dentry->d_inode
#else
#define FILE_NODE(file) file->f_path.dentry->d_inode
#endif
#if KERNEL_VERSION(3, 10, 0) > LINUX_VERSION_CODE
#define PDE_DATA(node)   (PDE(node)->data)
#endif

#define proc_ops_name(name) proc_ops_##name
#define proc_ops_open(name)  proc_open_##name
#define proc_ops_show(name)  proc_show_##name
#define proc_ops_write(name)  proc_write_##name

#if KERNEL_VERSION(5, 10, 0) > LINUX_VERSION_CODE
#define proc_ops_init(name) { \
	.open = proc_ops_open(name), \
	.read = seq_read, \
	.write = proc_ops_write(name), \
	.llseek = seq_lseek, \
	.release = single_release, \
}

#define proc_ops_define(name) \
static const struct file_operations proc_ops_name(name) =  proc_ops_init(name)
int proc_init(char *name, const struct file_operations *proc_ops, void *data);
#else
#define proc_ops_init(name) { \
	.proc_open = proc_ops_open(name), \
	.proc_read = seq_read, \
	.proc_write = proc_ops_write(name), \
	.proc_lseek = seq_lseek, \
	.proc_release = single_release, \
}

#define proc_ops_define(name) \
static struct proc_ops proc_ops_name(name) =  proc_ops_init(name)
int proc_init(char *name, const struct proc_ops *proc_ops, void *data);
#endif

void proc_exit(const char *name);
#endif

