/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2024. All rights reserved.
 * Description: pcpinfo header file
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 23 17:05:22 2024
 */

#ifndef ULIBS_INCLUDE_LIBPCPINFO_PCPINFO_H
#define ULIBS_INCLUDE_LIBPCPINFO_PCPINFO_H

#include <hmkernel/compiler.h>
#include <hmkernel/ctrlmem/pcpinfo.h>

#ifdef __HOST_LLT__
struct __ctrlmem_pcpinfo_s fake_pcpinfo_addr[CONFIG_NR_CPUS];
#endif

#ifndef __define_pcpinfo_current_thread_op
/* See Documentation/pcpinfo_synchronization.md for more discussions */
#define __define_pcpinfo_current_thread_op(field, type, read_op)		\
static inline __attribute__((always_inline)) type				\
__pcpinfo_current_thread_##field(const struct __ctrlmem_pcpinfo_s *pcpinfo)	\
{										\
	type val; 								\
	__u64 gen; 								\
	__u64 new_gen; 								\
	unsigned int cpuid;							\
	unsigned int new_cpuid;							\
	do {									\
		cpuid = __arch_current_cpuid(); 				\
		gen = read_op(pcpinfo[cpuid].generation);			\
		val = read_op(pcpinfo[cpuid].ptcb.field);			\
		new_cpuid = __arch_current_cpuid(); 				\
		new_gen = read_op(pcpinfo[cpuid].generation);			\
	} while ((gen != new_gen) || (cpuid != new_cpuid));			\
	return val;								\
}
#endif

__define_pcpinfo_current_thread_op(tcb_cref, __u64, __read_once)
__define_pcpinfo_current_thread_op(sched_tcb_cref, __u64, __read_once)
__define_pcpinfo_current_thread_op(running_prio, __u32, __read_once)
__define_pcpinfo_current_thread_op(vtid, __u32, __read_once)
__define_pcpinfo_current_thread_op(vpid, __u32, __read_once)
__define_pcpinfo_current_thread_op(tid, __u32, __read_once)
__define_pcpinfo_current_thread_op(pid, __u32, __read_once)
__define_pcpinfo_current_thread_op(kcov_idx_state, __u32, __read_once)
__define_pcpinfo_current_thread_op(vs_key, __u64, __read_once)
__define_pcpinfo_current_thread_op(xdep_critical, __u8, __read_once)

#endif
