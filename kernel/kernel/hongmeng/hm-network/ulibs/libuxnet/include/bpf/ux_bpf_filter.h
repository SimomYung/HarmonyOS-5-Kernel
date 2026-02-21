/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for bpf filter
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 10 09:31:16 CST 2020
 */

#ifndef _BPF_FILTER_H_
#define _BPF_FILTER_H_

#include <bpf/ux_bpf_def.h>
#include <bpf/ux_bpf_common.h>
#include "ux_socket.h"

#define ux_bpf_code_exec(ctx, filter, data_len)  (*(filter)->ux_bpf_func)((ctx), (filter)->insns, (data_len))

int ux_attach_filter(const struct ux_sock_fprog *fprog, ux_sock_t *sock);
void ux_detach_filter(ux_sock_t *sock);
struct ux_bpf_prog *ux_bpf_prog_alloc(unsigned int size);
void ux_bpf_func_set(struct ux_bpf_prog *fp);

#endif /* _BPF_FILTER_H_ */
