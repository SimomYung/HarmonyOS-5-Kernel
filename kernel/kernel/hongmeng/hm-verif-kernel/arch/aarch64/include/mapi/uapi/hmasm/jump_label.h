/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Header of jump label
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 01 09:59:52 2023
 */
#ifndef AARCH64_MAPI_UAPI_ASM_JUMP_LABEL_H
#define AARCH64_MAPI_UAPI_ASM_JUMP_LABEL_H

#include <hmasm/types.h>
#include <hmkernel/strict.h>

struct static_key;
#if defined(__HOST_LLT__) || defined(__HOST_UT__) || (defined(__clang__) && __clang_major__ < 9)
static inline __bool arch_static_branch(struct static_key * const key)
{
	UNUSED(key);

	return __false;
}
#else
/* This function adds the triplet (code, jump, key) to the __jump_table section.
 * The code indicates the address of the instruction.
 * When the branch is enabled, the insn is changed to the b command.
 * When the branch is disabled, the insn is changed to the nop command.
 * The jump indicates the address of the jump.
 * The key is used to distinguish different branches.
 */
static inline __bool arch_static_branch(struct static_key * const key)
{
	asm goto ("1: nop	\n\t"
		  ".pushsection __jump_table,  \"aw\" \n\t"
		  ".quad 1b, %l[l_yes], %c0 \n\t"
		  ".popsection \n\t"
		  :
		  : "S"(key)
		  :
		  : l_yes);
	return __false;
l_yes:
	return __true;
}
#endif /* __HOST_LLT__ || __HOST_UT__ */
#endif /* AARCH64_MAPI_UAPI_ASM_JUMP_LABEL_H */
