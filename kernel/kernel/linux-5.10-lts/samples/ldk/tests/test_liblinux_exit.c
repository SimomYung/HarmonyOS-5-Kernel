// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/namei.h>
#include <linux/file.h>
#include <linux/module.h>
#include <linux/cred.h>

static int ldk_file_release(struct inode *knode, struct file *kfile)
{
	struct task_struct *tsk = current;
	BUG_ON(tsk->mm == NULL);
	return 0;
}

static const struct file_operations ldk_files_fops = {
	.owner		= THIS_MODULE,
	.release	= ldk_file_release,
};

static struct miscdevice ldk_test_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "ldk_liblinux_exit_test_dev",
	.fops	= &ldk_files_fops,
};

static int __init liblinux_exit_test_init(void)
{
	pr_info("liblinux_exit_test case\n");
	return misc_register(&ldk_test_dev);
}

static void __exit liblinux_exit_test_deinit(void)
{
	misc_deregister(&ldk_test_dev);
}

module_init(liblinux_exit_test_init);
module_exit(liblinux_exit_test_deinit);

MODULE_LICENSE("GPL");
