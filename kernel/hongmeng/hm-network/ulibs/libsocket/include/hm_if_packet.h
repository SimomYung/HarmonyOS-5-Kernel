/*
* Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
* Description: header file for packet
* Author: Huawei OS Kernel Lab
* Create: Thu Aug 24 10:06:31 2023
*/

#ifndef _HM_IF_PACKET_H_
#define _HM_IF_PACKET_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TP_STATUS_KERNEL			0
#define TP_STATUS_USER				(1U)
#define TP_STATUS_COPY				(1U << 1U)
#define TP_STATUS_CSUMNOTREADY		(1U << 3U)
#define TP_STATUS_VLAN_VALID		(1U << 4U) /* auxdata has valid tp_vlan_tci */
#define TP_STATUS_VLAN_TPID_VALID	(1U << 6U) /* auxdata has valid tp_vlan_tpid */

struct tpacket_auxdata {
	uint32_t	tp_status;
	uint32_t	tp_len;
	uint32_t	tp_snaplen;
	uint32_t	tp_mac;
	uint32_t	tp_net;
	uint16_t	tp_vlan_tci;
	uint16_t	tp_vlan_tpid;
};

#ifdef __cplusplus
}
#endif

#endif
