/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony source file for network create sec chk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#include <libhmseharmony/avc.h>
#include <libhmseharmony/flask.h>
#include <libhmseharmony/seharmony.h>
#include <libhmseharmony/services.h>
#include <libhmseharmony/network.h>

#include <hongmeng/errno.h>
#include <netinet/in.h>

#include <hmnet/hm_netlink.h>

#ifndef IPPROTO_MINTP
#define IPPROTO_MINTP 200 /* Huawei Minimal Transport Protol */
#endif

struct type_to_sclass {
	int type;
	hmsel_class_t	sclass;
};

static const struct type_to_sclass unix_to_class[] = {
	{SOCK_STREAM,			HM_SECCLASS_UNIX_STREAM_SOCKET},
	{SOCK_SEQPACKET,		HM_SECCLASS_UNIX_STREAM_SOCKET},
	{SOCK_DGRAM,			HM_SECCLASS_UNIX_DGRAM_SOCKET},
	{SOCK_RAW,			HM_SECCLASS_UNIX_DGRAM_SOCKET},
};

static const struct type_to_sclass nl_proto_to_class[] = {
	{NETLINK_ROUTE,			HM_SECCLASS_NETLINK_ROUTE_SOCKET},
	{NETLINK_SOCK_DIAG,		HM_SECCLASS_NETLINK_TCPDIAG_SOCKET},
	{NETLINK_NFLOG,			HM_SECCLASS_NETLINK_NFLOG_SOCKET},
	{NETLINK_XFRM,			HM_SECCLASS_NETLINK_XFRM_SOCKET},
	{NETLINK_SELINUX,		HM_SECCLASS_NETLINK_SELINUX_SOCKET},
	{NETLINK_ISCSI,			HM_SECCLASS_NETLINK_ISCSI_SOCKET},
	{NETLINK_AUDIT,			HM_SECCLASS_NETLINK_AUDIT_SOCKET},
	{NETLINK_FIB_LOOKUP,		HM_SECCLASS_NETLINK_FIB_LOOKUP_SOCKET},
	{NETLINK_CONNECTOR,		HM_SECCLASS_NETLINK_CONNECTOR_SOCKET},
	{NETLINK_NETFILTER,		HM_SECCLASS_NETLINK_NETFILTER_SOCKET},
	{NETLINK_DNRTMSG,		HM_SECCLASS_NETLINK_DNRT_SOCKET},
	{NETLINK_KOBJECT_UEVENT,	HM_SECCLASS_NETLINK_KOBJECT_UEVENT_SOCKET},
	{NETLINK_GENERIC,		HM_SECCLASS_NETLINK_GENERIC_SOCKET},
	{NETLINK_SCSITRANSPORT,		HM_SECCLASS_NETLINK_SCSITRANSPORT_SOCKET},
	{NETLINK_RDMA,			HM_SECCLASS_NETLINK_RDMA_SOCKET},
	{NETLINK_CRYPTO,		HM_SECCLASS_NETLINK_CRYPTO_SOCKET},
};

static const struct type_to_sclass ext_domain_to_class[] = {
	{AF_AX25,		HM_SECCLASS_AX25_SOCKET},
	{AF_IPX,		HM_SECCLASS_IPX_SOCKET},
	{AF_NETROM,		HM_SECCLASS_NETROM_SOCKET},
	{AF_ATMPVC,		HM_SECCLASS_ATMPVC_SOCKET},
	{AF_X25,		HM_SECCLASS_X25_SOCKET},
	{AF_ROSE,		HM_SECCLASS_ROSE_SOCKET},
	{AF_DECnet,		HM_SECCLASS_DECNET_SOCKET},
	{AF_ATMSVC,		HM_SECCLASS_ATMSVC_SOCKET},
	{AF_RDS,		HM_SECCLASS_RDS_SOCKET},
	{AF_IRDA,		HM_SECCLASS_IRDA_SOCKET},
	{AF_PPPOX,		HM_SECCLASS_PPPOX_SOCKET},
	{AF_LLC,		HM_SECCLASS_LLC_SOCKET},
	{AF_CAN,		HM_SECCLASS_CAN_SOCKET},
	{AF_TIPC,		HM_SECCLASS_TIPC_SOCKET},
	{AF_BLUETOOTH,		HM_SECCLASS_BLUETOOTH_SOCKET},
	{AF_IUCV,		HM_SECCLASS_IUCV_SOCKET},
	{AF_RXRPC,		HM_SECCLASS_RXRPC_SOCKET},
	{AF_ISDN,		HM_SECCLASS_ISDN_SOCKET},
	{AF_PHONET,		HM_SECCLASS_PHONET_SOCKET},
	{AF_IEEE802154,		HM_SECCLASS_IEEE802154_SOCKET},
	{AF_CAIF,		HM_SECCLASS_CAIF_SOCKET},
	{AF_ALG,		HM_SECCLASS_ALG_SOCKET},
	{AF_NFC,		HM_SECCLASS_NFC_SOCKET},
	{AF_VSOCK,		HM_SECCLASS_VSOCK_SOCKET},
	{AF_KCM,		HM_SECCLASS_KCM_SOCKET},
	{AF_QIPCRTR,		HM_SECCLASS_QIPCRTR_SOCKET},
	{AF_SMC,		HM_SECCLASS_SMC_SOCKET},
};

static int type_to_sclass(int type, hmsel_class_t *sclass, const struct type_to_sclass *tab, size_t tabsize)
{
	size_t i;
	int rc = E_HM_INVAL;

	for (i = 0; i < tabsize / sizeof(struct type_to_sclass); i++) {
		if (type == tab[i].type) {
			*sclass = tab[i].sclass;
			rc = E_HM_OK;
			break;
		}
	}

	return rc;
}

static hmsel_class_t inet_to_sclass(int type, int protocol, int extsclass)
{
	hmsel_class_t rc = HM_SECCLASS_SOCKET;

	switch (type) {
	case SOCK_STREAM:
	case SOCK_SEQPACKET:
		if ((protocol == IPPROTO_IP) || (protocol == IPPROTO_TCP)) {
			rc = HM_SECCLASS_TCP_SOCKET;
			break;
		} else if (extsclass && protocol == IPPROTO_SCTP) {
			rc = HM_SECCLASS_SCTP_SOCKET;
			break;
		} else {
			rc = HM_SECCLASS_RAWIP_SOCKET;
			break;
		}
	case SOCK_DGRAM:
		if ((protocol == IPPROTO_IP) || (protocol == IPPROTO_UDP)) {
			rc = HM_SECCLASS_UDP_SOCKET;
			break;
        } else if (protocol == IPPROTO_MINTP) {
            rc = HM_SECCLASS_TCP_SOCKET;
			break;
        } else if (extsclass && (protocol == IPPROTO_ICMP || protocol == IPPROTO_ICMPV6)) {
			rc = HM_SECCLASS_ICMP_SOCKET;
			break;
		} else {
			rc = HM_SECCLASS_RAWIP_SOCKET;
			break;
		}
	case SOCK_DCCP:
		rc = HM_SECCLASS_DCCP_SOCKET;
		break;
	default:
		rc = HM_SECCLASS_RAWIP_SOCKET;
		break;
	}

	return rc;
}

hmsel_class_t socket_type_to_sclass(int domain, int type, int protocol)
{
	hmsel_class_t rc = HM_SECCLASS_SOCKET;
	int ret;
	int extsclass = hmsel_policycap_extsockclass();

	switch (domain) {
	case AF_UNIX:
		ret = type_to_sclass(type, &rc, unix_to_class, sizeof(unix_to_class));
		if (ret == E_HM_INVAL) {
			hm_warn("failed to get AF_UNIX type\n");
		}
		break;
	case AF_INET:
	case AF_INET6:
		rc = inet_to_sclass(type, protocol, extsclass);
		break;
	case AF_NETLINK:
		ret = type_to_sclass(protocol, &rc, nl_proto_to_class, sizeof(nl_proto_to_class));
		if (ret == E_HM_INVAL) {
			rc = HM_SECCLASS_NETLINK_SOCKET;
		}
		break;
	case AF_PACKET:
		rc = HM_SECCLASS_PACKET_SOCKET;
		break;
	case AF_KEY:
		rc = HM_SECCLASS_KEY_SOCKET;
		break;
	case AF_APPLETALK:
		rc = HM_SECCLASS_APPLETALK_SOCKET;
		break;
	case AF_XDP:
		rc = HM_SECCLASS_XDP_SOCKET;
		break;
	default:
		break;
	}

	if (extsclass != 0) {
		ret = type_to_sclass(domain, &rc, ext_domain_to_class, sizeof(ext_domain_to_class));
		if (ret == E_HM_INVAL) {
			hm_debug("failed to get extsclass\n");
		}
	}

	return rc;
}
