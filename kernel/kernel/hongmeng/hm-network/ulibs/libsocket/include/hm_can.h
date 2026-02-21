/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Header file for can
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 30 09:56:33 2019
 */

#ifndef __HM_NET_CAN_H_
#define __HM_NET_CAN_H_

#include <sys/types.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t canid_t;

#define CAN_RAW		1 /* RAW sockets */
#define CAN_NPROTO	2

struct sockaddr_can {
	sa_family_t can_family;
	int can_ifindex;
	union {
		struct {
			uint32_t rx_id;
			uint32_t tx_id;
		} tp;
	} can_addr;
};

#define CAN_MAX_DLEN 8

struct can_frame {
	uint32_t can_id;
	uint8_t can_dlc;
	uint8_t __pad;
	uint8_t __res0;
	uint8_t __res1;
	uint8_t data[CAN_MAX_DLEN] __attribute__((aligned(8)));
};

#define CAN_MTU		(sizeof(struct can_frame))

#define CANFD_MAX_DLEN 64

struct canfd_frame {
	uint32_t can_id;
	uint8_t len;
	uint8_t flags;
	uint8_t __res0;
	uint8_t __res1;
	uint8_t data[CANFD_MAX_DLEN] __attribute__((aligned(8)));
};

#define CANFD_MTU	(sizeof(struct canfd_frame))

struct can_filter {
	uint32_t can_id;
	uint32_t can_mask;
};

#ifdef __cplusplus
}
#endif

#endif
