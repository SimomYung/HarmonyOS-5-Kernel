/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for network liblwip
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 12 10:30:47 2019
 */

#ifndef __HM_NET_UX_LIBLWIP_H
#define __HM_NET_UX_LIBLWIP_H

#include <lwip/sys.h>
#include <lwip/api.h>
#include <lwip/netif.h>

#include "ux_netdev.h"
#include "ux_netlog.h"

typedef struct ux_lwip_sock {
	ux_sock_t sk;

	int fd;
} ux_lwip_sock_t;

int af_inet_init(void);
void liblwip_set_log_level(int level);

int lwip_netdev_init(void);
int lwip_socket_init(void);
err_t netif_set_dev_mc(struct netif *netif, const ip4_addr_t *group, enum netif_mac_filter_action op);

void rx_zero_poll_stat(void);
void my_pbuf_free_custom(struct pbuf* p);
struct pbuf *my_pbuf_alloc(void *data, size_t len, unsigned int cell_offset,
			   unsigned long free_size, hmshm_mgr_t *shmmgr);
#endif
