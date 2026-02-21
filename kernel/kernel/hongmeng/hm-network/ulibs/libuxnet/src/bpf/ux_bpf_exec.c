/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for network bpf exec
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 10 09:31:16 CST 2020
 */

#include "bpf/ux_bpf_filter.h"
#include "bpf/ux_bpf_def.h"
#include <string.h>
#include <arpa/inet.h>


static inline bool
ux_data_copy_bits(const void *data, void *dataptr, u16_t len, u16_t offset, int data_len)
{
	if (len > (u16_t)data_len || offset > (u16_t)data_len || ((u16_t)data_len - offset) < len) {
		return false;
	}
	if (memcpy_s((char*)dataptr, len, &((char*)data)[offset], len) == 0) {
		return true;
	} else {
		return false;
	}
}

static unsigned int ux_bpf_classic_exec(const void *ctx, const struct ux_sock_filter *insn, int data_len)
{
	u64_t reg[UX_MAX_BPF_REG] = {0};
	u16_t pc = 0;
	u32_t tmp1;
	u16_t tmp2;
	u8_t  tmp3;

	if (insn == NULL) {
		return 0;
	}

	reg[UX_BPF_REG_A] = 0;
	reg[UX_BPF_REG_X] = 0;

	while (1) {
		const u16_t cur_pc = pc;
		struct ux_sock_filter inst = insn[pc++];
		switch (inst.code) {
		case UX_BPF_OP_ADD_IMM:
			reg[UX_BPF_REG_A] += inst.k;
			break;
		case UX_BPF_OP_ADD_REG:
			reg[UX_BPF_REG_A] += reg[UX_BPF_REG_X];
			break;
		case UX_BPF_OP_SUB_IMM:
			reg[UX_BPF_REG_A] -= inst.k;
			break;
		case UX_BPF_OP_SUB_REG:
			reg[UX_BPF_REG_A] -= reg[UX_BPF_REG_X];
			break;
		case UX_BPF_OP_MUL_IMM:
			reg[UX_BPF_REG_A] *= inst.k;
			break;
		case UX_BPF_OP_MUL_REG:
			reg[UX_BPF_REG_A] *= reg[UX_BPF_REG_X];
			break;
		case UX_BPF_OP_DIV_IMM:
			if (inst.k == 0) {
				net_error(HMNET_BPF, "UX_BPF_EXEC ERROR: division by zero at %hu\n", cur_pc);
				return 0;
			}
			reg[UX_BPF_REG_A] = (u32_t)(reg[UX_BPF_REG_A]) / (u32_t)(inst.k);
			break;
		case UX_BPF_OP_DIV_REG:
			if (reg[UX_BPF_REG_X] == 0) {
				net_error(HMNET_BPF, "UX_BPF_EXEC ERROR: division by zero at %hu\n", cur_pc);
				return 0;
			}
			reg[UX_BPF_REG_A] = (u32_t)(reg[UX_BPF_REG_A]) / (u32_t)(reg[UX_BPF_REG_X]);
			break;
		case UX_BPF_OP_OR_IMM:
			reg[UX_BPF_REG_A] |= inst.k;
			break;
		case UX_BPF_OP_OR_REG:
			reg[UX_BPF_REG_A] |= reg[UX_BPF_REG_X];
			break;
		case UX_BPF_OP_AND_IMM:
			reg[UX_BPF_REG_A] &= inst.k;
			break;
		case UX_BPF_OP_AND_REG:
			reg[UX_BPF_REG_A] &= reg[UX_BPF_REG_X];
			break;
		case UX_BPF_OP_LSH_IMM:
			reg[UX_BPF_REG_A] <<= inst.k;
			break;
		case UX_BPF_OP_LSH_REG:
			reg[UX_BPF_REG_A] <<= reg[UX_BPF_REG_X];
			break;
		case UX_BPF_OP_RSH_IMM:
			reg[UX_BPF_REG_A] = (u32_t)(reg[UX_BPF_REG_A]) >> inst.k;
			break;
		case UX_BPF_OP_RSH_REG:
			reg[UX_BPF_REG_A] = (u32_t)(reg[UX_BPF_REG_A]) >> reg[UX_BPF_REG_X];
			break;
		case UX_BPF_OP_NEG:
			reg[UX_BPF_REG_A] = (u32_t)(s32_t)-((s64_t)reg[UX_BPF_REG_A]);
			break;
		case UX_BPF_OP_MOD_IMM:
			if (inst.k == 0) {
				net_error(HMNET_BPF, "UX_BPF_EXEC ERROR: division by zero at %hu\n", cur_pc);
				return 0;
			}
			reg[UX_BPF_REG_A] = (u32_t)(reg[UX_BPF_REG_A]) % (u32_t)(inst.k);
			break;
		case UX_BPF_OP_MOD_REG:
			if (reg[UX_BPF_REG_X] == 0) {
				net_error(HMNET_BPF, "UX_BPF_EXEC ERROR: mod with zero at %hu\n", cur_pc);
				return 0;
			}
			reg[UX_BPF_REG_A] = (u32_t)(reg[UX_BPF_REG_A]) % (u32_t)(reg[UX_BPF_REG_X]);
			break;
		case UX_BPF_OP_XOR_IMM:
			reg[UX_BPF_REG_A] ^= inst.k;
			break;
		case UX_BPF_OP_XOR_REG:
			reg[UX_BPF_REG_A] ^= reg[UX_BPF_REG_X];
			break;

		case UX_BPF_OP_LD_ABSW:
			if (ux_data_copy_bits(ctx, &tmp1, UX_BPF_WORD, (u16_t)(inst.k), data_len)) {
				reg[UX_BPF_REG_A] = ntohl(tmp1);
				break;
			}
			return 0;
		case UX_BPF_OP_LD_ABSH:
			if (ux_data_copy_bits(ctx, &tmp2, UX_BPF_HALF, (u16_t)(inst.k), data_len)) {
				reg[UX_BPF_REG_A] = ntohs(tmp2);
				break;
			}
			return 0;

		case UX_BPF_OP_LD_ABSB:
			if (ux_data_copy_bits(ctx, &tmp3, UX_BPF_BYTE, (u16_t)(inst.k), data_len)) {
				reg[UX_BPF_REG_A] = tmp3;
				break;
			}
			return 0;

		case UX_BPF_OP_LD_INDW:
			if (ux_data_copy_bits(ctx, &tmp1, UX_BPF_WORD, (u16_t)(reg[UX_BPF_REG_X] + inst.k), data_len)) {
				reg[UX_BPF_REG_A] = ntohl(tmp1);
				break;
			}
			return 0;
		case UX_BPF_OP_LD_INDH:
			if (ux_data_copy_bits(ctx, &tmp2, UX_BPF_HALF, (u16_t)(reg[UX_BPF_REG_X] + inst.k), data_len)) {
				reg[UX_BPF_REG_A] = ntohs(tmp2);
				break;
			}
			return 0;
		case UX_BPF_OP_LD_INDB:
			if (ux_data_copy_bits(ctx, &tmp3, UX_BPF_BYTE, (u16_t)(reg[UX_BPF_REG_X] + inst.k), data_len)) {
				reg[UX_BPF_REG_A] = tmp3;
				break;
			}
			return 0;

		case UX_BPF_OP_LDXMSHB:
			if (ux_data_copy_bits(ctx, &tmp3, UX_BPF_BYTE, (u16_t)(inst.k), data_len)) {
				reg[UX_BPF_REG_X] = UX_BPF_LEN_SCALE * (tmp3 & 0xf);
				break;
			}
			return 0;

		case UX_BPF_OP_JA:
			pc += (u16_t)(inst.k + 1);
			break;
		case UX_BPF_OP_JEQ_IMM:
			if (reg[UX_BPF_REG_A] == inst.k) {
				pc += inst.jt;
			} else {
				pc += inst.jf;
			}
			break;
		case UX_BPF_OP_JEQ_REG:
			if (reg[UX_BPF_REG_A] == reg[UX_BPF_REG_X]) {
				pc += inst.jt;
			} else {
				pc += inst.jf;
			}
			break;
		case UX_BPF_OP_JGT_IMM:
			if (reg[UX_BPF_REG_A] > (uint32_t)inst.k) {
				pc += inst.jt;
			} else {
				pc += inst.jf;
			}
			break;
		case UX_BPF_OP_JGT_REG:
			if (reg[UX_BPF_REG_A] > reg[UX_BPF_REG_X]) {
				pc += inst.jt;
			} else {
				pc += inst.jf;
			}
			break;
		case UX_BPF_OP_JGE_IMM:
			if (reg[UX_BPF_REG_A] >= (uint32_t)inst.k) {
				pc += inst.jt;
			} else {
				pc += inst.jf;
			}
			break;
		case UX_BPF_OP_JGE_REG:
			if (reg[UX_BPF_REG_A] >= reg[UX_BPF_REG_X]) {
				pc += inst.jt;
			} else {
				pc += inst.jf;
			}
			break;
		case UX_BPF_OP_JSET_IMM:
			if ((reg[UX_BPF_REG_A] & inst.k) != 0U) {
				pc += inst.jt;
			} else {
				pc += inst.jf;
			}
			break;
		case UX_BPF_OP_JSET_REG:
			if ((reg[UX_BPF_REG_A] & reg[UX_BPF_REG_X]) != 0U) {
				pc += inst.jt;
			} else {
				pc += inst.jf;
			}
			break;
		case UX_BPF_OP_RET_K:
			return inst.k;
		default:
			net_error(HMNET_BPF, "unsupport BPF command: 0x%hx\n", inst.code);
			return 0;
		}
	}
}

void ux_bpf_func_set(struct ux_bpf_prog *fp)
{
	fp->ux_bpf_func = ux_bpf_classic_exec;
}
