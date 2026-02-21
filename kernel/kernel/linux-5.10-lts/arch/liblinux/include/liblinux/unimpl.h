/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_UNIMPL_H__
#define __LIBLINUX_UNIMPL_H__

#ifdef CONFIG_LIBLINUX
#ifndef __ASSEMBLY__

#include <linux/export.h>
#ifdef __KERNEL__
#include <linux/printk.h>
#define liblinux_unimpl_printf(fmt, ...) \
	pr_info_once(fmt, ##__VA_ARGS__)
#else
#include <stdio.h>
#define liblinux_unimpl_printf(fmt, ...) \
	fprintf(stderr, fmt, ##__VA_ARGS__)
#endif

#if !defined(CONFIG_LIBLINUX_UNIMPL_DFX_COUNT) || !defined(__KERNEL__)
#define __DEFINE_LIBLINUX_UNIMPL_DFX(name)
#define __CALL_LIBLINUX_UNIMPL_DFX(name)
#else /* CONFIG_LIBLINUX_UNIMPL_DFX_COUNT && __KERNEL__ */
#include <linux/atomic.h>
struct liblinux_unimpl_entry {
	atomic_t count;
	const char *name;
};
extern char __liblinux_unimpl_entry_start[];
extern char __liblinux_unimpl_entry_end[];

#define __DEFINE_LIBLINUX_UNIMPL_DFX(__name) \
	struct liblinux_unimpl_entry __attribute__((section(".__liblinux_unimpl_entry"))) \
	liblinux_unimpl_entry_##__name = { \
		.count = ATOMIC_INIT(0), \
		.name = #__name, \
	}

#ifdef CONFIG_HISYSEVENT
extern void liblinux_report_unimpl(struct liblinux_unimpl_entry *entry);
#else
static inline void liblinux_report_unimpl(struct liblinux_unimpl_entry *entry) { }
#endif

static inline void __call_liblinux_unimpl_dfx(struct liblinux_unimpl_entry *entry)
{
	int v = atomic_inc_return(&entry->count);
	if (unlikely(v == 1))
		liblinux_report_unimpl(entry);
}

#define __CALL_LIBLINUX_UNIMPL_DFX(__name) \
	__call_liblinux_unimpl_dfx(&liblinux_unimpl_entry_##__name)
#endif


#define LIBLINUX_UNIMPL(name) \
	__DEFINE_LIBLINUX_UNIMPL_DFX(name); \
	void __attribute__((section(".__liblinux_unimpl"))) name(void) \
	{ \
		liblinux_unimpl_printf("UN-IMPL: %s\n", __func__); \
		__CALL_LIBLINUX_UNIMPL_DFX(name); \
	}

#define LIBLINUX_UNIMPL_RET(name, type, ret) \
	__DEFINE_LIBLINUX_UNIMPL_DFX(name); \
	type __attribute__ ((section(".__liblinux_unimpl"))) name(void) \
	{ \
		liblinux_unimpl_printf("UN-IMPL: %s\n", __func__); \
		__CALL_LIBLINUX_UNIMPL_DFX(name); \
		return ret; \
	}

#define LIBLINUX_EXPORTED_UNIMPL(name) \
        LIBLINUX_UNIMPL(name)   \
        EXPORT_SYMBOL(name);

#define LIBLINUX_EXPORTED_UNIMPL_RET(name, type, ret) \
        LIBLINUX_UNIMPL_RET(name, type, ret)    \
        EXPORT_SYMBOL(name);

#define LIBLINUX_GPL_EXPORTED_UNIMPL(name) \
        LIBLINUX_UNIMPL(name)   \
        EXPORT_SYMBOL_GPL(name);

#define LIBLINUX_GPL_EXPORTED_UNIMPL_RET(name, type, ret) \
        LIBLINUX_UNIMPL_RET(name, type, ret)    \
        EXPORT_SYMBOL_GPL(name);

#define EX_SYMBOL(name) \
	extern unsigned long name; \
	EXPORT_SYMBOL(name);

#endif  /* !__ASSEMBLY__ */
#endif  /* CONFIG_LIBLINUX */
#endif  /* __LIBLINUX_UNIMPL_H__*/
