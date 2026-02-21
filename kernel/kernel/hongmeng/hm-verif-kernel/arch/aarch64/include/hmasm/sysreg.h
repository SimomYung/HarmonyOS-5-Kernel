/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 23 09:19:38 2018
 */
#ifndef AARCH64_HMASM_SYSREG_H
#define AARCH64_HMASM_SYSREG_H

#include <hmasm/esr.h>
#include <hmkernel/const.h>
#include <hmkernel/stringify.h>
#include <hmkernel/bitops/bits.h>
#include <hmasm/pac.h>

/*
 * ARMv8 reserves the following encoding for system registers:
 * System instruction class encoding overview
 *	[20-19] : OP0
 *	[18-16] : OP1
 *	[15-12] : CRN
 *	[11-8]  : CRM
 *	[7-5]   : OP2
 */
#define OP0_SHIFT	19
#define OP0_MASK	0x3
#define OP1_SHIFT	16
#define OP1_MASK	0x7
#define CRN_SHIFT	12
#define CRN_MASK	0xf
#define CRM_SHIFT	8
#define CRM_MASK	0xf
#define OP2_SHIFT	5
#define OP2_MASK	0x7

#define sys_reg_Op0(id)	(((id) >> OP0_SHIFT) & OP0_MASK)
#define sys_reg_Op1(id)	(((id) >> OP1_SHIFT) & OP1_MASK)
#define sys_reg_CRn(id)	(((id) >> CRN_SHIFT) & CRN_MASK)
#define sys_reg_CRm(id)	(((id) >> CRM_SHIFT) & CRM_MASK)
#define sys_reg_Op2(id)	(((id) >> OP2_SHIFT) & OP2_MASK)

#define sys_reg(op0, op1, crn, crm, op2) \
	(((op0) << OP0_SHIFT) | ((op1) << OP1_SHIFT) | \
	 ((crn) << CRN_SHIFT) | ((crm) << CRM_SHIFT) | \
	 ((op2) << OP2_SHIFT))

#define sys_insn	sys_reg

#ifdef __ASSEMBLY__
#define __emit_inst(x)			.inst(x)
#else
#define __emit_inst(x)			".inst " __stringify((x)) "\n\t"
#endif

#define SYS_MIDR_EL1			sys_reg(3, 0, 0, 0, 0)
#define SYS_MPIDR_EL1			sys_reg(3, 0, 0, 0, 5)
#define SYS_REVIDR_EL1			sys_reg(3, 0, 0, 0, 6)

#define SYS_ID_PFR2_EL1			sys_reg(3, 0, 0, 3, 4)
#define SYS_ID_PFR1_EL1			sys_reg(3, 0, 0, 1, 1)
#define SYS_ID_PFR0_EL1			sys_reg(3, 0, 0, 1, 0)

#define SYS_ID_DFR1_EL1			sys_reg(3, 0, 0, 3, 5)
#define SYS_ID_DFR0_EL1			sys_reg(3, 0, 0, 1, 2)

#define SYS_ID_MMFR5_EL1		sys_reg(3, 0, 0, 3, 6)
#define SYS_ID_MMFR4_EL1		sys_reg(3, 0, 0, 2, 6)
#define SYS_ID_MMFR3_EL1		sys_reg(3, 0, 0, 1, 7)
#define SYS_ID_MMFR2_EL1		sys_reg(3, 0, 0, 1, 6)
#define SYS_ID_MMFR1_EL1		sys_reg(3, 0, 0, 1, 5)
#define SYS_ID_MMFR0_EL1		sys_reg(3, 0, 0, 1, 4)

#define SYS_ID_ISAR6_EL1		sys_reg(3, 0, 0, 2, 7)
#define SYS_ID_ISAR5_EL1		sys_reg(3, 0, 0, 2, 5)
#define SYS_ID_ISAR4_EL1		sys_reg(3, 0, 0, 2, 4)
#define SYS_ID_ISAR3_EL1		sys_reg(3, 0, 0, 2, 3)
#define SYS_ID_ISAR2_EL1		sys_reg(3, 0, 0, 2, 2)
#define SYS_ID_ISAR1_EL1		sys_reg(3, 0, 0, 2, 1)
#define SYS_ID_ISAR0_EL1		sys_reg(3, 0, 0, 2, 0)

#define SYS_MVFR2_EL1			sys_reg(3, 0, 0, 3, 2)
#define SYS_MVFR1_EL1			sys_reg(3, 0, 0, 3, 1)
#define SYS_MVFR0_EL1			sys_reg(3, 0, 0, 3, 0)

#define SYS_ID_AA64PFR1_EL1		sys_reg(3, 0, 0, 4, 1)
#define SYS_ID_AA64PFR0_EL1		sys_reg(3, 0, 0, 4, 0)
#define SYS_ID_AA64ZFR0_EL1		sys_reg(3, 0, 0, 4, 4)

#define SYS_ID_AA64AFR1_EL1		sys_reg(3, 0, 0, 5, 5)
#define SYS_ID_AA64AFR0_EL1		sys_reg(3, 0, 0, 5, 4)
#define SYS_ID_AA64DFR1_EL1		sys_reg(3, 0, 0, 5, 1)
#define SYS_ID_AA64DFR0_EL1		sys_reg(3, 0, 0, 5, 0)

#define SYS_ID_AA64ISAR1_EL1		sys_reg(3, 0, 0, 6, 1)
#define SYS_ID_AA64ISAR0_EL1		sys_reg(3, 0, 0, 6, 0)

#define SYS_ID_AA64MMFR2_EL1		sys_reg(3, 0, 0, 7, 2)
#define SYS_ID_AA64MMFR1_EL1		sys_reg(3, 0, 0, 7, 1)
#define SYS_ID_AA64MMFR0_EL1		sys_reg(3, 0, 0, 7, 0)

#define SYS_ZCR_EL1			sys_reg(3, 0, 1, 2, 0)

#define SYS_CTR_EL0			sys_reg(3, 3, 0, 0, 1)
#define SYS_DCZID_EL0			sys_reg(3, 3, 0, 0, 7)

#define SYS_CNTFRQ_EL0			sys_reg(3, 3, 14, 0, 0)

/* Safe value for MPIDR_EL1: Bit31:RES1, Bit30:U:0, Bit24:MT:0 */
#define SYS_MPIDR_SAFE_VAL	(BIT(31))

#define ID_AA64_FILED_WITDH 			4

#define MIDR_IMPL_extract(x)			(((x) >> 24) & 0xffU)
#define MIDR_VARIANT_extract(x)			(((x) >> 20) & 0xfU)
#define MIDR_PARTNUM_extract(x)			(((x) >> 4) & 0xfffU)
#define MIDR_REVISION_extract(x)		((x) & 0xfU)

/* ID_AA64MMFR0_EL1 */
#define ID_AA64MMFR0_EL1_PARANGE_extract(x)	((x) & (0xfU))
#define ID_AA64MMFR0_EL1_ASIDRANGE_extract(x)	(((x) >> 4) & (0xfU))
#define ID_AA64MMFR0_EL1_TGRAN4_extract(x)	(((x) >> 28) & (0xfU))
#define ID_AA64MMFR0_EL1_TGRAN64_extract(x)	(((x) >> 24) & (0xfU))
#define ID_AA64MMFR0_EL1_TGRAN16_extract(x)	(((x) >> 20) & (0xfU))

/* ID_AA64ISAR0_EL1 */
#define ID_AA64ISAR0_EL1_AES_OFFSET		4

/* MPIDR */
#define MPIDR_HWID_BITMASK			UL(0xff00ffffff)

#define MPIDR_MT_extract(x)			(((x) >> 24U) & 0x1U)
#define MPIDR_AFF0_extract(x)			((x) & 0xffU)
#define MPIDR_AFF1_extract(x)			(((x) >> 8U) & 0xffU)
#define MPIDR_AFF2_extract(x)			(((x) >> 16U) & 0xffU)
#define MPIDR_AFF3_extract(x)			(((x) >> 32U) & 0xffU)
#define MPIDR_Affinity_Value_extract(x)		(((x) & 0xffffffUL) | \
						(((x) >> 8U) & 0xff000000UL))

/* SCTLR_ELx flags. */
#define SCTLR_ENALS	(BIT(56))
#define SCTLR_ENAS0	(BIT(55))
#define SCTLR_ENASR	(BIT(54))
#define SCTLR_DSSBS	(BIT(44))
#define SCTLR_BT1	(BIT(36))
#define SCTLR_BT0	(BIT(35))
#define SCTLR_UCI	(BIT(26))
#define SCTLR_EE	(BIT(25))
#define SCTLR_E0E	(BIT(24))
#define SCTLR_SPAN	(BIT(23))
#define SCTLR_IESB	(BIT(21))
#define SCTLR_WXN	(BIT(19))
#define SCTLR_nTWE	(BIT(18))
#define SCTLR_nTWI	(BIT(16))
#define SCTLR_UCT	(BIT(15))
#define SCTLR_DZE	(BIT(14))
#define SCTLR_I		(BIT(12))
#define SCTLR_CP15BEN	(BIT(5))
#define SCTLR_SA0	(BIT(4))
#define SCTLR_SA	(BIT(3))
#define SCTLR_C		(BIT(2))
#define SCTLR_A		(BIT(1))
#define SCTLR_M		(BIT(0))

#if defined(CONFIG_SOFT_REBOOT_CORE) || defined(CONFIG_CPU_PARK)
/* softreboot cleanup registers */
#define SOFTREBOOT_SCTLR_FLAGS (SCTLR_M | SCTLR_A |SCTLR_C | SCTLR_SA | SCTLR_SA0 | SCTLR_I | SCTLR_IESB)
#endif

/* TCR flags */
#define TCR_T0SZ(x)	(UL(64) - (x))
#define TCR_T1SZ(x)	((UL(64) - (x)) << 16)
#define TCR_TxSZ(x)	(TCR_T0SZ(x) | TCR_T1SZ(x))

#define TCR_T0SZ_MASK	0xff

#define TCR_EPD0(x)	(UL(x) << 7)

#define TCR_IRGN0(x)	(UL(x) << 8)
#define TCR_IRGN0_NC	TCR_IRGN0(0)
#define TCR_IRGN0_WBWA	TCR_IRGN0(1)
#define TCR_IRGN0_WT	TCR_IRGN0(2)
#define TCR_IRGN0_WBnWA	TCR_IRGN0(3)

#define TCR_ORGN0(x)	(UL(x) << 10)
#define TCR_ORGN0_NC    TCR_ORGN0(0)
#define TCR_ORGN0_WBWA  TCR_ORGN0(1)
#define TCR_ORGN0_WT    TCR_ORGN0(2)
#define TCR_ORGN0_WBnWA TCR_ORGN0(3)

#define TCR_SH0(x)	(UL(x) << 12)
#define TCR_SH0_INNER	TCR_SH0(3)
#define TCR_SH0_OUTER	TCR_SH0(2)

#define TCR_TG0(x)	(UL(x) << 14)
#define TCR_TG0_4K	TCR_TG0(0)
#define TCR_TG0_64K	TCR_TG0(1)
#define TCR_TG0_16K	TCR_TG0(2)

#define TCR_A1(x)	(UL(x) << 22)

#define TCR_EPD1(x)	(UL(x) << 23)

#define TCR_IRGN1(x)	(UL(x) << 24)
#define TCR_IRGN1_NC	TCR_IRGN1(0)
#define TCR_IRGN1_WBWA	TCR_IRGN1(1)
#define TCR_IRGN1_WT	TCR_IRGN1(2)
#define TCR_IRGN1_WBnWA	TCR_IRGN1(3)

#define TCR_ORGN1(x)	(UL(x) << 26)
#define TCR_ORGN1_NC    TCR_ORGN1(0)
#define TCR_ORGN1_WBWA  TCR_ORGN1(1)
#define TCR_ORGN1_WT    TCR_ORGN1(2)
#define TCR_ORGN1_WBnWA TCR_ORGN1(3)

#define TCR_SH1(x)	(UL(x) << 28)
#define TCR_SH1_INNER	TCR_SH1(3)
#define TCR_SH1_OUTER	TCR_SH1(2)

#define TCR_TG1(x)	(UL(x) << 30)
#define TCR_TG1_4K	TCR_TG1(2)
#define TCR_TG1_64K	TCR_TG1(3)
#define TCR_TG1_16K	TCR_TG1(1)

#define TCR_IPS(x)	(UL(x) << 32)
#define TCR_IPS_4G	TCR_IPS(0)
#define TCR_IPS_64G	TCR_IPS(1)
#define TCR_IPS_1T	TCR_IPS(2)
#define TCR_IPS_4T	TCR_IPS(3)
#define TCR_IPS_16T	TCR_IPS(4)
#define TCR_IPS_256T	TCR_IPS(5)
#define TCR_IPS_4P	TCR_IPS(6)

#define TCR_AS(x)	(UL(x) << 36)
#define TCR_TBI0(x)	(UL(x) << 37)
#define TCR_TBI1(x)	(UL(x) << 38)
#define TCR_HA(x)	(UL(x) << 39)
#define TCR_HD(x)	(UL(x) << 40)
#define TCR_HPD0(x)	(UL(x) << 41)
#define TCR_HPD1(x)	(UL(x) << 42)
#define TCR_HWU059(x)	(UL(x) << 43)
#define TCR_HWU060(x)	(UL(x) << 44)
#define TCR_HWU061(x)	(UL(x) << 45)
#define TCR_HWU062(x)	(UL(x) << 46)
#define TCR_HWU159(x)	(UL(x) << 47)
#define TCR_HWU160(x)	(UL(x) << 48)
#define TCR_HWU161(x)	(UL(x) << 49)
#define TCR_HWU162(x)	(UL(x) << 50)
#define TCR_TPID0(x)	(UL(x) << 51)
#define TCR_TPID1(x)	(UL(x) << 52)
#define TCR_NFD0(x)	(UL(x) << 53)
#define TCR_NFD1(x)	(UL(x) << 54)
#define TCR_E0PD0(x)	(UL(x) << 55)
#define TCR_E0PD1(x)	(UL(x) << 56)

#define MAIR(attr, mt)	((attr) << ((mt) * 8))

/* Data abort from EL0 to EL1 */
#define DFSR_ALIGNMENT_FAULT	0x21	// Data Alignment fault from EL0 to EL1

/* CTR_EL0 */
#define CTR_EL0_L1IP_SHIFT	14U
#define CTR_EL0_L1IP_EXTRACT(x)	(((x) & 0xC000ULL) >> CTR_EL0_L1IP_SHIFT)
#define CTR_EL0_L1IP_VPIPT	0U
#define CTR_EL0_L1IP_AIVIVT	1U
#define CTR_EL0_L1IP_VIPT	2U
#define CTR_EL0_L1IP_PIPT	3U
#define CTR_EL0_IDC		(BIT(28))
#define CTR_EL0_DIC		(BIT(29))

/* CLIDR */
#define CLIDR_LOUIS_SHIFT	21U
#define CLIDR_LOUIS_EXTRACT(x)	(((x) >> CLIDR_LOUIS_SHIFT) & 0x7)
#define CLIDR_LOC_SHIFT		24U
#define CLIDR_LOC_EXTRACT(x)	(((x) >> CLIDR_LOC_SHIFT) & 0x7)
#define CLIDR_LOUU_SHIFT	27U
#define CLIDR_LOUU_EXTRACT(x)	(((x) >> CLIDR_LOUU_SHIFT) & 0x7)

/* CPU features register */
#define AARCH64_HAFDBS_FEATURE_MASK	((u64)0xf << 0)  /* register: ID_AA64MMFR1_EL1 */
#define AARCH64_HPDS_FEATURE_MASK	((u64)0xf << 12)  /* register: ID_AA64MMFR1_EL1 */
#define AARCH64_PAN_FEATURE_MASK 	((u64)0xf << 20) /* register: ID_AA64MMFR1_EL1 */
#define AARCH64_UAO_FEATURE_MASK 	((u64)0xf << 4)  /* register: ID_AA64MMFR2_EL1 */
#define AARCH64_E0PD_FEATURE_MASK 	((u64)0xf << 60)  /* register: ID_AA64MMFR2_EL1 */

#define CURRENTEL_EL_extract(x)	(((x) >> 2) & (0x3UL))
#define CURRENTEL_EL_EL0	0UL
#define CURRENTEL_EL_EL1	1UL
#define CURRENTEL_EL_EL2	2UL
#define CURRENTEL_EL_EL3	3UL

#define HCR_FMO		BIT(3)
#define HCR_RW		BIT(31)

/* ID_PFR0_EL1, AArch32 Processor Feature Register 0 */
#define ID_PFR0_STATE0_SHIFT		0
#define ID_PFR0_STATE1_SHIFT		4
#define ID_PFR0_STATE2_SHIFT		8
#define ID_PFR0_STATE3_SHIFT		12
#define ID_PFR0_CSV2_SHIFT		16
#define ID_PFR0_DIT_SHIFT		24

/* ID_PFR1_EL1, AArch32 Processor Feature Register 1 */
#define ID_PFR1_PROGMOD_SHIFT		0
#define ID_PFR1_SECURITY_SHIFT		4
#define ID_PFR1_MPROGMOD_SHIFT		8
#define ID_PFR1_VIRTUALIZATION_SHIFT	12
#define ID_PFR1_GENTIMER_SHIFT		16
#define ID_PFR1_SEC_FRAC_SHIFT		20
#define ID_PFR1_VIRT_FRAC_SHIFT		24
#define ID_PFR1_GIC_SHIFT		28

/* ID_PFR2_EL1, AArch32 Processor Feature Register 2 */
#define ID_PFR2_CSV3_SHIFT		0
#define ID_PFR2_SSBS_SHIFT		4

/* ID_DFR0_EL1, AArch32 Debug Feature Register 0 */
#define ID_DFR0_COPDBG_SHIFT		0
#define ID_DFR0_COPSDBG_SHIFT		4
#define ID_DFR0_MMAPDBG_SHIFT		8
#define ID_DFR0_COPTRC_SHIFT		12
#define ID_DFR0_MMAPTRC_SHIFT		16
#define ID_DFR0_MPROFDBG_SHIFT		20
#define ID_DFR0_PERFMON_SHIFT		24

/* ID_DFR1_EL1, AArch32 Debug Feature Register 1 */
#define ID_DFR1_MTPMU_SHIFT		0

/* ID_MMFR0_EL1, AArch32 Memory Model Feature Register 0 */
#define ID_MMFR0_VMSA_SHIFT		0
#define ID_MMFR0_PMSA_SHIFT		4
#define ID_MMFR0_OUTERSHR_SHIFT		8
#define ID_MMFR0_SHARELVL_SHIFT		12
#define ID_MMFR0_TCM_SHIFT		16
#define ID_MMFR0_AUXREG_SHIFT		20
#define ID_MMFR0_FCSE_SHIFT		24
#define ID_MMFR0_INNERSHR_SHIFT		28

/* ID_MMFR4_EL1, AArch32 Memory Model Feature Register 4 */
#define ID_MMFR4_SPECSEI_SHIFT		0
#define ID_MMFR4_AC2_SHIFT		4
#define ID_MMFR4_XNX_SHIFT		8
#define ID_MMFR4_CNP_SHIFT		12
#define ID_MMFR4_HPDS_SHIFT		16
#define ID_MMFR4_LSM_SHIFT		20
#define ID_MMFR4_CCIDX_SHIFT		24
#define ID_MMFR4_EVT_SHIFT		28

/* ID_MMFR5_EL1, AArch32 Memory Model Feature Register 5 */
#define ID_MMFR5_ETS_SHIFT		0

/* ID_ISAR0_EL1, AArch32 Instruction Set Attribute Register 0 */
#define ID_ISAR0_SWAP_SHIFT		0
#define ID_ISAR0_BITCOUNT_SHIFT		4
#define ID_ISAR0_BITFIELD_SHIFT		8
#define ID_ISAR0_CMPBRANCH_SHIFT	12
#define ID_ISAR0_COPROC_SHIFT		16
#define ID_ISAR0_DEBUG_SHIFT		20
#define ID_ISAR0_DIVIDE_SHIFT		24

/* ID_ISAR4_EL1, AArch32 Instruction Set Attribute Register 4 */
#define ID_ISAR4_UNPRIV_SHIFT		0
#define ID_ISAR4_WITHSHIFTS_SHIFT	4
#define ID_ISAR4_WRITEBACK_SHIFT	8
#define ID_ISAR4_SMC_SHIFT		12
#define ID_ISAR4_BARRIER_SHIFT		16
#define ID_ISAR4_SYNCH_PRIM_FRAC_SHIFT	20
#define ID_ISAR4_PSR_M_SHIFT		24
#define ID_ISAR4_SWP_FRAC_SHIFT		28

/* ID_ISAR5_EL1, AArch32 Instruction Set Attribute Register 5 */
#define ID_ISAR5_SEVL_SHIFT		0
#define ID_ISAR5_AES_SHIFT		4
#define ID_ISAR5_SHA1_SHIFT		8
#define ID_ISAR5_SHA2_SHIFT		12
#define ID_ISAR5_CRC32_SHIFT		16
#define ID_ISAR5_RDM_SHIFT		24

/* ID_ISAR6_EL1, AArch32 Instruction Set Attribute Register 6 */
#define ID_ISAR6_JSCVT_SHIFT		0
#define ID_ISAR6_DP_SHIFT		4
#define ID_ISAR6_FHM_SHIFT		8
#define ID_ISAR6_SB_SHIFT		12
#define ID_ISAR6_SPECRES_SHIFT		16
#define ID_ISAR6_BF16_SHIFT		20
#define ID_ISAR6_I8MM_SHIFT		24

/* MVFR2_EL1, AArch32 Media and VFP Feature Register 2 */
#define MVFR2_SIMDMISC_SHIFT		0
#define MVFR2_FPMISC_SHIFT		4

/* ID_AA64PFR0_EL1, AArch64 Processor Feature Register 0 */
#define ID_AA64PFR0_EL1_FP_OFFSET		16
#define ID_AA64PFR0_EL1_ASIMD_OFFSET		20
#define ID_AA64PFR0_EL1_RAS_SHIFT		28
#define ID_AA64PFR0_EL1_RAS_MASK		(UL(0xf) << ID_AA64PFR0_EL1_RAS_SHIFT)
#define ID_AA64PFR0_EL0_MASK 0xFUL
#define ID_AA64PFR0_ELx_64BIT_ONLY	0x1
#define ID_AA64PFR0_ELx_32BIT_64BIT	0x2

#define ID_AA64PFR0_EL0_SHIFT		0
#define ID_AA64PFR0_EL1_SHIFT		4
#define ID_AA64PFR0_EL2_SHIFT		8
#define ID_AA64PFR0_EL3_SHIFT		12
#define ID_AA64PFR0_FP_SHIFT		16
#define ID_AA64PFR0_ASIMD_SHIFT		20
#define ID_AA64PFR0_GIC_SHIFT		24
#define ID_AA64PFR0_RAS_SHIFT		28
#define ID_AA64PFR0_SVE_SHIFT		32
#define ID_AA64PFR0_SEL2_SHIFT		36
#define ID_AA64PFR0_MPAM_SHIFT		40
#define ID_AA64PFR0_AMU_SHIFT		44
#define ID_AA64PFR0_DIT_SHIFT		48
#define ID_AA64PFR0_CSV2_SHIFT		56
#define ID_AA64PFR0_CSV3_SHIFT		60

#define ID_AA64PFR0_EL0_64BIT_ONLY	0x1
#define ID_AA64PFR0_EL0_32BIT_64BIT	0x2
#define ID_AA64PFR0_EL1_64BIT_ONLY	0x1
#define ID_AA64PFR0_EL1_32BIT_64BIT	0x2
#define ID_AA64PFR0_FP_NI		0xf
#define ID_AA64PFR0_FP_SUPPORTED	0x0
#define ID_AA64PFR0_ASIMD_NI		0xf
#define ID_AA64PFR0_ASIMD_SUPPORTED	0x0
#define ID_AA64PFR0_RAS_V1		0x1
#define ID_AA64PFR0_SVE			0x1
#define ID_AA64PFR0_AMU			0x1

/* ID_AA64PFR1_EL1, AArch64 Processor Feature Register 1 */
#define ID_AA64PFR1_BT_SHIFT		0
#define ID_AA64PFR1_SSBS_SHIFT		4
#define ID_AA64PFR1_MTE_SHIFT		8
#define ID_AA64PFR1_RASFRAC_SHIFT	12
#define ID_AA64PFR1_MPAMFRAC_SHIFT	16

#define ID_AA64PFR1_BT_BTI		0x1

#define ID_AA64PFR1_SSBS_PSTATE_NI	0x0
#define ID_AA64PFR1_SSBS_PSTATE_ONLY	0x1
#define ID_AA64PFR1_SSBS_PSTATE_INSNS	0x2

#define ID_AA64PFR1_MTE_NI		0x0
#define ID_AA64PFR1_MTE_EL0		0x1
#define ID_AA64PFR1_MTE			0x2

/* ID_AA64ZFR0_EL1, SVE Feature ID Register 0 */
#define ID_AA64ZFR0_SVEVER_SHIFT	0
#define ID_AA64ZFR0_AES_SHIFT		4
#define ID_AA64ZFR0_BITPERM_SHIFT	16
#define ID_AA64ZFR0_BF16_SHIFT		20
#define ID_AA64ZFR0_SHA3_SHIFT		32
#define ID_AA64ZFR0_SM4_SHIFT		40
#define ID_AA64ZFR0_I8MM_SHIFT		44
#define ID_AA64ZFR0_F32MM_SHIFT		52
#define ID_AA64ZFR0_F64MM_SHIFT		56

#define ID_AA64ZFR0_SVEVER_SVE2		0x1
#define ID_AA64ZFR0_AES			0x1
#define ID_AA64ZFR0_AES_PMULL		0x2
#define ID_AA64ZFR0_BITPERM		0x1
#define ID_AA64ZFR0_BF16		0x1
#define ID_AA64ZFR0_SHA3		0x1
#define ID_AA64ZFR0_SM4			0x1
#define ID_AA64ZFR0_I8MM		0x1
#define ID_AA64ZFR0_F32MM		0x1
#define ID_AA64ZFR0_F64MM		0x1

/* ID_AA64DFR0_EL1, AArch64 Debug Feature Register 0 */
#define ID_AA64DFR0_DEBUGVER_SHIFT	0
#define ID_AA64DFR0_TRACEVER_SHIFT	4
#define ID_AA64DFR0_PMUVER_SHIFT	8
#define ID_AA64DFR0_BRPS_SHIFT		12
#define ID_AA64DFR0_WRPS_SHIFT		20
#define ID_AA64DFR0_CTX_CMPS_SHIFT	28
#define ID_AA64DFR0_PMSVER_SHIFT	32
#define ID_AA64DFR0_DOUBLELOCK_SHIFT	36

#define ID_AA64DFR0_DEBUGVER_V8		0x6

/* ID_AA64ISAR0_EL1, AArch64 Instruction Set Attribute Register 0 */
#define ID_AA64ISAR0_AES_SHIFT		4
#define ID_AA64ISAR0_SHA1_SHIFT		8
#define ID_AA64ISAR0_SHA2_SHIFT		12
#define ID_AA64ISAR0_CRC32_SHIFT	16
#define ID_AA64ISAR0_ATOMICS_SHIFT	20
#define ID_AA64ISAR0_RDM_SHIFT		28
#define ID_AA64ISAR0_SHA3_SHIFT		32
#define ID_AA64ISAR0_SM3_SHIFT		36
#define ID_AA64ISAR0_SM4_SHIFT		40
#define ID_AA64ISAR0_DP_SHIFT		44
#define ID_AA64ISAR0_FHM_SHIFT		48
#define ID_AA64ISAR0_TS_SHIFT		52
#define ID_AA64ISAR0_TLB_SHIFT		56
#define ID_AA64ISAR0_RNDR_SHIFT		60

#define ID_AA64ISAR0_TLB_RANGE_NI	0x0
#define ID_AA64ISAR0_TLB_RANGE		0x2

/* ID_AA64ISAR1_EL1, AArch64 Instruction Set Attribute Register 1 */
#define ID_AA64ISAR1_DPB_SHIFT		0
#define ID_AA64ISAR1_APA_SHIFT		4
#define ID_AA64ISAR1_API_SHIFT		8
#define ID_AA64ISAR1_JSCVT_SHIFT	12
#define ID_AA64ISAR1_FCMA_SHIFT		16
#define ID_AA64ISAR1_LRCPC_SHIFT	20
#define ID_AA64ISAR1_GPA_SHIFT		24
#define ID_AA64ISAR1_GPI_SHIFT		28
#define ID_AA64ISAR1_FRINTTS_SHIFT	32
#define ID_AA64ISAR1_SB_SHIFT		36
#define ID_AA64ISAR1_SPECRES_SHIFT	40
#define ID_AA64ISAR1_BF16_SHIFT		44
#define ID_AA64ISAR1_DGH_SHIFT		48
#define ID_AA64ISAR1_I8MM_SHIFT		52

#define ID_AA64ISAR1_APA_ARCH_EPAC2_FPAC_CMB	0x5
#define ID_AA64ISAR1_APA_ARCH_EPAC2_FPAC	0x4
#define ID_AA64ISAR1_APA_ARCH_EPAC2		0x3
#define ID_AA64ISAR1_APA_ARCH_EPAC		0x2
#define ID_AA64ISAR1_APA_ARCHITECTED		0x1
#define ID_AA64ISAR1_APA_NI			0x0

#define ID_AA64ISAR1_API_IMP_DEF_EPAC2_FPAC_CMB	0x5
#define ID_AA64ISAR1_API_IMP_DEF_EPAC2_FPAC	0x4
#define ID_AA64ISAR1_API_IMP_DEF_EPAC2		0x3
#define ID_AA64ISAR1_API_IMP_DEF_EPAC		0x2
#define ID_AA64ISAR1_API_IMP_DEF		0x1
#define ID_AA64ISAR1_API_NI			0x0

#define ID_AA64ISAR1_GPA_ARCHITECTED		0x1
#define ID_AA64ISAR1_GPA_NI			0x0

#define ID_AA64ISAR1_GPI_IMP_DEF		0x1
#define ID_AA64ISAR1_GPI_NI			0x0

/* ID_AA64MMFR0_EL1, AArch64 Memory Model Feature Register 0 */
#define ID_AA64MMFR0_PARANGE_SHIFT	0
#define ID_AA64MMFR0_ASID_SHIFT		4
#define ID_AA64MMFR0_BIGENDEL_SHIFT	8
#define ID_AA64MMFR0_SNSMEM_SHIFT	12
#define ID_AA64MMFR0_BIGENDEL0_SHIFT	16
#define ID_AA64MMFR0_TGRAN16_SHIFT	20
#define ID_AA64MMFR0_TGRAN64_SHIFT	24
#define ID_AA64MMFR0_TGRAN4_SHIFT	28
#define ID_AA64MMFR0_TGRAN16_2_SHIFT	32
#define ID_AA64MMFR0_TGRAN64_2_SHIFT	36
#define ID_AA64MMFR0_TGRAN4_2_SHIFT	40
#define ID_AA64MMFR0_EXS_SHIFT		44
#define ID_AA64MMFR0_FGT_SHIFT		56
#define ID_AA64MMFR0_ECV_SHIFT		60

#define ID_AA64MMFR0_PARANGE_48		0x5
#define ID_AA64MMFR0_PARANGE_52		0x6

#define ID_AA64MMFR0_TGRAN16_NI		0x0
#define ID_AA64MMFR0_TGRAN16_SUPPORTED	0x1
#define ID_AA64MMFR0_TGRAN64_NI		0xf
#define ID_AA64MMFR0_TGRAN64_SUPPORTED	0x0
#define ID_AA64MMFR0_TGRAN4_NI		0xf
#define ID_AA64MMFR0_TGRAN4_SUPPORTED	0x0

#define ID_AA64MMFR0_TGRAN16_2_NI	0x1
#define ID_AA64MMFR0_TGRAN64_2_NI	0x1
#define ID_AA64MMFR0_TGRAN4_2_NI	0x1

/* ID_AA64MMFR1_EL1, AArch64 Memory Model Feature Register 1 */
#define ID_AA64MMFR1_HADBS_SHIFT	0
#define ID_AA64MMFR1_VMIDBITS_SHIFT	4
#define ID_AA64MMFR1_VHE_SHIFT		8
#define ID_AA64MMFR1_HPD_SHIFT		12
#define ID_AA64MMFR1_LOR_SHIFT		16
#define ID_AA64MMFR1_PAN_SHIFT		20
#define ID_AA64MMFR1_SPECSEI_SHIFT	24
#define ID_AA64MMFR1_XNX_SHIFT		28
#define ID_AA64MMFR1_TWED_SHIFT		32
#define ID_AA64MMFR1_ETS_SHIFT		36
#define ID_AA64MMFR1_AFP_SHIFT		44

#define ID_AA64MMFR1_VMIDBITS_8		0x0
#define ID_AA64MMFR1_VMIDBITS_16	0x2

/* ID_AA64MMFR2_EL1, AArch64 Memory Model Feature Register 2 */
#define ID_AA64MMFR2_CNP_SHIFT		0
#define ID_AA64MMFR2_UAO_SHIFT		4
#define ID_AA64MMFR2_LSM_SHIFT		8
#define ID_AA64MMFR2_IESB_SHIFT		12
#define ID_AA64MMFR2_LVA_SHIFT		16
#define ID_AA64MMFR2_CCIDX_SHIFT	20
#define ID_AA64MMFR2_NV_SHIFT		24
#define ID_AA64MMFR2_ST_SHIFT		28
#define ID_AA64MMFR2_AT_SHIFT		32
#define ID_AA64MMFR2_IDS_SHIFT		36
#define ID_AA64MMFR2_FWB_SHIFT		40
#define ID_AA64MMFR2_TTL_SHIFT		48
#define ID_AA64MMFR2_BBM_SHIFT		52
#define ID_AA64MMFR2_EVT_SHIFT		56
#define ID_AA64MMFR2_E0PD_SHIFT		60

/*
 * ZCR_ELx, SVE Control Register (ELx)
 * Intentionally include bits [8:4] which* are reserved by the SVE architecture
 * for future expansion of the LEN field, with compatible semantics.
 */
#define ZCR_ELx_LEN_SHIFT	0
#define ZCR_ELx_LEN_SIZE	9
#define ZCR_ELx_LEN_MASK	0x1ff

/* enable EL1 access */
#define CPACR_EL1_ZEN_EL1EN	(BIT(16))
#define CPACR_EL1_FPEN_EL1EN	(BIT(20))

/* CTR_EL0, Cache Type Register */
#define CTR_IMINLINE_SHIFT	0
#define CTR_L1IP_SHIFT		14
#define CTR_DMINLINE_SHIFT	16
#define CTR_ERG_SHIFT		20
#define CTR_CWG_SHIFT		24
#define CTR_IDC_SHIFT		28
#define CTR_DIC_SHIFT		29

#define CTR_IMINLINE_MASK	0xf
#define CTR_L1IP_MASK		0x3
#define CTR_CWG_MASK		0xf

#define ICACHE_POLICY_VIPT	0x2

/* DCZID_EL0, Data Cache Zero ID register */
#define DCZID_BS_SHIFT			0
#define DCZID_DZP_SHIFT			4

#define Op0_shift	19
#define Op0_mask	0x3
#define Op1_shift	16
#define Op1_mask	0x7
#define CRn_shift	12
#define CRn_mask	0xf
#define CRm_shift	8
#define CRm_mask	0xf
#define Op2_shift	5
#define Op2_mask	0x7

#define pstate_field(op1, op2)		((op1) << Op1_shift | (op2) << Op2_shift)
#define PSTATE_Imm_shift		CRm_shift

#define PSTATE_PAN			pstate_field(0, 4)
#define PSTATE_UAO			pstate_field(0, 3)
#define PSTATE_SSBS			pstate_field(3, 1)

#define SET_PSTATE_SSBS(x)	__emit_inst(0xd500401f | PSTATE_SSBS | ((!!x) << PSTATE_Imm_shift))

/* PSTATE register */
#define PSTATE_N_SHIFT		31
#define PSTATE_Z_SHIFT		30
#define PSTATE_C_SHIFT		29
#define PSTATE_V_SHIFT		28
#define PSTATE_D_SHIFT		9
#define PSTATE_A_SHIFT		8
#define PSTATE_I_SHIFT		7
#define PSTATE_F_SHIFT		6
#define PSTATE_SPSel_SHIFT	0

#ifndef __HOST_LLT__
#ifdef __ASSEMBLY__

	.irp num,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
	.equ .L__reg_n_x\num, \num
	.endr
	.equ .L__reg_n_xzr, 31

	.macro msr_s, sysreg, rt
	__emit_inst(0xd5000000|(\sysreg)|(.L__reg_n_\rt))
	.endm

	.macro mrs_s, rt, sysreg
	 __emit_inst(0xd5200000|(\sysreg)|(.L__reg_n_\rt))
	.endm

#else

/* define the register number as the local lable */
#define __DEFINE_MRS_MSR_S_REGNUM				\
"	.irp num,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30\n" \
"	.equ .L__reg_n_x\\num, \\num\n"				\
"	.endr\n"						\
"	.equ .L__reg_n_xzr, 31\n"

/* to make it work with clang and LTO */
#define DEFINE_MRS_S						\
	__DEFINE_MRS_MSR_S_REGNUM				\
"	.macro mrs_s, rt, sysreg\n"				\
	__emit_inst(0xd5200000|(\\sysreg)|(.L__reg_n_\\rt))	\
"	.endm\n"

#define DEFINE_MSR_S						\
	__DEFINE_MRS_MSR_S_REGNUM				\
"	.macro msr_s, sysreg, rt\n"				\
	__emit_inst(0xd5000000|(\\sysreg)|(.L__reg_n_\\rt))	\
"	.endm\n"

/* undefine the macro, leave the indent */
#define UNDEF_MRS_S "		.purgem mrs_s\n"
#define UNDEF_MSR_S "		.purgem msr_s\n"

#define __mrs_s(v, r)						\
	DEFINE_MRS_S						\
"	mrs_s " v ", " __stringify(r) "\n"			\
	UNDEF_MRS_S

#define __msr_s(r, v)						\
	DEFINE_MSR_S						\
"	msr_s " __stringify(r) ", " v "\n"			\
	UNDEF_MSR_S

/* For registers without architectural names, or simply unsupported by GAS */
#define read_sysreg_s(r) ({					\
	u64 __v;						\
	asm volatile(__mrs_s("%0", r) : "=r" (__v));		\
	__v;							\
})

#define write_sysreg_s(r, v) do {				\
	u64 __v = (u64)(v);					\
	asm volatile(__msr_s(r, "%x0") : : "rZ" (__v));		\
} while (0)

/*
 * Unlike read_cpuid which has attribute const, read_sysreg are never expected
 * to be optimized away or replaced with synthetic values.
 */
#define read_sysreg(r) ({					\
	u64 __v;						\
	asm volatile("mrs %0, " r : "=r" (__v));		\
	__v;							\
})

/*
 * https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html
 * Z: Integer constant zero
 * x: restricted to registers 0 to 15 inclusive.
 *
 * the "%x0" template means XZR.
 */
#define write_sysreg(r, v) do {					\
	u64 __v = (u64)(v);					\
	asm volatile("msr " r ", %x0" : : "rZ" (__v));		\
} while (0)

#endif /* __ASSEMBLY__ */

#endif /* !__HOST_LLT__ */

#endif
