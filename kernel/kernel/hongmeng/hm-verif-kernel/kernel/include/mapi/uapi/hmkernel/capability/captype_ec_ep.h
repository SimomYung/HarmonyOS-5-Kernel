/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: Event Channel endpoint capability definition
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 09 09:18:25 2021
 */

/*
 * Must choose one GRANT type from ALL/NO/COARSE/FINE
 * Must choose one GRANTMOVE type from ALL/NO/COARSE
 */

CAPTYPE(EcEp, 18, CAPNOGRANT(EcEp)
		CAPNOGRANTMOVE(EcEp)
		CAPNODEFAULTCREF(EcEp)
		CAPRAWCREFRLOOKUP(EcEp)
		CAPDEFAULTSIZE(EcEp),
	CAPMETHOD(EcEp, Attach)
	CAPMETHOD(EcEp, Detach)
	CAPMETHOD(EcEp, SendEvent)
	)

#ifdef CAP_EXPORT_API
#ifndef CAP_EC_EP_API_EXPORTED
#define CAP_EC_EP_API_EXPORTED

#define __CAP_EC_EP_OBJ_SIZE 256U

enum {
	CRIGHT_ECEP_MAX
};

#endif
#endif
