/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for bpf macro definition
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 10 09:31:16 CST 2020
 */

#ifndef _BPF_H_
#define _BPF_H_

enum {
	UX_BPF_REG_0 = 0,
	UX_BPF_REG_1,
	UX_BPF_REG_2,
	UX_BPF_REG_3,
	UX_BPF_REG_4,
	UX_BPF_REG_5,
	UX_BPF_REG_6,
	UX_BPF_REG_7,
	UX_BPF_REG_8,
	UX_BPF_REG_9,
	UX_BPF_REG_10,
	UX_MAX_BPF_REG,
};

#define UX_BPF_REG_A	UX_BPF_REG_0
#define UX_BPF_REG_X	UX_BPF_REG_7

#define UX_BPF_MAX_CODE 4096

#define UX_BPF_SHIFT_VALUE 32
#define UX_BPF_BYTE 1
#define UX_BPF_HALF 2
#define UX_BPF_WORD 4
#define UX_BPF_LEN_SCALE 4

/* below are used for bpf classic */
#define		UX_BPF_LD		0x00
#define		UX_BPF_LDX		0x01
#define		UX_BPF_ST		0x02
#define		UX_BPF_STX		0x03
#define		UX_BPF_ALU		0x04
#define		UX_BPF_JMP		0x05
#define		UX_BPF_RET		0x06
#define		UX_BPF_MISC		0x07

#define		UX_BPF_W		0x00
#define		UX_BPF_H		0x08
#define		UX_BPF_B		0x10

#define		UX_BPF_IMM		0x00
#define		UX_BPF_ABS		0x20
#define		UX_BPF_IND		0x40
#define		UX_BPF_MEM		0x60
#define		UX_BPF_LEN		0x80
#define		UX_BPF_MSH		0xa0

#define		UX_BPF_ADD		0x00
#define		UX_BPF_SUB		0x10
#define		UX_BPF_MUL		0x20
#define		UX_BPF_DIV		0x30
#define		UX_BPF_OR		0x40
#define		UX_BPF_AND		0x50
#define		UX_BPF_LSH		0x60
#define		UX_BPF_RSH		0x70
#define		UX_BPF_NEG		0x80
#define		UX_BPF_MOD		0x90
#define		UX_BPF_XOR		0xa0

#define		UX_BPF_JA		0x00
#define		UX_BPF_JEQ		0x10
#define		UX_BPF_JGT		0x20
#define		UX_BPF_JGE		0x30
#define		UX_BPF_JSET		0x40

#define		UX_BPF_K		0x00
#define		UX_BPF_X		0x08
#define         UX_BPF_A		0x10

/* below are macros for cBPF */
#define UX_BPF_OP_ADD_IMM  (UX_BPF_ALU | UX_BPF_ADD  | UX_BPF_K)
#define UX_BPF_OP_ADD_REG  (UX_BPF_ALU | UX_BPF_ADD  | UX_BPF_X)
#define UX_BPF_OP_SUB_IMM  (UX_BPF_ALU | UX_BPF_SUB  | UX_BPF_K)
#define UX_BPF_OP_SUB_REG  (UX_BPF_ALU | UX_BPF_SUB  | UX_BPF_X)
#define UX_BPF_OP_MUL_IMM  (UX_BPF_ALU | UX_BPF_MUL  | UX_BPF_K)
#define UX_BPF_OP_MUL_REG  (UX_BPF_ALU | UX_BPF_MUL  | UX_BPF_X)
#define UX_BPF_OP_DIV_IMM  (UX_BPF_ALU | UX_BPF_DIV  | UX_BPF_K)
#define UX_BPF_OP_DIV_REG  (UX_BPF_ALU | UX_BPF_DIV  | UX_BPF_X)
#define UX_BPF_OP_OR_IMM   (UX_BPF_ALU | UX_BPF_OR   | UX_BPF_K)
#define UX_BPF_OP_OR_REG   (UX_BPF_ALU | UX_BPF_OR   | UX_BPF_X)
#define UX_BPF_OP_AND_IMM  (UX_BPF_ALU | UX_BPF_AND  | UX_BPF_K)
#define UX_BPF_OP_AND_REG  (UX_BPF_ALU | UX_BPF_AND  | UX_BPF_X)
#define UX_BPF_OP_LSH_IMM  (UX_BPF_ALU | UX_BPF_LSH  | UX_BPF_K)
#define UX_BPF_OP_LSH_REG  (UX_BPF_ALU | UX_BPF_LSH  | UX_BPF_X)
#define UX_BPF_OP_RSH_IMM  (UX_BPF_ALU | UX_BPF_RSH  | UX_BPF_K)
#define UX_BPF_OP_RSH_REG  (UX_BPF_ALU | UX_BPF_RSH  | UX_BPF_X)
#define UX_BPF_OP_NEG      (UX_BPF_ALU | UX_BPF_NEG)
#define UX_BPF_OP_MOD_IMM  (UX_BPF_ALU | UX_BPF_MOD  | UX_BPF_K)
#define UX_BPF_OP_MOD_REG  (UX_BPF_ALU | UX_BPF_MOD  | UX_BPF_X)
#define UX_BPF_OP_XOR_IMM  (UX_BPF_ALU | UX_BPF_XOR  | UX_BPF_K)
#define UX_BPF_OP_XOR_REG  (UX_BPF_ALU | UX_BPF_XOR  | UX_BPF_X)

#define UX_BPF_OP_LD_ABSW  (UX_BPF_LD  | UX_BPF_W    | UX_BPF_ABS)
#define UX_BPF_OP_LD_ABSH  (UX_BPF_LD  | UX_BPF_H    | UX_BPF_ABS)
#define UX_BPF_OP_LD_ABSB  (UX_BPF_LD  | UX_BPF_B    | UX_BPF_ABS)
#define UX_BPF_OP_LD_INDW  (UX_BPF_LD  | UX_BPF_W    | UX_BPF_IND)
#define UX_BPF_OP_LD_INDH  (UX_BPF_LD  | UX_BPF_H    | UX_BPF_IND)
#define UX_BPF_OP_LD_INDB  (UX_BPF_LD  | UX_BPF_B    | UX_BPF_IND)

#define UX_BPF_OP_LDXW     (UX_BPF_LDX | UX_BPF_W    | UX_BPF_MEM)
#define UX_BPF_OP_LDXH     (UX_BPF_LDX | UX_BPF_H    | UX_BPF_MEM)
#define UX_BPF_OP_LDXB     (UX_BPF_LDX | UX_BPF_B    | UX_BPF_MEM)
#define UX_BPF_OP_LDXMSHB  (UX_BPF_LDX | UX_BPF_MSH  | UX_BPF_B)

#define UX_BPF_OP_STW      (UX_BPF_ST  | UX_BPF_W    | UX_BPF_MEM)
#define UX_BPF_OP_STH      (UX_BPF_ST  | UX_BPF_H    | UX_BPF_MEM)
#define UX_BPF_OP_STB      (UX_BPF_ST  | UX_BPF_B    | UX_BPF_MEM)

#define UX_BPF_OP_STXW  (UX_BPF_STX    | UX_BPF_W    | UX_BPF_MEM)
#define UX_BPF_OP_STXH  (UX_BPF_STX    | UX_BPF_H    | UX_BPF_MEM)
#define UX_BPF_OP_STXB  (UX_BPF_STX    | UX_BPF_B    | UX_BPF_MEM)

#define UX_BPF_OP_JA       (UX_BPF_JMP | UX_BPF_JA)
#define UX_BPF_OP_JEQ_IMM  (UX_BPF_JMP | UX_BPF_JEQ  | UX_BPF_K)
#define UX_BPF_OP_JEQ_REG  (UX_BPF_JMP | UX_BPF_JEQ  | UX_BPF_X)
#define UX_BPF_OP_JGT_IMM  (UX_BPF_JMP | UX_BPF_JGT  | UX_BPF_K)
#define UX_BPF_OP_JGT_REG  (UX_BPF_JMP | UX_BPF_JGT  | UX_BPF_X)
#define UX_BPF_OP_JGE_IMM  (UX_BPF_JMP | UX_BPF_JGE  | UX_BPF_K)
#define UX_BPF_OP_JGE_REG  (UX_BPF_JMP | UX_BPF_JGE  | UX_BPF_X)
#define UX_BPF_OP_JSET_IMM (UX_BPF_JMP | UX_BPF_JSET | UX_BPF_K)
#define UX_BPF_OP_JSET_REG (UX_BPF_JMP | UX_BPF_JSET | UX_BPF_X)

#define UX_BPF_OP_RET_K (UX_BPF_RET    | UX_BPF_K)
#define UX_BPF_OP_RET_A (UX_BPF_RET    | UX_BPF_A)

#endif /* _BPF_H_ */
