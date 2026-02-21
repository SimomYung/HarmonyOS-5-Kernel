/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Header file for procfs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 31 19:53:08 CST 2023
 */

#ifndef __HM_PROCFS_H_
#define __HM_PROCFS_H_

#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct hmnet_procfs_ldk_entry_ops {
	int (*read)(unsigned long long pos, unsigned long ubuf, unsigned long size,
		    void *ctx, unsigned long *rsize /* out */);
	int (*write)(unsigned long long pos, unsigned long ubuf, unsigned long size,
		     void *ctx, unsigned long *wsize /* out */);
	int (*poll)(int (*callback)(void *ctx, int pollable), int id, int pollable);
	void *(*open)(int id, unsigned int flags, unsigned int mode);
	void (*close)(void *filep);
	void (*flush)(void);
	int (*enter)(void);
	void (*exit)(void);
	void (*unlink)(void *ctx);
};

int hmnet_procfs_create_ldk_entry(unsigned int nsid, const char* path,
				struct hmnet_procfs_ldk_entry_ops *ops,
				void *ctx, unsigned int mode);
int hmnet_procfs_delete_ldk_entry_specific(const char *pathname, unsigned int nsid);

int hmnet_procfs_create_ldk_entries(void *parent, unsigned int size,
			struct hmnet_procfs_ldk_entry_ops *ops);
#ifdef __cplusplus
}
#endif

#endif
