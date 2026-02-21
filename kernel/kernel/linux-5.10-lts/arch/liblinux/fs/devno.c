// SPDX-License-Identifier: GPL-2.0-only
#include <linux/init.h>
#include <liblinux/pal.h>

#include <trace/hooks/liblinux.h>

static
void hook_chrdev_region(void* __data, int action, unsigned int *major_out,
			unsigned int baseminor, int minorct, const char *name,
			int *ret)
{
	unsigned int major = *major_out;

	if (action == VH_REG_REGION) {
		*ret = liblinux_pal_register_chrdev_region(major, baseminor, minorct,
							   name, major_out);
	} else if (action == VH_UNREG_REGION) {
		liblinux_pal_unregister_chrdev_region(major, baseminor, minorct);
	}
}
INIT_VENDOR_HOOK(ldk_rvh_chrdev_region, hook_chrdev_region);

static
void hook_blkdev_region(void* __data, int action, unsigned int *major_out,
			const char *name, int *major_ret)
{
	unsigned int major = *major_out;
	int ret;

	if (action == VH_REG_REGION) {
		ret = liblinux_pal_register_blkdev_region(major, name, major_out);
		if ((major == 0) && (ret == 0)) {
			*major_ret = *major_out;
		} else {
			*major_ret = ret;
		}
	} else if (action == VH_UNREG_REGION) {
		liblinux_pal_unregister_blkdev_region(major, name);
		if (major_ret != NULL) {
			*major_ret = 0;
		}
	}
}
INIT_VENDOR_HOOK(ldk_rvh_blkdev_region, hook_blkdev_region);
