/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 23 09:19:38 2018
 */
#ifndef AARCH64_HMASM_SYSREG_H
#define AARCH64_HMASM_SYSREG_H

#include <hmasm/esr.h>
#include <hmkernel/const.h>
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

#define SYS_MIDR_EL1			sys_reg(3, 0, 0, 0, 0)
#define SYS_MPIDR_EL1			sys_reg(3, 0, 0, 0, 5)
#define SYS_REVIDR_EL1			sys_reg(3, 0, 0, 0, 6)

/* Safe value for MPIDR_EL1: Bit31:RES1, Bit30:U:0, Bit24:MT:0 */
#define SYS_MPIDR_SAFE_VAL	(BIT(31))

#define ID_AA64_FILED_WITDH 			4

#define MIDR_IMPL_extract(x)			(((x) >> 24) & 0xffU)
#define MIDR_VARIANT_extract(x)			(((x) >> 20) & 0xfU)
#define MIDR_PARTNUM_extract(x)			(((x) >> 4) & 0xfffU)
#define MIDR_REVISION_extract(x)		((x) & 0xfU)

#define CPU_IMPL_ARM				0x41
#define CPU_IMPL_HUAWEI				0x48

#define CPU_PART_CORTEX_A53			0xD03
#define CPU_PART_CORTEX_A55			0xD05
#define CPU_PART_CORTEX_A510			0xD46

/* ID_AA64MMFR0_EL1 */
#define ID_AA64MMFR0_EL1_PARANGE_extract(x)	((x) & (0xfU))
#define ID_AA64MMFR0_EL1_ASIDRANGE_extract(x)	(((x) >> 4) & (0xfU))
#define ID_AA64MMFR0_EL1_TGRAN4_extract(x)	(((x) >> 28) & (0xfU))
#define ID_AA64MMFR0_EL1_TGRAN64_extract(x)	(((x) >> 24) & (0xfU))
#define ID_AA64MMFR0_EL1_TGRAN16_extract(x)	(((x) >> 20) & (0xfU))

/* ID_AA64ISAR0_EL1 */
#define ID_AA64ISAR0_EL1_AES_OFFSET		4

/* ID_AA64PFR0_EL1 */
#define ID_AA64PFR0_EL1_RAS_SHIFT		28
#define ID_AA64PFR0_EL1_RAS_MASK		(UL(0xf) << ID_AA64PFR0_EL1_RAS_SHIFT)
#define ID_AA64PFR0_EL1_FP_OFFSET		16
#define ID_AA64PFR0_EL1_ASIMD_OFFSET		20

/* ID_AA64PFR1_EL1 */
#define ID_AA64PFR1_SSBS_SHIFT			4
#define ID_AA64PFR1_SSBS_PSTATE_NI		0
#define ID_AA64PFR1_SSBS_PSTATE_ONLY		1
#define ID_AA64PFR1_SSBS_PSTATE_INSNS		2

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

#define ID_AA64PFR0_EL0_MASK 0xFUL

#define ID_AA64PFR0_ELx_64BIT_ONLY 0x1
#define ID_AA64PFR0_ELx_32BIT_64BIT 0x2

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

#define __emit_inst(x) ".inst " __stringify((x)) "\n\t"

#define pstate_field(op1, op2)		((op1) << Op1_shift | (op2) << Op2_shift)
#define PSTATE_Imm_shift		CRm_shift

#define PSTATE_PAN			pstate_field(0, 4)
#define PSTATE_UAO			pstate_field(0, 3)
#define PSTATE_SSBS			pstate_field(3, 1)

#define SET_PSTATE_SSBS(x)	__emit_inst(0xd500401f | PSTATE_SSBS | ((!!x) << PSTATE_Imm_shift))

#define PSTATE_N_SHIFT		31
#define PSTATE_Z_SHIFT		30
#define PSTATE_C_SHIFT		29
#define PSTATE_V_SHIFT		28
#define PSTATE_D_SHIFT		9
#define PSTATE_A_SHIFT		8
#define PSTATE_I_SHIFT		7
#define PSTATE_F_SHIFT		6
#define PSTATE_SPSel_SHIFT	0

#endif
