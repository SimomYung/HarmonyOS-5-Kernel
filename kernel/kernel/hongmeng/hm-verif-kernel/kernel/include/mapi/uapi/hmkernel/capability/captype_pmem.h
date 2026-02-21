/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Define capability pmem types
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 14 11:23:07 2020
 */
/*
 * Must choose one GRANT type from ALL/NO/COARSE/FINE
 * Must choose one GRANTMOVE type from ALL/NO/COARSE
 */
CAPTYPE(PMEM, 3, CAPALLGRANT(PMEM)
		 CAPNOGRANTMOVE(PMEM)
		 CAPNODEFAULTCREF(PMEM)
		 CAPRAWCREFRLOOKUP(PMEM)
		 CAPDEFAULTSIZE(PMEM),
	CAPMETHOD(PMEM, Stat)

	CAPERRNO(PMEM, EHMERROR,	PMEM_IN_USE,		"E_HM_PMEM_IN_USE")
	CAPERRNO(PMEM, EHMERROR,	PMEM_SRC_STATE,	"E_HM_PMEM_SRC_STATE")
	CAPERRNO(PMEM, EHMERROR,	PMEM_DST_STATE,	"E_HM_PMEM_DST_STATE")
	CAPERRNO(PMEM, EHMERROR,	PMEM_NOMEM,		"E_HM_PMEM_NOMEM")
	CAPERRNO(PMEM, EHMERROR,	PMEM_OBJSIZE,		"E_HM_PMEM_OBJSIZE")
	)

#ifdef CAP_EXPORT_API
#ifndef CAP_PMEM_API_EXPORTED
#define CAP_PMEM_API_EXPORTED

#include <hmkernel/types.h>

#define __CAP_PMEM_TYPE_FREE		1U
#define __CAP_PMEM_TYPE_KOBJ		2U
#define __CAP_PMEM_TYPE_CNODE_TABLE	3U

#define PMEM_STAT_PFN		1UL
#define PMEM_STAT_NRPFN	2UL
#define PMEM_STAT_TYPE	4UL

struct sysarg_pmem_stat {
	__u64 pfn;
	__u64 nrpfn;
	__u32 mem_type;
};

enum {
	CRIGHT_PMEM_STAT_BIT,
	CRIGHT_PMEM_MAX
};

#define CRIGHT_PMEM_STAT	(1U << CRIGHT_PMEM_STAT_BIT)

#endif /* !CAP_PMEM_API_EXPORTED */

#endif
