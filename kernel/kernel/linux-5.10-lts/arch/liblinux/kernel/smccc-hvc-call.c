// SPDX-License-Identifier: GPL-2.0-only
#include <linux/module.h>
#include <linux/init.h>
#include <linux/arm-smccc.h>
#include <liblinux/pal.h>

#include <liblinux/kapi.h>


#define SMCCC_HVC_MAX_REGS 18

#ifdef CONFIG_LIBLINUX_HVC_CALL
void __arm_smccc_hvc(unsigned long a0, unsigned long a1,
			unsigned long a2, unsigned long a3, unsigned long a4,
			unsigned long a5, unsigned long a6, unsigned long a7,
			struct arm_smccc_res *res, struct arm_smccc_quirk *quirk)
{
	unsigned long in_regs[SMCCC_HVC_MAX_REGS];
	unsigned long out_regs[SMCCC_HVC_MAX_REGS];

	in_regs[0] = a0;
	in_regs[1] = a1;
	in_regs[2] = a2;
	in_regs[3] = a3;
	in_regs[4] = a4;
	in_regs[5] = a5;
	in_regs[6] = a6;
	in_regs[7] = a7;
	liblinux_pal_hvc_call(in_regs, out_regs);
	res->a0 = out_regs[0];
	res->a1 = out_regs[1];
	res->a2 = out_regs[2];
	res->a3 = out_regs[3];
	if(unlikely(quirk != NULL && quirk->id == ARM_SMCCC_QUIRK_QCOM_A6)) {
		quirk->state.a6 = out_regs[6];
	}
}
EXPORT_SYMBOL(__arm_smccc_hvc);

void arm_smccc_1_2_hvc(const struct arm_smccc_1_2_regs *args,
				  struct arm_smccc_1_2_regs *res)
{
	liblinux_pal_hvc_call((unsigned long *)args, (unsigned long *)res);
}
EXPORT_SYMBOL(arm_smccc_1_2_hvc);
#endif
