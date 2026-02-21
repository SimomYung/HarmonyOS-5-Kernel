/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: utils for export lnxbase symbol
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 16 14:53:06 2021
 */
#ifndef __LNXBASE_EXPORT_H__
#define __LNXBASE_EXPORT_H__

struct lnxbase_symbol {
	unsigned long value;
	const char *name;
};

#define __EXPORT_SYMBOL(sym, sec, name)				\
	static const char __kstrtab_##name[]			\
	__attribute__((section("__ksymtab_strings"), aligned(1))) \
	= #name;						\
	extern const struct lnxbase_symbol __ksymtab_##name;	\
	__visible const struct lnxbase_symbol __ksymtab_##name	\
	__used							\
	__attribute__((section("___ksymtab" sec "+" #name), unused))	\
	= { (unsigned long)(sym), __kstrtab_##name }

#define EXPORT_SYMBOL(sym)					\
	__EXPORT_SYMBOL(sym, "", sym)

#define EXPORT_SYMBOL_ALIAS(name, sym)				\
	__EXPORT_SYMBOL(sym, "", name)

#endif /* __LNXBASE_EXPORT_H__ */
