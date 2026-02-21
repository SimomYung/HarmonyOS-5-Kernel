/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Definition of Livepatch capability
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 11 14:23:19 2022
 */
/*
 * Must choose one GRANT type from ALL/NO/COARSE/FINE
 * Must choose one GRANTMOVE type from ALL/NO/COARSE
 */
CAPTYPE(Livepatch, 17, CAPALLGRANT(Livepatch)
		   CAPNOGRANTMOVE(Livepatch)
		   CAPNODEFAULTCREF(Livepatch)
		   CAPRAWCREFRLOOKUP(Livepatch)
		   CAPDEFAULTSIZE(Livepatch),
	CAPMETHOD(Livepatch, Load)
	CAPMETHOD(Livepatch, Unload)
	CAPMETHOD(Livepatch, Patching)
	CAPMETHOD(Livepatch, List)
)

#ifdef CAP_EXPORT_API

enum {
	CRIGHT_LIVEPATCH_MAX
};

#endif
