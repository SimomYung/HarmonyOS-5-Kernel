/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Definition of scheduler capability
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 17 10:05:31 2018
 */
/*
 * Must choose one GRANT type from ALL/NO/COARSE/FINE
 * Must choose one GRANTMOVE type from ALL/NO/COARSE
 */
CAPTYPE(Scheduler, 11, CAPALLGRANT(Scheduler)
		       CAPNOGRANTMOVE(Scheduler)
		       CAPNODEFAULTCREF(Scheduler)
		       CAPRAWCREFRLOOKUP(Scheduler)
		       CAPDEFAULTSIZE(Scheduler),
	CAPMETHOD(Scheduler, Stat)
	)

#ifdef CAP_EXPORT_API
#ifndef CAP_SCHEDPARTITION_API_EXPORTED
#define CAP_SCHEDPARTITION_API_EXPORTED

#include <hmkernel/types.h>

struct scheduler_stat_s {
	__u32 flags;	/* in/out in: requested stat entry, out: filled entry */
	__u32 size;	/* mem size requested for a scheduler object */
};

#define SCHEDULER_STAT_SIZE		0x01UL

enum {
	CRIGHT_SCHEDULER_MAX
};

#endif
#endif
