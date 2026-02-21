/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Prototypes and type definitions for transfs APIs
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 21 08:00:14 2021
 */
#ifndef __DEVHOST_API_TRANSFS_H__
#define __DEVHOST_API_TRANSFS_H__

#if defined(__KERNEL__) /* linux kernel build env */
#  include <linux/types.h>
#  include <linux/fs.h>
#  include <linux/mount.h>
#  include <linux/dcache.h>
#else /* generic build env */
#  include <stddef.h>
#  include <unistd.h>
#  include <stdint.h>
#  include <sys/types.h>
#  include <sys/uio.h>
struct super_block;
struct vfsmount;
struct inode;
struct dentry;
struct file;
#endif

struct transfs_dn_meta_info_t {
	struct dentry *dentry;
	struct inode *inode;
	uint32_t ino;
};

struct transfs_timespec64 {
	int64_t tv_sec;
	long tv_nsec;
};

struct transfs_vattr {
	unsigned int valid;
	unsigned int mode;
	unsigned int uid;
	unsigned int gid;
	struct transfs_timespec64 atime;
	struct transfs_timespec64 ctime;
	struct transfs_timespec64 mtime;
};

struct transfs_fsid_t {
	int val[2];
};

struct transfs_statfs {
	unsigned long f_type;
	unsigned long f_bsize;
	unsigned long f_blocks;
	unsigned long f_bfree;
	unsigned long f_bavail;
	unsigned long f_files;
	unsigned long f_ffree;
	struct transfs_fsid_t f_fsid;
	unsigned long f_namelen;
	unsigned long f_frsize;
	unsigned long f_flags;
	unsigned long f_spare[4];
};

struct devhost_vma_info;

struct devhost_transfs_ops {
	void (*unmounted)(struct vfsmount *vmnt, int flags, void *data);
	int (*mkdir)(struct dentry *pdentry, mode_t mode, const char *dname,
		     struct transfs_dn_meta_info_t *dn_meta_out);
	int (*rmdir)(struct dentry *pdentry, struct dentry *dentry);
	int (*create)(struct dentry *pdentry, mode_t mode, const char *dname,
		      struct transfs_dn_meta_info_t *dn_meta_out);
	int (*unlink)(struct dentry *pdentry, struct dentry *dentry);
	long long (*write)(struct file *file, uint64_t pos, const void *src, size_t len);
	long long (*read)(struct file *file, uint64_t pos, const void *src, size_t len);
	ssize_t (*write_iter)(struct file *file, uint64_t pos, struct iovec *iov,
			      int nr_segs, size_t count);
	long long (*readpage)(struct vfsmount *vmnt, struct dentry* dentry, unsigned char *buf,
			      unsigned int offset, unsigned int size);
	int (*fsync)(struct file *file);
	int (*mknod)(struct dentry *pdentry, unsigned int mode, unsigned long long devno,
		     const char *dname, struct transfs_dn_meta_info_t *dn_meta_out);
	int (*link)(struct dentry *old_dentry, struct dentry *new_pdentry,
		    const char *dname, struct dentry **res_new_dentry);
	int (*symlink)(struct dentry *pdentry, const char *dname,
		       const char *path, struct transfs_dn_meta_info_t *dn_meta_out);
	int (*statfs)(struct vfsmount *vmnt, struct dentry *dentry,
		      struct transfs_statfs *transfs_st);
	int (*rename)(struct dentry *old_pdentry, struct dentry *old_dentry,
		      struct dentry *new_pdentry, struct dentry *new_dentry, const char *dname);
	int (*readdir)(struct file *file, uint64_t *pos, void *buf, size_t size);
	int (*lookup)(struct dentry *pdentry, const char *dname, void *meta,
		      struct transfs_dn_meta_info_t *dn_meta_out);
	int (*setattr)(struct dentry *dentry, struct transfs_vattr *vattr, uint64_t size);
	int (*readlink)(struct dentry *dentry, char *buf, size_t bufsize);
	int (*open)(struct vfsmount *vmnt, struct dentry *dentry, unsigned int f_modes,
		    unsigned int f_flags, struct file **res_file);
	int (*fallocate)(struct file *file, unsigned int mode, uint64_t pos, uint64_t len);
	int (*getattr)(struct vfsmount *vmnt, struct dentry *dentry, void *attr);
	long long (*listxattr)(struct dentry *dentry, char *buffer, size_t size);
	long long (*getxattr)(struct dentry *dentry, int prefix, const char *name,
			      char *value, size_t size);
	long long (*setxattr)(struct dentry *dentry, int prefix, const char *name,
			      const char *value, size_t size, int flags);
	int (*tmpfile)(struct dentry *pdentry, mode_t mode, const char *dname,
		       struct transfs_dn_meta_info_t *dn_meta_out);
	int (*sync)(struct super_block *sb);
	int (*drop_caches)(struct super_block *sb, unsigned int mode, int max_nr);
	void (*close)(struct file *file);
	int (*ioctl)(struct file *file, unsigned int cmd, unsigned long arg);
	long long (*direct_IO)(struct file *file, uint64_t offset, void *buf,
			       size_t len, unsigned int type);
	void (*kill_dentry)(struct dentry *dentry);
	void (*node_put)(struct inode *inode);
	long (*update_opts)(struct super_block *sb, char *buf, size_t buflen);
	int (*io_read)(struct vfsmount *vmnt, struct inode *inode, char *buf,
		       unsigned int size, unsigned long long offset, bool direct_map_page);
	int (*io_write)(struct vfsmount *vmnt, struct inode *inode, char *buf,
			unsigned int size, unsigned long long offset, bool direct_map_page);
	int (*iommap)(struct file *file, struct devhost_vma_info *vma);
};

struct devhost_transfs_file_ops {
	int (*get_file_info) (void *file, unsigned int *me_index, unsigned long long *hm_filp);
	int (*set_file_info) (void *file, unsigned int me_index, unsigned long long hm_filp);
};

struct transfs_mounted_ret {
	struct inode *inode;
	struct dentry *dentry;
	struct super_block *sb;
	struct vfsmount *vmnt;
	struct devhost_transfs_ops *ops;
	size_t block_size;
	unsigned int root_lnkcnt;
	void *data;
	uint32_t s_time_gran;
	int64_t s_max_time;
	int64_t s_min_time;
};

typedef int (*transfs_mounted_t)(const char *fs_name, const char *dev_path,
				 const char *mount_opts, int mount_flags,
				 struct transfs_mounted_ret *mount_ret);

typedef int (*ubi_major2num)(unsigned int major);
typedef void (*transfs_dget_t)(struct dentry *dentry);
void *devhost_transfs_get_dentry(const char *pathname);
void devhost_transfs_put_dentry(void *sb, void *dentry);
int devhost_transfs_inode_permission(const char *pathname, int mask);
void *devhost_transfs_fget(int fd, void (*fget)(void*));
void devhost_transfs_fput(void *file);
int devhost_register_transfs(transfs_mounted_t func, const struct devhost_transfs_file_ops *fops,
			     ubi_major2num ubi_major2num_func,
			     transfs_dget_t dget_func);
void transfs_iterate_supers(void (*f)(struct super_block *sb, void *data));

#endif /* __DEVHOST_API_TRANSFS_H__ */
