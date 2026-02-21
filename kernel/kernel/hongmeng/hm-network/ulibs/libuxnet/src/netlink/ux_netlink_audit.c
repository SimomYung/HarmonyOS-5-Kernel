/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Netlink route operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan  3 17:10:16 2019
 */
#include <netinet/if_ether.h>
#include <libhmaudit/audit.h>
#include <libstrict/strict.h>
#include <libhmsync/raw_rwlock.h>
#include <libsysif/auditmgr/api.h>

#include "hm_netlink.h"
#include "hm_neighbour.h"

#include "ux_netlink_core.h"
#include "ux_netlink_proto.h"
#include "ux_netlink_table.h"

#include "ux_netlink_audit.h"

static rref_t auditmgr_rref;

static int sysif_init_auditmgr(void)
{
	int err;
	rref_t rref;

	err = hm_path_acquire(AUDITMGR_PATH, &rref);
	if (err != E_HM_OK) {
		hm_warn("acquire auditmgr path failed: %s\n", hmstrerror(err));
		return err;
	}
	auditmgr_rref = rref;

	return E_HM_OK;
}

int netlink_audit_unicast(unsigned int dst_port, const char *msg, const size_t len)
{
	return ux_netlink_unicast(NETLINK_AUDIT, dst_port, msg, len, false);
}

static int ux_netlink_audit_recv(ux_sock_t *sk, ux_pktbuf_t *pkt)
{
	int err = 0;

	if (pkt->len >= NLMSG_HDRLEN) {
		// call auditmgr sysif
		err = actvxactcapcall_adcall_msg_recv(true, true, auditmgr_rref,
						      netlink_sock(sk)->port,
						      pkt->data, pkt->len);
		if (err != 0 ||
		    (((struct nlmsghdr *)pkt->data)->nlmsg_flags & NLM_F_ACK) != 0) {
			ux_netlink_error(sk, (struct nlmsghdr *)(pkt->data), -hmerrno2posix(err));
		}
	}

	return E_HM_OK;
}

static ux_netlink_proto_t netlink_audit = {
	.rcv = ux_netlink_audit_recv,
	.protocol = NETLINK_AUDIT,
	.is_dynamic = false,
	.refcnt = __RAW_REFCNT_NR_INITIALIZER(0),
};

int ux_netlink_audit_init(void)
{
	int ret;

	if (audit_sw_is_disabled()) {
		hm_info("audit service not enabled\n");
		return E_HM_OK;
	}

	ret = ux_netlink_create_service(&netlink_audit);
	if (ret) {
		net_error(HMNET_NETLINK, "audit: create audit service failed\n");
		return ret;
	}

	ret = ux_netlink_proto_register(&netlink_audit);
	if (ret) {
		net_error(HMNET_NETLINK, "audit: register audit proto failed\n");
		return ret;
	}

	ret = sysif_init_auditmgr();
	if (ret) {
		net_error(HMNET_NETLINK, "audit: init auditmgr sysif failed\n");
		return ret;
	}

	net_info(HMNET_NETLINK, "audit: register service success\n");
	return 0;
}

void ux_netlink_audit_exit(void)
{
	ux_netlink_proto_unregister(&netlink_audit);
}
