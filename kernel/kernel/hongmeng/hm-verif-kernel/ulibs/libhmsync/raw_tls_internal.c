/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: raw_tls_init
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 28 10:37:33 2019
 */

#include <bits/alltypes.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <lib/utils.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsync/raw_thread.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_mem.h>
#include <hmkernel/errno.h>
#include <libstrict/strict.h>
#include <hmasm/lsyscall.h>
#include <libcrt/hmcrt.h>

#include "raw_thread_impl.h"

#if defined __HOST_LLT__
struct __libc __libc;
struct llt_tls g_llt_tls;
#elif !defined CONFIG_RAW_THREAD_COMPATIBLE_WITH_PTHREAD
struct __libc __libc;
#endif
extern struct __libc *__get_libc(void);

#ifdef CONFIG_NEED_SHADOW_STACK
static void *mmap_shadow_stack(void)
{
	int err;
	void *sstack_base = NULL;
	void *sstack = NULL;
	uint32_t flags = __U(MAP_PRIVATE) | __U(MAP_ANON);
#ifdef CONFIG_SFI
	flags |= MFLAG_MAP_SAFE;
#endif
	sstack_base = hm_mem_mmap_lsyscall(0, SHADOW_STACK_TOTAL_SIZE, PROT_NONE, flags, -1, 0);
	/* MAP_FAILED is (void *)-1 */
	if (sstack_base == MAP_FAILED) {
		sstack_base = NULL;
	}

	if (sstack_base != NULL) {
		flags |= __U(MAP_FIXED);
		sstack = hm_mem_mmap_lsyscall((void *)((uintptr_t)sstack_base + SHADOW_STACK_GUARD_SIZE),
					   SHADOW_STACK_SIZE, __U(PROT_READ) | __U(PROT_WRITE),
					   flags, -1, 0);
		if (sstack == MAP_FAILED) {
			hm_warn("re-map sstack failed\n");
			err = hm_mem_munmap_lsyscall(sstack_base, SHADOW_STACK_TOTAL_SIZE);
			if (err != E_HM_OK) {
				hm_warn("hm_mem_munmap failed in alloc sstack failed, err=%s\n",
					hmstrerror(err));
			}
			sstack_base = NULL;
		}
	}

	return sstack_base;
}

static void __init_sstack(struct raw_thread *td)
{
	CMPT_THREAD_REG(td, sstack_base) = mmap_shadow_stack();
	BUG_ON(CMPT_THREAD_REG(td, sstack_base) == NULL);
	CMPT_THREAD_REG(td, sstack) = (void *)shadow_stack_start_addr((uintptr_t)CMPT_THREAD_REG(td, sstack_base));
	set_shadow_stack(CMPT_THREAD_REG(td, sstack));
}
#endif

#ifndef __HOST_LLT__
static void __raw_init_tp(void *p)
{
	struct raw_thread *td = p;
	pid_t tid = lsyscall_thread_gettid();
	BUG_ON(tid <= 0);
	/* higher 32 bits is cnode index, and lower 32 bits is inner index. */
	td->cref.tcb_cref = ((unsigned long long)hm_crt_cnode_idx_of() << 32) |
			    (unsigned long long)hm_crt_tcb_inner_idx_of();
	CMPT_THREAD_REG(td, tid) = tid;
	CMPT_THREAD_REG(td, THREAD_TSD) = __raw_tsd_main;
	CMPT_THREAD_REG(td, robust_list.head) = CMPT_THREAD_REF(td, robust_list.head);
	CMPT_THREAD_REG(td, next) = CMPT_THREAD_REG(td, prev) = RAW_THREAD_TO_PTHREAD(td);
	CMPT_THREAD_REG(td, locale) = __libc_global_locale(plibc);
	CMPT_THREAD_REG(td, DETACH_STAT) = DT_JOINABLE;
	/* When using himalloc, himalloc_tcache_addr in struct pthread should be copied to prevent memory leak. */
	if (raw_get_tls() != 0UL) {
		struct raw_thread *old = raw_thread_self();
		CMPT_THREAD_REG(td, THREAD_MALLOC_TCACHE) = CMPT_THREAD_REG(old, THREAD_MALLOC_TCACHE);
	}
#ifdef CONFIG_NEED_SHADOW_STACK
	__init_sstack(td);
#endif
	raw_thread_lockdep_depth_init(td);
	raw_thread_rdlock_lockowner_init(td);

#ifdef __aarch64__
	arch_lsyscall_entry1(__NR_set_thread_area, THREAD_TO_TLS(p));
#else
	arch_lsyscall_entry(__NR_set_thread_area, THREAD_TO_TLS(p));
#endif
	return;
}
#endif

int raw_tls_init(void)
{
	int rc = E_HM_OK;
#ifndef __HOST_LLT__
	void *mem = NULL;

	plibc = __get_libc();
	mem = hm_mem_mmap_lsyscall(NULL, __raw_thread_tls_size(),
				   PROT_READ | PROT_WRITE,
				   MAP_ANONYMOUS | MAP_PRIVATE | MFLAG_MAP_ACTV,
				   -1, 0);
	if (mem == MAP_FAILED) {
		hm_error("mmap size is 0x%zx failed\n",
			 __raw_thread_tls_size());
		rc = E_HM_NOMEM;
	}

	if (rc == E_HM_OK) {
		__raw_init_tp(raw_tls_copy(mem));
	}
#endif
	return rc;
}
