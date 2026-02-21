/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: TCP/UDP filter rule head file
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 22 15:12:25 2021
 */

#ifndef _UX_IPT_TCPUDP_H_
#define _UX_IPT_TCPUDP_H_

#define PORT_NUM 2

struct ux_ipt_tcp {
	uint16_t spts[PORT_NUM];
	uint16_t dpts[PORT_NUM];
	uint8_t option;
	uint8_t flg_mask;
	uint8_t flg_cmp;
	uint8_t invflags;
};

struct ux_ipt_udp {
	uint16_t spts[PORT_NUM];
	uint16_t dpts[PORT_NUM];
	uint8_t invflags;
};

int init_tcpudp_mt(void);

#endif
