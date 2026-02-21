/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: LDK library interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 16 19:30:47 2022
 */

#ifndef __LIBUXNET_UX_LIBLDK_PUB_H__
#define __LIBUXNET_UX_LIBLDK_PUB_H__

#include <hmkernel/capability.h>
#include <libhmsrv_io/type.h>

#include "ux_socket.h"
#include "ux_domain.h"

/* domain must be the first field */
typedef struct ux_ldk_domain {
	ux_domain_t domain;
	hmsrv_io_domain_ctx ctx;
} ux_ldk_domain_t;

typedef struct ux_ldk_sock {
	ux_sock_t sk;
	struct hmsrv_io_ctx ctx;
	void *recv_queue;
} ux_ldk_sock_t;

static inline ux_ldk_domain_t *ldk_domain(ux_domain_t *domain)
{
	return (ux_ldk_domain_t *)(void *)domain;
}

static inline ux_ldk_sock_t *ldk_sk(ux_sock_t *sk)
{
	return (ux_ldk_sock_t *)(void *)sk;
}

int ux_ldk_domain_register(int family, unsigned long long ap_rref);

bool ux_ldk_check_ops(struct ux_proto_ops *ops);
int ux_ldk_get_file_id(ux_socket_t *sock, unsigned long long *file_id);

int ux_ldk_bpf(int cmd, void *attr, unsigned int size);
int ux_ldk_dev_ioctl(int cmd, uintptr_t args_u);
#endif
