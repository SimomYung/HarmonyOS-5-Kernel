// SPDX-License-Identifier: GPL-2.0
#include <hmkernel/types.h>
#include <hmkernel/compiler.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhmsrv_key/key.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_proc_affinity.h>
#include <libhguard/hguard.h>

#define restrict __restrict
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsrv_sys/hm_init.h>
#include <libhmsrv_sys/hm_watchdog.h>

#include <libkbox/kbox.h>
#include <libhmpm/cpuhp.h>
#include <liblinux/pal.h>
#include <libhmpm/shutdown.h>
#include <lnxbase/lnxbase.h>
#include <devhost/log.h>
#include "libdh_linux.h"
#include <libkev/kev.h>
#include <libkcov/kcov.h>
#include <hmkernel/events/kev.h>
#include <hmkernel/power_reboot.h>
#include <hmkernel/lastklog.h>
#include <hmkernel/ctrlmem/sysconf.h>
#include <fcntl.h>
#include <stdlib.h>

char *liblinux_pal_realpath(const char *path, char *buf, unsigned long len)
{
	char *tmp = NULL;
	int ret;

	if (buf == NULL)
		return NULL;

	tmp = realpath(path, NULL);
	if (tmp == NULL)
		return NULL;

	ret = strncpy_s(buf, len, tmp, strlen(tmp));
	free(tmp);
	if (ret != 0)
		return NULL;
	return buf;
}
EXPORT_SYMBOL(liblinux_pal_realpath);

/* used in devhost.ko/dma.c */
EXPORT_SYMBOL(__dma_map_area);
EXPORT_SYMBOL(__dma_unmap_area);
EXPORT_SYMBOL(__dma_flush_area);

int liblinux_cpu_up(unsigned int cpu)
{
	return hm_cpu_up(cpu) ? -1 : 0;
}
EXPORT_SYMBOL(liblinux_cpu_up);

int liblinux_cpu_down(unsigned int cpu)
{
	return hm_cpu_down(cpu) ? -1 : 0;
}
EXPORT_SYMBOL(liblinux_cpu_down);

/* weak symbol for liblinux-4.4, which use the old name __dma_flush_range() */
void __attribute__((weak))
__dma_flush_area(const void *start, size_t size)
{
	liblinux_pal_flush_cache_range(LIBLINUX_PAL_DCACHE_FLUSH_RANGE,
				       (unsigned long)(uintptr_t)start,
				       (unsigned long)((uintptr_t)start + size));
}

/* weak symbols for liblinux arm32 nop cache model */
void __attribute__((weak)) __dma_map_area(const void *start, size_t size, int dir)
{
	UNUSED(start);
	UNUSED(size);
	UNUSED(dir);
}
void __attribute__((weak)) __dma_unmap_area(const void *start, size_t size, int dir)
{
	UNUSED(start);
	UNUSED(size);
	UNUSED(dir);
}

#ifdef CONFIG_MDC_GODEL_HACK
/* file_operation */
#include <libvsyscall/hmvsyscall.h>
EXPORT_SYMBOL(hm_vsyscall);
#endif /* CONFIG_MDC_GODEL_HACK */

EXPORT_SYMBOL(compat_alloc_user_space);
void *compat_alloc_user_space(unsigned long len)
{
	if (len == 0) {
		return NULL;
	}

	return malloc((size_t)len);
}

EXPORT_SYMBOL(strerror);

/*
 * Currently removing this override function on phone will reset time
 * to 1970.01.01 after rebooting. Solving this problem relys on an RTC
 * initialization refactor in kernel.
 */
#ifndef CONFIG_DEVHOST_SKIP_TIMESYNC
int __override do_settimeofday64(const struct timespec64 *ts64)
{
	struct timespec ts;

	if (ts64 == NULL) {
		return -EINVAL;
	}

	ts.tv_sec = (long)ts64->tv_sec;
	ts.tv_nsec = ts64->tv_nsec;
	return -hmerrno2posix(hm_clock_settime(CLOCK_REALTIME, &ts));
}
#endif

void read_boot_clock64(struct timespec64 *ts64)
{
	struct timespec ts;
	int ret;

	mem_zero_s(ts);
	ret = hm_clock_gettime(CLOCK_BOOTTIME, &ts);
	if (ret < 0) {
		dh_warn("get CLOCK_BOOTTIME failed: %s\n", hmstrerror(ret));
	}
	ts64->tv_sec = (long long)ts.tv_sec;
	ts64->tv_nsec = ts.tv_nsec;
}

EXPORT_SYMBOL(liblinux_pal_preempt_init);
EXPORT_SYMBOL(liblinux_pal_preempt_resched);
EXPORT_SYMBOL(liblinux_pal_preempt_enable);
EXPORT_SYMBOL(liblinux_pal_preempt_disable);
EXPORT_SYMBOL(liblinux_pal_process_set_slv);
EXPORT_SYMBOL(liblinux_pal_thread_set_slv);
EXPORT_SYMBOL(liblinux_pal_processor_id);
EXPORT_SYMBOL(liblinux_pal_local_irq_enable);
EXPORT_SYMBOL(liblinux_pal_local_irq_disable);
EXPORT_SYMBOL(liblinux_pal_local_irq_save);
EXPORT_SYMBOL(liblinux_pal_local_irq_restore);
EXPORT_SYMBOL(liblinux_pal_local_save_flags);
EXPORT_SYMBOL(liblinux_pal_irq_set_affinity_hint);

EXPORT_SYMBOL(lnxbase_default_clocksource);
EXPORT_SYMBOL(dh_post_dev_populate_call);

long liblinux_key_request(const char *type, const char *name,
			  const char *callout_info, int dest_keyring)
{
#ifdef CONFIG_KEYS
	return hm_key_request(type, name, callout_info, dest_keyring);
#else
	UNUSED(type, name, callout_info, dest_keyring);
	hm_info("not support key request\n");
	return -ENOTSUP;
#endif
}
EXPORT_SYMBOL(liblinux_key_request);

long liblinux_key_read(unsigned long id, unsigned long buf, unsigned long size)
{
#ifdef CONFIG_KEYS
	return hm_key_control(KEY_CONTROL_READ, id, buf, size, 0);
#else
	UNUSED(id, buf, size);
	hm_info("not support key control\n");
	return -ENOTSUP;
#endif
}
EXPORT_SYMBOL(liblinux_key_read);
EXPORT_SYMBOL(liblinux_dfx_vmalloc_hook);
EXPORT_SYMBOL(liblinux_pal_malloc);
EXPORT_SYMBOL(liblinux_pal_calloc);
EXPORT_SYMBOL(liblinux_pal_free);
EXPORT_SYMBOL(liblinux_pal_mincore);

/* ldk-5.10 symbols */
EXPORT_SYMBOL(alloc_file);

#ifdef LNXBASE_HELPER_ENABLE_BACKTRACE
/* backtrace related */
int liblinux_print_symbol(char *buf, size_t buf_sz, const void *caller)
{
	return snprintf(buf, buf_sz, "%pS", caller);
}

void *liblinux_return_address(unsigned int n)
{
	unsigned long raddr;
	unsigned int len;
	len = stack_trace_save(&raddr, 1, n + 1);
	if (len == 1) {
		return (void *)raddr;
	}
	return NULL;
}
#endif

void liblinux_thread_get_default_affinity(unsigned long *mask)
{
	struct proc_affinity_info_s affinity_info;

	mem_zero_s(affinity_info);
	int ret = hm_proc_affinity_get(0, &affinity_info);
	if (ret == 0) {
		*mask = (unsigned long)affinity_info.default_affinity.__bits;
	} else {
		dh_error("failed to get default activate mask: %s\n", hmstrerror(ret));
		*mask = 0;
	}
}

#ifdef CONFIG_RTOS_MODULE_CERT_REVOCATE
int kernel_verify_cms_data(const void *data, unsigned long len,
			   const void *sig, unsigned long sig_len)
{
	return lnxbase_verify_cms_data(data, len, sig, sig_len);
}
EXPORT_SYMBOL(kernel_verify_cms_data);
#endif

#ifdef CONFIG_ARM_ARCH_TIMER
__u32 arch_timer_get_rate(void)
{
	int ret;
	unsigned long clock_freq = 0UL;

	ret = hm_sysctrl_read_conf(__SYSCONF_CLOCK_FREQ,
				   (void *)&clock_freq, sizeof(clock_freq));
	if (ret < 0) {
		dh_error("get clock freq failed, ret=%s\n", hmstrerror(ret));
	}

	return (__u32)clock_freq;
}
#endif

void liblinux_stop_feed_harddog(unsigned int timeout_sec)
{
	int ret = 0;
	if (timeout_sec > 0) {
		dh_info("Stop feed harddog, timeout: %u\n", timeout_sec);
		ret = hm_stop_feed_harddog(timeout_sec);
		if (ret != 0) {
			dh_error("Failed to stop feed harddog. err:%s\n", hmstrerror(ret));
		}
	}
}

void liblinux_stop_feed_highdog(unsigned int timeout_sec)
{
	int ret = 0;
	if (timeout_sec > 0) {
		dh_info("Stop feed highdog, timeout: %u\n", timeout_sec);
		ret = hguard_procfs_stop_watchdog_for_reboot(timeout_sec);
		if (ret != 0) {
			dh_error("Failed to stop feed highdog. err:%s\n", hmstrerror(ret));
		}
	}
}

void reboot_with_snapshot(char *reboot_reason)
{
	(void)hm_sysctrl_sysmgr_reboot(reboot_reason, SYSCTRL_REBOOT_FLAG_TAKE_SNAPSHOT);
}

void reboot_to_bootloader(bool take_snapshot)
{
	int flags = 0;
	if (take_snapshot) {
		flags |= REBOOT_FLAG_TAKE_SNAPSHOT;
	}
	hminit_notify_reboot("bootloader", flags);
}

int liblinux_reboot(const char *cmd, bool is_abnormal)
{
	int ret;
	if (!is_abnormal) {
		ret = hm_reboot(cmd);
	} else {
		ret = hm_sysctrl_sysmgr_reboot(cmd,
					       (SYSCTRL_REBOOT_FLAG_TAKE_SNAPSHOT |
						SYSCTRL_REBOOT_FLAG_TAKE_COREDUMP));
	}
	return ret;
}

long __override ksys_access(const char *filename, int mode)
{
	return (long)liblinux_pal_ksys_access(filename, mode);
}
EXPORT_SYMBOL(ksys_access);

long __override ksys_rename(const char *oldname, const char *newname)
{
	return (long)liblinux_pal_ksys_rename(oldname, newname);
}
EXPORT_SYMBOL(ksys_rename);

long __override ksys_mkdir(const char *pathname, unsigned short mode)
{
	return (long)liblinux_pal_ksys_mkdir(pathname, mode);
}
EXPORT_SYMBOL(ksys_mkdir);

int liblinux_read_klog_config(uint64_t *paddr, uint32_t *size)
{
	int ret;
	struct kev_config config;

	ret = kev_read_event_config(KEV_TYPE_KLOG, &config);
	if (ret != 0 || !config.enable) {
		return -EINVAL;
	}

	if (paddr != NULL) {
		*paddr = (uint64_t)config.paddr;
	}

	if (size != NULL) {
		*size = (uint32_t)config.size;
	}

	return 0;
}

int liblinux_read_kbox_config(unsigned long long *paddr, unsigned long *size)
{
	int ret;
	struct kbox_info info = {};

	if (paddr == NULL || size == NULL)
		return -EINVAL;

	ret = kbox_query(&info);
	if (ret != 0) {
		dh_error("failed to query kbox info, ret=%s\n", hmstrerror(ret));
		return -EINVAL;
	}

	*paddr = info.paddr;
	*size = info.total_size;
	return 0;
}

int liblinux_erase_kbox_region(const char *name)
{
	int err = 0;
	int fd = kbox_open(name);
	if (fd >= 0) {
		err = kbox_erase_raw(name);
		(void)kbox_close(fd);
		err = -hmerrno2posix(err);
	} else {
		err = -ENOENT;
	}

	return err;
}

int liblinux_iter_last_klog(unsigned long max_read_len,
		int (*dump_func)(const char *log, unsigned int len, uintptr_t arg), uintptr_t arg)
{
	return kev_iter_last_klog(max_read_len, dump_func, arg);
}

int liblinux_iter_current_klog(unsigned long max_read_len,
		int (*dump_func)(const char *log, unsigned int len, uintptr_t arg), uintptr_t arg)
{
	return kev_iter_current_klog(max_read_len, dump_func, arg);
}

int liblinux_iter_on_klog_data(const char *buffer, size_t buflen,
			       int (*dump_func)(const char *log, unsigned int len, uintptr_t arg), uintptr_t arg)
{
       return kev_iter_on_klog_data(buffer, buflen, dump_func, arg);
}

int liblinux_read_klog_data(char *buffer, size_t buflen, ssize_t offset, size_t *ret_read_len)
{
       return kev_read_klog_data(buffer, buflen, offset, ret_read_len);
}

long __override ksys_rmdir(const char *pathname)
{
	return liblinux_pal_ksys_rmdir(pathname);
}

long __override ksys_unlink(const char *pathname)
{
	return liblinux_pal_ksys_unlink(pathname);
}

long __override do_sys_open(int dfd, const char *filename, int flags, unsigned short mode)
{
	return liblinux_pal_ksys_openat(dfd, filename, flags, mode);
}

int ksys_ioctl(unsigned int fd, unsigned int cmd, unsigned long arg)
{
	return liblinux_pal_ksys_ioctl(fd, cmd, arg, sizeof(arg));
}

ssize_t __override ksys_write(unsigned int fd, const char *buf, size_t count)
{
	return liblinux_pal_ksys_write(fd, buf, count);
}

ssize_t __override ksys_read(unsigned int fd, char *buf, size_t count)
{
	return liblinux_pal_ksys_read(fd, buf, count);
}

int __override ksys_getdents64(unsigned int fd, char *buf, size_t count)
{
	return liblinux_pal_ksys_readdir(fd, buf, count);
}

/* Need replace by ksys_chown */
int __override do_fchownat(int dfd, const char *filename, uid_t user,
			   gid_t group, int flag)
{
	int ret;
	int fd;

	fd = liblinux_pal_ksys_openat(dfd, filename, O_RDONLY, 0);
	if (fd < 0) {
		return fd;
	}

	ret = liblinux_pal_ksys_fchown(fd, user, group);
	liblinux_pal_ksys_close(fd);
	return ret;
}

long __override ksys_symlink(const char *oldname, const char *newname)
{
	return liblinux_pal_ksys_symlink(oldname, newname);
}

off_t __override ksys_lseek(unsigned int fd, off_t offset, unsigned int whence)
{
	int64_t tmp = 0;
	int ret = 0;
	ret = liblinux_pal_ksys_lseek(fd, offset, whence, &tmp);
	return ret >= 0 ? tmp : ret;
}

#ifdef CONFIG_BOOT_DETECTOR
long __override ksys_statfs(const char *path, struct statfs *buf)
{
	return (long)liblinux_pal_ksys_statfs(path, buf);
}
#endif

int __override ksys_fsync(unsigned int fd)
{
	return liblinux_pal_ksys_fsync(fd);
}

void __override ksys_sync(void)
{
	(void)liblinux_pal_ksys_sync();
}

bool liblinux_is_lastklog_valid(const char *klog_buff, size_t len, unsigned int *ret_header_len)
{
	bool valid = false;
	unsigned int header_len = (unsigned int)sizeof(struct __lastklog_header_s);
	if (len >= header_len) {
		valid = __lastklog_header_is_valid(klog_buff);
	}
	if (ret_header_len != NULL) {
		*ret_header_len = header_len;
	}

	return valid;
}

int liblinux_read_lastklog_memory_config(uint64_t *addr, uint32_t *size)
{
       return hm_reserved_memory_read_config(__CTRLMEM_SYSCONF_RESERV_MEM_LASTKLOG, addr, size);
}

EXPORT_SYMBOL(__sanitizer_cov_trace_pc);
EXPORT_SYMBOL(__sanitizer_cov_trace_cmp1);
EXPORT_SYMBOL(__sanitizer_cov_trace_cmp2);
EXPORT_SYMBOL(__sanitizer_cov_trace_cmp4);
EXPORT_SYMBOL(__sanitizer_cov_trace_cmp8);
EXPORT_SYMBOL(__sanitizer_cov_trace_const_cmp1);
EXPORT_SYMBOL(__sanitizer_cov_trace_const_cmp2);
EXPORT_SYMBOL(__sanitizer_cov_trace_const_cmp4);
EXPORT_SYMBOL(__sanitizer_cov_trace_const_cmp8);
EXPORT_SYMBOL(__sanitizer_cov_trace_switch);
