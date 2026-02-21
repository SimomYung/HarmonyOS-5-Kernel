/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Capability-based IrqCtrl management
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 6 16:42:04 2022
 */
CAPTYPE(DsuContext, 20, CAPNOGRANT(DsuContext)
		    CAPNOGRANTMOVE(DsuContext)
		    CAPNODEFAULTCREF(DsuContext)
		    CAPRAWCREFRLOOKUP(DsuContext),
	CAPMETHOD(DsuContext, Attach)
	CAPMETHOD(DsuContext, Detach)
	CAPMETHOD(DsuContext, Config)
	)

#ifdef CAP_EXPORT_API
#ifndef CAP_DSUCONTEXT_API_EXPORTED
#define CAP_DSUCONTEXT_API_EXPORTED

enum {
	CRIGHT_DSUCONTEXT_CONFIG_BIT,
	CRIGHT_DSUCONTEXT_ATTACH_DETACH_BIT,
	CRIGHT_DSUCONTEXT_MAX
};

#define CRIGHT_DSUCONTEXT_CONFIG		(1U << CRIGHT_DSUCONTEXT_CONFIG_BIT)
#define CRIGHT_DSUCONTEXT_ATTACH_DETACH		(1U << CRIGHT_DSUCONTEXT_ATTACH_DETACH_BIT)

#define DSUCONTEXT_ARM_DSU_STRUCT_SIZE		900UL

#endif
#endif
