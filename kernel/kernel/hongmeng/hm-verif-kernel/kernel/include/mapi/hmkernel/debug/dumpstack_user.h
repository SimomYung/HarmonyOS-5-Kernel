/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Dump user stack for modules
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 14 16:47:07 2023
 */
#ifndef MAPI_HMKERNEL_DUMPSTACK_USER_H
#define MAPI_HMKERNEL_DUMPSTACK_USER_H

#include <hmkernel/types.h>

struct tcb_s;
#ifdef CONFIG_DUMP_USER_STACK
void dumpstack_user_callchain(struct tcb_s *tcb, int seq, bool nolock, bool is_panic, bool dump_uctx_regs);
void dumpstack_user_callchain_lastword(struct tcb_s *tcb);
void dumpstack_online_cpus(void);
#else
static inline void dumpstack_user_callchain(struct tcb_s *tcb, int seq, bool nolock, bool is_panic, bool dump_uctx_regs)
{
	UNUSED(tcb, seq, nolock, is_panic);
}
static inline void dumpstack_user_callchain_lastword(struct tcb_s *tcb)
{
	UNUSED(tcb);
}
static inline void dumpstack_online_cpus(void) {};
#endif
#endif
