/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for network bpf filter
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 10 09:31:16 CST 2020
 */

#include <errno.h>
#include <libhmactv/actv.h>
#include "bpf/ux_bpf_def.h"
#include "bpf/ux_bpf_common.h"
#include "bpf/ux_bpf_filter.h"
#include <stdio.h>
#include <unistd.h>

static unsigned char ux_opcode[] = {
	/* ALU opcode */
	[UX_BPF_OP_ADD_IMM] = 1,
	[UX_BPF_OP_ADD_REG] = 1,
	[UX_BPF_OP_SUB_IMM] = 1,
	[UX_BPF_OP_SUB_REG] = 1,
	[UX_BPF_OP_MUL_IMM] = 1,
	[UX_BPF_OP_MUL_REG] = 1,
	[UX_BPF_OP_DIV_IMM] = 1,
	[UX_BPF_OP_DIV_REG] = 1,
	[UX_BPF_OP_MOD_IMM] = 1,
	[UX_BPF_OP_MOD_REG] = 1,
	[UX_BPF_OP_AND_IMM] = 1,
	[UX_BPF_OP_AND_REG] = 1,
	[UX_BPF_OP_OR_IMM] = 1,
	[UX_BPF_OP_OR_REG] = 1,
	[UX_BPF_OP_XOR_IMM] = 1,
	[UX_BPF_OP_XOR_REG] = 1,
	[UX_BPF_OP_LSH_IMM] = 1,
	[UX_BPF_OP_LSH_REG] = 1,
	[UX_BPF_OP_RSH_IMM] = 1,
	[UX_BPF_OP_RSH_REG] = 1,
	[UX_BPF_OP_NEG] = 1,
	/* load and store opcode */
	[UX_BPF_OP_LD_ABSW] = 1,
	[UX_BPF_OP_LD_ABSH] = 1,
	[UX_BPF_OP_LD_ABSB] = 1,
	[UX_BPF_OP_LD_INDW] = 1,
	[UX_BPF_OP_LD_INDH] = 1,
	[UX_BPF_OP_LD_INDB] = 1,
	[UX_BPF_OP_LDXW] = 1,
	[UX_BPF_OP_LDXH] = 1,
	[UX_BPF_OP_LDXB] = 1,
	[UX_BPF_OP_LDXMSHB] = 1,
	[UX_BPF_OP_STW] = 1,
	[UX_BPF_OP_STH] = 1,
	[UX_BPF_OP_STB] = 1,
	[UX_BPF_OP_STXW] = 1,
	[UX_BPF_OP_STXH] = 1,
	[UX_BPF_OP_STXB] = 1,
	/* exit opcode */
	[UX_BPF_OP_RET_K] = 1,
	[UX_BPF_OP_RET_A] = 1,
	/* jump opcode */
	[UX_BPF_OP_JA] = 1,
	[UX_BPF_OP_JEQ_IMM] = 1,
	[UX_BPF_OP_JEQ_REG] = 1,
	[UX_BPF_OP_JGE_IMM] = 1,
	[UX_BPF_OP_JGE_REG] = 1,
	[UX_BPF_OP_JGT_IMM] = 1,
	[UX_BPF_OP_JGT_REG] = 1,
	[UX_BPF_OP_JSET_IMM] = 1,
	[UX_BPF_OP_JSET_REG] = 1,
};

struct ux_bpf_prog *ux_bpf_prog_alloc(unsigned int size)
{
	struct ux_bpf_prog *fp = NULL;

	if (size < sizeof(struct ux_bpf_prog)) {
		return NULL;
	}

	fp = malloc(size);
	if (fp == NULL) {
		return NULL;
	}

	return fp;
}

static void ux_bpf_prog_release(struct ux_bpf_prog *prog)
{
	free(prog);
}

static int ux_bpf_check_basic(const struct ux_sock_filter *filter,
			      unsigned int flen)
{
	if (filter == NULL) {
		return 0;
	}
	if (flen == 0 || flen > UX_BPF_MAX_CODE) {
		return 0;
	}

	return 1;
}

static int ux_bpf_check_alu(const struct ux_sock_filter *filter,
			    unsigned int flen)
{
	u32_t cur;

	for (cur = 0; cur < flen; cur++) {
		const struct ux_sock_filter *fop = &filter[cur];

		switch (fop->code) {
		case UX_BPF_OP_DIV_IMM:
		case UX_BPF_OP_MOD_IMM:
			if (fop->k == 0) {
				return E_HM_INVAL;
			}
			break;
		case UX_BPF_OP_LSH_IMM:
		case UX_BPF_OP_RSH_IMM:
			if (fop->k >= UX_BPF_SHIFT_VALUE) {
				return E_HM_INVAL;
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

static int ux_bpf_check_jmp(const struct ux_sock_filter *filter,
			    unsigned int flen)
{
	u32_t cur;

	for (cur = 0; cur < flen; cur++) {
		const struct ux_sock_filter *fop = &filter[cur];

		switch (fop->code) {
		case UX_BPF_OP_JA:
			if (fop->k >= (unsigned int)((flen - cur) - 1)) {
				return E_HM_INVAL;
			}
			break;
		case UX_BPF_OP_JEQ_IMM:
		case UX_BPF_OP_JEQ_REG:
		case UX_BPF_OP_JGE_IMM:
		case UX_BPF_OP_JGE_REG:
		case UX_BPF_OP_JGT_IMM:
		case UX_BPF_OP_JGT_REG:
		case UX_BPF_OP_JSET_IMM:
		case UX_BPF_OP_JSET_REG:
			if (((cur + fop->jt) + 1) >= flen ||
			    ((cur + fop->jf) + 1) >= flen) {
				return E_HM_INVAL;
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

static int ux_bpf_check_code(const struct ux_sock_filter *filter,
			     unsigned int flen)
{
	u32_t array_size = sizeof(ux_opcode) / sizeof((ux_opcode)[0]);
	u16_t last_code = filter[flen - 1].code;
	u32_t cur;

	if (last_code != UX_BPF_OP_RET_K && last_code != UX_BPF_OP_RET_A) {
		return E_HM_INVAL;
	}

	for (cur = 0; cur < flen; cur++) {
		const struct ux_sock_filter *fop = &filter[cur];

		if (fop->code >= array_size ||
		    ux_opcode[fop->code] == 0) {
			return E_HM_INVAL;
		}
	}
	return 0;
}

static struct ux_bpf_prog *ux_bpf_check_filter(struct ux_bpf_prog *fp)
{
	int err;

	fp->ux_bpf_func = NULL;
	fp->jited = 0;

	err = ux_bpf_check_code(fp->insns, fp->len);
	if (err != E_HM_OK) {
		ux_bpf_prog_release(fp);
		return NULL;
	}

	err = ux_bpf_check_alu(fp->insns, fp->len);
	if (err != E_HM_OK) {
		ux_bpf_prog_release(fp);
		return NULL;
	}

	err = ux_bpf_check_jmp(fp->insns, fp->len);
	if (err != E_HM_OK) {
		ux_bpf_prog_release(fp);
		return NULL;
	}

	ux_bpf_func_set(fp);

	return fp;
}

static int ux_attach_prog(struct ux_bpf_prog *prog, ux_sock_t *sock)
{
	struct ux_conn_filter *filter = NULL;

	if (sock == NULL || prog == NULL) {
		return E_HM_INVAL;
	}

	filter = malloc(sizeof(struct ux_conn_filter));
	if (filter == NULL) {
		return E_HM_NOMEM;
	}

	filter->prog = prog;
	filter->prog->ux_bpf_func = prog->ux_bpf_func;
	filter->refcnt = 1;

	ux_detach_filter(sock);

	sock->filter = filter;

	return 0;
}

static inline unsigned int ux_bpf_prog_size(unsigned int proglen)
{
	unsigned int size = sizeof(struct ux_bpf_prog);
	unsigned int offset = (unsigned int)offsetof(struct ux_bpf_prog, insns[proglen]);

	return (size > offset ? size : offset);
}

int ux_attach_filter(const struct ux_sock_fprog *fprog, ux_sock_t *sock)
{
	unsigned int fsize = fprog->len * sizeof(fprog->filter[0]);
	unsigned int alloc_size = ux_bpf_prog_size(fprog->len);
	struct ux_bpf_prog *prog = NULL;
	int err;

	if (ux_bpf_check_basic(fprog->filter, fprog->len) == 0) {
		return E_HM_INVAL;
	}

	prog = ux_bpf_prog_alloc(alloc_size);
	if (prog == NULL) {
		return E_HM_NOMEM;
	}

	if (hm_actv_read_xact_vm(prog->insns, fprog->filter, fsize) != E_HM_OK) {
		ux_bpf_prog_release(prog);
		return E_HM_POSIX_FAULT;
	}

	prog->len = fprog->len;
	prog = ux_bpf_check_filter(prog);
	if (prog == NULL) {
		return E_HM_INVAL;
	}
	err = ux_attach_prog(prog, sock);
	if (err < 0) {
		ux_bpf_prog_release(prog);
		return err;
	}

	return 0;
}

void ux_detach_filter(ux_sock_t *sock)
{
	ux_sock_lock(sock);
	if (sock->filter && sock->filter->prog) {
		ux_bpf_prog_release(sock->filter->prog);
		sock->filter->prog = NULL;
		free(sock->filter);
		sock->filter = NULL;
	}
	ux_sock_unlock(sock);
}
