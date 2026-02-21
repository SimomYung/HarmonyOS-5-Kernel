/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Procfs header file
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 20 15:23:30 2020
 */

#ifndef LIBHMSRV_FS_PROCFS_H
#define LIBHMSRV_FS_PROCFS_H

#include <libsysif/fs/api.h>

typedef struct {
	int tagid;
	uint64_t pos;
	uint64_t vaddr_key;
	uint64_t buf_size;
	uint64_t ctx;
	uint32_t need_passthrough;
}__attribute__((packed)) actvcall_procfs_read_args;

typedef struct {
	int tagid;
	uint32_t need_passthrough;
	uint64_t ctx;
	uint64_t pos;
	uint64_t vaddr_key;
	uint64_t size;
}__attribute__((packed)) actvcall_procfs_write_args;

typedef struct {
	int tagid;
	uint64_t ctx;
	unsigned int flags;
	unsigned int mode;
}__attribute__((packed)) actvcall_procfs_open_args;

typedef struct {
	int tagid;
	uint64_t ctx;
	uint64_t fd;
}__attribute__((packed)) actvcall_procfs_close_args;

/* name: is name relative to procfs mountpoint, for example: "test/test",
 *       will create /proc/test/test.
 * type: could be dir/file/symlink
 * ops: for file, this is read/write handler, it is MUST. For other types, ignored.
 * linkdata: if type is symlink, this is MUST. For other types, it's ignored.
 */
int procfs_create_data(const char *name, unsigned int type,
		       const struct procfs_ops_cb *ops, const char *linkdata);
int procfs_delete_data(const char *name);
int procfs_create_data_net(unsigned int nsid, const char *path,
				    mode_t mode, const struct procfs_ops_cb *ops);
int procfs_delete_data_net_specific(unsigned int nsid, const char *pathname);
int procfs_create_net_proc_entries(struct net_procfs_node *proc_entry_list, unsigned int cnt);
#endif
