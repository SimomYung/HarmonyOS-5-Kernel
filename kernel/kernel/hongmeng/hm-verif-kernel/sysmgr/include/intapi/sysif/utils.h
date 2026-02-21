/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: intapi of sysmgr sysif
 * Author: Huawei OS Kernel Lab
 * Create: Sun Nov 12 15:50:02 2023
 */
#ifndef INTAPI_SYSIF_UTILS_H
#define INTAPI_SYSIF_UTILS_H

/*
 * Indicate to use the pointer of process as the credential
 * when acquiring path.
 */
#define __ACTV_CRED_PROCESS		1UL

int sysmgr_cred_of_cnode_idx(unsigned int cnode_idx, unsigned long *p_cred);
int sysmgr_cred_set_vfs_data(unsigned long cred, unsigned long vfs_data);
unsigned long vfs_data_of_sysmgr_cred(unsigned long cred);

#ifdef __HOST_LLT__
#define get_cpuid_register_cpuset() 0xffffffff
#else
extern unsigned long get_cpuid_register_cpuset(void);
#endif

#define cpuset_for_each_cpu(cpu, cpuset)               \
	for (unsigned long cpubits = cpuset;                     \
		(cpu) = ((unsigned long)__builtin_ffs((long)cpubits) - 1),   \
		(unsigned long)(cpu) < CONFIG_NR_CPUS;                 \
		cpubits &= ~(1U << (cpu)))

#define cpuid_for_each_registered_cpu(cpu)	\
	cpuset_for_each_cpu(cpu, get_cpuid_register_cpuset())

#endif /* !INTAPI_SYSIF_UTILS_H */
