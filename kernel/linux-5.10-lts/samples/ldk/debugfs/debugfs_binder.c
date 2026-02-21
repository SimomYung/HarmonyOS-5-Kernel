// SPDX-License-Identifier: GPL-2.0
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/debugfs.h>
#include <asm/uaccess.h>
#include "hmbinder_info.h"

#define DEVICE_NAME ("binder")

struct binder_node {
	char *node_name;
	int index;
};

static struct binder_node g_nodes[] = {
	{"stats", 0},
	{"state", 1},
	{"transactions", 2},
	{"transaction_log", 3},
	{"failed_transaction_log", 4},
};

static DEFINE_MUTEX(binder_lock);
static struct dentry *root_dentry;
static uint64_t fsmgr_rref;

static ssize_t do_debugfs_binder_read(struct file *filp, char __user *buf,
				      size_t len, loff_t *off, int index)
{
	int ret = 0;
	struct binderfs_private_buffer *pdata;

	pdata = filp->private_data;
	if (pdata == NULL)
		return -EINVAL;

	mutex_lock(&binder_lock);
	if (pdata->data == NULL) {
		pdata->rref = fsmgr_rref;
		ret = hmbinder_info_read_remote(pdata, index);
		if (ret < 0)
			pr_err("debugfs binder: read failed, index=%d, ret=%d\n", index, ret);
	}
	mutex_unlock(&binder_lock);

	if (ret == 0)
		ret = simple_read_from_buffer(buf, len, off, pdata->data, pdata->size);

	return ret;
}

static ssize_t binder_debugfs_node_read(struct file *filp, char __user *buf,
					size_t len, loff_t *off)
{
	struct binder_node *node = (struct binder_node *)filp->f_inode->i_private;

	if (node == NULL)
		return -EINVAL;

	return do_debugfs_binder_read(filp, buf, len, off, node->index);
}

static int binder_debugfs_node_open(struct inode *node, struct file *filp)
{
	int err = 0;
	struct binderfs_private_buffer *pdata = kzalloc(sizeof(struct binderfs_private_buffer), GFP_KERNEL);

	filp->private_data = pdata;
	if (filp->private_data == NULL)
		return -ENOMEM;
	return 0;
}

static int binder_debugfs_node_close(struct inode *node, struct file *filp)
{
	if (filp->private_data != NULL) {
		hmbinder_info_release(filp->private_data);
		kfree(filp->private_data);
		filp->private_data = NULL;
	}
	return 0;
}

static struct file_operations binder_node_ops = {
	.open = binder_debugfs_node_open,
	.read = binder_debugfs_node_read,
	.release = binder_debugfs_node_close,
};

static int __init binder_debugfs_init(void)
{
	int err = 0;
	uint32_t i;
	uint32_t count = ARRAY_SIZE(g_nodes);
	struct dentry *sub_dentry;

	err = hmbinder_info_acquire_fsmgr_rref(&fsmgr_rref);
	if (err != 0)
		return err;

	root_dentry = debugfs_create_dir(DEVICE_NAME, NULL);
	if (IS_ERR(root_dentry)) {
		hmbinder_info_release_fsmgr_rref(fsmgr_rref);
		err = PTR_ERR(root_dentry);
		return err;
	}

	for (i = 0U; i < count; i++) {
		sub_dentry = debugfs_create_file(g_nodes[i].node_name, 0444, 
						root_dentry, &g_nodes[i], &binder_node_ops);
		if (IS_ERR(sub_dentry)) {
			debugfs_remove_recursive(root_dentry);
			hmbinder_info_release_fsmgr_rref(fsmgr_rref);
			err = PTR_ERR(sub_dentry);
			break;
		}
	}

	return err;
}

static void __exit binder_debugfs_exit(void)
{
	debugfs_remove_recursive(root_dentry);
	hmbinder_info_release_fsmgr_rref(fsmgr_rref);
}

MODULE_LICENSE("GPL");
module_init(binder_debugfs_init);
module_exit(binder_debugfs_exit);
