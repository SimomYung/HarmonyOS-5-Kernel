/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Prototypes of sysfs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 9 09:45:52 2020
 */

#ifndef ULIBS_LIBDEVHOST_SYSFS_H
#define ULIBS_LIBDEVHOST_SYSFS_H

#include <sys/stat.h>
#include <libhmactv/actv.h>
#include <libdevhost/kernfs.h>

struct libdh_sysfs_args {
	void *ops;
	void *ctx;
	mode_t mode;
};

int libdh_sysfs_init(void);

int libdh_sysfs_create_dir(const char *name, unsigned int mode);

int libdh_sysfs_create_dir_ns(const char *name, unsigned int mode, unsigned int nsid);

int libdh_sysfs_create_file(const char *name, struct libdh_kernfs_ops *fops,
			    const struct libdh_sysfs_args *create_args,
			    int *tag_out);

int libdh_sysfs_create_file_simple(const char *name, unsigned int mode, int id);
int libdh_sysfs_create_file_simple_ex(const char *name, unsigned int mode, unsigned long long size,
									  int id, unsigned int nsid);

int libdh_sysfs_create_symlink(const char *name, mode_t mode,
							   const char *linkdata);

int libdh_sysfs_create_symlink_ns(const char *name, mode_t mode,
								  const char *linkdata, unsigned int nsid);

int libdh_sysfs_delete(const char *name);

int libdh_sysfs_delete_simple(const char *name, int *tagid, unsigned int nsid);

int libdh_sysfs_notify(uintptr_t kernfs_node_ctx, const char *name);

int libdh_sysfs_rename_link(const char *old_name, const char *new_name,
							unsigned int old_nsid, unsigned int new_nsid);

int libdh_sysfs_read(unsigned int tagid, unsigned long long pos,
		     const struct hm_actv_buf *ubuf, uintptr_t ctx,
		     size_t *rsize);

int libdh_sysfs_write(unsigned int tagid, unsigned long long pos,
		      const struct hm_actv_buf *ubuf, uintptr_t ctx,
		      size_t *wsize);

int libdh_sysfs_poll(unsigned int tagid, uintptr_t ctx, bool pollable);

int libdh_sysfs_notify_insert(uintptr_t kernfs_node_ctx);

void libdh_sysfs_notify_remove(uintptr_t kernfs_node_ctx);

const struct libdh_sysfs_args *
libdh_sysfs_acquire_args(const struct kernfs_entry *_entry);

int libdh_fs_check(uintptr_t cnode_index);

size_t libdh_sysfs_entry_size(void);

void *libdh_sysfs_entry_malloc(void);

void libdh_sysfs_entry_free(void *ptr);

int libdh_sysfs_devmgr_create_batch(void *ldk_entry, int cnt);
#endif /* ULIBS_LIBDEVHOST_SYSFS_H */
