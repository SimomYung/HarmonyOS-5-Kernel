/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: header for seccomp instrumentation point
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 13 16:51:53 2023
 */
#ifndef ULIBS_LIBSEC_SECCOMP_H
#define ULIBS_LIBSEC_SECCOMP_H

#include <libhmactv/actv.h>
#include <hmasm/actv_rpc.h>
#include <hmasm/types.h>
#include <libstrict/strict.h>
#include <hmkernel/errno.h>
#include <libhmsync/raw_scopedptr.h>
#include <libsysif/utils.h>
#include <libhmsrv_audit/hm_audit.h>
#include <hmkernel/capability.h>
#include <hmkernel/extensions/lsyscall.h>

/* syscall number definitions */
#include <hmsyscall.h>
#include <sys/syscall.h>
#include <hmkernel/strict.h>
#include <libsec/pac.h>

#define IS(x) ____IS((x))
#define NOT(x) ____NOT((x))

#define HM_SECCOMP_ACTION_KILL_PROCESS	0x80000000U
#define HM_SECCOMP_ACTION_KILL_THREAD	0x00000000U
#define HM_SECCOMP_ACTION_KILL		HM_SECCOMP_ACTION_KILL_THREAD
#define HM_SECCOMP_ACTION_ERRNO		0x00050000U
#define HM_SECCOMP_ACTION_ALLOW		0x7fff0000U
#define HM_SECCOMP_ACTION_LOG		0x7ffc0000U
#define HM_SECCOMP_ACTION_TRACE		0x7ff00000U
#define HM_SECCOMP_ACTION_NOTIF		0x7fc00000U
#define HM_SECCOMP_ACTION_TRAP		0x00030000U
#define __ACTV_WFERET_SECCOMP_TRAP		0x7ddd0000U // upcall signal for trap
#define __ACTV_WFERET_SECCOMP_KILL		0x7eee0000U // upcall signal for kill
#define __ACTV_WFERET_SECCOMP_KILL_STRICT	0x7eef0000U // upcall signal for kill strict mode
#define __ACTV_WFERET_SECCOMP_KILL_PROCESS	0x7eff0000U // upcall signal for kill process

#define HM_MAX_ERRNO			4095

/* Masks for the return value */
#define HM_SECCOMP_ACTION_FULL	0xffff0000U
#define HM_SECCOMP_ACTION_DATA	0x0000ffffU
#define HM_SECCOMP_ACTION_ONLY(ac) ((int32_t)((ac) & HM_SECCOMP_ACTION_FULL))

#define SECCOMP_RECORD_SYSCALL_ARG_NUMBER 6

#define HM_SECCOMP_SET_COMPAT_MODE_STRICT		0U
#define HM_SECCOMP_SET_COMPAT_MODE_FILTER		1U
#define HM_SECCOMP_GET_ACTION_AVAIL			2U

#define HM_SECCOMP_FILTER_FLAG_TSYNC			1U
#define HM_SECCOMP_FILTER_FLAG_LOG			(1U << 1)
#define HM_SECCOMP_FILTER_FLAG_SUPPORT			(HM_SECCOMP_FILTER_FLAG_TSYNC | HM_SECCOMP_FILTER_FLAG_LOG)

#define MAX_LINUX_SYSCALL_NUMBER 512
#define CACHE_MODE_SET_NATIVE 0U
#define CACHE_MODE_SET_COMPAT 1U
#define CACHE_MODE_GET_NATIVE 2U
#define CACHE_MODE_GET_COMPAT 3U
#define CACHE_MODE_INHERIT 4U

/* strict mode related syscall no defs */
#define __NR_SECCOMP_READ		__NR_read
#define __NR_SECCOMP_WRITE		__NR_write
#define __NR_SECCOMP_EXIT		__NR_exit
#define __NR_SECCOMP_EXIT_GROUP		__NR_exit_group
#define __NR_SECCOMP_SIGRERTURN		__NR_rt_sigreturn

static const int strict_allowed_syscalls[] = {
	__NR_SECCOMP_READ,
	__NR_SECCOMP_WRITE,
	__NR_SECCOMP_EXIT,
	__NR_SECCOMP_EXIT_GROUP,
	__NR_SECCOMP_SIGRERTURN,
};

#define HM_SECCOMP_STATE_DISABLED			0U
#define HM_SECCOMP_STATE_COMPAT_STRICT			1U
#define HM_SECCOMP_STATE_COMPAT_FILTER			2U

/* compat mode definition */
#define MAX_LINUX_SYSCALL_NUMBER 512
#define MAX_NATIVE_SYSCALL_NUMBER 4096

#define MAX_NR_LEN 3
#define STATUS_LEN 6
#define SPACES_AND_NEWLINE 3
#define CACHE_INFO_LENS(name) ((strlen(name) + MAX_NR_LEN + STATUS_LEN + SPACES_AND_NEWLINE) * MAX_LINUX_SYSCALL_NUMBER)

#ifdef CONFIG_AARCH64
#define HM_SECCOMP_ARCH_NATIVE		HM_AUDIT_ARCH_AARCH64
#define HM_SECCOMP_ARCH_NATIVE_NAME	"AARCH64"
#ifdef CONFIG_COMPAT
#define HM_SECCOMP_ARCH_COMPAT	HM_AUDIT_ARCH_ARM
#define HM_SECCOMP_ARCH_COMPAT_NAME	"ARM"
#endif
#else /* !CONFIG_64BIT */
#define HM_SECCOMP_ARCH_NATIVE		HM_AUDIT_ARCH_ARM
#define HM_SECCOMP_ARCH_NATIVE_NAME	"ARM"
#endif

#define NATIVE_CACHE_INFO_LENS (CACHE_INFO_LENS(HM_SECCOMP_ARCH_NATIVE_NAME))
#ifdef CONFIG_COMPAT
#define COMPAT_CACHE_INFO_LENS (CACHE_INFO_LENS(HM_SECCOMP_ARCH_COMPAT_NAME))
#define MIN_CACHE_INFO_LENS (NATIVE_CACHE_INFO_LENS + COMPAT_CACHE_INFO_LENS)
#else
#define MIN_CACHE_INFO_LENS NATIVE_CACHE_INFO_LENS
#endif

struct hm_seccomp_data_s {
	uint32_t linux_syscall_number; /* 0-4bytes: linux syscall number */
	uint32_t arch; /* 4-8bytes: arch */
	uint64_t instruction_pointer; /* 8-16bytes: instruction pointer */
	uint64_t args[6]; /* 16-64bytes: first six arguments */
	uint32_t native_syscall_number; /* 64-68bytes: offset in native actvhdlrs table */
};

typedef size_t cofilter_idx;
struct cbpf_prog_s;
struct seccomp_filter_s {
	int state; /* also put state in filter for ease of access */
	bool log;
	cofilter_idx curr_idx;
	cofilter_idx prev_idx;
	cofilter_idx prog_idx;
};

struct seccomp_filter_sysmgr {
	struct raw_refcnt refcnt;
	struct seccomp_filter_sysmgr *prev;
	struct cbpf_prog_s *prog;
	__sec_data(libsec, ptr) struct seccomp_filter_s *sc_filter;
};

struct seccomp_action_flags {
	bool request;
	bool recheck_after_trace;
	bool is_fast_path;
};

/* globals */
extern bool g_seccomp_fast_access_mode;

/* inline functions */
static inline void seccomp_set_fast_access_mode(bool mode)
{
	g_seccomp_fast_access_mode = mode;
}

static inline bool seccomp_is_in_fast_access_mode(void)
{
	return g_seccomp_fast_access_mode;
}

static inline int seccomp_current_pid_of(void)
{
	return hm_actv_local_thread_vpid_of();
}

static inline int seccomp_current_tid_of(void)
{
	return hm_actv_local_thread_vtid_of();
}

static inline int seccomp_current_native_scno_of(void)
{
	int native_scno;
	struct arch_actv_local *actv_local = hm_actv_current_arch_actv_local();
	BUG_ON(actv_local == NULL);

	native_scno = (int)__RPC_INFO_DECODE_CALLNO(actv_local->rpcinfo);

	return native_scno;
}

static inline int seccomp_current_linux_scno_of(void)
{
	if (!hm_actv_local_is_seccomp_info_inited()) {
		hm_actv_local_set_seccomp_info_inited();
	}

	return hm_actv_local_seccomp_scno_of();
}

static inline struct hm_seccomp_data_s *seccomp_current_info_of(void)
{
	if (!hm_actv_local_is_seccomp_info_inited()) {
		hm_actv_local_set_seccomp_info_inited();
	}

	return (struct hm_seccomp_data_s *)(void *)hm_actv_local_seccomp_info_of();
}

static inline uint64_t *seccomp_current_sc_args_of(void)
{
	if (!hm_actv_local_is_seccomp_info_inited()) {
		hm_actv_local_set_seccomp_info_inited();
	}

	return hm_actv_local_seccomp_args_of();
}

static inline void *seccomp_current_filter_of_caller(void)
{
	if (!hm_actv_local_is_seccomp_info_inited()) {
		hm_actv_local_set_seccomp_info_inited();
	}

	return (void *)(unsigned long)hm_actv_local_seccomp_filter_of();
}

static inline cofilter_idx seccomp_current_cofilter_index_of_caller(void)
{
	if (!hm_actv_local_is_seccomp_info_inited()) {
		hm_actv_local_set_seccomp_info_inited();
	}

	return (cofilter_idx)hm_actv_local_seccomp_cofilter_index_of();
}

int hm_secure_computing_fusionmgr(void);
int hm_secure_computing_copath(void);
int seccomp_handle_actions(unsigned int action, unsigned short data, struct hm_seccomp_data_s *sc_data,
					struct seccomp_action_flags seccomp_action_flags);
int proc_seccomp_cache_info(char *dest, size_t lens, unsigned long long *allow_syscalls_native,
			    unsigned long long *allow_syscalls_compat);

/* co-object filter operations */
int sec_cofilter_init(bool is_holder);
int sec_filter_alloc(size_t sz, cofilter_idx *idx);
void sec_filter_free(cofilter_idx idx);
void *sec_filter_idx_to_ptr(cofilter_idx idx);
#endif
