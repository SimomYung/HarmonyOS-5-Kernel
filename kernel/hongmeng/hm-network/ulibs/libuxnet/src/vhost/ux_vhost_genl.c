/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implements for vhost server
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 23 15:16:13 2021
 */

#include "vhost/ux_vhost.h"

#include "submod.h"
#include "hm_netlink.h"
#include "../netlink/ux_netlink_core.h"
#include "../netlink/ux_netlink_generic.h"
#include "ux_revoke_handler.h"

static struct vhost_server_s *vhost_server = NULL;

static int ux_genl_vhost_request(ux_sock_t *sk, const struct nlmsghdr *nlh,
				 void *attrs, uint32_t size)
{
	const size_t vhost_msg_size = sizeof(struct vhost_user_msg_s);
	struct vhost_user_msg_s response = { 0 };
	ux_pktbuf_t *pkt = NULL;
	int ret;

	if (attrs == NULL || size != vhost_msg_size) {
		net_warn(HMNET_VHOST, "invalid vhost user msg size\n");
		ret = E_HM_INVAL;
	} else {
		uint32_t caller_cnode_idx = sysif_actv_src_cnode_idx();
		struct vhost_user_msg_s *request = (struct vhost_user_msg_s*)attrs;

		if (caller_cnode_idx != request->cnode_idx) {
			ret = E_HM_PERM;
		} else {
			ret = ux_vhost_recv_msg(request->cnode_idx, request, &response);
		}
	}

	pkt = ux_pktbuf_alloc(NETLINK_PAYLOAD_MAX);
	if (pkt == NULL) {
		net_warn(HMNET_VHOST, "alloc genl response pkt failed\n");
		ret = E_HM_NOMEM;
	} else {
		struct genlmsghdr *genlh = NULL;
		struct nlmsghdr *rsp_nlh = NULL;

		rsp_nlh = ux_pktbuf_push(pkt, NLMSG_SPACE(sizeof(struct genlmsghdr)));

		rsp_nlh->nlmsg_type = nlh->nlmsg_type;
		rsp_nlh->nlmsg_len = NLMSG_SPACE(sizeof(struct genlmsghdr));
		rsp_nlh->nlmsg_flags = 0;
		rsp_nlh->nlmsg_pid = nlh->nlmsg_pid;
		rsp_nlh->nlmsg_seq = nlh->nlmsg_seq;

		genlh = ux_nlmsg_data(rsp_nlh);
		genlh->cmd = UX_VHOST_CMD_REQUEST;
		genlh->version = 0x02;
		genlh->reserved = 0;

		response.return_code = ret;
		NOFAIL(memcpy_s(ux_pktbuf_push(pkt, vhost_msg_size),
				vhost_msg_size, &response, vhost_msg_size));

		rsp_nlh->nlmsg_len = (uint32_t)((char *)pkt->tail - (char *)rsp_nlh);

		ret = ux_netlink_recv_data(sk, pkt, false);
		if (ret != 0) {
			ux_pktbuf_free(pkt);
		}
	}

	return ret;
}

static const ux_netlink_generic_ops_t ux_genl_vhost_ops[] = {
	{
		.cmd = UX_VHOST_CMD_REQUEST,
		.exec = ux_genl_vhost_request,
		.dump = NULL,
	}
};

static ux_netlink_generic_t ux_genl_vhost = {
	.name = VHOST_GENL_FAMILY_NAME,
	.ops = ux_genl_vhost_ops,
	.id = 0,
	.ops_cnt = ARRAY_SIZE(ux_genl_vhost_ops),
	.mcgrps_offset = 0,
	.mcgrps_cnt = 0,
	.dh_ap_rref = 0
};

static void ux_vhost_revoke(uint32_t cnode_idx)
{
	if (vhost_server != NULL) {
		vhost_server_revoke(vhost_server, cnode_idx);
	}
}

int ux_vhost_init(void)
{
	int ret = E_HM_OK;

	vhost_server = vhost_new_server();
	if (vhost_server == NULL) {
		ret = E_HM_NOMEM;
	}

	if (ret == E_HM_OK) {
		ret = ux_netlink_register_generic(&ux_genl_vhost);
	}

	if (ret == E_HM_OK) {
		ret = ux_vhost_vsock_init();
	}

	if (ret == E_HM_OK) {
		ret = ux_vhost_vnet_init();
	}

	if (ret == E_HM_OK) {
		ret = ux_register_revoke_handler("vhost", ux_vhost_revoke);
	}

	if (ret != E_HM_OK) {
		vhost_destroy_server(vhost_server);
	}

	return ret;
}

void ux_vhost_release(void)
{
	vhost_destroy_server(vhost_server);
	vhost_server = NULL;
}

int ux_vhost_recv_msg(uint32_t caller_cnode, struct vhost_user_msg_s *req,
		      struct vhost_user_msg_s *rsp)
{
	return vhost_server_handle_msg(vhost_server, caller_cnode, req, rsp);
}

int ux_vhost_register_backend(struct vhost_backend_s *backend)
{
	return vhost_register_backend(vhost_server, backend);
}
