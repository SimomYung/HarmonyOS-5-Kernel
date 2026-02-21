/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: devhost backend APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 4 09:40:43 2021
 */
#ifndef __DEVHOST_API_BACKEND_H__
#define __DEVHOST_API_BACKEND_H__

#if defined(MODULE)
#include <linux/types.h>
#elif defined(UDK_MODULE) || defined(UDK_FRAMEWORK)
#include <udk/stdint.h>
#include <stddef.h>
#else
#include <stdint.h>
#include <stddef.h>
#endif
#include <stdbool.h>

struct devhost_umap_ops {
	int (*munmap)(void *data, int vs_is_releasing);
	unsigned long __res[3];
};

/* ops for backend constructor */
struct dh_backend_ctor {
	/*
	 * backend thread/actv context setup, will be called in
	 * thread/actv context.
	 */
	int (*ctx_setup)(void);

	/* backend actv/syscall hook */
	void (*syscall_enter)(void); /* full info */
	void (*syscall_enter_nosec)(void); /* nodac */
	void (*syscall_enter_nomm)(void); /* no mm_struct */
	void (*syscall_enter_basic)(void); /* basic info, such as sched tcb_cref */
	void (*syscall_exit)(void);

	/* memory allocator */
	void *(*alloc)(size_t size, void **handle);
	int (*query)(void *handle, void *ranges, unsigned int ranges_num);
	void  (*free)(void *handle);
	void *(*alloc_page)(unsigned int order);
	void  (*free_page)(void *ptr, unsigned int order);

	/* vma ops */
	void *(*pre_map)(unsigned long long pa, unsigned long size);
	void (*pre_unmap)(void *va, unsigned long size);
	int (*handle_page_fault)(void *va, unsigned long long vr_key, unsigned long long addr, unsigned int flags, void *regs);
	int (*handle_mm_release)(unsigned long long vspace_key);
	int (*handle_mm_rdlock)(void);
	void (*handle_mm_rdunlock)(void);
	struct devhost_umap_ops *umap_ops; /* for user app mmap */

	/* preempt ops */
	int (*preempt_enable)(void);
	int (*preempt_disable)(void);

	/* vhangup ops */
	void (*vhangup)(void);
};

/* backend entry */
#define DH_BACKEND_INIT_SYM "dh_backend_init"
typedef int (*dh_backend_init_fn_t)(struct dh_backend_ctor *, const char *backend_cmdline);
int dh_backend_init(struct dh_backend_ctor *ctor, const char *backend_cmdline);

/* The first argv of backend args is backend lib name,
 * the rests are common arguments */
struct devhost_backend_args {
	int argc;
	char **argv;
};
const struct devhost_backend_args *devhost_get_backend_args(void);

/* backend firmware support */
int devhost_pal_fw_acquire_contents(const char *filename, void *data, unsigned int size,
				    unsigned int *rsize);

/* umap ops */
int devhost_backend_register_umap_ops(struct devhost_umap_ops *ops);
int devhost_backend_umap_munmap(void *data, int vs_is_releasing);
int devhost_backend_mm_release(unsigned long long vspace_key);

/* backend module loader support */
struct devhost_loader {
	int (*is_compatible)(struct devhost_loader *loader,
			     const char *libname, const void *data, size_t size);
	int (*load_module)(struct devhost_loader *loader,
			   const char *libname, const void *data, size_t size, char *args);
	int (*unload_module)(struct devhost_loader *loader, const char *libname);
};
int devhost_register_loader(struct devhost_loader *loader);

int devhost_pal_request_module(const char *name);
int devhost_pal_request_module_args(const char *name, const char *mod_args);

/* for PMU */
int devhost_pmu_enable(void);
int devhost_pmu_disable(void);

/* for kconsole */
int devhost_kconsole_print(const char* str, size_t size);
int devhost_kconsole_enable(void);
int devhost_kconsole_disable(void);

/* for kill foregroud process group hm */
void devhost_kill_process_group(int sid, int signum);
void devhost_kill_process(int pid, int signum);

/* for settime */
int devhost_time_sync_init(void (*handler)(void));

/*
 * Register a post-init hook, which will be called after register to devmgr.
 *
 * Args:
 *   - function:  Hook function to be called, return `0` for success.
 *		  Otherwise, devhost will unregister itself from devmgr
 *		  and exit with returned value.
 *   - arg:	  Args to hook function.
 *
 * RETURN VALUE
 *   `0` for success, hmerrno for error.
 */
int devhost_postinit(int (*function)(void *arg), void *arg);

#endif /* __DEVHOST_API_BACKEND_H__ */
