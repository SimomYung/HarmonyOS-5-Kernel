/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_LINUX_H__
#define __LIBDH_LINUX_H__

#include <stddef.h>
#include <stdbool.h>
#ifndef __KERNEL__
#include <stdint.h>
#include <hmkernel/types.h>
#endif

/* for liblinux arch setup */
extern void *__fdt_pointer;

/* for writeback flusher */
extern void liblinux_drop_slab(void);
extern void liblinux_drop_pagecache(void);

/* GFP for backend ctor */
int dh_ctor_handle_fault(void *va, unsigned long long vr_key, unsigned long address, unsigned int flags, void *regs);
int dh_ctor_mm_rdlock(void);
void dh_ctor_mm_rdunlock(void);
void *dh_backend_alloc_page(unsigned int order);

int dh_post_dev_populate_call(void);

/* keymgr related */
long liblinux_key_request(const char *type, const char *name,
			 const char *callout_info, int dest_keyring);

long liblinux_key_read(unsigned long id, unsigned long buf, unsigned long size);

/* backtrace related */
unsigned int stack_trace_save(unsigned long *store, unsigned int size,
			      unsigned int skipnr);
int liblinux_print_symbol(char *buf, size_t buf_sz, const void *caller);
void *liblinux_return_address(unsigned int n);

/* samples/ldk/power/wakelock.c */
struct wakeup_source *liblinux_acquire_deleted_ws(void);

/* reboot related */
void reboot_to_bootloader(bool take_snapshot);
void reboot_with_snapshot(char *reboot_reason);

void current_thread_name_fill(char *comm, unsigned int size);
int liblinux_process_pid(void);

void __liblinux_assign_vma_info(void *dh_vma, void *ldk_vma);
int __liblinux_mm_release(unsigned long long vspace_key);

extern void emergency_restart(void);

void liblinux_fill_thread_info(int *pid, int *tgid, int *vtgid, unsigned long long *vspace_key,
				char *comm, unsigned int size);
void liblinux_enter_nodac(void);

#ifndef __KERNEL__
/*
 * Prototypes used by non-linux namespace
 */
extern __attribute__((format(printf, 2, 3)))
int __request_module(int wait, const char *name, ...);
#define request_module(mod...) __request_module(1, mod)

/* used in devhost.ko/dma.c */
extern void __dma_map_area(const void *, size_t, int);
extern void __dma_unmap_area(const void *, size_t, int);
extern void __dma_flush_area(const void *, size_t);

/* for extralib init */
typedef int (*initcall_t)(void);
extern initcall_t __extralib_initcall_start[];
extern initcall_t __extralib_initcall_end[];
int dh_extralib_init(void);

/*
 * since devhost_post_populate_register is defined in devhost.ko
 * but we can not include its head file, we extern declare it here
 */
typedef int (*post_populate_fn)(void);
extern int dh_post_dev_populate_register(post_populate_fn fn);

/* used in devhost.ko/kvic.c */
#ifdef CONFIG_DEVHOST_LIBLINUX_4_14_M536A
extern struct fwnode_handle *__irq_domain_alloc_fwnode(unsigned int type, int id,
						       const char *name, void *data);
#else
extern struct fwnode_handle *irq_domain_alloc_fwnode(void *data);
#endif
extern void irq_domain_free_fwnode(struct fwnode_handle *fwnode);
extern void irq_domain_reset_irq_data(void *irq_data);
extern void *irq_domain_get_irq_data(void *domain, unsigned int virq);

/* for framebuffer */
void console_lock(void);
void console_unlock(void);

void *compat_alloc_user_space(unsigned long len);

extern char *strerror(int e);

struct timespec64 {
	long long tv_sec;
	long tv_nsec;
};

/* liblinux timekeeping */
void read_persistent_clock64(struct timespec64 *ts64);
void read_boot_clock64(struct timespec64 *ts64);
int do_settimeofday64(const struct timespec64 *ts64);

__u64 get_cpu_idle_time_us(int cpu, __u64 *total_time);

/* module symbol APIs */
void *__symbol_get(const char *symbol);
/* the type of return value must be int */
#define dh_linux_call_symbol_int(sym_name, ...)		\
({							\
	int __ret = -ENOENT;				\
	void *__tmp_sym_ptr = __symbol_get(#sym_name);	\
	if (__tmp_sym_ptr != NULL) {			\
		__ret = ((typeof(sym_name) *)(uintptr_t)__tmp_sym_ptr)(__VA_ARGS__);	\
	}						\
	__ret;						\
})

/* ldk-5.10 symbols */
struct path;
struct file_operations;
struct file *alloc_file(const struct path *path, int flags,
			const struct file_operations *fop);
#endif /* __KERNEL__ */

/* hw_vote APIs used in drivers of ko */
extern int send_to_user(int sock_no, size_t len, const int *data);

extern void liblinux_try_stop_tick(int cpu);
extern bool check_do_timer_none();

void liblinux_vhangup(void);

#endif /* __LIBDH_LINUX_H__ */
