/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Xdep Infer
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 21 16:32:55 2024
 */

#ifndef MAPI_HMKERNEL_XDEP_INFER_H
#define MAPI_HMKERNEL_XDEP_INFER_H

#ifdef CONFIG_XDEP_INFER
#include <hmkernel/tcb/struct.h>
void xdep_tcb_init(struct tcb_s *tcb);
void xdep_tcb_exit(struct tcb_s *tcb);
void xdep_wake_check(struct tcb_s *wakee, enum tcb_block_reason block_reason);
int xdep_infer_ctrl(struct tcb_s *tcb, struct sched_xdep_infer_ctrl_s *xdep_args,
		    void __user *result, size_t result_size);
void xdep_init(void);

#ifdef __HOST_LLT__
int llt_xdep_infer_tcb_index_num(void);
int llt_xdep_infer_wake_side_num(void);
int llt_xdep_infer_key_thread_num(void);
#endif

#endif

#endif /* MAPI_HMKERNEL_XDEP_INFER_H */
