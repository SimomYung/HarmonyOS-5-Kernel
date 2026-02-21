/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Interface for network vsock transmission
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 24 19:35:43 2021
 */

#include "ux_vsock_trans.h"
#include "vsock/ux_vsock_netdev.h"

void ux_trans_alloc(ux_vsock_sock_t *vsock)
{
	vsock->trans_context.buf_alloc = VSOCK_BUFFER_SIZE;
}

static int ux_send_msg(const ux_vsock_sock_t *sock, const struct virtio_vsock_hdr *hdr,
		       const char *buf, size_t buf_len)
{
	UNUSED(sock);
	unsigned int dst_cid = (unsigned int)(hdr->dst_cid);

	return (int)vsock_send_msg(dst_cid, hdr, buf, buf_len);
}

static void make_hdr_by_vsock(struct virtio_vsock_hdr *hdr,
			      const ux_vsock_sock_t *vsock, enum virtio_vsock_op op,
			      uint32_t len)
{
	mem_zero_s(*hdr);

	hdr->op = (uint16_t)op;
	hdr->type = (uint16_t)VIRTIO_VSOCK_TYPE_STREAM;
	hdr->src_cid = vsock->local_addr.svm_cid;
	hdr->src_port = vsock->local_addr.svm_port;
	hdr->dst_cid = vsock->remote_addr.svm_cid;
	hdr->dst_port = vsock->remote_addr.svm_port;
	hdr->buf_alloc = vsock->trans_context.buf_alloc;
	hdr->flags = 0;
	hdr->len = len;
	hdr->fwd_cnt = vsock->trans_context.fwt_cnt;
}

static void make_hdr_by_hdr(struct virtio_vsock_hdr *hdr,
			    const struct virtio_vsock_hdr *req, enum virtio_vsock_op op)
{
	mem_zero_s(*hdr);

	hdr->op = (uint16_t)op;
	hdr->type = (uint16_t)VIRTIO_VSOCK_TYPE_STREAM;
	hdr->src_cid = req->dst_cid;
	hdr->src_port = req->dst_port;
	hdr->dst_cid = req->src_cid;
	hdr->dst_port = req->src_port;
	hdr->buf_alloc = VSOCK_BUFFER_SIZE;
}

int ux_trans_request(const ux_vsock_sock_t *vsock, const struct sockaddr_vm *dst_addr)
{
	UNUSED(dst_addr);
	struct virtio_vsock_hdr hdr;

	make_hdr_by_vsock(&hdr, vsock, VIRTIO_VSOCK_OP_REQUEST, 0);

	return ux_send_msg(vsock, &hdr, NULL, 0);
}

int ux_trans_response(const ux_vsock_sock_t *vsock, const struct virtio_vsock_hdr *req)
{
	struct virtio_vsock_hdr hdr;

	make_hdr_by_vsock(&hdr, vsock, VIRTIO_VSOCK_OP_RESPONSE, 0);
	hdr.dst_cid = req->src_cid;
	hdr.dst_port = req->src_port;

	return ux_send_msg(vsock, &hdr, NULL, 0);
}

int ux_trans_reset(const ux_vsock_sock_t *vsock, const struct virtio_vsock_hdr *hdr)
{
	struct virtio_vsock_hdr reply;

	if (hdr == NULL) {
		make_hdr_by_vsock(&reply, vsock, VIRTIO_VSOCK_OP_RST, 0);
	} else {
		make_hdr_by_hdr(&reply, hdr, VIRTIO_VSOCK_OP_RST);
	}

	return ux_send_msg(vsock, &reply, NULL, 0);
}

int ux_trans_shutdown(const ux_vsock_sock_t *vsock, unsigned int mode)
{
	struct virtio_vsock_hdr hdr;

	make_hdr_by_vsock(&hdr, vsock, VIRTIO_VSOCK_OP_SHUTDOWN, 0);
	if ((mode & SHUTDOWN_RD) != 0U) {
		hdr.flags |= VIRTIO_VSOCK_SHUTDOWN_RCV;
	}

	if ((mode & SHUTDOWN_WR) != 0U) {
		hdr.flags |= VIRTIO_VSOCK_SHUTDOWN_SEND;
	}

	return ux_send_msg(vsock, &hdr, NULL, 0);
}

int ux_trans_update_credit(const ux_vsock_sock_t *vsock)
{
	struct virtio_vsock_hdr hdr;

	make_hdr_by_vsock(&hdr, vsock, VIRTIO_VSOCK_OP_CREDIT_UPDATE, 0);

	return ux_send_msg(vsock, &hdr, NULL, 0);
}

ssize_t ux_trans_write(const ux_vsock_sock_t *vsock, const char *buf, size_t len)
{
	struct virtio_vsock_hdr hdr;

	make_hdr_by_vsock(&hdr, vsock, VIRTIO_VSOCK_OP_RW, (uint32_t)len);

	return ux_send_msg(vsock, &hdr, buf, len);
}

uint32_t ux_trans_space_left(ux_vsock_sock_t *vsock)
{
	struct vsock_trans *ctx = &(vsock->trans_context);
	int64_t left = ctx->peer_buf_alloc - (ctx->send_cnt - ctx->peer_fwt_cnt);

	if (left < 0) {
		left = 0;
	}

	return (uint32_t)left;
}

bool ux_trans_has_space(ux_vsock_sock_t *vsock)
{
	return ux_trans_space_left(vsock) > 0;
}

uint32_t ux_trans_get_buffer_size(const ux_vsock_sock_t *vsock)
{
	return vsock->trans_context.buf_alloc;
}

void ux_trans_set_buffer_size(ux_vsock_sock_t *vsock, uint64_t buffer_size)
{
	uint64_t update_size = buffer_size;

	if (update_size > VSOCK_MAX_BUF_SIZE_LIMIT) {
		update_size = VSOCK_MAX_BUF_SIZE_LIMIT;
	}
	vsock->trans_context.buf_alloc = update_size;
}

void update_trans_info(ux_vsock_sock_t *vsock, const struct virtio_vsock_hdr *hdr)
{
	uint32_t space_left;

	vsock->trans_context.peer_buf_alloc = hdr->buf_alloc;
	vsock->trans_context.peer_fwt_cnt = hdr->fwd_cnt;

	space_left = ux_trans_space_left(vsock);
	if (space_left > 0) {
		vsock->sk.set_write_space(&vsock->sk, (int)space_left);
	}
}

int ux_trans_recv_msg(struct vm_msg *msg)
{
	ux_vsock_sock_t *vsock = NULL;
	int ret = E_HM_OK;

	if (msg->hdr.dst_cid != vsock_local_cid()) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		struct sockaddr_vm local;
		struct sockaddr_vm remote;

		make_vm_addr(&local, (unsigned int)(msg->hdr.dst_cid), msg->hdr.dst_port);
		make_vm_addr(&remote, (unsigned int)(msg->hdr.src_cid), msg->hdr.src_port);

		vsock = vsock_find_connected(&local, &remote);
		if (vsock == NULL) {
			vsock = vsock_find_binded(&local);
		}

		if (vsock == NULL) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		lock_vsock(vsock);
		update_trans_info(vsock, &msg->hdr);
		unlock_vsock(vsock);

		ret = vsock_recv_msg(vsock, msg);
		vsock_put(vsock);
	} else {
		(void)ux_trans_reset(NULL, &msg->hdr);
	}

	return ret;
}
