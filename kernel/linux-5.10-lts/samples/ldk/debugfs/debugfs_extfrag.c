// SPDX-License-Identifier: GPL-2.0
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/debugfs.h>
#include <asm/uaccess.h>

#include "hmextfrag.h"

#define MAX_LEN 1024
#define DEVICE_NAME ("extfrag")

static struct dentry *root_dentry;

static ssize_t extfrag_index_node_read(struct file *filp, char __user *ubuf,
				       size_t len, loff_t *off)
{
	ssize_t ret = 0;

	if (*off == 0 && len != 0) {
		char buf[MAX_LEN] = {'\0'};
		int cnt = extfrag_index_read(buf, MAX_LEN);
		if (cnt > 0) {
			ret = simple_read_from_buffer(ubuf, len, off, buf, cnt);
		}
	}

	return ret;
}

static struct file_operations extfrag_index_node_ops = {
	.read = extfrag_index_node_read,
	.write = NULL,
};

static int __init extfrag_debugfs_init(void)
{
	int err = 0;
	struct dentry *sub_dentry;

	root_dentry = debugfs_create_dir(DEVICE_NAME, NULL);
	if (IS_ERR(root_dentry)) {
		err = PTR_ERR(root_dentry);
		return err;
	}

	sub_dentry = debugfs_create_file("extfrag_index", 0444,
					 root_dentry, NULL, &extfrag_index_node_ops);
	if (IS_ERR(sub_dentry)) {
		debugfs_remove_recursive(root_dentry);
		err = PTR_ERR(sub_dentry);
	}

	return err;
}

static void __exit extfrag_debugfs_exit(void)
{
	debugfs_remove_recursive(root_dentry);
}

MODULE_LICENSE("GPL");
module_init(extfrag_debugfs_init);
module_exit(extfrag_debugfs_exit);
