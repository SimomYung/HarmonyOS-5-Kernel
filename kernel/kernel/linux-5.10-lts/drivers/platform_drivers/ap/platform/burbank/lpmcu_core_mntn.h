/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: lpmcu core mntn
 * Author: zhuzhangwei
 * Create: 2020-04-09
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __LPMCU_CORE_MNTN_H__
#define __LPMCU_CORE_MNTN_H__

enum {
	RDR_BACKUP_IDEX0 = 0,
	RDR_BACKUP_IDEX1,
	RDR_BACKUP_IDEX2,
	RDR_BACKUP_IDEX3,
	RDR_BACKUP_IDEX_MAX
};

struct exc_spec_data {
	unsigned char reset_reason[RDR_BACKUP_IDEX_MAX];
	unsigned int slice;
	unsigned int rtc;
	unsigned int r13;
	unsigned int lr1;
	unsigned int pc;
	unsigned int xpsr;
	unsigned int cfsr;
	unsigned int hfsr;
	unsigned int bfar;
	unsigned char exc_trace;
	unsigned char ddr_exc;
	unsigned short irq_id;
	unsigned int task_id;
};

/*
 * Exception:
 *  order: r0~r13 msp psp lr(1) lr(2) pc xpsr PRIMASK BASEPRI FAULTMASK
 *  r4~r11 call _save_reg, if no change in exc, as exc point
 *  lr(1)  enter exc handle, as EXC_RETURN
 *  r13 msp psp r0~r3,r12,lr(2),pc,xpsr exc point
 *
 * systemError:
 *  all current
 *  order: r0~r13 msp psp CONTROL lr(2) pc xpsr PRIMASK BASEPRI FAULTMASK
 */
struct rdr_lpmcu_cregs { /* core registers */
	unsigned int r0;
	unsigned int r1;
	unsigned int r2;
	unsigned int r3;
	unsigned int r4;
	unsigned int r5;
	unsigned int r6;
	unsigned int r7;
	unsigned int r8;
	unsigned int r9;
	unsigned int r10;
	unsigned int r11;
	unsigned int r12;
	unsigned int r13;
	unsigned int sp; /* msp */
	unsigned int psp;
	unsigned int lr0;
	unsigned int lr1;
	unsigned int pc;
	unsigned int xpsr;
	unsigned int primask;
	unsigned int basepri;
	unsigned int faultmask;
	unsigned int control;
};

struct rdr_reg_backup {
	unsigned int idex; /* 值为RDR_REG_BACKUP_IDEX */
	unsigned int reason[RDR_BACKUP_IDEX_MAX];
	struct rdr_lpmcu_cregs core_regs[RDR_BACKUP_IDEX_MAX];
};

#endif
