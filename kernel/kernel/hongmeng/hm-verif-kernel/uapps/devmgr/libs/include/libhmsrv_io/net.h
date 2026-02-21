/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: I/O service APIs of net device operation
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 22 13:58:38 2019
 */
#ifndef ULIBS_LIBHMSRV_IO_NET_H
#define ULIBS_LIBHMSRV_IO_NET_H

#include <libhmsrv_io/type.h>
#include <libsysif/devmgr/types.h>

struct hmsrv_io_net_pkt {
	int devno;
	unsigned int pool_offset;
	int shm_len;
	int rx_len;
	int offset;
};

typedef int (*netstack_recv_func_t)(const struct hmsrv_io_net_pkt *pkt);

struct hmsrv_net_config {
	uint32_t pool_size;
	uint32_t unit_size;
	uint64_t pool_shm_id;
	uint64_t rx_uring_shm_id;
	netstack_recv_func_t ns_input;
};

/* net ops */
int hmsrv_io_net_init(int ifindex, struct hmsrv_io_ctx *ctx, unsigned int flags);
int hmsrv_io_net_open(int ifindex, struct hmsrv_io_ctx *ctx, unsigned int flags);
int hmsrv_io_net_prepare(const struct hmsrv_io_ctx *ctx,
			 struct hmsrv_net_config *config);
int hmsrv_io_net_destroy(struct hmsrv_io_ctx *ctx);
int hmsrv_io_net_close(struct hmsrv_io_ctx *ctx);
int hmsrv_io_net_status(const struct hmsrv_io_ctx *ctx, struct netdev_status *status_out);
int hmsrv_io_net_ioctl(const struct hmsrv_io_ctx *ctx, int request, unsigned long args, bool is_fwd);
int hmsrv_io_net_send(const struct hmsrv_io_ctx *ctx,
		      unsigned int offset, unsigned int size,
		      unsigned int payload_len, unsigned long flags);

/* misc */
int hmsrv_io_net_ready(void);

/* netlink */
int hmsrv_io_netlink_send(rref_t dh_ap, const void *msg, size_t msg_size,
			  int protocol, unsigned int src_portid);

#endif	/* ULIBS_LIBHMSRV_IO_NET_H */
