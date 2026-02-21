/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of timer capability
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 22 23:23:19 2019
 */
/*
 * Must choose one GRANT type from ALL/NO/COARSE/FINE
 * Must choose one GRANTMOVE type from ALL/NO/COARSE
 */
CAPTYPE(Timer, 13, CAPALLGRANT(Timer)
		   CAPNOGRANTMOVE(Timer)
		   CAPNODEFAULTCREF(Timer)
		   CAPRAWCREFRLOOKUP(Timer)
		   CAPDEFAULTSIZE(Timer),
	CAPMETHOD(Timer, Cancel)
	CAPMETHOD(Timer, Start)
	CAPMETHOD(Timer, Remain)
	CAPMETHOD(Timer, OverrunCount)

	CAPERRNO(Timer, EHMERROR, TIMER_STARTED,  "E_HM_TIMER_STARTED")
)

#ifdef CAP_EXPORT_API

#define __CAP_TIMER_OBJ_SIZE    256u

struct vtimekeep_tock_policy;

enum {
	CRIGHT_TIMER_DEFAULT_BIT,
	CRIGHT_TIMER_MAX
};

#define CRIGHT_TIMER_DEFAULT	(1U << CRIGHT_TIMER_DEFAULT_BIT)

#endif
