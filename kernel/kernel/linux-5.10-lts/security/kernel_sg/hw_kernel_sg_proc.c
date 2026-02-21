// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: the hw_kernel_sg_proc.c for proc file create and destroy
 * Create: 2022-07-23
 */

#include "hw_kernel_sg_proc.h"
#include "hw_kernel_sg_scanner.h"
#include "hw_kernel_sg_uploader.h"
#include <linux/version.h>

#define KSG_LOG_TAG "kernel_sg_proc"

static struct proc_dir_entry *g_proc_entry;
static const umode_t file_creat_ro_mode = 0200;
static const kgid_t kernel_sg_gid = KGIDT_INIT((gid_t)3520);
static const kuid_t kernel_sg_uid = KUIDT_INIT((uid_t)3520);

static int handle_sg_proc_write(unsigned long cmd, char *str)
{
	switch (cmd) {
	case KERNEL_SG_SCANNER_REPORT:
		kernel_sg_scanner();
		ksg_log_trace(KSG_LOG_TAG, "proc trigger scanner done");
		return KSG_SUCCESS;
	case KERNEL_SG_SCANNER_WHITE:
		return kernel_sg_update_white(str);
	default:
		ksg_log_error(KSG_LOG_TAG, "proc cmd unknown %lu", cmd);
		return -EINVAL;
	}
}

static ssize_t kernel_sg_proc_write(struct file *file, const char __user *buffer,
	size_t count, loff_t *pos)
{
	char *sub_str = NULL;
	char *str = NULL;
	char *backup_str = NULL;
	int ret;
	unsigned long cmd = 0;
	(void)file;
	(void)pos;
	if (buffer == NULL) {
		ksg_log_error(KSG_LOG_TAG, "proc_write buffer NULL");
		return -EINVAL;
	}
	str = kzalloc(KSG_PROC_MAX_LEN + 1, GFP_KERNEL);
	if (str == NULL) {
		ksg_log_error(KSG_LOG_TAG, "str kzalloc NULL");
		return -ENOMEM;
	}
	backup_str = str;
	do {
		if ((count <= 0) || (count > KSG_PROC_MAX_LEN) ||
			copy_from_user(str, buffer, count)) {
			ret = -EFAULT;
			ksg_log_error(KSG_LOG_TAG, "copy error, %d", ret);
			break;
		}

		sub_str = strsep(&str, SPLIT_STR);
		if (strlen(sub_str) > KSG_NUMBER_MAX_LEN) {
			ret = -EINVAL;
			ksg_log_error(KSG_LOG_TAG, "str len error");
			break;
		}

		if (kstrtoul(sub_str, KSG_PROC_HEX_BASE, &cmd)) {
			ret = -EINVAL;
			ksg_log_error(KSG_LOG_TAG, "kstr error, %d", ret);
			break;
		}

		ret = handle_sg_proc_write(cmd, str);
	} while (0);

	kfree(backup_str);
	if (ret < KSG_SUCCESS) {
		ksg_log_error(KSG_LOG_TAG, "proc process error, %d", ret);
		return ret;
	}

	ksg_log_trace(KSG_LOG_TAG, "proc write done");
	return count;
}

/*
 * the function is called by kerenl function
 * single_open(struct file *, int (*)(struct seq_file *, void *), void *)
 */
static int kernel_sg_proc_show(struct seq_file *m, void *v)
{
	(void)v;
	seq_printf(m, "kernel_sg proc_show %d", 0);
	return KSG_SUCCESS;
}

static int kernel_sg_proc_open(struct inode *inode, struct file *file)
{
	(void)inode;
	return single_open(file, kernel_sg_proc_show, NULL);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
static const struct file_operations kernel_sg_proc_fops = {
	.owner          = THIS_MODULE,
	.open           = kernel_sg_proc_open,
	.read           = seq_read,
	.write          = kernel_sg_proc_write,
	.llseek         = seq_lseek,
};
#else
static const struct proc_ops kernel_sg_proc_fops = {
	.proc_open          = kernel_sg_proc_open,
	.proc_read          = seq_read,
	.proc_write         = kernel_sg_proc_write,
	.proc_lseek         = seq_lseek,
};
#endif

int kernel_sg_proc_init(void)
{
	g_proc_entry = proc_create("kernel_sg", file_creat_ro_mode, NULL,
		&kernel_sg_proc_fops);
	if (g_proc_entry == NULL) {
		ksg_log_error(KSG_LOG_TAG, "proc_entry create failed");
		return -ENOMEM;
	}

	/* set proc file gid to sg gid */
	proc_set_user(g_proc_entry, kernel_sg_uid, kernel_sg_gid);

	ksg_log_trace(KSG_LOG_TAG, "proc_entry init success");
	return KSG_SUCCESS;
}

void kernel_sg_proc_exit(void)
{
	remove_proc_entry("kernel_sg", NULL);
	ksg_log_trace(KSG_LOG_TAG, "proc_entry cleanup success");
}
