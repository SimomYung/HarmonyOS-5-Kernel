/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * syscall_wrapper.h - liblinux specific wrappers to syscall definitions
 * liblinux does not need compat syscalls and each syscall is exported
 */
#ifndef __LIBLINUX_ASM_SYSCALL_WRAPPER_H
#define __LIBLINUX_ASM_SYSCALL_WRAPPER_H

#define SYSCALL_EXPORT_SYMBOL(name)    EXPORT_SYMBOL_GPL(__liblinux_sys##name)

/*
 * currently we do not need to support __arm64 syscall which takes pt_regs
 * pointer as parameter
 */
#define __SYSCALL_DEFINEx(x, name, ...)					\
	asmlinkage long __liblinux_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));	\
	SYSCALL_EXPORT_SYMBOL(name);			\
	asmlinkage long __liblinux_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__))

#define SYSCALL_DEFINE0(sname)					\
	SYSCALL_METADATA(_##sname, 0);				\
	asmlinkage long __liblinux_sys_##sname(void);			\
	SYSCALL_EXPORT_SYMBOL(_##sname);			\
	asmlinkage long __liblinux_sys_##sname(void)

#endif /* __LIBLINUX_ASM_SYSCALL_WRAPPER_H */
