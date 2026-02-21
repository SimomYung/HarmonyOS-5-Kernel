/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of vfs
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jan 13 14:26:43 2019
 */
#ifndef LIBHMSRV_FS_VFS_H
#define LIBHMSRV_FS_VFS_H

#include <unistd.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#include <fcntl.h>
#include <time.h>
#undef _GNU_SOURCE
#else
#include <fcntl.h>
#include <time.h>
#endif

#include <kconfig_fslibs.h>

#include <stddef.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <lib/utils.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <sys/epoll.h>
#include <fs_mutex.h>
#include <libsysif/fs/api.h>
#include <hmasm/page.h>
#include <fs_server.h>
#include <hongmeng/types.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/statfs.h>

#include <libhwsecurec/securec.h>
#include <libhmucap/ucap.h>
#include <libhmsync/atomic.h>
#include <libhmsync/bitops/generic.h>
#include <libhmsync/raw_scopedptr.h>

#include <libhmsrv_io/type.h>
#include <libhmsrv_sys/hm_select_common.h>

#include "vfs_device.h"

#define FS_NAME_MAXLEN		128
#define MAX_MNTOPTS_LEN		256
#define MAX_FQSN_LEN		256
#define XATTR_NAME_MAX		255
#define XATTR_SIZE_MAX		65536

#define MAX_LINK_COUNT		40
#define MAX_ACTV_COUNT		40

#define POLL_CALL_NUM_WAKEUP (1)

#define SERVER_ROOTFD		(-4096)
#define AT_FDROOT		(-300)

#define UNUSED_ME_IDX		(-10)

#define FSMGR_PATH_ACTV "fsmgr-actv"

/* vfs_tee have been implemented function point */
#define TEE_F_ALL		(O_NONBLOCK)
#define WALK_NONBLOCK   (O_NONBLOCK)

#define TYPE_CWD		0x1
#define TYPE_ROOT		0x2
#define TYPE_EXE		0x3

#define VFS_MIN_REGFILE_NUM	0
#define VFS_STDFILES_NUM	3
#define LDSO_MAX_FD_SUPPORTED	8
#define VFS_MAX_OPEN_FILES	CONFIG_FSLIB_DEFAULT_OPEN_FILES
#define VFS_FDTABLESIZE		CONFIG_FSLIB_DEFAULT_OPEN_FILES
#define VFS_NOFILE_MAX		CONFIG_FSLIB_MAX_OPEN_FILES
#define VFS_MMAP_FID_MIN_NUM	1
#define MAX_SHM_NAME_LEN	64

#define CNODE_IDX_SHIFT 32
#define DATA_XFER_LIMIT		4096u
#define DATA_XFER_SHM_LIMIT	(256 * 1024)

#define MISC_MAJOR	0x0A
#define ASHMFD_MINOR	129
#define EXAGEAR_MINOR	130

typedef enum {
	KIND_FILE,
	KIND_DIRECTORY,
	KIND_SYMLINK,
} vfs_file_kind_t;

#define S_IRWXUGO	(S_IRWXU | S_IRWXG | S_IRWXO)
#define S_IALLUGO	(S_ISUID | S_ISGID | S_ISVTX | S_IRWXUGO)
#define S_IRUGO		(S_IRUSR | S_IRGRP | S_IROTH)
#define S_IXUGO		(S_IXUSR | S_IXGRP | S_IXOTH)
#define S_IWUGO		(S_IWUSR | S_IWGRP | S_IWOTH)

/* high priority request, poll if possible */
#define RWF_HIPRI	((int)0x00000001)
/* per-IO O_DSYNC */
#define RWF_DSYNC	((int)0x00000002)
/* per-IO O_SYNC */
#define RWF_SYNC	((int)0x00000004)
/* per-IO, return -EAGAIN if operation would block */
#define RWF_NOWAIT	((int)0x00000008)
/* per-IO O_APPEND */
#define RWF_APPEND	((int)0x00000010)
/* per-IO hyperframe high priority request */
#define RWF_HF_HIPRI	((int)0x00008000)

#define RWF_SUPPORTED	(RWF_HIPRI | RWF_DSYNC | RWF_SYNC | \
			 RWF_NOWAIT | RWF_APPEND | RWF_HF_HIPRI)

enum vfs_pools_event {
	POOL_PAGES_TOTAL = 0,
	POOL_PAGES_RECLAIMED,
	POOL_PAGES_FREE,
	POOL_PAGES_USED,
	POOL_EVENT_MAX,
};

enum vfs_slab_event {
	BYTES_VNODE = 0,
	BYTES_DENTRY,
	MMAP_CACHE_LEN,
	SLAB_EVENT_MAX,
};

enum vfs_cache_event {
	/*
	 * should have the same layout with the corresponding structure
	 * in mapi/uapi fspgcache.h
	 */
	PAGES_MMAP = 0,
	PAGES_FREE,
	PAGE_METADATA_SIZE,
	PAGES_CACHED,
	PAGES_DIRTY_WB,
	HUGE_PAGE_2M,
	HUGE_PAGE_4M,
	HUGE_PAGE_1G,
	PAGES_UNEVICT,
	PAGES_FREE_ACTIVE,
	PAGES_UNMAP,
	PAGES_SCANNED_ASYNC,
	PAGES_SCANNED_SYNC,
	PAGES_UNMAP_FAILED,
	PAGES_DROP_MMAP_FAILED,
	PAGES_FREE_PAGE_FAILED,
	PAGES_ISOLATED,
	ASHMEM_TOTAL_SIZE,  /* size that has been setted   from ioctl ASHMFD_SET_SIZE */
	ASHMEM_ALLOC_SIZE,  /* size that has been alloced  from fs pagecache mapping */
	ASHMEM_FREE_SIZE,   /* size that has been lrued    from ioctl ASHMFD_UNPIN */
	ASHMEM_SHRINK_SIZE, /* size that has been shrinked from shrink (only for unpin) */
	NR_SHMEM_TOTAL,        /* shmem pages total */
	NR_SHMEM_ASHMEM,       /* shmem pages ashmem */
	NR_SHMEM_TMPFS,        /* shmem pages tmpfs/memfd */
	NR_SHMEM_UNEVICT,      /* shmem pages unevict */
	NR_SHMEM_PROTECT,      /* shmem pages protect */
	NR_SHMEM_SWAPPED,      /* shmem pages swapped */
	NR_SHMEM_SWAPOUT_SUCC, /* shmem pages swapout succ times */
	NR_SHMEM_SWAPOUT_FAIL, /* shmem pages swapout fail times */
	NR_SHMEM_SWAPIN_SUCC,  /* shmem pages swapin succ times */
	NR_SHMEM_SWAPIN_FAIL,  /* shmem pages swapin fail times */
	PAGES_TMPFS,
	PAGES_OVERLAYFS,
	PAGES_DEVFS,
	PROTECT_PAGES_FREE,
	PROTECT_PAGES_FREE_ACTIVE,
	PROTECT_OVERRATIO_RECLAIM_TIMES,
	PROTECT_OVERLIMIT_RECLAIM_TIMES,
	PROTECT_PAGES_RECLAIMED_OVERRATIO,
	PROTECT_PAGES_RECLAIMED_OVERLIMIT,
	PROTECT_MIN_FSCACHE_PAGES, /* minimum file pages when reclaim due to overratio */
	FSCACHE_EVENT_MAX,
};

typedef struct {
	unsigned int mpool_pages_total;
	unsigned int mpool_pages_free;
	unsigned int mpool_pages_used;
	unsigned int mpool_pages_reclaimed;
	unsigned int pages_cached;
	unsigned int lru_pages;
	unsigned int lru_slab_size;
	unsigned int pages_dirty_wb;
	unsigned int pages_slabs;
	unsigned int pages_mmap_private;
	unsigned int pages_mmap_cached;
	unsigned int pages_page_metadata;
	unsigned int pages_mapping;
	unsigned int pages_file;
	unsigned int pages_dentry;
	unsigned int pages_vnode;
	unsigned int pages_tmpfs_node;
	unsigned int pages_pipefs_node;
	unsigned int pages_ext2_node;
	unsigned int pages_ext4_node;
	unsigned int pages_fat_node;
	unsigned int pages_jffs2_node;
	unsigned int pages_overlayfs_node;
	unsigned int pages_transfs_node;
	unsigned int pages_ubifs_node;
	unsigned int pages_procfs_node;
	unsigned int pages_kernfs_node;
	unsigned int pages_squashfs_node;
	unsigned int pages_fuse_node;
	unsigned int pages_fuse_ctl_node;
	unsigned int pages_hffs_node;
	unsigned long bytes_vnode;
	unsigned long bytes_dentry;
	uint64_t huge_page_2m;
	uint64_t huge_page_4m;
	uint64_t huge_page_1g;
	uint64_t pages_mmap;
	uint64_t pages_unevict;
	uint64_t pages_free_inactive;
	uint64_t pages_unmap;
	uint64_t pages_scanned_async;
	uint64_t pages_scanned_sync;
	uint64_t pages_unmap_failed;
	uint64_t pages_drop_mmap_failed;
	uint64_t pages_free_page_failed;
	uint64_t pages_tmpfs;
	uint64_t pages_overlayfs;
	uint64_t pages_devfs;
	uint64_t protect_pages_free;
	uint64_t protect_pages_free_active;
	uint64_t ashmem_total_size;  /* size that has been setted   from ioctl ASHMFD_SET_SIZE */
	uint64_t ashmem_alloc_size;  /* size that has been alloced  from fs pagecache mapping */
	uint64_t ashmem_free_size;   /* size that has been lrued    from ioctl ASHMFD_UNPIN */
	uint64_t ashmem_shrink_size; /* size that has been shrinked from shrink (only for unpin) */
	uint64_t nr_shmem_total;        /* shmem pages total */
	uint64_t nr_shmem_ashmem;       /* shmem pages ashmem */
	uint64_t nr_shmem_tmpfs;        /* shmem pages tmpfs/memfd */
	uint64_t nr_shmem_unevict;      /* shmem pages unevict */
	uint64_t nr_shmem_protect;      /* shmem pages protect */
	uint64_t nr_shmem_swapped;      /* shmem pages swapped */
	uint64_t nr_shmem_swapout_succ; /* shmem pages swapout succ times */
	uint64_t nr_shmem_swapout_fail; /* shmem pages swapout fail times */
	uint64_t nr_shmem_swapin_succ;  /* shmem pages swapin succ times */
	uint64_t nr_shmem_swapin_fail;  /* shmem pages swapin fail times */
} vfs_fscache_stat_t;

typedef struct {
	unsigned pages_total;
	unsigned pages_reclaimed;
	unsigned pages_free;
	unsigned pages_used;
} vfs_stat_pool_t;

typedef struct {
	raw_atomic_long_t pages_used_total;
	raw_atomic_long_t mmap_private_total;
	unsigned int mmap_cached_len; /* length of the unallocated memory */
	unsigned int pages_page_metadata;
	unsigned int pages_mapping;
	unsigned int pages_file;
	unsigned int pages_dentry;
	unsigned int pages_vnode;
	unsigned int pages_tmpfs_node;
	unsigned int pages_pipefs_node;
	unsigned int pages_ext4_node;
	unsigned int pages_ext2_node;
	unsigned int pages_fat_node;
	unsigned int pages_jffs2_node;
	unsigned int pages_overlayfs_node;
	unsigned int pages_transfs_node;
	unsigned int pages_ubifs_node;
	unsigned int pages_procfs_node;
	unsigned int pages_kernfs_node;
	unsigned int pages_squashfs_node;
	unsigned int pages_fuse_node;
	unsigned int pages_fuse_ctl_node;
	unsigned int pages_hffs_node;
	raw_atomic_long_t bytes_vnode;
	raw_atomic_long_t bytes_dentry;
} vfs_stat_slab_t;

typedef struct {
	vfs_stat_pool_t mpools;
	raw_atomic_long_t fscache[FSCACHE_EVENT_MAX];
	vfs_stat_slab_t slab;
} vfs_stat_common_t;

typedef struct {
	dev_t dev_id;
	unsigned long index;
} vfs_unix_node_t;

#define MOUNT_USER_ARGS_SRC 0x1
#define MOUNT_USER_ARGS_FSTYPE 0x2
#define MOUNT_USER_ARGS_OPTS 0x4

struct fs_opts {
	char opts[MAX_MNTOPTS_LEN];
	char fs_name[FS_NAME_MAXLEN];
	char fqsn[PATH_MAX];
	unsigned long flag;
	unsigned int args_mask;
};

typedef struct {
	bool rw;
	size_t n_byte;
	char name[MAX_SHM_NAME_LEN];
} fs_shm_info;

struct fs_buf_info {
	char *buf;
	size_t len;
};

struct vfs_iommap_ret {
	uintptr_t rvaddr;
	uint32_t iomem_id;
	uint32_t cnode_idx;
};

struct vfs_iommap_info {
	uint32_t cnode_idx;
	uint32_t prot;
	uint32_t flags;
	int64_t offset;
	rref_t vs_rref;
	uintptr_t vaddr;
	size_t aligned_len;
	bool iomap_prepare_ahead;
};

#define VFS_IOMMAP_INFO_INIT(_cnode_idx, _prot, _flags, _offset, _vs_rref, _vaddr, _aligned_len, \
			     _iomap_prepare_ahead) \
{ \
	.cnode_idx = (_cnode_idx),  \
	.prot = (_prot),  \
	.flags = (_flags),  \
	.offset = (_offset),  \
	.vs_rref = (_vs_rref),  \
	.vaddr = (_vaddr),  \
	.aligned_len = (_aligned_len), \
	.iomap_prepare_ahead = (_iomap_prepare_ahead), \
}

int vfs_rename(const char *oldname, const char *newname);
int vfs_symlink(const char *target, const char *linkpath);
int vfs_ftruncate(int fd, uint64_t length);
int vfs_unlink(const char *filename);
int vfs_rmdir(const char *dirname);
int vfs_mount(const char *src, const char *tgt, const char *fstype,
	      unsigned long flag, const char *opts);
int vfs_umount2(const char *mp, int flags);
bool vfs_is_dev_mounted(dev_t major, dev_t minor);
void vfs_emergency_remount(void);
/* vfs_ksys ops */
int vfs_ksys_rename(const char *oldpath, const char *newpath);
int vfs_ksys_unlink(const char *filename);
int vfs_ksys_rmdir(const char *dirname);
int vfs_ksys_mount(const char *src, const char *tgt, const char *fstype,
	       unsigned long flag, const char *opts);
long vfs_ksys_symlink(const char *oldname, const char *newname);

int vfs_unix_mknod(const char *name, mode_t mode, dev_t devno, vfs_unix_node_t *unode);
int vfs_unix_put_node(vfs_unix_node_t unode);
int vfs_mknodat(int dfd, const char *name, mode_t mode, dev_t devno);
int vfs_lseek(int fd, int64_t offset, int whence, int64_t *pos);
int vfs_trans(int fd, const char *buf, int pos, size_t nbyte);
int vfs_binder_read_debugfs_info(uint64_t shm_key, size_t len, int type, int pid);
/* vfs_ksys ops */
int vfs_ksys_lseek(int fd, int64_t offset, int whence, int64_t *pos);

#define MPOOL_SUMVEC_PER_PAGE		(PAGE_SIZE / sizeof(vfs_mmap_mpool_sumvec_t))
#define FILL_NODE_SUMVEC_PER_PAGE	(PAGE_SIZE / sizeof(vfs_mmap_fill_node_sumvec_t))
#define CHANGE_NODE_SUMVEC_PER_PAGE	(PAGE_SIZE / sizeof(vfs_mmap_change_node_sumvec_t))

enum mmap_flag_bits {
	__MMAP_FL_PAGE_MAPPED,
	__MMAP_FL_PAGE_UNMAPPED,
	__MMAP_FL_PAGE_UNMOVE,
	__MMAP_FL_PAGE_DIRTIED,
	__MMAP_FL_PAGE_CLEANED,
	__MMAP_FL_PAGE_ASYNCED,
	__MMAP_FL_PAGE_SYNCED,
	__MMAP_FL_PAGE_INVALIDATED,
	__MMAP_FL_PAGE_MLOCK,
	__MMAP_FL_PAGE_MUNLOCK,
	__MMAP_FL_PAGE_SEQ_READ,
	__MMAP_FL_PAGE_RECLAIM,
	__MMAP_FL_PAGE_SWAPLESS,
	/* In fusion, PAGE_ADDR means node_page_idx is actually fscache_page_t's addr.
	 * We can use this addr to get page directly, instead of finding it in tree.
	 */
	__MMAP_FL_PAGE_ADDR,
	__MMAP_FL_PAGE_HKIP,
	__NR_MMAP_FLAGS,
};

#define MMAP_FL_NONE 0
#define MMAP_FL_PAGE_MAPPED (1U << (uint32_t)__MMAP_FL_PAGE_MAPPED)
#define MMAP_FL_PAGE_UNMAPPED (1U << (uint32_t)__MMAP_FL_PAGE_UNMAPPED)
#define MMAP_FL_PAGE_UNMOVE (1U << (uint32_t)__MMAP_FL_PAGE_UNMOVE)
#define MMAP_FL_PAGE_DIRTIED (1U << (uint32_t)__MMAP_FL_PAGE_DIRTIED)
#define MMAP_FL_PAGE_CLEANED (1U << (uint32_t)__MMAP_FL_PAGE_CLEANED)
#define MMAP_FL_PAGE_ASYNCED (1U << (uint32_t)__MMAP_FL_PAGE_ASYNCED)
#define MMAP_FL_PAGE_SYNCED (1U << (uint32_t)__MMAP_FL_PAGE_SYNCED)
#define MMAP_FL_PAGE_INVALIDATED (1U << (uint32_t)__MMAP_FL_PAGE_INVALIDATED)
#define MMAP_FL_PAGE_MLOCK (1U << (uint32_t)__MMAP_FL_PAGE_MLOCK)
#define MMAP_FL_PAGE_MUNLOCK (1U << (uint32_t)__MMAP_FL_PAGE_MUNLOCK)
#define MMAP_FL_PAGE_SEQ_READ (1U << (uint32_t)__MMAP_FL_PAGE_SEQ_READ)
#define MMAP_FL_PAGE_RECLAIM (1U << (uint32_t)__MMAP_FL_PAGE_RECLAIM)
#define MMAP_FL_PAGE_SWAPLESS (1U << (uint32_t)__MMAP_FL_PAGE_SWAPLESS)
#define MMAP_FL_PAGE_ADDR (1U << (uint32_t)__MMAP_FL_PAGE_ADDR)
#define MMAP_FL_PAGE_HKIP (1U << (uint32_t)__MMAP_FL_PAGE_HKIP)
#define MMAP_FL_PAGE_LOCKED (1U << (uint32_t)__MMAP_FL_PAGE_LOCKED)

typedef struct {
	uint32_t index;
	uint32_t dax;
	uint32_t i_generation;
	uint64_t dev_id;
	unsigned long mapping;
} vfs_mmap_node_t;

typedef struct {
	int32_t mpool_id;
	uint64_t mpool_page_idx;
	uint32_t nr_pages;
} __attribute__((packed)) vfs_mmap_mpool_sumvec_t;

typedef struct {
	vfs_mmap_mpool_sumvec_t vec[MPOOL_SUMVEC_PER_PAGE];
} __attribute__((packed)) vfs_mmap_mpool_summary_t;

typedef struct {
	uint64_t node_page_idx;
	uint32_t nr_pages;
} __attribute__((packed)) vfs_mmap_fill_node_sumvec_t;

typedef struct {
	uint64_t node_page_idx;
	uint32_t nr_pages;
	uint32_t flag;
} __attribute__((packed)) vfs_mmap_change_node_sumvec_t;

typedef struct {
	vfs_mmap_fill_node_sumvec_t vec[FILL_NODE_SUMVEC_PER_PAGE];
} __attribute__((packed)) vfs_mmap_fill_node_summary_t;

typedef struct {
	vfs_mmap_change_node_sumvec_t vec[CHANGE_NODE_SUMVEC_PER_PAGE];
} __attribute__((packed)) vfs_mmap_change_node_summary_t;

extern int vfs_pac_fault_dfi(void);
extern int vfs_pipe2(int *fd, int flags);
extern int vfs_setrlimit(__u32 cnode_idx, unsigned int type, rlim_t rlim_cur, rlim_t rlim_max);
extern int vfs_set_label(int fd, uint64_t label);
extern int vfs_set_hkip_label(int fd);
extern int vfs_get_label(int fd, uint64_t *label);
int vfs_set_label_by_path(const char *path, int flag, uint64_t label);
int vfs_proc_unauthoried_files(void);

int vfs_lock_in(unsigned int user_id);
int vfs_unlock_in(unsigned int user_id, unsigned int file, unsigned char *iv, unsigned int iv_len);

typedef void (*audit_wp_start_hook_func_t)(void);
typedef void (*audit_wp_end_hook_func_t)(bool is_exec, const char *path, const char *srv_name);
struct audit_watch_field;

int vfs_audit_init(void);
int vfs_add_audit_rule(const char *path, uint16_t rule_id, uint16_t rule_num, uint32_t flags);
int vfs_del_audit_rule(const char *path, uint64_t rule_id, uint32_t flags);
int vfs_match_audit_watch_file(uint64_t rule_id, const struct audit_watch_field *field);
int vfs_match_audit_watch_dir(uint64_t rule_id, int me_idx, const struct audit_watch_field *field);
void vfs_set_audit_wp_hook(audit_wp_start_hook_func_t start, audit_wp_end_hook_func_t end);
void vfs_audit_watch_point_start(void);
void vfs_audit_watch_point_end(bool is_exec, const char *path, const char *srv_name);

struct splice_libinfo {
	int fd_in;
	int64_t *offset_in;
	int fd_out;
	int64_t *offset_out;
	size_t len;
	unsigned int flags;
	ssize_t spliced_bytes;
};

#ifndef FS_RECLAIM_FLAG
#define FS_RECLAIM_FLAG
/* used for sysmgr vfs_drop_caches */
enum fs_reclaim_flag {
	/* reclaim only clean pages */
	__FS_RECLAIM_NIO,
	 /* reclaim only dirty pages */
	__FS_RECLAIM_IO,
	/* reclaim both clean and dirty pages */
	__FS_RECLAIM_ALL,
	/* reclaim only vnode and dentry lru */
	__FS_RECLAIM_META,
	/* reclaim all and iterator every sb hook, for ldk drop */
	__FS_RECLAIM_ALL_ITER,
	/* don't reclaim filemap page */
	__FS_RECLAIM_NOMAP,
	__FS_RECLAIM_SYNC,
	__FS_QUERY_FSSLAB,
	__FS_RECLAIM_NOKZ,
	__FS_RECLAIM_NR,
};

#define FS_RECLAIM_NIO		(1 << (int)__FS_RECLAIM_NIO)
#define FS_RECLAIM_IO		(1 << (int)__FS_RECLAIM_IO)
#define FS_RECLAIM_ALL		(1 << (int)__FS_RECLAIM_ALL)
#define FS_RECLAIM_META		(1 << (int)__FS_RECLAIM_META)
#define FS_RECLAIM_ALL_ITER	(1 << (int)__FS_RECLAIM_ALL_ITER)
#define FS_RECLAIM_NOMAP	(1 << (int)__FS_RECLAIM_NOMAP)
#define FS_RECLAIM_SYNC		(1 << (int)__FS_RECLAIM_SYNC)
#define FS_QUERY_FSSLAB		(1 << (int)__FS_QUERY_FSSLAB)
#define FS_RECLAIM_NOKZ		(1 << (int)__FS_RECLAIM_NOKZ)
#define FS_RECLAIM_NR		(1 << (int)__FS_RECLAIM_NR)
#endif

uint64_t vfs_stat_pools_event(vfs_stat_common_t *stat, enum vfs_pools_event event);
uint64_t vfs_stat_slab_event(vfs_stat_common_t *stat, enum vfs_slab_event event);
uint64_t vfs_stat_fscache_event(vfs_stat_common_t *stat, enum vfs_cache_event event);
int vfs_fscache_local_stat(vfs_stat_common_t *stat,
			   vfs_fscache_stat_t *fscache_stat);
int vfs_fscache_free_local_stat(vfs_stat_common_t *stat,
				uint32_t *fscache_free_stat);
int vfs_fscache_free_local_stat_fusion(vfs_stat_common_t *stat,
				       uint32_t *fscache_free_stat);
int vfs_fchmod(int fd, mode_t mode);
int vfs_chmod(const char *pathname, mode_t mode);

int vfs_drop_caches(rref_t fsmgr_rref, unsigned int mode, int max, int reason);
int vfs_fadvise(int fd, int64_t offset, int64_t len, int advise);
int vfs_fd_get_path(int fd, __u32 cnode_idx, char *path_buf, unsigned int path_len);
int vfs_sysmgr_get_fd_path(int fd, __u32 cnode_idx, char *path_buf);
int vfs_sysmgr_get_fid_path(rref_t rref, uint32_t cnode_idx, unsigned long fid,
			    char *buf, size_t buflen);
int vfs_is_normal_fd(int fd, unsigned int cnode_idx, bool *is_normal_fd);

/* src/proc.c */
int vfs_revoke_process(rref_t rref, uint32_t cnode_idx);
int vfs_setrlimit(uint32_t cnode_idx, unsigned int type,
		  rlim_t rlim_cur, rlim_t rlim_max);

struct vfs_mmap_create_info {
	unsigned long fd_file;
	int64_t offset;
	uint64_t len;
	uint32_t cnode_idx;
	uint32_t prot;
	uint32_t flags;
};

typedef struct {
	vfs_mmap_node_t node;
	unsigned long fid;
	unsigned int page_size;
	unsigned int seals;
	unsigned int setprot;
} vfs_mmap_fileinfo;

/* src/mmap.c */
int vfs_mmap_query(int fd, __u32 cnode_idx, char *service,
		   uint32_t len, unsigned int *file_type);
int vfs_mmap_create(rref_t rref, const struct vfs_mmap_create_info *info,
		    vfs_mmap_fileinfo *fileinfo);
int vfs_mmap_release_file(rref_t rref, unsigned long fid, __u32 cnode_idx);
int vfs_mmap_change(rref_t rref, unsigned long fid, __u32 cnode_idx,
		    int64_t offset, uint64_t len, uint32_t prot);
int vfs_mmap_fill_page(rref_t rref, unsigned long fid, __u32 cnode_idx,
		       uint64_t node_page_idx, uint64_t flags,
		       uintptr_t *vaddr);
int vfs_mmap_fetch_page(rref_t rref, unsigned long fid, __u32 cnode_idx,
			uint64_t node_page_idx, uintptr_t *vaddr, bool set_mmap);
int vfs_mmap_change_page(rref_t rref, const vfs_mmap_node_t *node, unsigned long fid,
			 uint32_t cnode_idx, uint64_t node_page_idx, uint32_t flag, int mmapcnt);
int vfs_mprotect_check(rref_t rref, unsigned long fid, __u32 cnode_idx, uint32_t prot, bool shared);
/* src/ioctl.c */
int vfs_ioctl(int, unsigned int, unsigned long, int);
int vfs_ioctl_nocheck(int, unsigned int, unsigned long, int);
/* vfs_ksys ops */
int vfs_ksys_ioctl(int, unsigned int, unsigned long, int);

#define FSXATTR_PAD_SIZE 8

/* used for FS_IOC_FSGETXATTR and FS_IOC_FSSETXATTR */
struct fsxattr {
	__u32 fsxattr_xflags;
	__u32 fsxattr_extsize;
	__u32 fsxattr_nextents;
	__u32 fsxattr_projid;
	__u32 fsxattr_cowextsize;
	unsigned char fsxattr_pad[FSXATTR_PAD_SIZE];
};

/* flags for xflags field in fsxattr */
#define FSXATTR_XFLAG_PREALLOC		0x00000002 /* The file has preallocated space */
#define FSXATTR_XFLAG_IMMUTABLE		0x00000008 /* The file is immutable - it cannot be modified */
#define FSXATTR_XFLAG_APPEND		0x00000010 /* The file can only be opened in append mode for writing */
#define FSXATTR_XFLAG_SYNC		0x00000020 /* All writes to the file are synchronous */
#define FSXATTR_XFLAG_NOATIME		0x00000040 /* The file's atime record is not modified when access */
#define FSXATTR_XFLAG_NODUMP		0x00000080 /* The file should be skipped by backup utilities */
#define FSXATTR_XFLAG_PROJINHERIT	0x00000200 /* Project inheritance bit */
#define FSXATTR_XFLAG_EXTSIZE		0x00000800 /* Extent size bit */
#define FSXATTR_XFLAG_EXTSZINHERIT	0x00001000 /* Inherit inode's extent size */
#define FSXATTR_XFLAG_DAX		0x00008000 /* Use DAX for IO */
#define FSXATTR_XFLAG_COWEXTSIZE	0x00010000 /* CoW extent size allocator hint */


#define FS_IOC_FSGETXATTR		_IOR('X', 31, struct fsxattr)
#define FS_IOC_FSSETXATTR		_IOW('X', 32, struct fsxattr)

/* src/stat.c */
int vfs_fstat(int fd, struct stat *st);
int vfs_cnode_fstat(int fd, __u32 cnode_idx, struct stat *st);
int vfs_stat(const char *pathname, struct stat *st);
int vfs_stat_nocheck(const char *pathname, struct stat *st);
int vfs_fstatat(int dfd, const char *filename, struct stat *st, unsigned int flags);
/* vfs_ksys ops */
int vfs_ksys_fstat(int fd, struct stat *st);
int vfs_ksys_fstat_nocheck(int fd, struct stat *st);
int vfs_ksys_stat(const char *pathname, struct stat *st);
int vfs_ksys_fstatat(int dfd, const char *filename, struct stat *st, unsigned int flags);

/* src/sync.c */
int vfs_fsync(int file);
void vfs_sync(void);
/* vfs_ksys ops */
int vfs_ksys_fsync(int file);
int vfs_ksys_sync(void);

/* src/xattr.c */
int vfs_getxattr(const char *path, const char *name, char *value, size_t size, ssize_t *nbyte);
int vfs_setxattr(const char *path, const char *name, const char *value, size_t size, int flags);
int vfs_fsetxattr(int fd, const char *name, const char *value, size_t size, int flags);
int vfs_removexattr(const char *path, const char *name);

/* src/open.c */
struct vfs_open_exec_info {
	uint32_t cnode_idx;
	uint32_t cnode_idx_for_root;
	int *fd;
	bool *mnt_nosuid;
	struct vfs_file_cred *file_cred;
};

int vfs_chown(const char *filename, uid_t uid, gid_t gid);
int vfs_lchown(const char *filename, uid_t uid, gid_t gid);
int vfs_fchown(int fd, uid_t uid, gid_t gid);
int vfs_dup(int oldfd);
int vfs_fwd_dup2(int oldfd, int newfd, uint32_t cnode_idx);
int vfs_fcntl(int fd, int cmd, unsigned long arg);
int vfs_mkdir(const char *pathname, mode_t mode);
int vfs_access(const char *pathname, int mode);
int vfs_fwd_access(const char *pathname, int mode);
int vfs_unix_access(const char *name, int mode, vfs_unix_node_t *unode);
ssize_t vfs_readlink(const char *path, char *buf, size_t bufsize);
int vfs_sys_open(const char *filename, unsigned int flag, mode_t mode);
int vfs_cnode_open(const char *filename, unsigned int flag, mode_t mode, uint32_t cidx);
int vfs_fwd_openat(int dfd, const char *filename, unsigned int flag, mode_t modes);
int vfs_fwd_open(const char *filename, unsigned int flag, mode_t mode);
int vfs_open(const char *filename, unsigned int flag, mode_t mode);
int vfs_openat(int dfd, const char *filename, unsigned int flag, mode_t modes);
int vfs_open_exec(const char *path, struct vfs_open_exec_info *exec_info);
int vfs_close_exec(int sys_fd, uint32_t cnode_idx, int uapp_rfd);
int vfs_close(int fd);
int vfs_cnode_close(int fd, uint32_t cidx);
int vfs_fallocate(int fd, int mode, uint64_t offset, uint64_t length);
/* vfs_ksys ops */
ssize_t vfs_ksys_readlinkat(int dfd, const char *path, char *buf, size_t bufsize);
int vfs_ksys_fchown(int fd, uid_t uid, gid_t gid);
int vfs_ksys_mkdir(const char *pathname, mode_t mode);
int vfs_ksys_access(const char *pathname, int mode);
int vfs_ksys_open(const char *filename, unsigned int flag, mode_t mode);
int vfs_ksys_openat(int dfd, const char *filename, unsigned int flag, mode_t mode);
int vfs_ksys_close(int fd);

/* src/statfs.c */
int vfs_statfs(const char *path, struct statfs *stfs);
/* vfs_ksys ops */
int vfs_ksys_statfs(const char *pathname, struct statfs *stfs);

/* src/read_write.c */
ssize_t vfs_read(int fd, void *buf, size_t nbyte);
ssize_t vfs_write(int fd, const void *buf, size_t nbyte);
ssize_t vfs_pread(int fd, int64_t pos, void *buf, size_t nbyte);
ssize_t vfs_preadv(int fd, const struct iovec *vec, int vlen, int64_t pos);
ssize_t vfs_pwrite(int fd, int64_t pos, const void *buf, size_t nbyte);
ssize_t vfs_writev(int fd, const struct iovec *vec, int vlen);
ssize_t vfs_pwritev(int fd, const struct iovec *vec, int vlen, int64_t pos);
ssize_t vfs_readdir(int fd, void *buf, size_t size);
/* vfs_ksys ops */
ssize_t vfs_ksys_read(int fd, void *buf, size_t nbyte);
ssize_t vfs_ksys_readdir(int fd, void *buf, size_t size);
ssize_t vfs_ksys_readv(int fd, const struct iovec *vec, int vlen);
ssize_t vfs_ksys_pread(int fd, int64_t pos, void *buf, size_t nbyte);
ssize_t vfs_ksys_preadv(int fd, const struct iovec *vec, int vlen, int64_t pos);
ssize_t vfs_ksys_write(int fd, const void *buf, size_t nbyte);
ssize_t vfs_ksys_writev(int fd, const struct iovec *vec, int vlen);
ssize_t vfs_ksys_pwrite(int fd, int64_t pos, const void *buf, size_t nbyte);
ssize_t vfs_ksys_pwritev(int fd, const struct iovec *vec, int vlen, int64_t pos);

enum {
	FS_IOCB_CMD_PREAD = 0,
	FS_IOCB_CMD_PWRITE = 1,
	FS_IOCB_CMD_FSYNC = 2,
	FS_IOCB_CMD_POLL = 5,
	FS_IOCB_CMD_PREADV = 7,
	FS_IOCB_CMD_PWRITEV = 8,
};

struct fs_aiocb {
	__u64 fs_aio_data;
	__u32 fs_aio_key;
	int   fs_aio_rw_flags;

	__u16 fs_aio_lio_opcode;
	__s16 fs_aio_reqprio;
	__u32 fs_aio_fildes;

	__u64 fs_aio_buf;
	__u64 fs_aio_nbytes;
	__s64 fs_aio_offset;

	__u64 fs_aio_reserved2;

	__u32 fs_aio_flags;
	__u32 fs_aio_resfd;
};

struct fs_aio_event {
	__u64 fs_aio_data;
	__u64 fs_aio_obj;
	__s64 res;
	__s64 res2;
};

/* posix-clock support */
struct timex;
int vfs_clock_settime(clockid_t clk_id, const struct timespec *tp);
int vfs_clock_gettime(clockid_t clk_id, struct timespec *tp);
int vfs_clock_getres(clockid_t clk_id, struct timespec *tp);
int vfs_clock_adjtime(clockid_t clk_id, struct timex *tx);

/* src/fd.c */
int hm_get_fd_type(int fd);

unsigned int hm_spawn_clone_flags(void);

/* src/eventfd.c */
int vfs_write_eventfd(int fd, const void *buf, size_t len,
		      __u32 cnode_idx);

/* src/signalfd.c */
int vfs_init_signalfd(int fd, __u64 sigmask, int flags);
int vfs_signalfd_notify(__u32 cnode_idx, const void *buf, int fdcnt);

/* src/timerfd.c */
void vfs_timerfd_cancel(void);
int vfs_timerfd_next_alarm_pid_and_name(int *pid_rp, char *name_rp, int name_size);

void set_fs_relocated(int relocated);
int get_fs_relocated(void);
int vfs_transfs_query_dinfo(const char *pathname, unsigned int *s_index,
			    unsigned int *d_index);
int vfs_transfs_put_dentry(unsigned int sb_index, unsigned int d_index);
int vfs_transfs_fget(int fd, unsigned int *sb_index, unsigned int *f_index,
		     unsigned int *me_index, unsigned long long *filp);
int vfs_transfs_fput(unsigned int me_idx, unsigned long long hm_filp);
int fqsn_to_dev_id(const char *fqsn, dev_t *dev_id);
int vfs_proc_has_binder_work(rref_t rref, int pid, int *has_binder_work);
#endif	/* LIBHMSRV_FS_VFS_H */
