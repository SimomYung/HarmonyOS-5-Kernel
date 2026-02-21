/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes for liblinux backend
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 22 10:40:12 2019
 */
#ifndef __LIBDH_LINUX_H__
#define __LIBDH_LINUX_H__

#include <stddef.h>
#include <stdint.h>
#include <hmkernel/types.h>

/* for writeback flusher */
extern void liblinux_drop_slab(void);

extern int liblinux_devices_autoprobe;
extern int liblinux_drivers_autoprobe;

int dh_post_dev_populate_call(void);

extern __attribute__((format(printf, 2, 3)))
int __request_module(int wait, const char *name, ...);
#define request_module(mod...) __request_module(1, mod)
/* extra linux symbols */
struct device;
struct class;
extern int (*platform_notify)(struct device *dev);
extern int (*platform_notify_remove)(struct device *dev);
extern struct class block_class;
extern struct blocking_notifier_head reboot_notifier_list;

extern void calibrate_delay(void);
extern void machine_power_off(void);
extern void machine_restart(char *cmd);
extern void syscore_shutdown(void);

/* used in transfs.ko/transfs.c */
#ifdef DEVHOST_BACKEND_LIBLINUX_5_10
void wakeup_flusher_threads(int reason);
#else
void wakeup_flusher_threads(long nr_pages, int reason);
#endif

/* used in devhost.ko/dma.c */
extern void __dma_map_area(const void *, size_t, int);
extern void __dma_unmap_area(const void *, size_t, int);
extern void __dma_flush_area(const void *, size_t);

/* used in of.ko, for clk subsystem init */
struct of_device_id;
extern void of_clk_init(const struct of_device_id *matches);

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

/* used in devhost.ko/virqchip.c */
extern unsigned long __handle_domain_irq;

/* used in devhost.ko/kvic.c */
extern struct fwnode_handle *irq_domain_alloc_fwnode(void *data);
extern void irq_enter(void);
extern void irq_exit(void);
extern void irq_domain_free_fwnode(struct fwnode_handle *fwnode);
extern void irq_domain_reset_irq_data(void *irq_data);
extern void *irq_domain_get_irq_data(void *domain, unsigned int virq);

extern void of_alias_scan(void* (*dt_alloc)(__u64 size, __u64 align));

/* used in devhost.ko/core.c */
extern const char *device_get_devnode(struct device *dev,
				      unsigned short *mode, unsigned int *uid,
				      unsigned int *gid, const char **tmp);

/* for framebuffer */
void console_lock(void);
void console_unlock(void);

void *compat_alloc_user_space(unsigned long len);

extern char *strerror(int e);

/* liblinux timekeeping */
struct timespec64;
void read_persistent_clock64(struct timespec64 *ts64);
void read_boot_clock64(struct timespec64 *ts64);
int __getnstimeofday64(struct timespec64 *ts64);
int do_settimeofday64(const struct timespec64 *ts64);
void getrawmonotonic64(struct timespec64 *ts64);

/* liblinux reboot */
void machine_emergency_restart(void);

__u64 get_cpu_idle_time_us(int cpu, __u64 *total_time);

/* libfdt APIs */
int fdt_create(void *buf, int bufsize);
int fdt_add_reservemap_entry(void *fdt, uint64_t addr, uint64_t size);
int fdt_finish_reservemap(void *fdt);
int fdt_begin_node(void *fdt, const char *name);
int fdt_property(void *fdt, const char *name, const void *val, int len);
int fdt_end_node(void *fdt);
int fdt_finish(void *fdt);

/* OF_DYNAMIC APIs */
struct property;
struct device_node;
typedef unsigned gfp_t;
struct property *__of_prop_dup(const struct property *prop, gfp_t allocflags);
struct device_node *__of_node_dup(const struct device_node *np, const char *fmt, ...);

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
unsigned long kallsyms_lookup_name(const char *name);

/* keymgr related */
long liblinux_key_request(const char *type, const char *name,
			 const char *callout_info, int dest_keyring);

long liblinux_key_read(unsigned long id, unsigned long buf, unsigned long size);

#endif /* __LIBDH_LINUX_H__ */
