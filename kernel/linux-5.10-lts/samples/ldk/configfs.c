// SPDX-License-Identifier: GPL-2.0
#include <linux/path.h>
#include <linux/configfs.h>

#include <trace/hooks/liblinux.h>

static void hook_gdentry(void *args, const char *pathname,
			 struct path *path, int *ret)
{
	if (path == NULL) {
		*ret = -ENOENT;
		return;
	}

	path->dentry = liblinux_pal_gdentry(pathname);
	if (path->dentry == NULL) {
		*ret = -ENOENT;
	} else {
		*ret = 0;
		path->mnt = NULL;
	}
}
INIT_VENDOR_HOOK(ldk_rvh_gdentry, hook_gdentry);

static void hook_pdentry(void *args, void *sb, void *dentry)
{
	liblinux_pal_pdentry(sb, dentry);
}
INIT_VENDOR_HOOK(ldk_rvh_pdentry, hook_pdentry);
