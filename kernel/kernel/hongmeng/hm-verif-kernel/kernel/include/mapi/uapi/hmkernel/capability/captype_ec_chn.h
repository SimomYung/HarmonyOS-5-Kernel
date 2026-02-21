/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: Event Channel channel capability definition
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 09 09:18:25 2021
 */

/*
 * Must choose one GRANT type from ALL/NO/COARSE/FINE
 * Must choose one GRANTMOVE type from ALL/NO/COARSE
 */

CAPTYPE(EcChn, 19, CAPNOGRANT(EcChn)
		 CAPNOGRANTMOVE(EcChn)
		 CAPNODEFAULTCREF(EcChn)
		 CAPRAWCREFRLOOKUP(EcChn)
		 CAPDEFAULTSIZE(EcChn),
	)

#ifdef CAP_EXPORT_API
#ifndef CAP_EC_CHN_API_EXPORTED
#define CAP_EC_CHN_API_EXPORTED

#define __CAP_EC_CHN_OBJ_SIZE 32U

enum {
	CRIGHT_ECCHN_MAX
};

#endif
#endif
