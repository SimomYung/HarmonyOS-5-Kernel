/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Interfaces of arm smccc
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 9 17:10:48 2020
 */
#ifndef MAPI_HMKERNEL_SMCCC_H
#define MAPI_HMKERNEL_SMCCC_H

#define SMC_OK			0UL
#define SMC_UNK			((unsigned long)(-1L))

#define SMCCC_VERSION		0x80000000UL
#define SMCCC_ARCH_FEATURES	0x80000001UL
#ifdef CONFIG_ENABLE_FIX_SPECV2_MITIGATION
#define SMCCC_ARCH_WORKAROUND_1	0x80008000UL /* mitigation for CVE-2017-5715 */
#define SMCCC_ARCH_WORKAROUND_2	0x80007FFFUL /* mitigation for CVE-2018-3639 */
#endif

#define SMCCC_VERSION_1_0	0x10000
#define SMCCC_VERSION_1_1	0x10001
#define SMCCC_VERSION_1_2	0x10002

#define SMCCC_NOT_SUPPORTED	0xFFFFFFFFU

/*
 * arm manual:
 * https://developer.arm.com/documentation/den0028/latest
 *
 * See table 2-1 (Bit usage within the SMC and HVC Function Identifier for Fast Call)
 */
#define SMCCC_CALL_CONV_SHIFT	30
#define SMCCC_IS_64(smc_val) \
	((smc_val) & (1U << SMCCC_CALL_CONV_SHIFT))

#define __smccc_args(a, b, c, d, e, f, g, h, ...) a, b, c, d, e, f, g, h
#define smccc_args(...) __smccc_args(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0)


struct smccc_res {
	unsigned long r0;
	unsigned long r1;
	unsigned long r2;
	unsigned long r3;
	unsigned long r4;
	unsigned long r5;
	unsigned long r6;
	unsigned long r7;
};

struct smccc_1_2_res {
	unsigned long r0;
	unsigned long r1;
	unsigned long r2;
	unsigned long r3;
	unsigned long r4;
	unsigned long r5;
	unsigned long r6;
	unsigned long r7;
	unsigned long r8;
	unsigned long r9;
	unsigned long r10;
	unsigned long r11;
	unsigned long r12;
	unsigned long r13;
	unsigned long r14;
	unsigned long r15;
	unsigned long r16;
	unsigned long r17;
};

struct arch_regs;
extern void smccc_smc_call(unsigned long a0, unsigned long a1, unsigned long a2,
			   unsigned long a3, unsigned long a4, unsigned long a5,
			   unsigned long a6, unsigned long a7, struct smccc_res *ret);

extern void smccc_smc_1_2_call(const struct arch_regs *regs, struct smccc_1_2_res *ret);

extern void smccc_hvc_call(unsigned long a0, unsigned long a1, unsigned long a2,
			   unsigned long a3, unsigned long a4, unsigned long a5,
			   unsigned long a6, unsigned long a7, struct smccc_res *ret);

extern void smccc_hvc_1_2_call(const struct smccc_1_2_res *regs, struct smccc_1_2_res *ret);

extern void hvc_call_helper(struct smccc_1_2_res *in_regs, struct smccc_1_2_res *out_regs);

extern int check_hvc_fid_validity(unsigned long fid);

extern unsigned int smccc_get_version(void);
#endif
