/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for network domain
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 28 13:34:09 2019
 */

#ifndef __LIBUXNET_UX_DOMAIN_H
#define __LIBUXNET_UX_DOMAIN_H

#include "ux_socket.h"
#include "ux_netlog.h"

#define MAX_NET_DOMAIN (AF_MAX + 1)

typedef struct ux_domain {
	int family;
	int (*create)(ux_socket_t *socket, int domain, int type, int protocol, ux_namespace_t *ns);
} ux_domain_t;

int ux_domain_register(ux_domain_t *domain);
void ux_domain_unregister(int family);

ux_domain_t *ux_domain_get(int family);

#endif
