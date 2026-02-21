/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: liblinux ksymtab linker script helpers
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 9 17:05:45 2021
 */
#ifndef __LIBLINUX_KSYMTAB_H__
#define __LIBLINUX_KSYMTAB_H__

struct lnxbase_syminfo {
	void *__start___ksymtab;
	void *__stop___ksymtab;
	void *__start___ksymtab_gpl;
	void *__stop___ksymtab_gpl;
};

/* Provided by linker script */
extern unsigned long __start___ksymtab[];
extern unsigned long __stop___ksymtab[];
extern unsigned long __start___ksymtab_gpl[];
extern unsigned long __stop___ksymtab_gpl[];

#define __LIBLINUX_SYMINFO_INITIALIZER { 	\
	__start___ksymtab,			\
	__stop___ksymtab,			\
	__start___ksymtab_gpl,			\
	__stop___ksymtab_gpl 			\
}

#define DECLARE_LIBLINUX_SYMINFO(name) 	\
	struct lnxbase_syminfo name = __LIBLINUX_SYMINFO_INITIALIZER

#endif /* __LIBLINUX_KSYMTAB_H__ */
