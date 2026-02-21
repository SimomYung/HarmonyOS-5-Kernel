/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: declare of transfs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 1 13:58:38 2021
 */

#ifndef ULIBS_LIBHMSRV_TRANSFS_H
#define ULIBS_LIBHMSRV_TRANSFS_H

#include <stdlib.h>
#include <libhmsrv_io/type.h>
#include <libsysif/devmgr/api.h>

struct hmsrv_io_transfs_mnt_ctx {
	struct hmsrv_io_ctx ctx;
	/* super block index */
	unsigned int sb_index;
};

struct hmsrv_io_transfs_mount_info {
	const char *mount_opts;
	unsigned long mount_flags;
	/* Number of bits occupied by the dentry index in the I/O sector */
	unsigned int sector_node_bits;
	unsigned int trans_per_write_size;
	const char *devname;
};

struct hmsrv_io_transfs_node_ctx {
	unsigned int v_index;
};

struct hmsrv_io_transfs_dentry_ctx {
	unsigned int d_index;
};

struct hmsrv_io_transfs_file_ctx {
	unsigned int f_index;
	const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx;
};

/* Passed to devhost as slowdata */
struct hmsrv_io_transfs_mount_recv {
	dev_t dev_id;
	int dev_mode;
	int mount_flags;
	size_t per_write_size;
	unsigned int sector_node_bits;
};

/* transfs operations about mount */
int hmsrv_transfs_mounted(dev_t dev_id, int dev_mode, const char *fs_name,
			  const struct hmsrv_io_transfs_mount_info *mnt_info,
			  struct hmsrv_io_transfs_mnt_ctx **mnt_ctx);

unsigned int hmsrv_transfs_get_sb_index(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx);

unsigned int hmsrv_transfs_get_rv_index(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx);

uint32_t hmsrv_transfs_get_dev_id(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx);

unsigned int hmsrv_transfs_get_rd_index(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx);

unsigned int hmsrv_transfs_get_r_lnkcnt(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx);

size_t hmsrv_transfs_get_block_size(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx);

uint32_t hmsrv_transfs_get_time_gran(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx);

int64_t hmsrv_transfs_get_max_time(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx);

int64_t hmsrv_transfs_get_min_time(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx);

int hmsrv_transfs_unmounted(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx, int flags);

int hmsrv_transfs_drop_caches(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			      unsigned int mode, int max_nr);

/* transfs operations about dentry */
int hmsrv_transfs_mkdir(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			const struct hmsrv_io_transfs_dentry_ctx *pd_ctx,
			mode_t mode, const char *dname, size_t len,
			struct transfs_dn_info_t *dn_info_out);

int hmsrv_transfs_rmdir(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			const struct hmsrv_io_transfs_dentry_ctx *pd_ctx,
			const struct hmsrv_io_transfs_dentry_ctx *d_ctx);

int hmsrv_transfs_create(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *pd_ctx,
			 mode_t mode, const char *dname, size_t len,
			 struct transfs_dn_info_t *dn_info_out);

int hmsrv_transfs_unlink(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *pd_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *d_ctx);

ssize_t hmsrv_transfs_readpage(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			       const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
			       unsigned char *buf, unsigned int offset,
			       unsigned int size);

int hmsrv_transfs_mknod(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			const struct hmsrv_io_transfs_dentry_ctx *pd_ctx,
			unsigned int mode, dev_t devno, const char *dname,
			size_t len, struct transfs_dn_info_t *dn_info_out);

int hmsrv_transfs_link(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
		       const struct hmsrv_io_transfs_dentry_ctx *old_d_ctx,
		       const struct hmsrv_io_transfs_dentry_ctx *new_pd_ctx,
		       const char *dname, size_t len,
		       struct transfs_dn_info_t *dn_info_out);

int hmsrv_transfs_symlink(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			  const struct hmsrv_io_transfs_dentry_ctx *pd_ctx,
			  const char *dname, size_t dlen,
			  const char *path, unsigned int len,
			  struct transfs_dn_info_t *dn_info_out);

struct statfs;
int hmsrv_transfs_statfs(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
			 struct statfs *buf);

int hmsrv_transfs_rename(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *old_pd_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *old_d_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *new_pd_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *new_d_ctx,
			 const char *dname, size_t len);

int hmsrv_transfs_lookup(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *pd_ctx,
			 const char *dname, size_t len, void *meta,
			 struct transfs_dn_info_t *dn_info_out);

int hmsrv_transfs_setattr(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			  const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
			  const void *attr, uint64_t size);

ssize_t hmsrv_transfs_readlink(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			       const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
			       char *buf, size_t bufsize);

int hmsrv_transfs_getattr(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			  const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
			  void *meta);

ssize_t hmsrv_transfs_listxattr(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
				const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
				char *buffer, size_t size);

ssize_t hmsrv_transfs_getxattr(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			       const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
			       int prefix, const char *name, char *value,
			       size_t size);

ssize_t hmsrv_transfs_setxattr(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			       const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
			       int prefix, const char *name, const char *value,
			       size_t size, int flags);

int hmsrv_transfs_tmpfile(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			  const struct hmsrv_io_transfs_dentry_ctx *pd_ctx,
			  mode_t mode, const char *dname, size_t len,
			  struct transfs_dn_info_t *dn_info_out);

int hmsrv_transfs_kill_dentry(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			      const struct hmsrv_io_transfs_dentry_ctx *d_ctx);

int hmsrv_transfs_node_put(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			   const struct hmsrv_io_transfs_node_ctx *v_ctx);

/* transfs operations about node */
int hmsrv_transfs_sync(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx);

/* transfs operations about file */
ssize_t hmsrv_transfs_write(const struct hmsrv_io_transfs_file_ctx *f_ctx,
			    uint64_t pos, const void *src, size_t len);

ssize_t hmsrv_transfs_read(const struct hmsrv_io_transfs_file_ctx *f_ctx,
			    uint64_t pos, const void *src, size_t len);

ssize_t hmsrv_transfs_write_iter(const struct hmsrv_io_transfs_file_ctx *f_ctx,
				 uint64_t pos, const void *iov, int nr_segs, size_t count);

int hmsrv_transfs_fsync(const struct hmsrv_io_transfs_file_ctx *f_ctx);

int hmsrv_transfs_readdir(const struct hmsrv_io_transfs_file_ctx *f_ctx,
			  uint64_t *pos, void *buf, size_t size);

int hmsrv_transfs_open(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
		       const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
		       unsigned int f_modes, unsigned int f_flags,
		       struct hmsrv_io_transfs_file_ctx *f_ctx);

int hmsrv_transfs_fallocate(const struct hmsrv_io_transfs_file_ctx *f_ctx,
			    unsigned int mode, uint64_t pos, uint64_t len);

int hmsrv_transfs_close(const struct hmsrv_io_transfs_file_ctx *f_ctx);

int hmsrv_transfs_ioctl(const struct hmsrv_io_transfs_file_ctx *f_ctx,
			unsigned int cmd, unsigned long arg);

ssize_t hmsrv_transfs_direct_IO(const struct hmsrv_io_transfs_file_ctx *f_ctx,
				uint64_t offset, void *buf, size_t len,
				unsigned int type);

int hmsrv_transfs_update_opts(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			      char *buf, size_t buflen);

int hmsrv_transfs_iommap(const struct hmsrv_io_transfs_file_ctx *f_ctx,
			 const struct vfs_iommap_info *info, struct vfs_iommap_ret *iommap_ret);
#endif
