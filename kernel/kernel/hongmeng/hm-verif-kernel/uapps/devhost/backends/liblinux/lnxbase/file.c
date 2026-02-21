/*
* Copyright (c) Huawei Technologies Co., Ltd. 2020-2021 All rights reserved.
* Description: Implement
* Author: Huawei OS Kernel Lab
* Create: Fri Jul 23 23:17:19 2021
*/
#include <lnxbase/lnxbase.h>
#include <sys/stat.h>
#include <sys/uio.h>

#include <liblinux/pal.h>
#include <hongmeng/errno.h>
#include <libsysif/utils.h>
#include <libhmactv/actv.h>
#include <libhmucap/ucap.h>
#include <libdevhost/devhost.h>
#include <hmsysmgr/process/pid.h>
#include <libhwsecurec/securec.h>
#include <devhost/device.h>

#include <fd.h>
#include <vfs.h>

#include <devhost/log.h>
#include <devhost/file.h>
#include <devhost/device.h>

/* for anonymous file */
static struct device_info *anon_devinfo;

static void *lnxbase_default_fget(int fd, void (*fget)(void*));
static void *lnxbase_anon_fget(int fd, void (*fget)(void*));
static lnxbase_fget_fn_t lnxbase_fget_fn[MAX_FD] = {
	lnxbase_default_fget,
	NULL,
	NULL,
};

static lnxbase_fput_fn_t lnxbase_fput_fn[MAX_FD];

static lnxbase_gdentry_fn_t lnxbase_gdentry_fn;
static lnxbase_pdentry_fn_t lnxbase_pdentry_fn;

/* Cache devfs rref for further actvcalls */
#define DEVFS_PATH	"/dev"
static rref_t devfs_rref = 0;
static uint32_t devfs_cnode_idx = 0;

static int lnxbase_devfs_info_init(void)
{
	int srv_fd;
	int err;
	struct server_info *si = NULL;

	srv_fd = vfs_open(DEVFS_PATH, O_PATH, 0);
	if (srv_fd < 0) {
		dh_warn("get server name failed: %s\n", hmstrerror(srv_fd));
		return srv_fd;
	}

	err = get_fsmgr_si(&si);
	if (err != E_HM_OK) {
		dh_warn("get server fd failed\n");
		(void)vfs_close(srv_fd);
		return E_HM_BADF;
	}

	devfs_rref = si->actv_rref;
	devfs_cnode_idx = si->cnode_idx;

	put_server_info(si);
	(void)vfs_close(srv_fd);
	return E_HM_OK;
}

static void lnxbase_devfs_info_clear(void)
{
	devfs_rref = 0;
	devfs_cnode_idx = 0;
}

static int lnxbase_anon_register_devhost(void)
{
	int ret;
	uref_t dh_ap_uref;

	dh_ap_uref = libdh_ap_export(devfs_cnode_idx);
	if (IS_REF_ERR(dh_ap_uref)) {
		dh_error("export actv pool to sysmgr failed, err=%s\n",
			 hmstrerror(REF_TO_ERR(dh_ap_uref)));
		return REF_TO_ERR(dh_ap_uref);
	}

	ret = actvcapcall_fscall_vfs_anon_register_devhost(devfs_rref,
							   dh_ap_uref);
	if (ret != E_HM_OK) {
		(void)hm_ucap_revoke(0, dh_ap_uref);
		dh_error("register devhost to fs failed: %s\n", hmstrerror(ret));
	}
	return ret;
}

/*
 * This function should return posix errno since
 * it is called in devhost.ko
 */
int lnxbase_anon_devinfo_init(struct device_info *anon)
{
	int ret;

	if (anon_devinfo != NULL) {
		dh_warn("anon_devinfo already initialized\n");
		return -hmerrno2posix(E_HM_POSIX_EXIST);
	}

	if (anon == NULL) {
		return -hmerrno2posix(E_HM_INVAL);
	}

	ret = lnxbase_devfs_info_init();
	if (ret != E_HM_OK) {
		dh_error("lnxbase: get devfs rref failed, ret=%d, %s\n", ret, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	ret = lnxbase_anon_register_devhost();
	if (ret != E_HM_OK) {
		dh_error("lnxbase: register devhost to devfs failed, ret=%d, %s\n", ret, hmstrerror(ret));
		lnxbase_devfs_info_clear();
		return -hmerrno2posix(ret);
	}

	ret = lnxbase_register_fget(ANON_FD, lnxbase_anon_fget);
	if (ret != E_HM_OK) {
		dh_error("lnxbase: failed to register fget callback for ANON_FD, ret=%d, %s\n", ret, hmstrerror(ret));
		lnxbase_devfs_info_clear();
		return -hmerrno2posix(ret);
	}

	anon_devinfo = anon;
	return 0;
}

static int query_file_id(int fd, uint32_t app_cnode_idx,
			 unsigned long long *file_idx,
			 unsigned int *fastpath)
{
	int ret;
	struct __actvret_fscall_vfs_query_info fscall_ret = {0};

	if (file_idx == NULL) {
		return E_HM_INVAL;
	}

	/*
	 * pass cnode_idx as 0 and let fs get it automatically, otherwise
	 * fs only allows sysmgr to query other process' info
	 */
	ret = actvxactcapcall_fscall_vfs_query_info(true, true,
						    devfs_rref, fd, 0,
						    IO_DEV_ALL_GET | IO_DEV_CALLER_CHECK, &fscall_ret);
	if (ret != E_HM_OK) {
		dh_error("query vfs info failed, cnode_idx:%u, fd:%d, %s\n",
			 app_cnode_idx, fd, hmstrerror(ret));
		return ret;
	}
	*file_idx = (unsigned long long)fscall_ret.file_idx;
	*fastpath = fscall_ret.fastpath;

	return E_HM_OK;
}

int lnxbase_register_fget(enum liblinux_fd_type fd_type, lnxbase_fget_fn_t fn)
{
	if (fd_type >= MAX_FD) {
		return -EINVAL;
	}

	if (lnxbase_fget_fn[fd_type] != NULL) {
		return -EEXIST;
	}

	lnxbase_fget_fn[fd_type] = fn;
	return 0;
}

int lnxbase_register_fput(enum liblinux_fd_type fd_type, lnxbase_fput_fn_t fn)
{
	if (fd_type >= MAX_FD) {
		return -EINVAL;
	}

	if (lnxbase_fput_fn[fd_type] != NULL) {
		return -EEXIST;
	}

	lnxbase_fput_fn[fd_type] = fn;
	return 0;
}

int lnxbase_register_dentry(lnxbase_gdentry_fn_t gfn, lnxbase_pdentry_fn_t pfn)
{
	if (lnxbase_gdentry_fn != NULL || lnxbase_pdentry_fn != NULL) {
		return E_HM_EXIST;
	}

	lnxbase_gdentry_fn = gfn;
	lnxbase_pdentry_fn = pfn;
	return 0;
}

static void *lnxbase_default_fget(int fd, void (*fget)(void*))
{
	int ret;
	struct filp_node *node = NULL;

	UNUSED(fget);
	/* param fd is file_id in devhost */
	acquire_devhost_filp_node(node, sysif_actv_src_cnode_idx(),
				  0, (unsigned int)fd);
	if (node == NULL) {
		return NULL;
	}
	ret = raw_compounded_auto_refcnt_counter(&node->refcnt);
	BUG_ON(ret <= 1);
	return node->filp;
}

static void *lnxbase_anon_fget(int fd, void (*fget)(void*))
{
	struct filp_node *node = NULL;
	unsigned long long file_idx;
	int ret;
	uint32_t app_cnode_idx = sysif_actv_fwd_cnode_idx();
	unsigned int fastpath;
	void *filp = NULL;

	/* param fd is hm fd, use it to query file_id from fs */
	ret = query_file_id(fd, app_cnode_idx, &file_idx, &fastpath);
	if (ret != E_HM_OK) {
		return NULL;
	}

	/*
	 * As the backends of vfs's devfs, all filp_node's owner is vfs.
	 * So use the vfs_cnode_idx to acquire_devhost_filp_node.
	 * devfs_cnode_idx is vfs's cnode_idx.
	 */
	acquire_devhost_filp_node(node, devfs_cnode_idx, fastpath, file_idx);
	if (node == NULL) {
		return NULL;
	}
	/*
	 * When a node is created, devhost_register_file() holds its
	 * refcnt, and release it in devhost_unregister_file(). So
	 * when we acquire the node, refcnt must be 2 or more.
	 */
	ret = raw_compounded_auto_refcnt_counter(&node->refcnt);
	BUG_ON(ret <= 1);

	/*
	 * In order to ensure the validity of filp_node(in devhost) and
	 * node->filp(in ldk) when the fd is closed and queried concurrently,
	 * it will get filp_node refcnt in query_file_id() before return from vfs,
	 * and get node->filp refcnt in ldk, then put filp_node refcnt.
	 * So the node->filp is valid even if the fd is closed after put filp_node.
	 */
	filp = node->filp;
	fget(filp);
	devhost_put_file(node);

	return filp;
}

lnxbase_fget_fn_t lnxbase_get_fget_fn(enum liblinux_fd_type fd_type)
{
	if (fd_type >= MAX_FD) {
		dh_warn("not support fd type fget:%u\n", (unsigned int)fd_type);
		return NULL;
	}

	return lnxbase_fget_fn[fd_type];
}

lnxbase_fput_fn_t lnxbase_get_fput_fn(enum liblinux_fd_type fd_type)
{
	if (fd_type >= MAX_FD) {
		dh_warn("not support fd type fput:%u\n", (unsigned int)fd_type);
		return NULL;
	}

	return lnxbase_fput_fn[fd_type];
}

lnxbase_gdentry_fn_t lnxbase_get_dentry_fn(void)
{
	return lnxbase_gdentry_fn;
}

lnxbase_pdentry_fn_t lnxbase_put_dentry_fn(void)
{
	return lnxbase_pdentry_fn;
}

int lnxbase_get_unused_fd_flags(unsigned int flags)
{
	int ret;

	ret = actvxactcapcall_fscall_vfs_get_unused_fd(true, true,
						       devfs_rref,
						       flags);
	if (ret < 0) {
		dh_error("get unused fd failed: %s\n", hmstrerror(ret));
	}

	return ret;
}

void lnxbase_fd_install(unsigned int fd, void *file, struct dh_fd_install_context *ctx)
{
	struct filp_node *node = NULL;
	unsigned int dev_fops_flags = 0;
	int ret = 0;

	node = devhost_alloc_filp_node_fast(anon_devinfo);
	if (node == NULL) {
		dh_error("failed to allocate file_node\n");
		return;
	}

	node->filp = (void *)file;
	dev_fops_flags = devhost_device_get_fops_flags(node->devinfo, node->filp);
	if (ctx != NULL) {
		node->f_flags = ctx->f_flags;
		node->f_modes = trans_acc_mode(get_accmode(ctx->f_flags));
	}

	if (ctx == NULL || ctx->file_name == NULL) {
		ret = actvxactcapcall_fscall_vfs_fd_info_update(true, true, devfs_rref,
								(int)fd, ptr_to_u64(node),
								NULL, 0, dev_fops_flags);
	} else {
		hm_actv_set_accessible_mem_range(ptr_to_ulong(ctx->file_name), (unsigned long)ctx->buffer_len);
		ret = actvxactcapcall_fscall_vfs_fd_info_update(true, true, devfs_rref,
								(int)fd, ptr_to_u64(node),
								ctx->file_name, ctx->buffer_len,
								dev_fops_flags);
	}
	if (ret < 0) {
		dh_error("fscall_vfs_fd_info_update failed, err = %d, %s\n",
			 ret, hmstrerror(ret));
		devinfo_put(anon_devinfo);
		free(node);
	}
}

int lnxbase_alloc_unused_fd(unsigned int flags, void *file, struct dh_fd_install_context *ctx)
{
	struct filp_node *node = NULL;
	unsigned int dev_fops_flags = 0;
	int ret = 0;

	node = devhost_alloc_filp_node_fast(anon_devinfo);
	if (node == NULL) {
		dh_error("failed to allocate file_node\n");
		return E_HM_NOMEM;
	}

	node->filp = (void *)file;
	dev_fops_flags = devhost_device_get_fops_flags(node->devinfo, node->filp);
	node->f_flags = flags;
	node->f_modes = trans_acc_mode(get_accmode(flags));

	if (ctx == NULL || ctx->file_name == NULL) {
		ret = actvxactcapcall_fscall_vfs_alloc_unused_fd(true, true, devfs_rref,
								 flags, ptr_to_u64(node),
								 NULL, 0, dev_fops_flags);
	} else {
		hm_actv_set_accessible_mem_range(ptr_to_ulong(ctx->file_name), (unsigned long)ctx->buffer_len);
		ret = actvxactcapcall_fscall_vfs_alloc_unused_fd(true, true, devfs_rref,
								 flags, ptr_to_u64(node),
								 ctx->file_name, ctx->buffer_len,
								 dev_fops_flags);
	}
	if (ret < 0) {
		dh_error("fscall_vfs_alloc_unused_fd failed, err = %d, %s\n",
			 ret, hmstrerror(ret));
		devinfo_put(anon_devinfo);
		free(node);
	}
	return ret;
}

void lnxbase_put_unused_fd(int fd)
{
	int ret = 0;

	/* free fd and file in fs */
	ret = actvxactcapcall_fscall_vfs_put_unused_fd(true, true,
						       devfs_rref, fd);
	if (ret < 0) {
		dh_error("fscall_vfs_put_unused_fd failed, err = %d, %s\n",
			 ret, hmstrerror(ret));
	}
}

int lnxbase_ksys_access(const char *pathname, int mode)
{
	int ret = E_HM_OK;

	ret = vfs_ksys_access(pathname, mode);
	if (ret < 0) {
		dh_debug("vfs access failed, src cidx:%x, fwd cidx:%x,pathname:%s, mode:%d, ret:%s\n",
			 sysif_actv_src_cnode_idx(), sysif_actv_fwd_cnode_idx(),
			 pathname, mode, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}
	return 0;
}

static uint32_t init_task_cidx = 0;
static uint32_t __get_init_task_cidx(void)
{
	int ret = E_HM_OK;
	if (init_task_cidx == 0) {
		ret = hm_get_cnode_idx(PID_INIT_PROC, &init_task_cidx);
		if (ret < 0) {
			dh_error("get init_task cnode idx failed\n");
		}
	}
	return init_task_cidx;
}

int lnxbase_vfs_open(const char *filename, unsigned int flag, mode_t mode)
{
	int fd = 0;

	fd = vfs_cnode_open(filename, flag, mode, __get_init_task_cidx());
	if (fd < 0) {
		dh_debug("vfs open failed, src cidx:%x, fwd cidx:%x, init:%x, filename:%s, flag:0x%x, mode:%u, err:%s\n",
			 sysif_actv_src_cnode_idx(), sysif_actv_fwd_cnode_idx(), __get_init_task_cidx(),
			 filename, flag, mode, hmstrerror(fd));
		return -hmerrno2posix(fd);
	}

	return fd;
}

ssize_t lnxbase_ksys_pwrite(int fd, int64_t pos, const void *buf, size_t nbyte)
{
	ssize_t ret = vfs_ksys_pwrite(fd, pos, buf, nbyte);
	return (ret >= 0) ? ret : -hmerrno2posix((int)ret);
}

ssize_t lnxbase_ksys_pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset)
{
	ssize_t ret = vfs_ksys_pwritev(fd, iov, iovcnt, offset);
	return (ret >= 0) ? ret : -hmerrno2posix((int)ret);
}

ssize_t lnxbase_ksys_pread(int fd, int64_t pos, void *buf, size_t nbyte)
{
	ssize_t ret = vfs_ksys_pread(fd, pos, buf, nbyte);
	return (ret >= 0) ? ret : -hmerrno2posix((int)ret);
}

ssize_t lnxbase_ksys_preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset)
{
	ssize_t ret = vfs_ksys_preadv(fd, iov, iovcnt, offset);
	return (ret >= 0) ? ret : -hmerrno2posix((int)ret);
}

int lnxbase_vfs_close(int fd)
{
	int ret = vfs_close(fd);
	return (ret >= 0) ? 0 : -hmerrno2posix(ret);
}

int lnxbase_vfs_fstat(int fd, struct stat *stat)
{
	int ret = vfs_fstat(fd, stat);
	if (ret < 0) {
		dh_debug("vfs fstat failed, src cidx:%x, fwd cidx:%x, init:%x, fd:%d, err:%s\n",
			 sysif_actv_src_cnode_idx(), sysif_actv_fwd_cnode_idx(), __get_init_task_cidx(),
			 fd, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}
	return (ret >= 0) ? 0 : -hmerrno2posix(ret);
}

int lnxbase_ksys_statfs(const char *pathname, struct statfs *statfs)
{
	int ret = vfs_ksys_statfs(pathname, statfs);
	return (ret >= 0) ? 0 : -hmerrno2posix(ret);
}

int lnxbase_ksys_fstat(int fd, struct stat *stat)
{
	int ret = vfs_ksys_fstat(fd, stat);
	return (ret >= 0) ? 0 : -hmerrno2posix(ret);
}

int lnxbase_ksys_fstat_nocheck(int fd, struct stat *stat)
{
	int ret = vfs_ksys_fstat_nocheck(fd, stat);
	return (ret >= 0) ? 0 : -hmerrno2posix(ret);
}

int lnxbase_ksys_fstatat(int dfd, const char *filename, struct stat *stat, unsigned int flags)
{
	int ret = vfs_ksys_fstatat(dfd, filename, stat, flags);
	return (ret >= 0) ? 0 : -hmerrno2posix(ret);
}

int lnxbase_vfs_ioctl(int fd, unsigned int cmd, unsigned long arg, int arg_sz)
{
	int ret = vfs_ioctl(fd, cmd, arg, arg_sz);
	return (ret >= 0) ? 0 : -hmerrno2posix(ret);
}

int lnxbase_ksys_rename(const char *oldpath, const char *newpath)
{
	int ret = vfs_ksys_rename(oldpath, newpath);
	return (ret >= 0) ? 0 : -hmerrno2posix(ret);
}

int lnxbase_ksys_readlinkat(int dfd, const char *path, char *buf, size_t bufsize)
{
	ssize_t ret = vfs_ksys_readlinkat(dfd, path, buf, bufsize);
	if (ret > INT_MAX) {
		return -hmerrno2posix(E_HM_OVERFLOW);
	} else {
		return (ret >= 0) ? (int)ret : -hmerrno2posix((int)ret);
	}
}

int lnxbase_ksys_mkdir(const char *pathname, mode_t mode)
{
	int ret = E_HM_OK;
	ret = vfs_ksys_mkdir(pathname, mode);
	if (ret < 0) {
		dh_error("vfs mkdir failed, pathname:%s, mode:%u, ret:%s\n",
			 pathname, mode, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}
	return 0;
}

int lnxbase_ksys_rmdir(const char *pathname)
{
	int ret = E_HM_OK;
	ret = vfs_ksys_rmdir(pathname);
	if (ret < 0) {
		dh_error("vfs rmdir failed, pathname:%s, ret:%s\n",
			 pathname, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}
	return 0;
}

int lnxbase_ksys_unlink(const char *pathname)
{
	int ret = E_HM_OK;
	ret = vfs_ksys_unlink(pathname);
	if (ret < 0) {
		dh_error("vfs vfs_ksys unlink failed, pathname:%s, ret:%s\n",
			 pathname, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}
	return 0;
}

long lnxbase_ksys_symlink(const char *oldname, const char *newname)
{
	long ret = vfs_ksys_symlink(oldname, newname);
	return (ret >= 0) ? 0 : -hmerrno2posix(ret);
}

int lnxbase_ksys_fchown(int fd, uid_t uid, gid_t gid)
{
	int ret = E_HM_OK;
	ret = vfs_ksys_fchown(fd, uid, gid);
	if (ret < 0) {
		dh_error("vfs vfs_ksys fchown failed, fd:%d, uid:%u, gid:%u, ret:%s\n",
			 fd, uid, gid, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}
	return 0;
}

int lnxbase_ksys_open(const char *filename, unsigned int flag, mode_t mode)
{
	int fd = 0;
	fd = vfs_ksys_open(filename, flag, mode);
	if (fd < 0) {
		dh_debug("ksys open failed, filename:%s, flag:0x%x, mode:%u, err:%s\n",
			 filename, flag, mode, hmstrerror(fd));
		return -hmerrno2posix(fd);
	}

	return fd;
}

int lnxbase_ksys_openat(int dfd, const char *filename, unsigned int flag, mode_t mode)
{
	int fd = 0;
	fd = vfs_ksys_openat(dfd, filename, flag, mode);
	if (fd < 0) {
		dh_debug("ksys openat failed, dfd:%d, filename:%s, flag:0x%x, mode:%u, err:%s\n",
			 dfd, filename, flag, mode, hmstrerror(fd));
		return -hmerrno2posix(fd);
	}

	return fd;
}

int lnxbase_ksys_close(int fd)
{
	int ret = vfs_ksys_close(fd);
	return (ret >= 0) ? 0 : -hmerrno2posix(ret);
}

int lnxbase_ksys_ioctl(int fd, unsigned int cmd, unsigned long arg, int arg_sz)
{
	int ret = vfs_ksys_ioctl(fd, cmd, arg, arg_sz);
	return (ret >= 0) ? 0 : -hmerrno2posix(ret);
}

int lnxbase_ksys_lseek(int fd, int64_t offset, int whence, int64_t *pos)
{
	int ret = E_HM_OK;
	ret = vfs_ksys_lseek(fd, offset, whence, pos);

	return (ret >= 0) ? ret : -hmerrno2posix(ret);
}

ssize_t lnxbase_ksys_write(int fd, const void *buf, size_t nbyte)
{
	ssize_t ret = vfs_ksys_write(fd, buf, nbyte);
	return (ret >= 0) ? ret : -hmerrno2posix((int)ret);
}

ssize_t lnxbase_vfs_read(int fd, void *buf, size_t nbyte)
{
	ssize_t ret = vfs_read(fd, buf, nbyte);
	if (ret < 0) {
		dh_debug("vfs fstat failed, src cidx:%x, fwd cidx:%x, init:%x, fd:%d, err:%s\n",
			 sysif_actv_src_cnode_idx(), sysif_actv_fwd_cnode_idx(), __get_init_task_cidx(),
			 fd, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}
	return (ret >= 0) ? ret : -hmerrno2posix((int)ret);
}

ssize_t lnxbase_ksys_read(int fd, void *buf, size_t nbyte)
{
	ssize_t ret = vfs_ksys_read(fd, buf, nbyte);
	return (ret >= 0) ? ret : -hmerrno2posix((int)ret);
}

ssize_t lnxbase_ksys_readdir(int fd, void *buf, size_t nbyte)
{
	ssize_t ret = vfs_ksys_readdir(fd, buf, nbyte);
	return (ret >= 0) ? ret : -hmerrno2posix((int)ret);
}

int lnxbase_ksys_fsync(int fd)
{
	int ret = vfs_ksys_fsync(fd);
	return (ret >= 0) ? 0 : -hmerrno2posix(ret);
}

int lnxbase_ksys_sync(void)
{
	int ret = vfs_ksys_sync();
	return (ret >= 0) ? 0 : -hmerrno2posix(ret);
}
