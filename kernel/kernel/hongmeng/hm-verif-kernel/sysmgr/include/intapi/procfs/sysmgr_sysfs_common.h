/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Common interface for sysmgr sysfs
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 31 17:46:59 2020
 */
#ifndef SYSMGR_PROCFS_SYSMGR_SYSFS_COMMON_H
#define SYSMGR_PROCFS_SYSMGR_SYSFS_COMMON_H

#include <inttypes.h>

#include <lib/dlist.h>
#include <fs_sysfs.h>

/* r-------- */
#define SYSFS_MODE_0400		S_IRUSR
/* -w------- */
#define SYSFS_MODE_0200		S_IWUSR
/* r--r----- */
#define SYSFS_MODE_0440		(S_IRUSR | S_IRGRP)
/* r--r--r-- */
#define SYSFS_MODE_0444		(S_IRUSR | S_IRGRP | S_IROTH)
/* rw-rw---- */
#define SYSFS_MODE_0660		(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
/* rw-r--r-- */
#define SYSFS_MODE_0644		(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
/* rwxr-xr-x */
#define SYSFS_MODE_0755		(S_IRWXU | S_IRGRP | S_IXGRP | \
				 S_IROTH | S_IXOTH)
/* rwxrwxrwx */
#define SYSFS_MODE_0777		(S_IRWXU | S_IRWXG | S_IRWXO)

int sysmgr_sysfs_create_dir(const char *name);
int sysmgr_sysfs_create_file(const char *name, mode_t mode,
			     sysfs_file_ops *ops, void *ctx);
int sysmgr_sysfs_delete(const char *name);

#endif
