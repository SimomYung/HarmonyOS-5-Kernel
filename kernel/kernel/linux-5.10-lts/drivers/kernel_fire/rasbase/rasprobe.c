/*
 * Copyright(C) 2023 Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include "rasprobe.h"

enum REG_INDEX {
	REG_ARG0_INDEX = 0,
	REG_ARG1_INDEX,
	REG_ARG2_INDEX,
	REG_ARG3_INDEX
};

#ifdef __ASM_PTRACE_H
/* ARM64bit arm arg's regs : r0-r3 */
#define REG_RET(regs)  regs->regs[0]
#define REG_ARG0(regs) regs->regs[0]
#define REG_ARG1(regs) regs->regs[1]
#define REG_ARG2(regs) regs->regs[2]
#define REG_ARG3(regs) regs->regs[3]

#elif defined __ASM_ARM_PTRACE_H
/* ARM32bit arm arg's regs : r0-r3 */
#define REG_RET(regs)  regs->uregs[0]
#define REG_ARG0(regs) regs->uregs[0]
#define REG_ARG1(regs) regs->uregs[1]
#define REG_ARG2(regs) regs->uregs[2]
#define REG_ARG3(regs) regs->uregs[3]

#else
/* x86 Linux 9 arg's regs : RDI,RSI,RDX,RCX,RAX,R8,R9,R10,R11 */
#define REG_RET(regs) regs->ax
#define REG_ARG0(regs) regs->di
#define REG_ARG1(regs) regs->si
#define REG_ARG2(regs) regs->dx
#define REG_ARG3(regs) regs->cx
#endif

int rasprobe_entry(struct rasprobe_instance *ri, struct pt_regs *regs)
{
	struct RasRegs *rd = (struct RasRegs *)ri->data;
	/* arm arg's regs : r0-r3 */
	rd->args[REG_ARG0_INDEX] = (void *)REG_ARG0(regs);
	rd->args[REG_ARG1_INDEX] = (void *)REG_ARG1(regs);
	rd->args[REG_ARG2_INDEX] = (void *)REG_ARG2(regs);
	rd->args[REG_ARG3_INDEX] = (void *)REG_ARG3(regs);
	return 0;
}

void rasprobe_seturn(struct pt_regs *regs, long val)
{
	REG_RET(regs) = val;
}

void rasprobe_setarg(struct pt_regs *regs, int index, long val)
{
	switch (index) {
	case REG_ARG0_INDEX:
		REG_ARG0(regs) = val;
		break;
	case REG_ARG1_INDEX:
		REG_ARG1(regs) = val;
		break;
	case REG_ARG2_INDEX:
		REG_ARG2(regs) = val;
		break;
	case REG_ARG3_INDEX:
		REG_ARG3(regs) = val;
		break;
	default:
		break;
	}
}

void rasprobe_setargptr(struct pt_regs *regs, int index, void *val)
{
	switch (index) {
	case REG_ARG0_INDEX:
		REG_ARG0(regs) = (uintptr_t)val;
		break;
	case REG_ARG1_INDEX:
		REG_ARG1(regs) = (uintptr_t)val;
		break;
	case REG_ARG2_INDEX:
		REG_ARG2(regs) = (uintptr_t)val;
		break;
	case REG_ARG3_INDEX:
		REG_ARG3(regs) = (uintptr_t)val;
		break;
	default:
		break;
	}
}

int register_rasprobes(struct rasprobe **rps, int num)
{
	int i, ret;

	if (num <= 0)
		return -EINVAL;
	for (i = 0; i < num; i++) {
		ret = register_kretprobe(rps[i]);
		if (ret < 0) {
			pr_err("rasprobes failed when registering %s, ret = %d\n", rps[i]->kp.symbol_name, ret);
			unregister_rasprobes(rps, i);
			break;
		}
	}
	return ret;
}

void unregister_rasprobes(struct rasprobe **rps, int num)
{
	int i;

	for (i = 0; i < num; i++)
		unregister_kretprobe(rps[i]);
}
