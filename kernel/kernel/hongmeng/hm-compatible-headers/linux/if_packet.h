/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Add linux/if_packet.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 10 21:33:23 2022
 */
#ifndef _LINUX_IF_PACKET_H
#define _LINUX_IF_PACKET_H

#include <netpacket/packet.h>

struct tpacket_auxdata {
	__u32 tp_status;
	__u32 tp_len;
	__u32 tp_snaplen;
	__u16 tp_mac;
	__u16 tp_net;
	__u16 tp_vlan_tci;
	__u16 tp_vlan_tpid;
};

#define TP_STATUS_CSUMNOTREADY (1 << 3)

#define TP_STATUS_KERNEL 0
#define TP_STATUS_USER 1

struct tpacket_hdr {
	unsigned long	tp_status;
	unsigned int	tp_len;
	unsigned int	tp_snaplen;
	unsigned short	tp_mac;
	unsigned short	tp_net;
	unsigned int	tp_sec;
	unsigned int	tp_usec;
};

struct tpacket_req {
	unsigned int	tp_block_size;	/* Minimal size of contiguous block */
	unsigned int	tp_block_nr;	/* Number of blocks */
	unsigned int	tp_frame_size;	/* Size of frame */
	unsigned int	tp_frame_nr;	/* Total number of frames */
};

#endif
