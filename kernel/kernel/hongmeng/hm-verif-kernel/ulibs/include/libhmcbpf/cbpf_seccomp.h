/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Definition of seccomp cbpf filter check & cache
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 01 11:27:14 2023
 */

#ifndef _CBPF_SECCOMP_H__
#define _CBPF_SECCOMP_H__

#include <libhmcbpf/cbpf.h>

/* for seccomp */
int seccomp_cbpf_check(struct cbpf_sock_filter_s *filter, uint32_t flen);
void seccomp_cbpf_const_allow_cache(struct cbpf_prog_s *prog, unsigned long long *bitmap_cur,
				    unsigned long long *bitmap_prev, uint32_t bitmap_size, uint32_t arch);

struct linux_seccomp_data_s {
	int nr;
	uint32_t arch;
	uint64_t instruction_pointer;
	uint64_t args[6];
};

#define HM_SECCOMP_ACTION_KILL_PROCESS	0x80000000U
#define HM_SECCOMP_ACTION_KILL_THREAD	0x00000000U
#define HM_SECCOMP_ACTION_KILL			HM_SECCOMP_ACTION_KILL_THREAD
#define HM_SECCOMP_ACTION_TRAP			0x00030000U
#define HM_SECCOMP_ACTION_ERRNO			0x00050000U
#define HM_SECCOMP_ACTION_ALLOW			0x7fff0000U

#endif
