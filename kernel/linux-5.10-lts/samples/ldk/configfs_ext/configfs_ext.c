// SPDX-License-Identifier: GPL-2.0
#include <linux/types.h>
#include "hmdfs.h"
#include "configfs_ext_if.h"

static int __init configfs_ext_init(void)
{
	int ret;

	ret = hmdfs_init_configfs();
	if (ret) {
		return ret;
	}

	ret = sharefs_init_configfs();
	if (ret) {
		hmdfs_exit_configfs();
		return ret;
	}

	ret = devhost_register_configfs_ext();
	if (ret) {
		hmdfs_exit_configfs();
		sharefs_exit_configfs();
		return ret;
	}

	return 0;
}
late_initcall(configfs_ext_init);
