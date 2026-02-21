/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: The Actv Upcall
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 16 10:22:00 2021
 */

#ifndef AARCH64_UAPI_ASM_ACTIVATION_UPCALL_H
#define AARCH64_UAPI_ASM_ACTIVATION_UPCALL_H

#include <hmasm/types.h>
#include <hmasm/exception.h>
#include <hmkernel/ipc/hmsignal.h>

#define __ARCH_ACTV_UPCALL_ARG0		0
#define __ARCH_ACTV_UPCALL_ARG1		1
#define __ARCH_ACTV_UPCALL_ARG2		2
#define __ARCH_ACTV_UPCALL_ARG_NR	3

/*
 * Define the format of the args transfered to the actv handler to
 * handle the specific task that cannot be completed in kernel.
 */
union __arch_actv_upcall_args_u {
	__u64 buf[__ARCH_ACTV_UPCALL_ARG_NR];

	struct actv_upcall_kpagefault_info_s {
		__u64 fault_addr;
		__u64 fault_size;
		__u64 write;
	} kpagefault;

	struct actv_upcall_linux_syscall_s {
		__u64 type;
	} linux_syscall;

	struct actv_upcall_seccomp_s {
		__u64 method;
	} seccomp;

	struct actv_upcall_hmsignal_s {
		struct __hmsigmask hmsignals;
		__u64 timer_cref;
		__u32 restartable_flags;
	} hmsignal;

	struct actv_upcall_vsfault_s {
		__u64 vspace_cref;
		__u64 credential;
	} vsfault;

	struct actv_upcall_hyp_vmexit_s {
		__u64 opaque;
	} hyp_vmexit;

	struct actv_upcall_hyp_mmio_dispatcher_s {
		__u64 opaque;
		__u64 data;
	} hyp_mmio_dispatcher;

#ifdef CONFIG_VM_DYNAMIC_MEMORY
	struct actv_upcall_hyp_pagefault_s {
		__u64 vm_cref;
		__u64 faulted_gpa;
	} hyp_pagefault;

#endif
	struct actv_upcall_set_sigmask_s {
		__u64 sigmask;
	} set_sigmask;

#ifdef CONFIG_BAD_FD_DSM
	struct actv_upcall_badfd_s {
		__u32 syscall_no;
	} set_badfd;
#endif

	struct actv_upcall_sync_revents_s {
		__u32 cnode;
		__u32 fd;
		__u32 fd_type;
	} sync_revents;

	struct actv_upcall_epoll_recheck_s {
		__u32 cnode;
		__u32 fd;
		__u32 events;
	} ep_recheck;

	struct actv_upcall_ppoll_s {
		__u64 fds;
		__u32 nfds;
		__u64 tcb_cref;
	} eventpoll_ppoll;

	struct actv_upcall_select_s {
		__u32 type;
		__u64 tcb_cref;
	} select;

	struct actv_upcall_common_file_close_s {
		__u32 fd;
		__u32 cnode_idx;
		__u32 rpccnt;
	} common_file_close;

	struct actv_upcall_pool_page_free_s {
		__u64 pfn;
	} pool_page_free;

	struct actv_upcall_irq_fastpath_s {
		__u32 logic_irq;
	} irq;

	struct actv_upcall_close_on_exec_s {
		__u32 cnode_idx;
	} close_on_exec;

	struct actv_upcall_rpccnt_put_s {
		__u64 common_file;
		int fd_type;
	} rpccnt;

	struct actv_upcall_binder_ioctl_s {
		__u64 file;
		__u64 cmd;
		__u64 arg;
	} binder_ioctl;
#ifdef CONFIG_RT_FUTEX
	struct actv_upcall_thread_tid2cref_s {
		__u64 uaddr;
		__u32 tid;
	} thread_tid2cref;
#endif

	struct __arch_exception_info ex_info;
	struct __arch_fastpfoob_info fastpfoob_info;
	struct actv_upcall_signals_s {
		__u32 signum;
	} signals;

	struct actv_upcall_eventpoll_recheck_s {
		__u32 fd;
		__u32 idx;
	} eventpoll_recheck;
};

#endif /* !AARCH64_UAPI_ASM_ACTIVATION_UPCALL_H */
