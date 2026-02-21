// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/namei.h>
#include <linux/file.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/mman.h>

#define LDK_MM_ALLOC_MEM	1U
#define LDK_MM_QUERY_MEM	2U

static vm_fault_t test_ldk_fault(struct vm_fault *vmf)
{
	return VM_FAULT_SIGBUS;
}

static void test_ldk_vma_close(struct vm_area_struct *area)
{
	pr_info("vma unmap: 0x%lx + 0x%lx\n", area->vm_start, area->vm_end);
}

const struct vm_operations_struct test_vm_ops = {
	.fault = test_ldk_fault,
	.close = test_ldk_vma_close,
};

static long ldk_files_ioctl(struct file *fdev, unsigned int cmd, unsigned long args)
{
	struct vm_area_struct *vma = NULL;
	int ret = -ENOSYS;
	unsigned long addr;

	if (cmd == LDK_MM_ALLOC_MEM) {
		addr = vm_mmap(fdev, 0, PAGE_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, 0);
		if (IS_ERR_VALUE(addr)) {
			ret = addr;
			pr_err("alloc mem failed\n");
		} else {
			ret = 0;
			if (copy_to_user((void *)args, &addr, sizeof(long))) {
				ret = -EFAULT;
				vm_munmap(addr, PAGE_SIZE);
				pr_err("return mem failed\n");
			}
		}
	}
	if (cmd == LDK_MM_QUERY_MEM) {
		vma = find_vma(current->mm, args);
		if (vma != NULL && vma->vm_ops == &test_vm_ops) {
			pr_info("find_vma succ\n");
			ret = 0;
		} else {
			pr_err("find_vma failed\n");
			ret = -EINVAL;
		}
	}

	return ret;
}

static int ldk_file_mmap(struct file *f, struct vm_area_struct *vma)
{
	vma->vm_ops = &test_vm_ops;
	return 0;
}

static const struct file_operations ldk_files_fops = {
	.unlocked_ioctl	= ldk_files_ioctl,
	.mmap		= ldk_file_mmap,
};

static struct miscdevice ldk_files_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "ldk_mm_dev",
	.fops	= &ldk_files_fops
};

static int __init find_vma_test_init(void)
{
	pr_info("test find_vma case\n");
	return misc_register(&ldk_files_dev);
}

static void __exit find_vma_test_deinit(void)
{
	misc_deregister(&ldk_files_dev);
}
module_init(find_vma_test_init);
module_exit(find_vma_test_deinit);
