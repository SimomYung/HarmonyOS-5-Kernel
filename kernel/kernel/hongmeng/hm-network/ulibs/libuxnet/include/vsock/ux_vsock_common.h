/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  Header file for network vsock common declarations
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 24 19:35:43 2021
 */

#ifndef __UX_VSOCK_COMMON_H_
#define __UX_VSOCK_COMMON_H_

#include <stdint.h>
#include <stdlib.h>

#include <lib/dlist.h>
#include <hm_vsock.h>

#define SHUTDOWN_RD		0x01
#define SHUTDOWN_WR		0x02
#define SHUTDOWN_MASK		0x03

#define LAST_RESERVED_PORT	1023

#define VSOCK_OPT_BUFFER_SIZE		0
#define VSOCK_OPT_MIN_BUFFER_SIZE	1
#define VSOCK_OPT_MAX_BUFFER_SIZE	2

/**
 * The desc of rx virt-queue is filled by guest, and the default desc data length is 4096
 * both in linux 4.xx and 5.xx, so the max data length we can send to uvmm is limited
 * by the VSOCK_DEFAULT_RX_BUF_SIZE macro.
 */
#define VSOCK_DEFAULT_RX_BUF_SIZE	4096U

/**
 * The desc of tx virt-queue of guest is filled by guest when guest send package,
 * the max package length is diff in linux 4.xx and 5.xx, so here we enlarge the max
 * package we can receive.
 */
#define VSOCK_MAX_PKT_BUF_SIZE		(1024U * 64U)

enum virtio_vsock_type {
	VIRTIO_VSOCK_TYPE_STREAM = 1,
};

enum virtio_vsock_op {
	VIRTIO_VSOCK_OP_INVALID = 0,
	VIRTIO_VSOCK_OP_REQUEST = 1,
	VIRTIO_VSOCK_OP_RESPONSE = 2,
	VIRTIO_VSOCK_OP_RST = 3,
	VIRTIO_VSOCK_OP_SHUTDOWN = 4,
	VIRTIO_VSOCK_OP_RW = 5,
	VIRTIO_VSOCK_OP_CREDIT_UPDATE = 6,
	VIRTIO_VSOCK_OP_CREDIT_REQUEST = 7,
};

/* VIRTIO_VSOCK_OP_SHUTDOWN flags values */
enum virtio_vsock_shutdown {
	VIRTIO_VSOCK_SHUTDOWN_RCV = 1,
	VIRTIO_VSOCK_SHUTDOWN_SEND = 2,
};

struct virtio_vsock_hdr {
	uint64_t src_cid;
	uint64_t dst_cid;
	uint32_t src_port;
	uint32_t dst_port;
	uint32_t len;
	uint16_t type;		 /* enum virtio_vsock_type */
	uint16_t op;		 /* enum virtio_vsock_op */
	uint32_t flags;
	uint32_t buf_alloc;
	uint32_t fwd_cnt;
} __attribute__((packed));

enum ux_vsockmon_transport {
	UX_VSOCK_TRANSPORT_UNKNOWN	= 0,
	UX_VSOCK_TRANSPORT_NO_INFO	= 1,
	UX_VSOCK_TRANSPORT_VIRTIO	= 2,
};

enum ux_vsockmon_op {
	UX_VSOCK_OP_UNKNOWN	= 0,
	UX_VSOCK_OP_CONNECT	= 1,
	UX_VSOCK_OP_DISCONNECT	= 2,
	UX_VSOCK_OP_CONTROL	= 3,
	UX_VSOCK_OP_PAYLOAD	= 4,
};

struct ux_vsockmon_hdr {
	uint64_t	src_cid;
	uint64_t	dst_cid;
	uint32_t	src_port;
	uint32_t	dst_port;
	uint16_t	op;		 /* @see ux_vsockmon_op */
	uint16_t	transport;	 /* @see ux_vosckmon_transport */
	uint16_t	len;		 /* size of transport header */
	uint8_t		reserved[2];
};

/**
 * @vm_msg represents a message from uvmm(VM) and mainly contains two parts:
 * 	1. The virtio vsock message header.
 * 	2. The real message data which may be empty
 */
struct vm_msg {
	struct dlist_node node;

	struct virtio_vsock_hdr hdr;

	char *data;
	size_t len;
};

#define	VIRTIO_VSOCK_HDR_LEN	sizeof(struct virtio_vsock_hdr)

typedef void (*cid_revoke_handler_t)(unsigned int cid);
#endif
