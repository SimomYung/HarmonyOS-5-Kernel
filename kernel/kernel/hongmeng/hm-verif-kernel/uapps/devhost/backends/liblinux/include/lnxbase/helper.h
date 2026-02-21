/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: lnxbase linux helper
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 4 09:40:43 2021
 *
 * Feature Macros:
 *   LNXBASE_HELPER_SKIP_NETLINK	skip netlink feature
 *   LNXBASE_HELPER_SKIP_UNLOAD_MODULE	skip module unload feature
 *   LNXBASE_HELPER_ENABLE_TIMESYNC	enable timesync feature
 *   LNXBASE_HELPER_WITH_MODULE_NS	EXPORT_SYMBOL have module namespace field (5.10+)
 */
#ifndef __LNXBASE_HELPER_H__
#define __LNXBASE_HELPER_H__

#if defined(__linux__) && defined(__KERNEL__)
#  include <linux/gfp.h>
#  include <linux/slab.h>
#  include <linux/module.h>
#  include <linux/syscalls.h>
#elif defined(__hongmeng__)
#include <lnxbase/export.h>
/* linux/gfp.h */
typedef unsigned gfp_t;
#define GFP_KERNEL	0
/*
 * After linux 5.0, GFP_ZERO value is changed to 0x100u,
 * since this file will be replaced in the future, currently
 * we set GFP_ZERO to a value compatible with before and after 5.0
 * - after linux 5.0, 0x8100u is GFP_NOFAIL & GFP_ZERO.
 * - before linux 5.0, 0x8100u is GFP_ZERO & GFP_COLD
 */
#define __GFP_ZERO	0x8100u
unsigned long __get_free_pages(gfp_t gfp_mask, unsigned int order);
void free_pages(unsigned long addr, unsigned int order);
/* linux/slab.h */
void *__kmalloc(size_t size, gfp_t flags);
void kfree(const void *);
/* linux/module.h */
void *__symbol_get(const char *symbol);
int liblinux_preempt_enable(void);
int liblinux_preempt_disable(void);
#ifdef LNXBASE_HELPER_WITH_MODULE_NS
/* kernel symbol with namespace support */
struct kernel_symbol {
	unsigned long value;
	const char *name;
	const char *namespace;
};
#endif
/* linux/syscalls.h */
int sys_init_module(const void *umod,
		    unsigned long len, const char *uargs);
int sys_delete_module(const char *name_user, unsigned int flags);
#else
#  error "This helper can only be used in linux or hongmeng build env"
#endif

#include <devhost/backend.h>
#include <lnxbase/lnxbase.h>
#include <liblinux/pal.h>

/* liblinux allocator wrapper */
static inline void *__dh_linux_alloc(size_t size, void **handle)
{
	return lnxbase_mm_alloc(size, handle);
}

static inline int __dh_linux_query(void *handle, void *ranges,
				   unsigned int ranges_num)
{
	return lnxbase_mm_query(handle, ranges, ranges_num);
}

static inline void __dh_linux_free(void *handle)
{
	lnxbase_mm_free(handle);
}

static inline void *__dh_linux_alloc_page(unsigned int order)
{
	return (void *)__get_free_pages(GFP_KERNEL | __GFP_ZERO, order);
}

static inline void __dh_linux_free_page(void *ptr, unsigned int order)
{
	free_pages((unsigned long)ptr, order);
}

static inline void *__dh_linux_mm_pre_map(unsigned long long pa, unsigned long size)
{
	return liblinux_mm_prepare_map(pa, size);
}

static inline void __dh_linux_mm_pre_unmap(void *va, unsigned long size)
{
	if (va == NULL) {
		return;
	}
	liblinux_mm_prepare_unmap(va, size);
}

static inline int __dh_linux_ctx_setup(void)
{
	return liblinux_thread_setup();
}

#ifdef __aarch64__
__attribute__((target("general-regs-only")))
#endif
__attribute__((weak)) void liblinux_enter(int pid, ...)
{
	(void)pid;
}

__attribute__((weak)) void liblinux_exit(void)
{
}

static inline void __dh_linux_syscall_enter(void)
{
	struct lnxbase_cred_dac cred;
	int pid;

	lnxbase_get_dac(&cred);
	pid = devhost_current_pid();
	BUG_ON(pid < 0, "pid=%d\n", pid);
	liblinux_enter(pid, cred.uid, cred.gid, cred.suid, cred.sgid,
		       cred.euid, cred.egid, cred.fsuid, cred.fsgid, cred.umask, cred.cnode_idx);
}

static inline void __dh_linux_syscall_exit(void)
{
	liblinux_exit();
}

static inline void lnxbase_helper_ctor_init(struct dh_backend_ctor *ctor)
{
	ctor->ctx_setup = __dh_linux_ctx_setup;
	ctor->syscall_enter = ctor->syscall_enter_nosec = __dh_linux_syscall_enter;
	ctor->syscall_enter_nomm = __dh_linux_syscall_enter;
	ctor->syscall_exit = __dh_linux_syscall_exit;
	ctor->alloc = __dh_linux_alloc;
	ctor->query = __dh_linux_query;
	ctor->free = __dh_linux_free;
	ctor->alloc_page = __dh_linux_alloc_page;
	ctor->free_page = __dh_linux_free_page;
	ctor->pre_map = __dh_linux_mm_pre_map;
	ctor->pre_unmap = __dh_linux_mm_pre_unmap;
	ctor->preempt_enable = liblinux_preempt_enable;
	ctor->preempt_disable = liblinux_preempt_disable;
}

/*
 * module support wrapper
 */
static inline int
__lnxbase_helper_mod_load_module(const char *libname, const void *data, size_t size, char *args)
{
	if (libname == NULL) {
		return -EINVAL;
	}
	return sys_init_module(data, size, args);
}

static inline int
__lnxbase_helper_mod_unload_module(const char *libname)
{
#ifdef LNXBASE_HELPER_SKIP_UNLOAD_MODULE
	return -ENOSYS;
#else
	return sys_delete_module(libname, 0);
#endif
}

#ifdef LNXBASE_HELPER_WITH_MODULE_NS
static void
__lnxbase_syminfo_fill_ksymtab(struct kernel_symbol *ksymtab,
			       const struct lnxbase_symbol *lnxsym,
			       unsigned long count)
{
	struct kernel_symbol *sym = ksymtab;
	unsigned long i;
	for (i = 0; i < count; i++) {
		sym->value = lnxsym[i].value;
		sym->name= lnxsym[i].name;
		sym->namespace = NULL;
		sym++;
	}
}

static int
__lnxbase_syminfo_append_ns(struct liblinux_syminfo *si)
{
	struct kernel_symbol *ksymtab = NULL;
	unsigned long symcnt, gplcnt;

	/* count symbols */
	symcnt = (ptr_to_ulong(si->__stop___ksymtab) - ptr_to_ulong(si->__start___ksymtab))
			/ sizeof(struct lnxbase_symbol);
	gplcnt = (ptr_to_ulong(si->__stop___ksymtab_gpl) - ptr_to_ulong(si->__start___ksymtab_gpl))
			/ sizeof(struct lnxbase_symbol);

	/*
	 * alloc mem for symbols with ns support
	 *  - no need to keep track of this allocation, because symbol modules
	 *    are not allowed to be unloaded.
	 */
	ksymtab = (struct kernel_symbol *)malloc(sizeof(ksymtab[0]) * (symcnt + gplcnt));
	if (ksymtab == NULL) {
		return -ENOMEM;
	}

	/* fill symbols */
	__lnxbase_syminfo_fill_ksymtab(ksymtab,
				       (struct lnxbase_symbol *)si->__start___ksymtab, symcnt);
	__lnxbase_syminfo_fill_ksymtab(ksymtab + symcnt,
				       (struct lnxbase_symbol *)si->__start___ksymtab_gpl, gplcnt);

	/* set back to kernel si */
	si->__start___ksymtab = (void *)ksymtab;
	si->__stop___ksymtab = (void *)(ksymtab + symcnt);
	si->__start___ksymtab_gpl = (void *)(ksymtab + symcnt);
	si->__stop___ksymtab_gpl = (void *)(ksymtab + symcnt + gplcnt);

	return 0;
}
#endif

static int
__lnxbase_helper_mod_load_symbol_module(const char *libname,
					const struct lnxbase_syminfo *lnxbase_si)
{
	struct liblinux_syminfo si = {
		.__start___ksymtab = lnxbase_si->__start___ksymtab,
		.__stop___ksymtab = lnxbase_si->__stop___ksymtab,
		.__start___ksymtab_gpl = lnxbase_si->__start___ksymtab_gpl,
		.__stop___ksymtab_gpl = lnxbase_si->__stop___ksymtab_gpl,
		/*
		 * only ksymtab and ksymtab_gpl were supported,
		 * set other ksymtab as empty.
		 */
		.__start___ksymtab_gpl_future = lnxbase_si->__stop___ksymtab_gpl,
		.__stop___ksymtab_gpl_future = lnxbase_si->__stop___ksymtab_gpl,
		.__start___kcrctab = NULL,
		.__start___kcrctab_gpl = NULL,
		.__start___kcrctab_gpl_future = NULL,
	};

#ifdef LNXBASE_HELPER_WITH_MODULE_NS
	__lnxbase_syminfo_append_ns(&si);
#endif

	return liblinux_init_symbol_module(libname, &si);
}

static inline void *
__lnxbase_helper_mod_find_symbol(const char *symbol)
{
	return __symbol_get(symbol);
}

static inline int
lnxbase_helper_enable_module(struct lnxbase_ctx *ctx)
{
	static struct lnxbase_module_ops mod_ops __read_mostly = {
		.is_compatible = NULL,	/* always compatible */
		.load_module =  __lnxbase_helper_mod_load_module,
		.unload_module = __lnxbase_helper_mod_unload_module,
		.load_symbol_module = __lnxbase_helper_mod_load_symbol_module,
		.symbol_get = __lnxbase_helper_mod_find_symbol,
	};
	return lnxbase_enable_module(ctx, &mod_ops);
}

static int
lnxbase_helper_simple_init(struct dh_backend_ctor *ctor,
			   lnxbase_postinit_fn_t fn, void *args,
			   struct lnxbase_ctx **ctx_out)
{
	struct lnxbase_ctx *ctx = NULL;
	int ret;

	/* override backend ctors */
	lnxbase_helper_ctor_init(ctor);

	/* alloc lnxbase context */
	ctx = lnxbase_newctx(ctor);
	if (ctx == NULL) {
		return -EINVAL;
	}

	/* enable module support */
	ret = lnxbase_helper_enable_module(ctx);
	if (ret < 0) {
		goto err_out;
	}

#ifndef LNXBASE_HELPER_SKIP_NETLINK
	/* enable netlink */
	ret = lnxbase_enable_netlink(ctx,
				     liblinux_pal_netlink_init);
	if (ret < 0) {
		goto err_out;
	}
#endif

#ifdef LNXBASE_HELPER_ENABLE_TIMESYNC
	/* enable timesync */
	ret = lnxbase_enable_timesync(ctx, liblinux_time_sync);
	if (ret < 0) {
		goto err_out;
	}
#endif

#ifdef LNXBASE_HELPER_ENABLE_BACKTRACE
	ret = lnxbase_enable_backtrace(ctx, liblinux_return_address, liblinux_vsnprintf);
	if (ret < 0) {
		goto err_out;
	}
	ret = lnxbase_enable_vmallocinfo();
	if (ret < 0) {
		goto err_out;
	}
#endif

	/* init lnxbase context */
	ret = lnxbase_init(ctx, fn, args);
	if (ret < 0) {
		goto err_out;
	}

	/* return lnxbase context */
	if (ctx_out != NULL) {
		*ctx_out = ctx;
	}
	(void)lnxbase_set_default_ctx(ctx);

	return 0;

err_out:
	lnxbase_destroy(ctx);
	return ret;
}

/* call lnxbase init before real backend init */
extern unsigned long long memstart_addr;
int __dh_backend_init(struct dh_backend_ctor *, const char *);

/*
 * Disable stack protector for backend init, because the stack canary
 * has not been initialized yet.
 */
#ifdef __clang__
__attribute__((no_stack_protector))
#else
__attribute__((optimize("-fno-stack-protector")))
#endif
int dh_backend_init(struct dh_backend_ctor *ctor, const char *backend_cmdline)
{
	struct lnxbase_sysinfo info;
	int ret;
	if ((ret = lnxbase_sysinfo_get(&info)) < 0) {
		return ret;
	}
	memstart_addr = info.memstart;
	ctor->pre_map = __dh_linux_mm_pre_map;
	ctor->pre_unmap = __dh_linux_mm_pre_unmap;
	return __dh_backend_init(ctor, backend_cmdline);
}
#define dh_backend_init __dh_backend_init

#endif /* __LNXBASE_HELPER_H__ */
