// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/namei.h>
#include <linux/file.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>

#define BUF_SIZE (32 * PAGE_SIZE)
static void *buff;

static int ldk_file_open(struct inode *inode, struct file *file)
{
	if (buff != NULL) {
		pr_info("test vmalloc user buff is ready\n");
		return 0;
	}
	buff = vmalloc_user(BUF_SIZE);
	if (buff == NULL) {
		pr_err("test_vmalloc_remapp case: vmalloc_user fail.\n");
		return -ENOMEM;
	}
	return 0;
}

static int ldk_file_mmap(struct file *f, struct vm_area_struct *vma)
{
	pr_info("test_vmalloc_remap case: mmap start: %s (%d) kaddr 0x%lx\n", current->comm, current->pid, buff);
	return remap_vmalloc_range(vma, buff, vma->vm_pgoff);
}

static const struct file_operations ldk_files_fops = {
    .open       = ldk_file_open,
	.mmap		= ldk_file_mmap,
};

static struct miscdevice ldk_files_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "ldk_vmalloc_remap_dev",
	.fops	= &ldk_files_fops
};

static int __init vmalloc_remap_test_init(void)
{
	pr_info("test_vmalloc_remap case\n");
	return misc_register(&ldk_files_dev);
}

static void __exit vmalloc_remap_test_deinit(void)
{
	if (buff != NULL) {
		vfree(buff);
		buff = NULL;
	}
	misc_deregister(&ldk_files_dev);
}
module_init(vmalloc_remap_test_init);
module_exit(vmalloc_remap_test_deinit);
