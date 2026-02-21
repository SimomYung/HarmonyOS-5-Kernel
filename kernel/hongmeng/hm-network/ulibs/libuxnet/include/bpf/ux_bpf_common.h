/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for bpf common structure
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 10 09:31:16 CST 2020
 */

#ifndef _BPF_COMMON_H_
#define _BPF_COMMON_H_

#include <stdlib.h>
#include "ux_sock_filter.h"

struct ux_bpf_prog {
	u16_t    jited;
	u32_t    len;
	unsigned int    (*ux_bpf_func)(const void *buf,
				       const struct ux_sock_filter *insn,
				       int data_len);

	struct ux_sock_filter  insns[];
};

struct ux_conn_filter {
	int refcnt;
	struct ux_bpf_prog *prog;
};

#endif /* _BPF_COMMON_H_ */
