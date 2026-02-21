// SPDX-License-Identifier: GPL-2.0
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/debugfs.h>
#include <asm/uaccess.h>
#include "hmxpm.h"

#define DEVICE_NAME ("xpm")

static DEFINE_MUTEX(xpm_lock);
static struct dentry *root_dentry;

static ssize_t xpm_debugfs_node_read(struct file *filp, char __user *buf,
				     size_t len, loff_t *off)
{
	ssize_t ret = 0;

	mutex_lock(&xpm_lock);
	if (*off == 0 && len != 0) {
		ret = xpm_mode_read(buf, len);
		if (ret > 0) {
			*off += ret;
		} else {
			*off += len;
		}
	}
	mutex_unlock(&xpm_lock);

	return ret;
}

static ssize_t xpm_debugfs_node_write(struct file *filp, const char __user *buf,
				      size_t len, loff_t *off)
{
	ssize_t ret = 0;

	mutex_lock(&xpm_lock);
	if (*off == 0 && len != 0) {
		ret = xpm_mode_write(buf, len);
		if (ret > 0) {
			*off += ret;
			ret = len;
		} else {
			*off += len;
		}
	}
	mutex_unlock(&xpm_lock);

	return ret;
}

static struct file_operations xpm_node_ops = {
	.read = xpm_debugfs_node_read,
	.write = xpm_debugfs_node_write,
};

static int __init xpm_debugfs_init(void)
{
	int err = 0;
	struct dentry *sub_dentry;

	root_dentry = debugfs_create_dir(DEVICE_NAME, NULL);
	if (IS_ERR(root_dentry)) {
		err = PTR_ERR(root_dentry);
		return err;
	}

	sub_dentry = debugfs_create_file("xpm_mode", 0600,
					 root_dentry, NULL, &xpm_node_ops);
	if (IS_ERR(sub_dentry)) {
		debugfs_remove_recursive(root_dentry);
		err = PTR_ERR(sub_dentry);
	}

	return err;
}

static void __exit xpm_debugfs_exit(void)
{
	debugfs_remove_recursive(root_dentry);
}

MODULE_LICENSE("GPL");
module_init(xpm_debugfs_init);
module_exit(xpm_debugfs_exit);
