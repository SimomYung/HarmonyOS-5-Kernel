/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: interface for libdopra
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 29 09:56:30 2021
 */

#ifndef __UX_VISP_ARP_H__
#define __UX_VISP_ARP_H__

#include <net/if_arp.h>

int visp_arp_ioctl(int cmd, struct arpreq *req);
int visp_dump_arp_table(char **msg, int *msg_len, int type);

int visp_get_neigh_info_nl(void **buf, size_t *len);
void visp_del_neigh_info_nl(uint32_t dst, char *ifname, uint16_t state);

#endif
