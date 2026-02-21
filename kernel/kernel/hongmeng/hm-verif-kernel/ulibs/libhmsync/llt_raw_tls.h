/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Tls function declaration for llt
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 24 12:37:33 2019
 */

#ifndef ULIBS_LIBHMSYNC_LLT_RAW_TLS_H
#define ULIBS_LIBHMSYNC_LLT_RAW_TLS_H

#include <elf.h>

#define THREAD_TO_TLS(p) (p)
#define TLS_TO_THREAD(tls_base) (struct raw_thread *)(uintptr_t)(tls_base)
#define tls_to_pthread(tls_base) (struct pthread *)(uintptr_t)(tls_base)

#if __SIZEOF_LONG__ == 4
typedef Elf32_Phdr Elf_Phdr;
#else
typedef Elf64_Phdr Elf_Phdr;
#endif

/*
 * NOTE:
 *  1. The front, rear have no practical meaning, only a block
 *     of memory which represent tls When not initialized.
 *  2. The base is tls that you set.
 *
 *  The llt_tls may be adjusted according to llt actual scene
 *  in the future.
 */
struct llt_tls {
	unsigned long front[0x100];
	unsigned long base;
	unsigned long rear[0x100] __attribute__((aligned(16)));
};

extern struct llt_tls g_llt_tls;

static inline void raw_set_tls(void *tls)
{
	g_llt_tls.base = (unsigned long)(uintptr_t)tls;
}

static inline void raw_set_actv_tls(void *tls)
{
}

static inline unsigned long raw_get_tls(void)
{
	if (g_llt_tls.base == 0) {
		return (unsigned long)(uintptr_t)&g_llt_tls.rear[0];
	}
	return (unsigned long)g_llt_tls.base;
}

#define crt_jmp(pc,sp)  pc()

#endif
