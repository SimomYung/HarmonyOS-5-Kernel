/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: libdh prototypes for procfs support
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 19 15:14:30 2020
 */
#ifndef ULIBS_LIBDEVHOST_PROCFS_H
#define ULIBS_LIBDEVHOST_PROCFS_H

#include <fs_procfs.h>
#include <libdevhost/kernfs.h>

/*
 * procfs entry helper APIs, create entry with name
 *  "/proc/devhost/<cnode_idx>/<name>"
 */

#define DEVHOST_PROCFS_PREFIX_FMT	"devhost/%x/"

struct libdh_procfs_args {
	void *ops;
	void *ctx;
	char *pathname;
	uint32_t need_passthrough;
	mode_t mode;
};

/* procfs handler */
int __attribute__((format(printf, 2, 3)))
libdh_procfs_init(bool remote, const char *basefmt, ...);

int __attribute__((format(printf, 1, 3)))
libdh_procfs_mkdir(const char *namefmt, mode_t mode, ...);

int __attribute__((format(printf, 1, 4)))
libdh_procfs_symlink(const char *namefmt, mode_t mode, char *linkdata, ...);

int __attribute__((format(printf, 1, 2)))
libdh_procfs_delete(const char *namefmt, ...);

/* Used to create file not in libdh entry */
int libdh_procfs_create_file_ldk_simple(const char *name,
			 struct libdh_procfs_args *create_args);

int libdh_procfs_delete_ldk_simple(const char *name);

int __attribute__((format(printf, 1, 5)))
libdh_procfs_create_file(const char *namefmt, struct libdh_kernfs_ops *fops,
			 struct libdh_procfs_args *create_args, int *tagid_out, ...);

int libdh_procfs_read(unsigned int tagid, unsigned long long pos,
		      const struct hm_actv_buf *ubuf, uintptr_t ctx,
		      size_t *rsize /* out */);

int libdh_procfs_write(unsigned int tagid, unsigned long long pos,
		       const struct hm_actv_buf *ubuf, uintptr_t ctx,
		       size_t *wsize /* out */);

int libdh_procfs_open_simple(unsigned int tagid, int flags, int mode, uintptr_t ctx, unsigned long long *fd);

int libdh_procfs_close_simple(unsigned int tagid, unsigned long long fd, uintptr_t ctx);

const struct libdh_procfs_args *
libdh_procfs_acquire_args(const struct kernfs_entry *_entry);

int libdh_procfs_set_remote_ops(int (*create)(const char *path, mode_t mode,
					      const struct procfs_ops_cb *ops,
					      const char *linkpath),
				int (*unlink)(const char *path));

#endif	/* ULIBS_LIBDEVHOST_PROCFS_H */
