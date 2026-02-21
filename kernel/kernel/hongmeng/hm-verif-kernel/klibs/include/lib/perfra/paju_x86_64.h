/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: The Patch-Jump infrastructure
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 29 06:23:51 2021
 */
#ifndef KLIBS_PERFRA_PAJU_X86_64_H
#define KLIBS_PERFRA_PAJU_X86_64_H

#include <lib/perfra/paju_utils.h>

#define __PAJU_X86_64_INSN_SIZE			4

/* x86_64 nop */
#define __paju_x86_64_asm_nop(point_succ) 	"nop 0(%%eax,%%eax)"
#define __PAJU_X86_64_ASM_PTR_SIZE		8

#define __PAJU_X86_64_JUMP_SIZE			5U
#define __PAJU_X86_64_JUMP_OPCODE		0xe9
#define __PAJU_X86_64_JUMP_IMM_RANGE		(((__s64)1) << (32 - 1))
#define __PAJU_X86_64_JUMP_IMM_MIN		((__s64)(-__PAJU_X86_64_JUMP_IMM_RANGE))
#define __PAJU_X86_64_JUMP_IMM_MAX		((__s64)(__PAJU_X86_64_JUMP_IMM_RANGE))
static inline void __paju_x86_64_create_insn(
	void *buf,
	__u8 opcode,
	__u32 data)
{
	__u8 *buf_ins = (__u8 *)buf;
	*(__u8 *)(&buf_ins[0]) = opcode;
	*(__u32 *)(&buf_ins[1]) = data;
}

static inline __bool __paju_x86_64_check_buf(
	const void *buf,
	__u64 bufsize)
{
	return (buf != __PAJU_NULL) && (bufsize >= __PAJU_X86_64_JUMP_SIZE);
}

static inline __bool __paju_x86_64_check_jump(
	__s64 diff)
{
	return (diff >= __PAJU_X86_64_JUMP_IMM_MIN) && (diff < __PAJU_X86_64_JUMP_IMM_MAX);
}

static inline void __paju_x86_64_create_jump_imm(
	void *buf,
	__u32 imm,
	__bool is_le __attribute__((unused)))
{
	__paju_x86_64_create_insn(buf, __PAJU_X86_64_JUMP_OPCODE, imm);
}

static inline int __paju_x86_64_create_jump(
	void *buf,
	__u64 bufsize,
	__u64 dst,
	__u64 src,
	__bool is_le)
{
	int err = E_HM_INVAL;
	__s64 diff = (__s64)(dst - src);
	if (__paju_x86_64_check_buf(buf, (__u32)bufsize) && (__paju_x86_64_check_jump(diff))) {
		__paju_x86_64_create_jump_imm(buf, (__u32)((__s32)diff - __PAJU_X86_64_JUMP_SIZE), is_le);
		err = E_HM_OK;
	}
	return err;
}

#undef __PAJU_X86_64_JUMP_OPCODE
#undef __PAJU_X86_64_JUMP_IMM_RANGE
#undef __PAJU_X86_64_JUMP_IMM_MIN
#undef __PAJU_X86_64_JUMP_IMM_MAX

#define __PAJU_X86_64_NOP_OPCODE		0x0f
#define __PAJU_X86_64_NOP_DATA			0x0000441f
static inline int __paju_x86_64_create_nop(
	void *buf,
	__u64 bufsize)
{
	int err = E_HM_INVAL;
	if (__paju_x86_64_check_buf(buf, bufsize)) {
		__paju_x86_64_create_insn(buf, __PAJU_X86_64_NOP_OPCODE, __PAJU_X86_64_NOP_DATA);
		err = E_HM_OK;
	}
	return err;
}
#undef __PAJU_X86_64_NOP_OPCODE
#undef __PAJU_X86_64_NOP_DATA

#ifdef __x86_64__

/* The paju data structure of assembly code is similar. However, since they are separately developped, we don't forcely
 * extract a common pattern, so that modifying each one will not affect the other. */
#define __paju_x86_64_asm_group_data(group_name) \
	"\n\t.pushsection " __paju_group_datasec_name_str",\"awG\",@progbits," \
			    __paju_group_data_name_str(group_name) ",comdat" \
	"\n" __paju_group_data_name_str(group_name) ":" \
	"\n\t.8byte " __paju_pointsec_start_str(group_name) \
	"\n\t.8byte " __paju_pointsec_stop_str(group_name) \
	__paju_asm_size_weak_type(__paju_group_data_name_str(group_name), "@object") \
	"\n\t.popsection"
#define __paju_x86_64_asm_group(group_name) \
	"\n\t.ifndef " __paju_group_data_name_str(group_name) \
	__paju_x86_64_asm_group_data(group_name) \
	"\n\t.size " __paju_group_datasec_stop_str ", 64" \
	"\n\t.endif"

#define __paju_x86_64_asm_point_declare(group_name, point_name) \
	"\n\t.pushsection " __paju_pointsec_name_str(group_name) ",\"awM\",@progbits,1" \
	"\n\t.p2align 3" \
	"\n" __paju_name_str(group_name, point_name) ":" \
	__paju_asm_size_global_type(__paju_name_str(group_name, point_name), "@object", "16") \
	"\n\t.previous"
#define __paju_x86_64_asm_point_section(dst) \
	"\n5:" \
	"\n\t" __paju_x86_64_asm_nop(dst) \
	"\n\t.previous" \
	"\n6:" \
	"\n\t.8byte 5b, %l[" __paju_str(dst) "]" \
	"\n\t.popsection"

#ifndef __clang__
__attribute__((always_inline)) static inline
__bool __paju_x86_64_value_true(void)
{
	__paju_asm_goto(__paju_x86_64_asm_point_section(dst) :::: dst);
	__bool ret = __PAJU_VALUE_TRUE;
	for (;;) {
		break;
dst:		ret = __PAJU_VALUE_FALSE;
		break;
	}
	return ret;
}

#define __paju_x86_64_true(group_name, point_name) ({ \
		__paju_asm_volatile( \
			__paju_x86_64_asm_group(group_name) \
			__paju_x86_64_asm_point_declare(group_name, point_name)); \
		__paju_asm_volatile("" ::: "memory"); \
		__paju_x86_64_value_true(); \
	})
#else
#define __paju_x86_64_true(group_name, point_name) 	__PAJU_VALUE_TRUE
#endif

#define __paju_arch_true(group_name, point_name)	__paju_x86_64_true(group_name, point_name)

static inline int __arch_paju_create_jump(
	void *buf,
	__u64 bufsize,
	__u64 dst,
	__u64 src,
	__bool is_le)
{
	return __paju_x86_64_create_jump(buf, bufsize, dst, src, is_le);
}

static inline int __arch_paju_create_nop(
	void *buf,
	__u64 bufsize)
{
	return __paju_x86_64_create_nop(buf, bufsize);
}

#endif

#endif
