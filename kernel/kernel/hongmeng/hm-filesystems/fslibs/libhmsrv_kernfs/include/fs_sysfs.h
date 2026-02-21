/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Sysfs libs header file
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 20 15:25:09 2020
 */

#ifndef LIBHMSRV_FS_SYSFS_H
#define LIBHMSRV_FS_SYSFS_H

#include <libsysif/fs/api.h>

/*
 * the valid nsid is from 0 to 8192,
 * use 8193 to identify nodes that do not distinguish nsid.
 */
#define KERNFS_COMMON_NSID 8193

struct sysfs_method_info {
	unsigned int read_method_id;
	unsigned int write_method_id;
};

typedef struct {
	 /* ctx: private data */
	int (*read)(void *ctx, void *dst, size_t pos, size_t size, size_t *rsize);
	int (*write)(void *ctx, void *src, size_t pos, size_t size, size_t *wsize);
} sysfs_file_ops;

typedef struct {
	int tagid;
	uint64_t fd;
	uint64_t pos;
	uint64_t vaddr_key;
	unsigned long long buf_size;
	uint64_t ctx;
	uint32_t need_rcache;
}__attribute__((packed)) actvcall_sysfs_read_args;

typedef struct {
	int tagid;
	uint64_t fd;
	uint64_t ctx;
	uint64_t pos;
	unsigned long long size;
}__attribute__((packed)) actvcall_sysfs_write_args;

typedef struct {
	int tagid;
	bool pollable;
	uint64_t ctx;
}__attribute__((packed)) actvcall_sysfs_poll_args;

/* maybe other attrs also need to be passed, such as uid and gid */
struct sysfs_create_file_info {
	const char *name;
	mode_t mode;
	size_t size;
};

/*
 * General APIs.
 *
 * name: path name relative to sysfs mount point, for example: "test/test",
 *       will create /sys/test/test.
 */
int sysfs_create_dir(const char *name, mode_t mode);
int sysfs_create_dir_ns(const char *name, mode_t mode, unsigned int nsid);
int sysfs_create_file(const char *name, mode_t mode, const struct sysfs_ops_cb *ops);
int sysfs_create_symlink(const char *name, mode_t mode, const char *linkdata);
int sysfs_create_symlink_ns(const char *name, mode_t mode, const char *linkdata, unsigned int nsid);
int sysfs_delete(const char *name, int *tagid);
int sysfs_delete_ns(const char *name, int *tagid, unsigned int nsid);
int sysfs_notify(const char *name, size_t name_size);
int sysfs_rename_link(const char *old_path, const char *new_path);
int sysfs_rename_link_ns(const char *old_path, const char *new_path,
						 unsigned int old_nsid, unsigned int new_nsid);
int sysfs_data_create_batch(ldk_sysfs_entry_t *ldk_enrty, struct sysfs_ops_cb *ops_arr, int cnt);

struct sysfs_svp_info {
	const char *svpname;
	unsigned int read_method_id;
	unsigned int write_method_id;
	unsigned int need_rcache;
};

/*
 * Helper APIs.
 *
 * Helper functions for easier usage when create/read/write/delete sysfs files.
 * They will help to manage ops,ctx -> tagid convesion and read/write auto dispatch.
 */
int sysfs_create_file_helper(const char *name, mode_t mode, sysfs_file_ops *fops, void *ctx,
			     const struct sysfs_svp_info *svp_info);
int sysfs_read_dispatch(const actvcall_sysfs_read_args *read_args);
int sysfs_write_dispatch(const actvcall_sysfs_write_args *write_args,  size_t len);
int sysfs_delete_helper(const char *name);
int __sysfs_delete_helper(const char *name, bool free_ctx);

int sysfs_create_file_attr(struct sysfs_create_file_info *f_info,
			   const struct sysfs_ops_cb *ops, unsigned int nsid);
int sysfs_create_file_attr_helper(struct sysfs_create_file_info *f_info,
				  sysfs_file_ops *fops, void *ctx,
				  const struct sysfs_svp_info *svp_info);

#endif
