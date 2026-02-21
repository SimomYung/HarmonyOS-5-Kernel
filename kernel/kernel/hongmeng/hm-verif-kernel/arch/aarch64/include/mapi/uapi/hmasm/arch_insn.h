/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 20 14:58:00 2019
 */

#ifndef ARCH_AARCH64_INCLUDE_MAPI_UAPI_ASM_ARCH_INSN_H
#define ARCH_AARCH64_INCLUDE_MAPI_UAPI_ASM_ARCH_INSN_H

#include <hmasm/types.h>

#define reg_code(rx)		(((__u32)(rx)) & 0x1FU)

#define ARCH_INSN_SIZE		4

#define DEF_INSN_IS(mnemonic, mask, code)		\
static inline int insn_is_##mnemonic(__u32 insn)	\
{ return (insn & ((__u32)(mask))) == (__u32)(code); }

DEF_INSN_IS(adr,	0x9F000000, 0x10000000)
DEF_INSN_IS(adrp,	0x9F000000, 0x90000000)
DEF_INSN_IS(adr_adrp,	0x1F000000, 0x10000000)
DEF_INSN_IS(b,		0xFC000000, 0x14000000)
DEF_INSN_IS(bl,		0xFC000000, 0x94000000)
DEF_INSN_IS(b_bl,	0x7C000000, 0x14000000)
DEF_INSN_IS(br,		0xFFFFFC1F, 0xD61F0000)
DEF_INSN_IS(blr,	0xFFFFFC1F, 0xD63F0000)
DEF_INSN_IS(br_blr,	0xFFDFFC1F, 0xD61F0000)
DEF_INSN_IS(bcond,	0xFF000010, 0x54000000)
DEF_INSN_IS(cbz,	0x7F000000, 0x34000000)
DEF_INSN_IS(cbnz,	0x7F000000, 0x35000000)
DEF_INSN_IS(tbz,	0x7F000000, 0x36000000)
DEF_INSN_IS(tbnz,	0x7F000000, 0x37000000)
DEF_INSN_IS(cb_tb,	0x7C000000, 0x34000000)
DEF_INSN_IS(ret,	0xFFFFFC1F, 0xD65F0000)
/*
 * PC-relative:
 * LDR (literal) 32-bit variant: 00011000
 * LDR (literal) 64-bit variant: 01011000
 * LDRSW (literal):		 10011000
 * PRFM (literal):		 11011000
 */
DEF_INSN_IS(pc_relative, 0x3F000000, 0x18000000)

#undef DEF_INSN_IS

#define itou32(n)		((__u32)(n))

static inline __u32 decode_adr_adrp_rd(__u32 insn)
{
	return insn & itou32(0x1F);
}

/* ADR */
/* sign extend to be implemented */
static inline __u32 encode_adr_insn(__u32 rd, __u64 imm)
{
	return (itou32(0x10) << itou32(24)) |
		((imm & itou32(0x3)) << itou32(29)) |
		((imm & itou32(0x1FFFFC)) << itou32(3)) |
		(reg_code(rd));
}

/* BR */
static inline __u32 encode_br_insn(__u32 rn)
{
	return itou32(0xD61F0000) | (reg_code(rn) << itou32(5));
}

/* ADD (shifted register), shift amount = 0 */
static inline __u32 encode_add_insn(__u32 rd, __u32 rn, __u32 rm)
{
	return (itou32(0x8B) << itou32(24)) |
		(reg_code(rm) << itou32(16)) |
		(reg_code(rn) << itou32(5)) |
		(reg_code(rd));
}

/* SUB (shifted register), shift amount = 0 */
static inline __u32 encode_sub_insn(__u32 rd, __u32 rn, __u32 rm)
{
	return (itou32(0xCB) << itou32(24)) |
		(reg_code(rm) << itou32(16)) |
		(reg_code(rn) << itou32(5)) |
		(reg_code(rd));
}

/* MOVZ, LSL shift = 0, 16, 32, 48 */
static inline __u32 encode_movz_insn(__u32 rd, __u32 imm, __u32 shift)
{
	return (itou32(0xD280) << itou32(16)) |
		(((shift / itou32(16)) & itou32(0x3)) << itou32(21)) |
		((imm & itou32(0xFFFF)) << itou32(5)) |
		(reg_code(rd));
}

/* MOVK, LSL shift = 0, 16, 32, 48 */
static inline __u32 encode_movk_insn(__u32 rd, __u32 imm, __u32 shift)
{
	return (itou32(0xF280) << itou32(16)) |
		(((shift / itou32(16)) & itou32(0x3)) << itou32(21)) |
		((imm & itou32(0xFFFF)) << itou32(5)) |
		(reg_code(rd));
}

#undef itou32

/* make instructions to MOV a 48 bits immediate to a register */
__u32 make_mov_imm48_insn(__u32 *result, __u64 imm, __u32 rd);

/*
 * make instructions to jump to the destination address.
 * maximum supported virtual address size is 48 bits.
 */
__u32 make_br_imm48_insn(__u32 *result, __u64 dest, __u32 rm);

/* make instructions to ADD a 48 bits immediate to a register */
__u32 make_add_imm48_insn(__u32 *result, __u64 imm, __u32 rm, __u32 rd);

/* make instructions to SUB a 48 bits immediate from a register */
__u32 make_sub_imm48_insn(__u32 *result, __u64 imm, __u32 rm, __u32 rd);

#endif
