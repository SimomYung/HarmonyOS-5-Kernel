/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Header for ESR
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 27 14:19:38 2022
 */

#ifndef AARCH64_HMASM_ESR_H
#define AARCH64_HMASM_ESR_H

#include <hmkernel/const.h>

#define ESR_ELx_EC_SHIFT	26
#define ESR_ELx_EC_UNKNOWN	0x00	// Unknown reason
#define ESR_ELx_EC_WFx		0x01	// WFE or WFI execution
/* Reserved EC: 0x02 */
#define ESR_ELx_EC_RESERVED_02	0x02
#define ESR_ELx_EC_CP15_32	0x03	// Access to cp15 by MCR or MRC
#define ESR_ELx_EC_CP15_64	0x04	// Access to cp15 by MCRR or MRRC
#define ESR_ELx_EC_CP14_32	0x05	// Access to cp14 by MCR or MRC
#define ESR_ELx_EC_CP14_LS	0x06	// Access to cp14 by LDC or STC
#define ESR_ELx_EC_FP_SIMD	0x07	// Access to Advanced SIMD or floating-point registers
#define ESR_ELx_EC_CP10_ID	0x08	// VMRS access
#define ESR_ELx_EC_PAC		0x09	// Pointer Authentication instruction
/* Reserved EC: 0x0A - 0x0B */
#define ESR_ELx_EC_RESERVED_0A	0x0A
#define ESR_ELx_EC_RESERVED_0B	0x0B
#define ESR_ELx_EC_CP14_64	0x0C	// Access to cp14 by MCRR or MRRC
#define ESR_ELx_EC_BTI		0x0D	// Branch Target Identification instruction
#define ESR_ELx_EC_ILL		0x0E	// Illegal Execution state
/* Reserved EC: 0x0F - 0x10 */
#define ESR_ELx_EC_RESERVED_0F	0x0F
#define ESR_ELx_EC_RESERVED_10	0x10
#define ESR_ELx_EC_SVC32	0x11	// SVC instruction execution in AArch32 state
#define ESR_ELx_EC_HVC32	0x12	// HVC instruction execution in AArch32 state
#define ESR_ELx_EC_SMC32	0x13	// SMC instruction execution in AArch32 state
/* Reserved EC: 0x14 */
#define ESR_ELx_EC_RESERVED_14	0x14
#define ESR_ELx_EC_SVC64	0x15	// SVC instruction execution in AArch64 state
#define ESR_ELx_EC_HVC64	0x16	// HVC instruction execution in AArch64 state
#define ESR_ELx_EC_SMC64	0x17	// SMC instruction execution in AArch64 state
#define ESR_ELx_EC_SYS64	0x18	// MSR, MRS instruction abort or System instruction execution
#define ESR_ELx_EC_SVE		0x19	// Trapped access to SVE functionality
#define ESR_ELx_EC_ERET		0x1A	// Trapped ERET , ERETAA or ERETAB instruction execution
/* Reserved EC: 0x1B */
#define ESR_ELx_EC_RESERVED_1B	0x1B
#define ESR_ELx_EC_FPAC		0x1C	// Pointer authentication instruction authentication failure
#define ESR_ELx_EC_SME		0x1D	// Scalable Matrix Extension
/* Reserved EC: 0x1E */
#define ESR_ELx_EC_RESERVED_1E	0x1E
#define ESR_ELx_EC_IMP_DEF	0x1F	// IMPLEMENTATION DEFINED exception taken to EL3
#define ESR_ELx_EC_IABT_LOW	0x20	// Instruction abort from lower exception level
#define ESR_ELx_EC_IABT_CUR	0x21	// Instruction abort from current exception level
#define ESR_ELx_EC_PC_ALIGN	0x22	// PC alignment fault
/* Reserved EC: 0x23 */
#define ESR_ELx_EC_RESERVED_23	0x23
#define ESR_ELx_EC_DABT_LOW	0x24	// Data abort from lower exception level
#define ESR_ELx_EC_DABT_CUR	0x25	// Data abort from current exception level
#define ESR_ELx_EC_SP_ALIGN	0x26	// SP alignment fault
/* Reserved EC: 0x27 */
#define ESR_ELx_EC_RESERVED_27	0x27
#define ESR_ELx_EC_FPE32	0x28	// Trapped floating-point exception taken from AArch32 state
/* Reserved EC: 0x29 - 0x2B */
#define ESR_ELx_EC_RESERVED_29	0x29
#define ESR_ELx_EC_RESERVED_2A	0x2A
#define ESR_ELx_EC_RESERVED_2B	0x2B
#define ESR_ELx_EC_FPE64	0x2C	// Trapped floating-point exception taken from AArch64 state
/* Reserved EC: 0x2D - 0x2E */
#define ESR_ELx_EC_RESERVED_2D	0x2D
#define ESR_ELx_EC_RESERVED_2E	0x2E
#define ESR_ELx_EC_SERROR	0x2F	// SError
#define ESR_ELx_EC_BREAKPT_LOW	0x30	// Breakpoint exception from lower Exception level
#define ESR_ELx_EC_BREAKPT_CUR	0x31	// Breakpoint exception from current Exception level
#define ESR_ELx_EC_SS_LOW	0x32	// Software step debug exception from lower exception level
#define ESR_ELx_EC_SOFTSTP_CUR	0x33	// Software step debug exception from current exception level
#define ESR_ELx_EC_WATCH_LOW	0x34	// Watchpoint exception from lower exception level
#define ESR_ELx_EC_WATCH_CUR	0x35	// Watchpoint exception from current exception level
/* Reserved EC: 0x36 - 0x37 */
#define ESR_ELx_EC_RESERVED_36	0x36
#define ESR_ELx_EC_RESERVED_37	0x37
#define ESR_ELx_EC_BKPT32	0x38	// BKPT instruction execution in AArch32 state
/* Reserved EC: 0x39 */
#define ESR_ELx_EC_RESERVED_39	0x39
#define ESR_ELx_EC_VECTOR32	0x3A	// Vector Catch exception from AArch32 state
/* Reserved EC: 0x3B */
#define ESR_ELx_EC_RESERVED_3B	0x3B
#define ESR_ELx_EC_BRK64	0x3C	// BRK instruction execution in AArch64 state
/* Reserved EC: 0x3D - 0x3F */
#define ESR_ELx_EC_RESERVED_3D	0x3D
#define ESR_ELx_EC_RESERVED_3E	0x3E
#define ESR_ELx_EC_RESERVED_3F	0x3F
#define ESR_ELx_EC_MAX		0x3F

#define ESR_ELx_EC_extract(x)	(((x) >> 26) & 0x3fU)
#define ESR_ELx_IL_extract(x)	(((x) >> 25) & 1U)
#define ESR_ELx_ISS_extract(x)	((x) & 0xffffffU)
#define ESR_ELx_WnR_extract(x)	(((x) >> 6) & 1U)
#define ESR_ELx_DFSC_extract(x)	((x) & 0x3fU)

/* Asynchronous Error Type */
#define ESR_ELx_IDS_SHIFT	24
#define ESR_ELx_IDS_MASK	(UL(1) << ESR_ELx_IDS_SHIFT)
#define ESR_ELx_AET_SHIFT	10
#define ESR_ELx_AET_MASK	(UL(0x7) << ESR_ELx_AET_SHIFT)
#define ESR_ELx_AET(esr)	(((esr) & ESR_ELx_AET_MASK) >> ESR_ELx_AET_SHIFT)

#define ESR_ELx_AET_UC		(0x0)
#define ESR_ELx_AET_UEU		(0x1)
#define ESR_ELx_AET_UEO		(0x2)
#define ESR_ELx_AET_UER		(0x3)
#define ESR_ELx_AET_CE		(0x6)

/* Shared ISS fault status code(IFSC/DFSC) */
#define ESR_ELx_FSC		(0x3F)
#define ESR_ELx_FSC_EXTABT	(0x10)
#define ESR_ELx_FSC_SERROR	(0x11)

/* ISS encoding for an exception from MSR, MRS, or System instruction execution in AArch64 state */
#define ESR_ELx_SYS64_ISS_RES0_SHIFT	22
#define ESR_ELx_SYS64_ISS_RES0_MASK	(UL(0x7) << ESR_ELx_SYS64_ISS_RES0_SHIFT)
#define ESR_ELx_SYS64_ISS_DIR_MASK	0x1
#define ESR_ELx_SYS64_ISS_DIR_READ	0x1
#define ESR_ELx_SYS64_ISS_DIR_WRITE	0x0

#define ESR_ELx_SYS64_ISS_OP0_SHIFT	20
#define ESR_ELx_SYS64_ISS_OP0_MASK	(UL(0x3) << ESR_ELx_SYS64_ISS_OP0_SHIFT)
#define ESR_ELx_SYS64_ISS_OP1_SHIFT	14
#define ESR_ELx_SYS64_ISS_OP1_MASK	(UL(0x7) << ESR_ELx_SYS64_ISS_OP1_SHIFT)
#define ESR_ELx_SYS64_ISS_OP2_SHIFT	17
#define ESR_ELx_SYS64_ISS_OP2_MASK	(UL(0x7) << ESR_ELx_SYS64_ISS_OP2_SHIFT)
#define ESR_ELx_SYS64_ISS_CRN_SHIFT	10
#define ESR_ELx_SYS64_ISS_CRN_MASK	(UL(0xf) << ESR_ELx_SYS64_ISS_CRN_SHIFT)
#define ESR_ELx_SYS64_ISS_CRM_SHIFT	1
#define ESR_ELx_SYS64_ISS_CRM_MASK	(UL(0xf) << ESR_ELx_SYS64_ISS_CRM_SHIFT)
#define ESR_ELx_SYS64_ISS_RT_SHIFT	5
#define ESR_ELx_SYS64_ISS_RT_MASK	(UL(0x1f) << ESR_ELx_SYS64_ISS_RT_SHIFT)

#define ESR_ELx_SYS64_ISS_SYS_MASK	(ESR_ELx_SYS64_ISS_OP0_MASK | \
					 ESR_ELx_SYS64_ISS_OP1_MASK | \
					 ESR_ELx_SYS64_ISS_OP2_MASK | \
					 ESR_ELx_SYS64_ISS_CRN_MASK | \
					 ESR_ELx_SYS64_ISS_CRM_MASK)
#define ESR_ELx_SYS64_ISS_SYS_VAL(_op0, _op1, _op2, _crn, _crm) \
					(((_op0) << ESR_ELx_SYS64_ISS_OP0_SHIFT) | \
					 ((_op1) << ESR_ELx_SYS64_ISS_OP1_SHIFT) | \
					 ((_op2) << ESR_ELx_SYS64_ISS_OP2_SHIFT) | \
					 ((_crn) << ESR_ELx_SYS64_ISS_CRN_SHIFT) | \
					 ((_crm) << ESR_ELx_SYS64_ISS_CRM_SHIFT))

#define ESR_ELx_SYS64_ISS_SYS_OP_MASK	(ESR_ELx_SYS64_ISS_SYS_MASK | \
					 ESR_ELx_SYS64_ISS_DIR_MASK)

#define ESR_ELx_SYS64_ISS_RT(esr) \
	(((esr) & ESR_ELx_SYS64_ISS_RT_MASK) >> ESR_ELx_SYS64_ISS_RT_SHIFT)

/*
 * EL0 mrs operations which are supported for emulation have the following
 * sysreg encoding in System instructions.
 * op0 = 3, op1= 0, crn = 0, {crm = 0, 4-7}, READ (L = 1)
 */
#define ESR_ELx_SYS64_ISS_SYS_MRS_OP_MASK	(ESR_ELx_SYS64_ISS_OP0_MASK | \
						 ESR_ELx_SYS64_ISS_OP1_MASK | \
						 ESR_ELx_SYS64_ISS_CRN_MASK | \
						 ESR_ELx_SYS64_ISS_DIR_MASK)
#define ESR_ELx_SYS64_ISS_SYS_MRS_OP_VAL \
				(ESR_ELx_SYS64_ISS_SYS_VAL(3, 0, 0, 0, 0) | \
				 ESR_ELx_SYS64_ISS_DIR_READ)

/* From ESR to sysreg ecode */
#define esr_sys64_to_sysreg(esr)				\
	sys_reg((((esr) & ESR_ELx_SYS64_ISS_OP0_MASK) >> ESR_ELx_SYS64_ISS_OP0_SHIFT), \
		(((esr) & ESR_ELx_SYS64_ISS_OP1_MASK) >> ESR_ELx_SYS64_ISS_OP1_SHIFT), \
		(((esr) & ESR_ELx_SYS64_ISS_CRN_MASK) >> ESR_ELx_SYS64_ISS_CRN_SHIFT), \
		(((esr) & ESR_ELx_SYS64_ISS_CRM_MASK) >> ESR_ELx_SYS64_ISS_CRM_SHIFT), \
		(((esr) & ESR_ELx_SYS64_ISS_OP2_MASK) >> ESR_ELx_SYS64_ISS_OP2_SHIFT))

#endif /* !AARCH64_HMASM_ESR_H */
