/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Netlink taskstats operations
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb  6 15:47:16 UTC 2020
 */

#include <fs_ioaccount.h>

#include "hm_netlink.h"

#include "ux_netlink_core.h"

#include "ux_netlink_generic.h"
#include "ux_netlink_taskstats.h"

static ux_netlink_generic_t ux_genl_taskstats;

static int ux_genl_ctl_fill_taskstats(uint32_t pid, ux_pktbuf_t *pkt, uint32_t seq)
{
	struct genlmsghdr *genlh = NULL;
	struct nlmsghdr *nlh = NULL;
	struct nlattr *na = NULL;
	struct taskstats stats;
	int ret;

	ret = fs_get_pid_io_stats(pid, &stats);
	if (ret != E_HM_OK) {
		return E_HM_INVAL;
	}

	nlh = ux_pktbuf_push(pkt, NLMSG_SPACE(sizeof(struct genlmsghdr)));
	if (nlh == NULL) {
		return E_HM_NOMEM;
	}

	nlh->nlmsg_type = ux_genl_taskstats.id;
	nlh->nlmsg_len = NLMSG_SPACE(sizeof(struct genlmsghdr));
	nlh->nlmsg_flags = 0;
	nlh->nlmsg_pid = 0;
	nlh->nlmsg_seq = seq;

	genlh = ux_nlmsg_data(nlh);
	genlh->cmd = UX_TASKSTATS_CMD_NEW;
	genlh->version = 0x01;
	genlh->reserved = 0;

	na = ux_nla_nest_start(pkt, UX_TASKSTATS_TYPE_AGGR_PID);
	if (na == NULL) {
		ret = E_HM_NOMEM;
		goto out;
	}

	ret = E_HM_MSGSIZE;
	if (ux_netlink_attr_put(pkt, UX_TASKSTATS_TYPE_PID, &pid, sizeof(pid))) {
		goto out;
	}

	/* fill taskstats */
	if (ux_netlink_attr_put(pkt, UX_TASKSTATS_TYPE_STATS, &stats,
				sizeof(stats))) {
		goto out;
	}

	(void)ux_nla_nest_end(pkt, na);

	nlh->nlmsg_len = (uint32_t)((char *)pkt->tail - (char *)nlh);

	return 0;

out:
	pkt->tail = (void *)nlh;
	pkt->len = (unsigned int)((char *)pkt->tail - (char *)pkt->data);

	return ret;
}

static int ux_genl_taskstats_get_stat(ux_sock_t *sk, const struct nlmsghdr *nlh,
				      void *attrs, uint32_t size)
{
	struct nlattr *attr = (struct nlattr *)attrs;
	ux_pktbuf_t *pkt = NULL;
	uint32_t pid;
	int ret;

	if (size < sizeof(struct nlattr) || size < attr->nla_len) {
		return E_HM_INVAL;
	}

	switch (attr->nla_type) {
	case UX_TASKSTATS_CMD_ATTR_PID:
		pid = *(uint32_t *)(attr + 1);
		break;
	default:
		return E_HM_INVAL;
	}

	pkt = ux_pktbuf_alloc(NETLINK_PAYLOAD_MAX);
	if (pkt == NULL) {
		return E_HM_NOMEM;
	}

	ret = ux_genl_ctl_fill_taskstats(pid, pkt, nlh->nlmsg_seq);
	if (ret != E_HM_OK) {
		ux_pktbuf_free(pkt);
		return ret;
	}

	/* should not hold sk lock */
	ret = ux_netlink_recv_data(sk, pkt, false);
	if (ret != 0) {
		ux_pktbuf_free(pkt);
		return ret;
	}

	return 0;
}

static const ux_netlink_generic_ops_t ux_genl_taskstats_ops[] = {
	{
		.cmd = UX_TASKSTATS_CMD_GET,
		.exec = ux_genl_taskstats_get_stat,
		.dump = NULL,
	}
};

static ux_netlink_generic_t ux_genl_taskstats = {
	.name = TASKSTATS_GENL_NAME,
	.ops = ux_genl_taskstats_ops,
	.id = 0,
	.ops_cnt = ARRAY_SIZE(ux_genl_taskstats_ops),
	.mcgrps_offset = 0,
	.mcgrps_cnt = 0,
	.dh_ap_rref = 0
};

int ux_netlink_taskstats_init(void)
{
	return ux_netlink_register_generic(&ux_genl_taskstats);
}

void ux_netlink_taskstats_exit(void)
{
	ux_netlink_generic_wrlock();
	ux_netlink_unregister_generic(&ux_genl_taskstats);
	ux_netlink_generic_unlock();
}

