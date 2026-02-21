/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: binfmt_miscfs header file
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 10 03:10:03 2025
 */

#ifndef SYSMGR_INTAPI_PROCESS_BINFMT_H
#define SYSMGR_INTAPI_PROCESS_BINFMT_H

#include <stddef.h>
#include <lib/dlist.h>
#include <intapi/vfs/binfmt_miscfs.h>
#include <libsysif/fs/api.h>

struct file;
/* file credentials */
struct loader_file_cred {
	struct vfs_file_cred file_cred;
	uint32_t cnode_idx; /* the creating process's cnode_idx */
	struct file *elf_file;
	/* the cnode_idx to specify the process whose root will be used */
	uint32_t cnode_idx_for_root;
	bool mnt_nosuid;
	bool file_cred_exist; /* if file_cred_exist, no need to get it again. */
};

struct elf_fs_context {
	bool file_without_fd; /* some elf_file don't have any fd, opened by dentry_open. */
	unsigned long fd_file;
	size_t size;
	void *data;
	struct loader_file_cred *loader_cred;
};

void binfmt_misc_insert(struct dlist_node *node);
void binfmt_misc_delete_all(void);
void binfmt_misc_delete(struct dlist_node *node);

#endif
