/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Implementation of vfs
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jan 13 14:26:43 2019
 */

#include <vfs.h>
#include <fd.h>
#include <fs/comp.h>
#include <hongmeng/version.h>

#include "internal.h"

#include <sys/mount.h>
#include <sys/quota.h>

#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsrv_io/type.h>
#include <libhmsync/atomic.h>
#include <libsysif/sysmgr/api.h>
#include <libstrict/strict.h>

#include <hmkernel/attributes/hmattributes.h>
__hmattr_tag(VFSCORE);

#ifdef FS_MUTEX_DEBUG
static bool fs_mutex_debug = true;

bool fs_mutex_debug_on(void)
{
	return fs_mutex_debug;
}
#endif

int fqsn_to_dev_id(const char *fqsn, dev_t *dev_id)
{
	int err;
	struct stat st = {0};
	dev_t devno;

	err = vfs_stat(fqsn, &st);
	if (err < 0) {
		hm_debug("get file[%s] stat filed, err[%s]\n", fqsn, hmstrerror(err));
		return err;
	}
	devno = st.st_rdev;
	*dev_id = MKDEV(major(devno), minor(devno));

	return E_HM_OK;
}

int ksys_mount(const char *src, const char *tgt, const char *fstype,
	       unsigned long flag, const char *opts)
{
	return actvxcap_fscall(ksys_mount, true, true, src, tgt, fstype, flag, opts);
}

int vfs_mount(const char *src, const char *tgt, const char *fstype,
	      unsigned long flag, const char *opts)
{
	return lsyscall_vfs_mount(src, tgt, fstype, flag, opts);
}

struct path_op_node {
	int dfd;
	const char *path;
	int pfd;
	char *subpath;
};

int vfs_rename(const char *oldpath, const char *newpath)
{
	return lsyscall_vfs_renameat(AT_FDCWD, oldpath, AT_FDCWD, newpath);
}

/* Resize file to a specified length */
int vfs_ftruncate(int fd, uint64_t length)
{
	int err;
#ifdef __arm__
	union {
		long long ll;
		long l[2]; /* 2 * 32bit splice to 64bit */
	} p;
	p.ll = (long long)(unsigned long long)length;
	err = lsyscall_vfs_ftruncate64(fd, 0, p.l[0], p.l[1]);
#else
	err = lsyscall_vfs_ftruncate(fd, (long)(unsigned long)length);
#endif
	if (err < 0) {
		hm_debug("FS resize failed: %s\n", hmstrerror(err));
	}

	return err;
}


int vfs_unlink(const char *filename)
{
	return lsyscall_vfs_unlinkat(AT_FDCWD, filename, 0);
}

int vfs_rmdir(const char *dirname)
{
	return lsyscall_vfs_unlinkat(AT_FDCWD, dirname, AT_REMOVEDIR);
}

int vfs_umount2(const char *mp, int flags)
{
	return lsyscall_vfs_unmount(mp, (unsigned int)flags);
}

bool vfs_is_dev_mounted(dev_t major, dev_t minor)
{
	int err;

	err = actvcap_fscall(vfs_is_dev_mounted, major, minor);

	return (err < 0 ? false : (bool)err);
}

int vfs_symlink(const char *target, const char *linkpath)
{
	return lsyscall_vfs_symlinkat(target, AT_FDCWD, linkpath);
}

int vfs_mknodat(int dfd, const char *name, mode_t mode, dev_t devno)
{
	dev_t tmp_devno = makedev(MAJOR(devno), MINOR(devno));
	return lsyscall_vfs_mknodat(dfd, name, mode, tmp_devno);
}

int vfs_unix_mknod(const char *name, mode_t mode, dev_t devno,
		   vfs_unix_node_t *unode)
{
	struct __actvret_fscall_vfs_unix_mknod ret = {0};
	int err;

	if (unode == NULL) {
		return E_HM_POSIX_FAULT;
	}
	err = actvxcap_fscall(vfs_unix_mknod, true, true, name, mode, devno, &ret);
	if (err == E_HM_OK) {
		unode->dev_id = ret.dev_id;
		unode->index = ret.index;
	}

	return err;
}

int vfs_unix_put_node(vfs_unix_node_t unode)
{
	return actvxcap_fscall(vfs_unix_put_node, true, true, unode.dev_id, unode.index);
}

int vfs_pipe2(int *fd, int flags)
{
	return lsyscall_vfs_pipe2(fd, flags);
}

static int vfs_fscache_local_pools(const vfs_stat_common_t *stat,
				   vfs_fscache_stat_t *fscache_stat)
{
	int rc = 0;

	fscache_stat->mpool_pages_total =
		stat->mpools.pages_total - stat->mpools.pages_reclaimed;
	fscache_stat->mpool_pages_free =
		stat->mpools.pages_free - stat->mpools.pages_reclaimed;
	fscache_stat->mpool_pages_used = stat->mpools.pages_used;
	fscache_stat->mpool_pages_reclaimed = stat->mpools.pages_reclaimed;

	if (stat->mpools.pages_total < stat->mpools.pages_free ||
	    stat->mpools.pages_free < stat->mpools.pages_reclaimed) {
		rc++;
	}
	if (stat->mpools.pages_total != (stat->mpools.pages_free +
					 stat->mpools.pages_used)) {
		 rc++;
	}

	return rc;
}

static int vfs_fscache_local_slabs(vfs_stat_common_t *stat,
				   vfs_fscache_stat_t *fscache_stat)
{
	fscache_stat->pages_slabs =
		(unsigned)raw_atomic_long_read_relaxed(&stat->slab.pages_used_total);
	fscache_stat->pages_mmap_private =
		(unsigned)raw_atomic_long_read_relaxed(&stat->slab.mmap_private_total);
	fscache_stat->pages_page_metadata = stat->slab.pages_page_metadata;
	fscache_stat->pages_mapping = stat->slab.pages_mapping;
	fscache_stat->pages_file = stat->slab.pages_file;
	fscache_stat->pages_dentry = stat->slab.pages_dentry;
	fscache_stat->pages_vnode = stat->slab.pages_vnode;
	fscache_stat->pages_fat_node = stat->slab.pages_fat_node;
	fscache_stat->pages_tmpfs_node = stat->slab.pages_tmpfs_node;
	fscache_stat->pages_pipefs_node = stat->slab.pages_pipefs_node;
	fscache_stat->pages_ext2_node = stat->slab.pages_ext2_node;
	fscache_stat->pages_jffs2_node = stat->slab.pages_jffs2_node;
	fscache_stat->pages_mmap_cached = stat->slab.mmap_cached_len >> PAGE_SHIFT;
	fscache_stat->pages_transfs_node = stat->slab.pages_transfs_node;
	fscache_stat->pages_ubifs_node = stat->slab.pages_ubifs_node;
	fscache_stat->pages_procfs_node = stat->slab.pages_procfs_node;
	fscache_stat->pages_kernfs_node = stat->slab.pages_kernfs_node;
	fscache_stat->pages_squashfs_node = stat->slab.pages_squashfs_node;
	fscache_stat->pages_fuse_node = stat->slab.pages_fuse_node;
	fscache_stat->pages_fuse_ctl_node = stat->slab.pages_fuse_ctl_node;
	fscache_stat->bytes_vnode =
		(unsigned)raw_atomic_long_read_relaxed(&stat->slab.bytes_vnode);
	fscache_stat->bytes_dentry =
		(unsigned)raw_atomic_long_read_relaxed(&stat->slab.bytes_dentry);

	return E_HM_OK;
}

static int vfs_fscache_local_fscache(vfs_stat_common_t *stat,
				     vfs_fscache_stat_t *fscache_stat)
{
	fscache_stat->pages_cached = (unsigned int)raw_atomic_long_read_relaxed(&stat->fscache.pages_cached);
	fscache_stat->lru_pages = (unsigned int)raw_atomic_long_read_relaxed(&stat->fscache.pages_free);
	fscache_stat->lru_slab_size = (unsigned int)raw_atomic_long_read_relaxed(&stat->fscache.pages_free) *
				      (unsigned)stat->fscache.page_metadata_size;
	fscache_stat->pages_dirty_wb = (unsigned int)raw_atomic_long_read_relaxed(&stat->fscache.pages_dirty_wb);
	fscache_stat->huge_page_2m = (unsigned int)raw_atomic_long_read_relaxed(&stat->fscache.huge_page_2m);
	fscache_stat->huge_page_4m = (unsigned int)raw_atomic_long_read_relaxed(&stat->fscache.huge_page_4m);
	fscache_stat->huge_page_1g = (unsigned int)raw_atomic_long_read_relaxed(&stat->fscache.huge_page_1g);
	fscache_stat->pages_mmap = (unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.pages_mmap);
	fscache_stat->pages_unevict = (unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.pages_unevict);
	fscache_stat->pages_unmap = (unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.pages_unmap);
	fscache_stat->pages_unmap_failed = (unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.pages_unmap_failed);
	fscache_stat->pages_isolated = (unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.pages_isolated);
	fscache_stat->pages_tmpfs = (unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.pages_tmpfs);
	fscache_stat->pages_overlayfs = (unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.pages_overlayfs);
	fscache_stat->pages_devfs = (unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.pages_devfs);
	fscache_stat->protect_pages_free = (unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.protect_pages_free);
	fscache_stat->protect_pages_free_active =
		(unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.protect_pages_free_active);
	fscache_stat->protect_overratio_reclaim_times =
		(unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.protect_overratio_reclaim_times);
	fscache_stat->protect_overlimit_reclaim_times =
		(unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.protect_overlimit_reclaim_times);
	fscache_stat->protect_pages_reclaimed_overratio =
		(unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.protect_pages_reclaimed_overratio);
	fscache_stat->protect_pages_reclaimed_overlimit =
		(unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.protect_pages_reclaimed_overlimit);
	fscache_stat->protect_min_fscache_pages =
		(unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.protect_min_fscache_pages);
	fscache_stat->workingset_refault_file =
		(unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.workingset_refault_file);
	fscache_stat->workingset_activate_file =
		(unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.workingset_activate_file);
	fscache_stat->workingset_restore_file =
		(unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.workingset_restore_file);
	fscache_stat->workingset_eviction_file =
		(unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.workingset_eviction_file);
	fscache_stat->page_scanned =
		(unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.page_scanned);
	fscache_stat->ashmem_total_size = (unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.ashmem_total_size);
	fscache_stat->ashmem_alloc_size = (unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.ashmem_alloc_size);
	fscache_stat->ashmem_free_size = (unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.ashmem_free_size);
	fscache_stat->ashmem_shrink_size = (unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.ashmem_shrink_size);

	if (((long)fscache_stat->lru_pages -
	    raw_atomic_long_read_relaxed(&stat->fscache.pages_free_active)) < 0) {
		fscache_stat->pages_free_inactive = 0;
	} else {
		/* sysmgr need pages free inactive */
		fscache_stat->pages_free_inactive = fscache_stat->lru_pages -
					    (unsigned long)raw_atomic_long_read_relaxed(&stat->fscache.pages_free_active);
	}

	return E_HM_OK;
}

/* If the data is inconsistent, E_HM_AGAIN needs to be returned and the process is complete.
   stat is snapshot of struct vfs_stat_common_t */
int vfs_fscache_local_stat(vfs_stat_common_t *stat, vfs_fscache_stat_t *fscache_stat)
{
	int rc = 0;

	if (fscache_stat == NULL || stat == NULL) {
		return E_HM_POSIX_FAULT;
	}
	rc += vfs_fscache_local_pools(stat, fscache_stat);
	rc += vfs_fscache_local_slabs(stat, fscache_stat);
	rc += vfs_fscache_local_fscache(stat, fscache_stat);
	if (rc > 0) {
		hm_debug("fscache_stat is not consistent\n");
		return E_HM_AGAIN;
	}

	return E_HM_OK;
}

int vfs_fscache_free_local_stat_fusion(vfs_stat_common_t *stat, uint32_t *fscache_free_stat)
{
	if (unlikely(fscache_free_stat == NULL || stat == NULL)) {
		return E_HM_POSIX_FAULT;
	}

	*fscache_free_stat += (unsigned int)raw_atomic_long_read_relaxed(&stat->fscache.pages_free);

	return E_HM_OK;
}

int vfs_fscache_free_local_stat(vfs_stat_common_t *stat, uint32_t *fscache_free_stat)
{
	int rc = 0;

	if (unlikely(fscache_free_stat == NULL || stat == NULL)) {
		return E_HM_POSIX_FAULT;
	}

	*fscache_free_stat +=
		stat->mpools.pages_free - stat->mpools.pages_reclaimed;

	if (stat->mpools.pages_total < stat->mpools.pages_free ||
	    stat->mpools.pages_free < stat->mpools.pages_reclaimed) {
		rc++;
	}
	if (stat->mpools.pages_total != (stat->mpools.pages_free +
					 stat->mpools.pages_used)) {
		 rc++;
	}

	*fscache_free_stat += (unsigned int)raw_atomic_long_read_relaxed(&stat->fscache.pages_free);
	*fscache_free_stat += stat->slab.mmap_cached_len >> PAGE_SHIFT;

	if (rc > 0) {
		hm_debug("fscache_stat is not consistent\n");
		return E_HM_AGAIN;
	}

	return E_HM_OK;
}

int vfs_fchmod(int fd, mode_t mode)
{
	return lsyscall_vfs_fchmod(fd, mode);
}

int vfs_chmod(const char *pathname, mode_t mode)
{
	return lsyscall_vfs_fchmodat(AT_FDCWD, pathname, mode, 0);
}

int vfs_drop_caches(rref_t fsmgr_rref, unsigned int mode, int max, int reason)
{
	int err;

	err = actvcapcall_fscall_vfs_drop_caches(fsmgr_rref, mode, max, reason);
	if (err < 0) {
		hm_debug("fs call drop_caches failed: %s\n", hmstrerror(err));
	}

	return err;
}

int vfs_fadvise(int fd, int64_t offset, int64_t len, int advice)
{
#ifdef __arm__
	long off_low_addr;
	long off_high_addr;
	long len_low_addr;
	long len_high_addr;
	union {
		long long ll;
		long l[2]; /* 2 * 32bit splice to 64bit */
	} p;

	p.ll = (long long)offset;
	off_low_addr = p.l[0];
	off_high_addr = p.l[1];

	p.ll = (long long)len;
	len_low_addr = p.l[0];
	len_high_addr = p.l[1];

	return lsyscall_vfs_fadvise64_64(fd, 0, off_low_addr, off_high_addr, len_low_addr, len_high_addr, advice);
#else
	return lsyscall_vfs_fadvise64(fd, (long)offset, (long)len, advice);
#endif
}

int vfs_fd_get_path(int fd, __u32 cnode_idx, char *path, unsigned int path_len)
{
	int err;

	if (path == NULL) {
		hm_debug("path is invalid\n");
		return E_HM_INVAL;
	}

	err = actvcap_fscall(vfs_get_fd_path, fd, cnode_idx, path, (int)path_len);
	if (err < 0) {
		hm_debug("FS get fd path failed %s\n", hmstrerror(err));
	}

	return err;
}

int vfs_sysmgr_get_fid_path(rref_t rref, uint32_t cnode_idx, unsigned long fid,
			    char *buf, size_t buflen)
{
	return actvxactcapcall_fscall_vfs_sysmgr_get_fid_path(
		true, true, rref, (unsigned int)cnode_idx, fid, buf, buflen);
}

int vfs_sysmgr_get_fd_path(int fd, __u32 cnode_idx, char *path)
{
	int err;

	if (path == NULL) {
		hm_debug("path is invalid\n");
		return E_HM_INVAL;
	}

	err = actvxcap_fscall(vfs_sysmgr_get_fd_path, true, true, fd, cnode_idx, path);
	if (err < 0) {
		hm_warn("FS get fd path failed %s\n", hmstrerror(err));
	}

	return err;
}

int vfs_is_normal_fd(int fd, unsigned int cnode_idx, bool *is_normal_fd)
{
	int err;
	struct __actvret_fscall_vfs_query_info query_ret = {0};

	if (is_normal_fd == NULL) {
		return E_HM_INVAL;
	}

	err = actvcap_fscall(vfs_query_info, fd, cnode_idx, 0, &query_ret);
	if (err != E_HM_OK) {
		hm_debug("err:%d,%s fd:%d\n", err, hmstrerror(err), fd);
		return err;
	}

	*is_normal_fd = (query_ret.file_type == NORMAL_FD);

	return E_HM_OK;
}

/* for fs dynamic link, fs_relocated show whether fs has done self relocated */
static int fs_relocated = 0;
void set_fs_relocated(int relocated)
{
	fs_relocated = relocated;
}

int get_fs_relocated(void)
{
	return fs_relocated;
}

int vfs_proc_has_binder_work(rref_t rref, int pid, int *has_binder_work)
{
	int err;
	struct __actvret_fscall_vfs_proc_has_binder_work fscall_ret = {0};

	if (has_binder_work == NULL) {
		return E_HM_INVAL;
	}
	err = actvcapcall_fscall_vfs_proc_has_binder_work(rref, pid, &fscall_ret);
	if (err != E_HM_OK) {
		return err;
	} else {
		*has_binder_work = fscall_ret.has_binder_work;
	}

	return err;
}

int vfs_binder_read_debugfs_info(uint64_t shm_key, size_t len, int type)
{
	return actvxcap_fscall(vfs_binderfs_read_debugfs_info, true, true, shm_key, len, type);
}

void vfs_emergency_remount(void)
{
	int err;

	err = actvcap_fscall(vfs_emergency_remount);
	if (err < 0) {
		hm_debug("vfs_emergency_remount failed, err=%s\n", hmstrerror(err));
	}

	return;
}

int ksys_rename(const char *oldpath, const char *newpath)
{
	int err;

	err = actvxcap_fscall(ksys_renameat, get_ksys_param_is_fwd(), true, AT_FDCWD, oldpath, AT_FDCWD, newpath);
	if (err < 0) {
		hm_debug("FS ksys_rename failed: %s\n", hmstrerror(err));
	}

	return err;
}

int ksys_unlink(const char *filename)
{
	int err;

	err = actvxcap_fscall(ksys_unlinkat, get_ksys_param_is_fwd(), true, AT_FDCWD, filename, 0);
	if (err < 0) {
		hm_debug("FS ksys_unlink failed: %s\n", hmstrerror(err));
	}

	return err;
}

long ksys_symlink(const char *oldname, const char *newname)
{
	int err;

	err = actvxcap_fscall(ksys_symlink, get_ksys_param_is_fwd(), true, oldname, newname);
	if (err < 0) {
		hm_debug("FS ksys_symlink failed: %s\n", hmstrerror(err));
	}

	return err;
}

int ksys_rmdir(const char *dirname)
{
	int err;

	err = actvxcap_fscall(ksys_unlinkat, get_ksys_param_is_fwd(), true, AT_FDCWD, dirname, AT_REMOVEDIR);
	if (err < 0) {
		hm_debug("FS ksys_rmdir failed: %s\n", hmstrerror(err));
	}

	return err;
}
