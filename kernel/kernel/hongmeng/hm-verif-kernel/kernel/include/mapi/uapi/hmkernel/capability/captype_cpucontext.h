/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Captype definition for CpuContext
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 13 12:47:00 2018
 */
/*
 * Must choose one GRANT type from ALL/NO/COARSE/FINE
 * Must choose one GRANTMOVE type from ALL/NO/COARSE
 */
CAPTYPE(CpuContext, 10, CAPNOGRANT(CpuContext)
			CAPNOGRANTMOVE(CpuContext)
			CAPNODEFAULTCREF(CpuContext)
			CAPRAWCREFRLOOKUP(CpuContext),
	CAPMETHOD(CpuContext, Config)
	CAPMETHOD(CpuContext, AttachCPU)
	CAPMETHOD(CpuContext, DetachCPU)
	)

#ifdef CAP_EXPORT_API
#ifndef CAP_CPUCONTEXT_API_EXPORTED
#define CAP_CPUCONTEXT_API_EXPORTED

enum {
	CRIGHT_CPUCONTEXT_ATTACH_CPU_BIT,
	CRIGHT_CPUCONTEXT_THREAD_ATTACH_BIT,
	CRIGHT_CPUCONTEXT_MAX
};

#define CRIGHT_CPUCONTEXT_ATTACH_CPU		(1U << CRIGHT_CPUCONTEXT_ATTACH_CPU_BIT)
#define CRIGHT_CPUCONTEXT_THREAD_ATTACH		(1U << CRIGHT_CPUCONTEXT_THREAD_ATTACH_BIT)

/*
 * sizeof(struct perf_cpu_ctx_s) is larger than 27504 in the kernel currently,
 * set PMU_STRUCT_SIZE to PAGE_SIZE * 8
 */
#define CPUCONTEXT_ARM_PMU_STRUCT_SIZE		32767UL
#define CPUCONTEXT_ARM_DSU_STRUCT_SIZE		900UL
#define CPUCONTEXT_ARM_DEBUG_STRUCT_SIZE	600UL

#endif
#endif
