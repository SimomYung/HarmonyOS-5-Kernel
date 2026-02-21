/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: This is the header of udk vfs interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 22 10:12:53 2022
 */
#ifndef __UDK_VFS_H__
#define __UDK_VFS_H__

#include <udk/types.h>
#include <udk/char.h>

struct stat;
struct iovec;
struct file;
int udk_vfs_open(const char *filename, unsigned int flag, mode_t mode);
int udk_vfs_close(int fd);
ssize_t udk_vfs_pread(int fd, int64_t pos, void *buf, size_t nbyte);
ssize_t udk_vfs_sysmgr_preadv(struct file *file, const struct iovec *vec, int vlen, int64_t pos);
ssize_t udk_vfs_pwrite(int fd, int64_t pos, const void *buf, size_t nbyte);
ssize_t udk_vfs_sysmgr_pwritev(struct file *file, const struct iovec *vec, int vlen, int64_t pos);
void udk_vfs_set_file_mode(struct file *file, unsigned int mode);
void udk_vfs_unset_file_mode(struct file *file, unsigned int mode);
int udk_vfs_fstat(int fd, struct stat *stat);
int udk_vfs_ioctl(int fd, unsigned int cmd, unsigned long arg, int arg_sz);
int udk_ksys_fstat_nocheck(int fd, struct stat *stat);
bool udk_vfs_is_dev_mounted(dev_t major, dev_t minor);
int udk_vfs_fsync(int fd);
int udk_vfs_transfer_fd(int send_fd);
int udk_vfs_fd_get_path(int fd, char *path_buf, unsigned int path_len);
bool udk_vfs_need_fsync(int fd, uint32_t cnode_idx);

/* anon device/file */
struct anon_file {
	/*
	 * always keep ctx as the first member of anon_file,
	 * anon_file can be freed in native_chrdev_close
	 */
	struct udk_char_context ctx;
	struct filp_node *filp_node;
	struct udk_char_operations *fops;
};

/* udk anon file/device */
struct udk_char_operations;
int udk_anon_unused_fd_get(unsigned int flags);
void udk_anon_unused_fd_put(int fd);
void udk_anon_fd_install(unsigned int fd, struct anon_file *file);
struct anon_file *udk_anon_file_alloc(const char *name,
				      struct udk_char_operations *fops,
				      void *ctx, unsigned long flags);
struct anon_file *udk_anon_fget(int fd);
void udk_anon_fput(struct anon_file *file);

typedef int (*encrypt_decrypt_alg_func)(void *src_vec, void *dst_vec, unsigned long vec_cnt,
    void* crypt_ctx, bool enc, bool endecrypt_for_fs);
int udk_vfs_encrypt_decrypt_alg_register(encrypt_decrypt_alg_func endecrypt, const char *alg_name);
#endif /* __UDK_VFS_H__ */
