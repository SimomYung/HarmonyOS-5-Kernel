/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Set/Get tls area on aarch64
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 24 14:37:33 2019
 */

#ifndef ULIBS_LIBHMYSNC_AARCH64_RAW_TLS_H
#define ULIBS_LIBHMYSNC_AARCH64_RAW_TLS_H

#ifndef ULIBS_LIBHMSYNC_RAW_TLS_H
#error only allow included by raw_tls.h
#endif

#include <elf.h>

#define THREAD_TO_TLS(p) ((char *)(p) + sizeof(struct raw_thread))
#define TLS_TO_THREAD(tls_base) \
	(struct raw_thread *)(uintptr_t)((unsigned long)(tls_base) - sizeof(struct raw_thread))
#define tls_to_pthread(tls_base) \
	(struct pthread *)(uintptr_t)((unsigned long)(tls_base) - sizeof(struct pthread))

#define TLS_ABOVE_TP
#ifndef GAP_ABOVE_TP
#define GAP_ABOVE_TP 16
#endif

typedef Elf64_Phdr Elf_Phdr;

static inline void raw_set_tls(void *tls)
{
	NOFAIL(sysfast_set_tls(tls));
}

static inline void raw_set_actv_tls(void *tls)
{
	NOFAIL(sysfast_set_tls(tls));
}

static inline unsigned long raw_get_tls(void)
{
	unsigned long val = 0UL;
	asm volatile ("mrs %0, tpidr_el0\n" : "=r"(val));
	return val;
}

#define crt_jmp(pc,sp) __asm__ __volatile__( \
	"mov sp, %1; br %0" : : "r"(pc), "r"(sp) : "memory" )

#endif
