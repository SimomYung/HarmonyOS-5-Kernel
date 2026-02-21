/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: devhost backend APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 4 09:40:43 2021
 */
#ifndef __DEVHOST_BACKEND_H__
#define __DEVHOST_BACKEND_H__

#include <devhost/backend.h>

/* helper APIs for backend ctor call */
int backend_ctx_setup(void);
enum devhost_syscall_enter_type {
	SYSCALL_ENTER_FULL_INFO,
	SYSCALL_ENTER_NO_SEC,
	SYSCALL_ENTER_NO_MM,
	SYSCALL_ENTER_BASIC,
	NR_SYSCALL_ENTER_TYPE,
};
void backend_syscall_enter(enum devhost_syscall_enter_type type);
void backend_basic_syscall_enter(void);
void backend_syscall_exit(void);
void *devhost_backend_alloc(size_t size, void **handle);
struct mem_raw_ranges;
int devhost_backend_query(void *handle, struct mem_raw_ranges *ranges,
			  unsigned int ranges_num);
void devhost_backend_free(void *handle);
void *devhost_backend_alloc_page(unsigned int order);
void devhost_backend_free_page(void *ptr, unsigned int order);
void *devhost_backend_prepare_map(unsigned long long pa, unsigned long size);
void devhost_backend_prepare_unmap(void *va, unsigned long size);
int devhost_backend_handle_mm_rdlock(void);
void devhost_backend_handle_mm_rdunlock(void);
int devhost_backend_handle_page_fault(void *va, unsigned long long vr_key, unsigned long long addr, unsigned int flags);
void devhost_backend_vhangup(void);

int hm_user_preempt_disable(void);
int hm_user_preempt_enable(void);

#endif /* __DEVHOST_BACKEND_H__ */
