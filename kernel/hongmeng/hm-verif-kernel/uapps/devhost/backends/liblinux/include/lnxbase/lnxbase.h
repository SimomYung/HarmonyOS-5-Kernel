/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: base APIs for liblinux backend
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 4 09:40:43 2021
 */
#ifndef __LNXBASE_H__
#define __LNXBASE_H__

#ifndef __KERNEL__
#include <stdlib.h> /* for size_t, no need for linux */
#endif

#include <asm/types.h>
#include <lnxbase/ksymtab.h>
#include <liblinux/pal.h>

#define RTC_OWNER_NAME_LEN	16U

struct lnxbase_ctx;
struct wl_stat;

struct lnxbase_module_ops {
	int (*is_compatible)(const char *libname,
			     const void *data, size_t size);
	int (*load_module)(const char *libname,
			   const void *data, size_t size, char *args);
	int (*unload_module)(const char *libname);
	int (*load_symbol_module)(const char *libname,
				  const struct lnxbase_syminfo *si);
	void *(*symbol_get)(const char *symbol);
};

int lnxbase_rtc_read_pid_and_name(int *pid_rp, char *name_rp, int name_size);
bool lnxbase_hmkernel_is_support_rtc(void);

typedef void *(*lnxbase_fget_fn_t)(int fd, void (*fget)(void*));
typedef void (*lnxbase_fput_fn_t)(void *file);
typedef void *(*lnxbase_gdentry_fn_t)(const char *pathname);
typedef void (*lnxbase_pdentry_fn_t)(void *sb, void *dentry);
typedef int (*lnxbase_postinit_fn_t)(void *args);

void lnxbase_register_reclaim_func(void (*func)(void), const char *name);

/* lnxbase backend APIs */
struct dh_backend_ctor;
struct lnxbase_ctx *lnxbase_newctx(struct dh_backend_ctor *ctor);
void lnxbase_destroy(struct lnxbase_ctx *ctx);

int lnxbase_enable_module(struct lnxbase_ctx *ctx,
			  struct lnxbase_module_ops *ops);

int lnxbase_enable_netlink(struct lnxbase_ctx *ctx,
			   int (*netlink_init_fn)(struct netlink_pal_handler *));

int lnxbase_enable_timesync(struct lnxbase_ctx *ctx, void (*sync_hdlr)(void));
void lnxbase_enable_skip_first_timesync(struct lnxbase_ctx *ctx);
int lnxbase_enable_backtrace(struct lnxbase_ctx *ctx,
			     int (*caller_resolver)(char *buf, size_t buf_sz, const void *caller),
			     void *(return_address)(unsigned int n));
int lnxbase_enable_vmallocinfo(void);

int lnxbase_init(struct lnxbase_ctx *ctx,
		 lnxbase_postinit_fn_t fn, void *args);

int lnxbase_load_symbols(const struct lnxbase_ctx *ctx, const char *modname,
			 const struct lnxbase_syminfo *si);

typedef int (*lnxbase_post_populate_fn_t)(void);
int lnxbase_enable_post_dev_populate(struct lnxbase_ctx *ctx,
				     lnxbase_post_populate_fn_t fn);
int lnxbase_root_notify(struct lnxbase_ctx *ctx);
int lnxbase_support_device_autoprobe(void);
void lnxbase_enable_device_autoprobe(struct lnxbase_ctx *ctx);
int lnxbase_set_default_ctx(const struct lnxbase_ctx *ctx);
long lnxbase_ksys_symlink(const char *oldname, const char *newname);

#define LNXBASE_FWTYPE_LEGACY	(0x1U)
#define LNXBASE_FWTYPE_FDT	(0x2U)
#define LNXBASE_FWTYPE_ALL	(0xFFU)
void lnxbase_enable_bootfdt(struct lnxbase_ctx *ctx);
void *__lnxbase_acquire_bootfdt(unsigned long *size, unsigned int fwtype_mask);

static inline void *lnxbase_acquire_bootfdt(unsigned long *size)
{
	return __lnxbase_acquire_bootfdt(size, LNXBASE_FWTYPE_ALL);
}

struct lnxbase_cred_dac {
	unsigned int cnode_idx;
	unsigned int uid;
	unsigned int gid;
	unsigned int suid;
	unsigned int sgid;
	unsigned int euid;
	unsigned int egid;
	unsigned int fsuid;
	unsigned int fsgid;
	int umask;
};
void lnxbase_get_dac(struct lnxbase_cred_dac *lnxbase_cred_ids);
unsigned int lnxbase_current_cnode_idx(void);

struct devhost_fops;
struct device_info;
int lnxbase_anon_devinfo_init(struct device_info *anon);
int lnxbase_get_unused_fd_flags(unsigned flags);
struct dh_fd_install_context;
void lnxbase_fd_install(unsigned int fd, void *file, struct dh_fd_install_context *ctx);
int lnxbase_alloc_unused_fd(unsigned int flags, void *file, struct dh_fd_install_context *ctx);
void lnxbase_put_unused_fd(int fd);

unsigned long lnxbase_shrink_mem(int size);
void lnxbase_flush_cache_range_by_pa(unsigned int cmd, unsigned long long start, unsigned long long end);

/*
 * module support helper
 */
int lnxbase_fill_mod_symbol(const char *name, const struct liblinux_mod_syminfo *si);
void lnxbase_drop_mod_symbol(const void *module_core);

int lnxbase_dev_synchronize(void);
void lnxbase_invoke_on_cpu(unsigned int cpu,
			   void (*callback)(void *), void *data);

/*
 * timer related helper
 */
struct lnxbase_clocksource {
	__u64 (*read)(void);
	__u32 mult;
	__u32 shift;
};
const struct lnxbase_clocksource *lnxbase_default_clocksource(void);

/* vtimer */
typedef void (*event_handler_p)(unsigned int);
typedef int (*thread_init_p)(unsigned int);
int lnxbase_vtimer_init(event_handler_p handler_func, thread_init_p init_func, uint32_t cpu_mask);
int lnxbase_vtimer_set_next_event(int expires_ns);

/*
 * extension prototypes
 *
 * @return 0 if success, POSIX errno if fail
 */
int dh_extension_init(struct lnxbase_ctx *ctx);
#define DH_EXTRALIB_INIT "dh_extension_init"
typedef int (*dh_extension_init_fn_t)(struct lnxbase_ctx *ctx);

int lnxbase_load_extension(struct lnxbase_ctx *ctx, const char *extension);

/* file related */
enum liblinux_fd_type;
int lnxbase_register_fget(enum liblinux_fd_type fd_type, lnxbase_fget_fn_t fn);
int lnxbase_register_fput(enum liblinux_fd_type fd_type, lnxbase_fput_fn_t fn);
int lnxbase_register_dentry(lnxbase_gdentry_fn_t gfn, lnxbase_pdentry_fn_t pfn);
lnxbase_fget_fn_t lnxbase_get_fget_fn(enum liblinux_fd_type fd_type);
lnxbase_fput_fn_t lnxbase_get_fput_fn(enum liblinux_fd_type fd_type);
lnxbase_gdentry_fn_t lnxbase_get_dentry_fn(void);
lnxbase_pdentry_fn_t lnxbase_put_dentry_fn(void);

struct lnxbase_args {
	char *extensions;
};

const struct lnxbase_args *lnxbase_backend_args(void);
const char *lnxbase_backend_args_find(const char *key);

/* backtrace related */
int lnxbase_resolve_caller(char *buf, size_t buf_sz, const void *caller);
void *lnxbase_return_address(unsigned int n);

/* sysinfo helper */
struct lnxbase_sysinfo {
	unsigned long long memstart;
	unsigned long long memend;
	unsigned long long total_ram;
};
int lnxbase_sysinfo_get(struct lnxbase_sysinfo *info);
int lnxbase_max_cspace_slots_get(unsigned long *max);

int lnxbase_verify_cms_data(const void *data, unsigned long len,
			    const void *sig, unsigned long sig_len);

int lnxbase_load_extra_modules(struct lnxbase_ctx *ctx);

int lnxbase_wakelock_stat_enqueue(const struct wl_stat *stat, void *args);

struct liblinux_kmap_info lnxbase_get_kmap_info(void);
void *lnxbase_mm_alloc(unsigned long size, void **handle);
int lnxbase_mm_query(void *handle, void *ranges, unsigned int ranges_num);
void lnxbase_mm_free(void *handle);

struct stat;
struct statfs;
struct iovec;
int lnxbase_ksys_access(const char *pathname, int mode);
int lnxbase_vfs_open(const char *filename, unsigned int flag, mode_t mode);
ssize_t lnxbase_ksys_pwrite(int fd, int64_t pos, const void *buf, size_t nbyte);
ssize_t lnxbase_ksys_pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset);
ssize_t lnxbase_ksys_pread(int fd, int64_t pos, void *buf, size_t nbyte);
ssize_t lnxbase_ksys_preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset);
int lnxbase_vfs_close(int fd);
int lnxbase_vfs_ioctl(int fd, unsigned int cmd, unsigned long arg, int arg_sz);
ssize_t lnxbase_vfs_read(int fd, void *buf, size_t nbyte);
int lnxbase_vfs_fstat(int fd, struct stat *stat);
int lnxbase_ksys_statfs(const char *pathname, struct statfs *statfs);
int lnxbase_ksys_fstat(int fd, struct stat *stat);
int lnxbase_ksys_fstat_nocheck(int fd, struct stat *stat);
int lnxbase_ksys_fstatat(int dfd, const char *filename, struct stat *stat, unsigned int flags);
int lnxbase_ksys_rename(const char *oldpath, const char *newpath);
int lnxbase_ksys_readlinkat(int dfd, const char *path, char *buf, size_t bufsize);
int lnxbase_ksys_mkdir(const char *pathname, mode_t mode);
int lnxbase_ksys_rmdir(const char *pathname);
int lnxbase_ksys_unlink(const char *pathname);
int lnxbase_ksys_fchown(int fd, uid_t uid, gid_t gid);

int lnxbase_ksys_open(const char *filename, unsigned int flag, mode_t mode);
int lnxbase_ksys_openat(int dfd, const char *filename, unsigned int flag, mode_t mode);
int lnxbase_ksys_close(int fd);
int lnxbase_ksys_ioctl(int fd, unsigned int cmd, unsigned long arg, int arg_sz);
int lnxbase_ksys_lseek(int fd, int64_t offset, int whence, int64_t *pos);
ssize_t lnxbase_ksys_write(int fd, const void *buf, size_t nbyte);
ssize_t lnxbase_ksys_readdir(int fd, void *buf, size_t nbyte);
ssize_t lnxbase_ksys_read(int fd, void *buf, size_t nbyte);
int lnxbase_ksys_fsync(int fd);
int lnxbase_ksys_sync(void);

typedef void (*try_stop_handler_p)(int);
void lnxbase_enable_stop_tick(try_stop_handler_p stop_func);

typedef unsigned long (*lnxbase_pages_stat_p)(void);
enum lnxbase_pages_stat_type {
	LNXBASE_DISCRETEPOOL_USED_PAGES,
	LNXBASE_DISCRETEPOOL_FREE_PAGES,
	LNXBASE_VMEMMAP_RSV_PAGES,
	LNXBASE_PAGES_STAT_MAX
};
void lnxbase_stat_func_register(enum lnxbase_pages_stat_type type, lnxbase_pages_stat_p func);

struct devhost_mem_info;
void lnxbase_pages_stat(struct devhost_mem_info *mem_info);
#endif /* __LNXBASE_H__ */
