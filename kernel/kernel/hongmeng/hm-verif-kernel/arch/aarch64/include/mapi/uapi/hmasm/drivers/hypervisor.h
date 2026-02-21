/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 19 13:40:16 2018
 */

#ifndef AARCH64_UAPI_ASM_DRIVERS_HYPERVISOR_H
#define AARCH64_UAPI_ASM_DRIVERS_HYPERVISOR_H

#include <hmkernel/types.h>

enum {
	/* General purpose registers */
	VCPU_X0,
	VCPU_X1,
	VCPU_X2,
	VCPU_X3,
	VCPU_X4,
	VCPU_X5,
	VCPU_X6,
	VCPU_X7,
	VCPU_X8,
	VCPU_X9,
	VCPU_X10,
	VCPU_X11,
	VCPU_X12,
	VCPU_X13,
	VCPU_X14,
	VCPU_X15,
	VCPU_X16,
	VCPU_X17,
	VCPU_X18,
	VCPU_X19,
	VCPU_X20,
	VCPU_X21,
	VCPU_X22,
	VCPU_X23,
	VCPU_X24,
	VCPU_X25,
	VCPU_X26,
	VCPU_X27,
	VCPU_X28,
	VCPU_X29,
	VCPU_X30,
	/*
	 * The guest's EL0 and EL1 registers. There's no state here
	 * that can't be changed by the guest anyway, so it is safe
	 * to save/restore these directly without sanitizing them.
	 *
	 * Note that vcpu switch depends on the following register
	 * pairs.
	 */
	VCPU_SP_EL0,      VCPU_TPIDR_EL0,
	VCPU_TPIDRRO_EL0, VCPU_ACTLR_EL1,

	VCPU_AFSR0_EL1,
	VCPU_AFSR1_EL1,   VCPU_AMAIR_EL1,
	VCPU_CNTKCTL_EL1, VCPU_CONTEXTIDR_EL1,
	VCPU_CSSELR_EL1,  VCPU_ELR_EL1,
	VCPU_ESR_EL1,     VCPU_FAR_EL1,
	VCPU_MAIR_EL1,    VCPU_MDCCINT_EL1,
	VCPU_MDSCR_EL1,   VCPU_PAR_EL1,
	VCPU_SCTLR_EL1,	  VCPU_SP_EL1,
	VCPU_SPSR_EL1,    VCPU_TCR_EL1,

	VCPU_TPIDR_EL1,
	VCPU_TTBR0_EL1,   VCPU_TTBR1_EL1,
	VCPU_VBAR_EL1,    VCPU_CPACR_EL1,
	VCPU_FPCR,        VCPU_FPSR,
	VCPU_ZCR_EL1,

	VCPU_APIAKeyLo_EL1, VCPU_APIAKeyHi_EL1,
	VCPU_APIBKeyLo_EL1, VCPU_APIBKeyHi_EL1,
	VCPU_APDAKeyLo_EL1, VCPU_APDAKeyHi_EL1,
	VCPU_APDBKeyLo_EL1, VCPU_APDBKeyHi_EL1,
	VCPU_APGAKeyLo_EL1, VCPU_APGAKeyHi_EL1,

	VCPU_MPAM1_EL1, VCPU_MPAM0_EL1,
	VCPU_SCXTNUM_EL1, VCPU_SCXTNUM_EL0,
	VCPU_AMUSERENR_EL0,
	/*
	 * EL2 registers that encode guest state, so it makes sense
	 * to late userspace control parts of them.
	 *
	 * Only the SPSR_EL2 is unsafe to save/restore without
	 * sanitization.
	 *
	 * Note that vcpu switch depends on the following register
	 * pairs.
	 */
	VCPU_ELR_EL2,
	VCPU_SPSR_EL2,
	HOST_DAIF_EL2 = VCPU_SPSR_EL2,
	VCPU_VMPIDR_EL2, VCPU_VPIDR_EL2,
	/* The lowvisor will only read, not write, the following */
	VCPU_ESR_EL2,
	VCPU_FAR_EL2,
	VCPU_HPFAR_EL2,
	VCPU_FPEXC32_EL2,

	/* host may use the following registers */
	VCPU_CPTR_EL2,
	NR_VCPU_REGISTERS,
};

#define VCPU_DEBUG_REG_MAX_NR	16
struct arch_vcpu_debug_ctx {
	__u64 bcr[VCPU_DEBUG_REG_MAX_NR];
	__u64 bvr[VCPU_DEBUG_REG_MAX_NR];
	__u64 wcr[VCPU_DEBUG_REG_MAX_NR];
	__u64 wvr[VCPU_DEBUG_REG_MAX_NR];
};

enum pmu_regs {
	PMCR_EL0,
	PMSELR_EL0,
	PMEVCNTR0_EL0,
	PMEVCNTR30_EL0 = PMEVCNTR0_EL0 + 30,
	PMCCNTR_EL0,
	PMEVTYPER0_EL0,
	PMEVTYPER30_EL0 = PMEVTYPER0_EL0 + 30,
	PMXEVCNTR_EL0,
	PMXEVTYPER_EL0,
	PMCCFILTR_EL0,
	PMCNTENSET_EL0,
	PMCNTENCLR_EL0,
	PMINTENSET_EL1,
	PMINTENCLR_EL1,
	PMOVSSET_EL0,
	PMOVSCLR_EL0,
	PMUSERENR_EL0,
	PMCEID0_EL0,
	PMCEID1_EL0,
	PMSWINC_EL0,
	N_PMU_REGS
};

struct arch_vcpu_pmu_ctx {
	__u64 registers[N_PMU_REGS];
};

typedef __uint128_t sve_data_reg_t[16];
typedef __uint128_t sve_pred_reg_t[2];

struct arch_vcpu_sve_ctx {
	sve_data_reg_t zregs[32];
	sve_pred_reg_t pregs[16];
	sve_pred_reg_t ffr;
} __attribute__((aligned(16)));

/* Structure that holds all of the shared vcpu data between userspace/kernel */
struct arch_vcpu_ctx {
	__u64 registers[NR_VCPU_REGISTERS];
	__uint128_t fpu_registers[32];
	struct arch_vcpu_pmu_ctx vpmu_ctx;
	struct arch_vcpu_debug_ctx dbg_ctx;
};

typedef union {
	__u64 raw;
	struct {
		__u32 m       : 4;
		__u32 m4      : 1;
		__u32 res0    : 1;
		__u32 f       : 1;
		__u32 i       : 1;
		__u32 a       : 1;
		__u32 d       : 1;
		__u32 btype   : 2;
		__u32 ssbs    : 1;
		__u32 res1    : 7;
		__u32 il      : 1;
		__u32 ss      : 1;
		__u32 pan     : 1;
		__u32 uao     : 1;
		__u32 dit     : 1;
		__u32 tco     : 1;
		__u32 res2    : 2;
		__u32 v       : 1;
		__u32 c       : 1;
		__u32 z       : 1;
		__u32 n       : 1;
		__u32 res3;
	};
} spsr_t;

/* SPSR_ELx.M[0:3] field */
#define __PSR_MODE_EL0t		0b0000
#define __PSR_MODE_EL1t		0b0100
#define __PSR_MODE_EL1h		0b0101
#define __PSR_MODE_EL2t		0b1000
#define __PSR_MODE_EL2h		0b1001
/* SPSR_ELx.M[0:3] field for AArch32 */
#define __PSR_MODE_USER		0b0000
#define __PSR_MODE_FIQ		0b0001
#define __PSR_MODE_IRQ		0b0010
#define __PSR_MODE_SVC		0b0011
#define __PSR_MODE_ABORT	0b0111
#define __PSR_MODE_HYP		0b1010
#define __PSR_MODE_UNDEF	0b1011
#define __PSR_MODE_SYS		0b1111

typedef struct {
	__u32 df_stat_code : 6;	/* DFSC  [5:0] */
	__u32 write        : 1;	/* WnR    [ 6] */
	__u32 stage1_trans : 1;	/* S1PTW  [ 7] */
	__u32 cache_maint  : 1;	/* CM     [ 8] */
	__u32 ext_abrt_typ : 1;	/* EA     [ 9] */
	__u32 far_invalid  : 1;	/* FnV    [10] */
	__u32 error_type   : 2;	/* SET [12:11] */
	__u32              : 1;	/* RES0   [13] */
	__u32 acq_rel      : 1;	/* AR     [14] */
	__u32 sixty_four   : 1;	/* SF     [15] */
	__u32 reg_transf   : 5;	/* SRT [20:16] */
	__u32 sign_extend  : 1;	/* SSE    [21] */
	__u32 access_size  : 2;	/* SAS [23:22] */
	__u32 syndr_valid  : 1;	/* ISV    [24] */
} ec_data_abort_t;

typedef struct {
	__u32 stat_code    : 6;  /* IFSC   [5:0] */
	__u32              : 1;  /* RES0    [ 6] */
	__u32 stage1_trans : 1;  /* S1PTW   [ 7] */
	__u32              : 1;  /* RES0    [ 8] */
	__u32 ext_abrt_typ : 1;  /* EA      [ 9] */
	__u32 far_invalid  : 1;  /* FnV     [10] */
	__u32 error_type   : 2;  /* SET  [12:11] */
	__u32              : 12; /* RES0 [24:13] */
} ec_inst_abort_t;

typedef struct {
	__u32 direction : 1; /*         [ 0] */
	__u32 crm       : 4; /*        [4:1] */
	__u32 rt        : 5; /*        [9:5] */
	__u32 crn       : 4; /*      [13:10] */
	__u32 op1       : 3; /*      [16:14] */
	__u32 op2       : 3; /*      [19:17] */
	__u32 op0       : 2; /*      [21:20] */
	__u32           : 3; /* RES0 [24:22] */
} ec_msr_mrs_ex_t;

typedef struct {
	__u32 direction : 1; /*         [ 0] */
	__u32 crm       : 4; /*        [4:1] */
	__u32 rt        : 5; /*        [9:5] */
	__u32 rt2       : 5; /*      [14:10] */
	__u32           : 1; /* RES0    [15] */
	__u32 opc1      : 4; /*      [19:16] */
	__u32 cond      : 4; /*      [23:20] */
	__u32 cv        : 1; /*         [24] */
} ec_mcrr_mrrc_t;

typedef struct {
	__u32 direction : 1; /*         [ 0] */
	__u32 crm       : 4; /*        [4:1] */
	__u32 rt        : 5; /*        [9:5] */
	__u32 crn       : 4; /*      [13:10] */
	__u32 opc1      : 3; /*      [16:14] */
	__u32 opc2      : 3; /*      [19:17] */
	__u32 cond      : 4; /*      [23:20] */
	__u32 cv        : 1; /*         [24] */
} ec_mcr_mrc_t;

typedef struct {
	__u32 imm16     :16; /*       [15:0] */
	__u32           : 8; /* RES0 [24:16] */
} ec_hvc_svc_t;

#define ESR_EL2_EC_WFX			0x1U
#define ESR_EL2_EC_MCRR_MRRC_C15	0x4U
#define ESR_EL2_EC_MCR_MRC_C14		0x5U
#define ESR_EL2_EC_FPU			0x7U
#define ESR_EL2_EC_SVC32		0x11U
#define ESR_EL2_EC_HVC32		0x12U
#define ESR_EL2_EC_SMC32		0x13U
#define ESR_EL2_EC_SVC64		0x15U
#define ESR_EL2_EC_HVC64		0x16U
#define ESR_EL2_EC_SMC64		0x17U
#define ESR_EL2_EC_MSREX		0x18U
#define ESR_EL2_EC_SVE			0x19U
#define ESR_EL2_EC_IABRT		0x20U
#define ESR_EL2_EC_DABRT		0x24U
#define ESR_EL2_EC_BREAKPT_LOW		0x30U
#define ESR_EL2_EC_BREAKPT_CUR		0x31U
#define ESR_EL2_EC_SOFTSTP_LOW		0x32U
#define ESR_EL2_EC_SOFTSTP_CUR		0x33U
#define ESR_EL2_EC_WATCHPT_LOW		0x34U
#define ESR_EL2_EC_WATCHPT_CUR		0x35U
#define ESR_EL2_EC_SOFTBRK32		0x38U
#define ESR_EL2_EC_SOFTBRK64		0x3CU

#define ESR_EL2_ISS_WFX_MASK		0x3U
#define ESR_EL2_ISS_WFI			0x0U
#define ESR_EL2_ISS_WFE			0x1U
#define ESR_EL2_ISS_WFIT		0x2U
#define ESR_EL2_ISS_WFET		0x3U

#define ESR_EL2_EC_WRITE		0x0U
#define ESR_EL2_EC_READ			0x1U

#define ESR_EL2_FSC_TRANS_FAULT_TTW0	0b000100
#define ESR_EL2_FSC_TRANS_FAULT_TTW1	0b000101
#define ESR_EL2_FSC_TRANS_FAULT_TTW2	0b000110
#define ESR_EL2_FSC_TRANS_FAULT_TTW3	0b000111

#define ESR_EL2_FSC_ACCESS_FLAG_FAULT_TTW0	0b001000
#define ESR_EL2_FSC_ACCESS_FLAG_FAULT_TTW1	0b001001
#define ESR_EL2_FSC_ACCESS_FLAG_FAULT_TTW2	0b001010
#define ESR_EL2_FSC_ACCESS_FLAG_FAULT_TTW3	0b001011

#define ESR_EL2_FSC_PERM_FAULT_TTW1	0b001101
#define ESR_EL2_FSC_PERM_FAULT_TTW2	0b001110
#define ESR_EL2_FSC_PERM_FAULT_TTW3	0b001111

/*
 * We use the maximum immediate for a hvc call to indicate that the hvc from guest
 * is just a test for exiting to lowvisor and should return to guest immediately.
 */
#define ESR_EL2_ISS_DEBUG		0xFFFFU

typedef union {
	__u32 v;
	struct {
		__u32 iss : 25; /*  [0:24] */
		__u32 il  : 1;  /*    [25] */
		__u32 ec  : 6;  /* [26:31] */
	};
	ec_data_abort_t dabrt;
	ec_inst_abort_t iabrt;
	ec_msr_mrs_ex_t msrex;
	ec_mcrr_mrrc_t  mcrr;
	ec_mcr_mrc_t    mcr;
	ec_hvc_svc_t    hvc;
} esr_t;

/*
 * Calculate faulted guest PA (IPA)
 */
static inline __u64 vcpu_get_faulted_gpa(__u64 regs[])
{
	/* FIPA[47:12] == HPFAR_EL2[39:4] */
	return (regs[(unsigned int)VCPU_HPFAR_EL2] << 8U) |
	       (regs[(unsigned int)VCPU_FAR_EL2] & 0xfffU);
}

#endif
