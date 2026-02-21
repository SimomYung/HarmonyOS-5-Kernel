// SPDX-License-Identifier: GPL-2.0-only
#include <linux/mm.h>
#include <linux/export.h>
#include <linux/printk.h>
#include <asm/pgtable.h>
#include <asm/uaccess.h>

#include <trace/hooks/liblinux.h>

unsigned long __must_check
__clear_user(void __user *to, unsigned long n)
{
	unsigned long ret = 0;
	unsigned long size = 0;
	mm_segment_t old_fs;

	old_fs = get_fs();
	set_fs(USER_DS);
	while(n > 0) {
		size = (n / PAGE_SIZE) ? PAGE_SIZE : n;
		ret = __arch_copy_to_user(to, page_to_virt(empty_zero_page), size);
		n -= size;
	}
	set_fs(old_fs);

	return ret;
}

unsigned long __must_check
__copy_in_user(void __user *to, const void __user *from, unsigned long n)
{
	int ret;
	ret = liblinux_pal_copy_in_user(to, from, n);
	if (ret < 0) {
		return n;
	};
	return 0;
}

unsigned long __must_check
__arch_copy_to_user(void __user *to, const void *from, unsigned long n)
{
	if ((!to) || (!from)) {
		return n;
	}
	if (get_fs() == USER_DS) {
		int ret = -1;

		trace_ldk_rvh_copy_to_user(to, from, n, &ret);
		if (ret == -1) {
			if (liblinux_pal_copy_to_user(to, from, n) < 0) {
				return n;
			};
			return 0;
		} else {
			return ret;
		}
	} else {
		memcpy(to, from, n);
	}
	return 0;
}

unsigned long __must_check
__arch_copy_from_user(void *to, const void __user *from, unsigned long n)
{
	if ((!to) || (!from)) {
		return n;
	}
	if (get_fs() == USER_DS) {
		if (liblinux_pal_copy_from_user(to, from, n) < 0) {
			/* security hole - plug it */
			memset(to, 0, n);
			return n;
		};
	} else {
		memcpy(to, from, n);
	}
	return 0;
}

EXPORT_SYMBOL(__arch_copy_from_user);
EXPORT_SYMBOL(__arch_copy_to_user);
EXPORT_SYMBOL(__clear_user);
EXPORT_SYMBOL(__copy_in_user);

long copy_from_kernel_nofault(void *dst, const void *src, size_t size)
{
	/* check null-page */
	if ((dst < (void *)PAGE_SIZE) || (src < (void *)PAGE_SIZE)) {
		return -EFAULT;
	}
	memcpy(dst, src, size);
	return 0;
}
long notrace copy_to_kernel_nofault(void *dst, const void *src, size_t size)
{
	/* check null-page */
	if ((dst < (void *)PAGE_SIZE) || (src < (void *)PAGE_SIZE)) {
		return -EFAULT;
	}
	memcpy(dst, src, size);
	return 0;
}
void __weak clear_page(void *to)
{
	memset(to, 0, PAGE_SIZE);
}
