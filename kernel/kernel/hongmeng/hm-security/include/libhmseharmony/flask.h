/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony head file for class define
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#ifndef HMSEL_EXPORT_FLASK_H
#define HMSEL_EXPORT_FLASK_H

#define HM_SECCLASS_FAST                                     (2u)

#define HM_SECCLASS_NULL                                     (0u)
#define HM_SECCLASS_FILE                                     (1u)
#define HM_SECCLASS_DIR                                      (2u)
#define HM_SECCLASS_CHR_FILE                                 (3u)
#define HM_SECCLASS_FIFO_FILE                                (4u)
#define HM_SECCLASS_PROCESS                                  (5u)
#define HM_SECCLASS_PROCESS2                                 (6u)
#define HM_SECCLASS_SYSTEM                                   (7u)
#define HM_SECCLASS_CAPABILITY                               (8u)
#define HM_SECCLASS_FILESYSTEM                               (9u)
#define HM_SECCLASS_FD                                      (10u)
#define HM_SECCLASS_LNK_FILE                                (11u)
#define HM_SECCLASS_BLK_FILE                                (12u)
#define HM_SECCLASS_SOCK_FILE                               (13u)
#define HM_SECCLASS_SECURITY                                (14u)
#define HM_SECCLASS_SOCKET                                  (15u)
#define HM_SECCLASS_TCP_SOCKET                              (16u)
#define HM_SECCLASS_UDP_SOCKET                              (17u)
#define HM_SECCLASS_RAWIP_SOCKET                            (18u)
#define HM_SECCLASS_NODE                                    (19u)
#define HM_SECCLASS_NETIF                                   (20u)
#define HM_SECCLASS_NETLINK_SOCKET                          (21u)
#define HM_SECCLASS_PACKET_SOCKET                           (22u)
#define HM_SECCLASS_KEY_SOCKET                              (23u)
#define HM_SECCLASS_UNIX_STREAM_SOCKET                      (24u)
#define HM_SECCLASS_UNIX_DGRAM_SOCKET                       (25u)
#define HM_SECCLASS_SEM                                     (26u)
#define HM_SECCLASS_MSG                                     (27u)
#define HM_SECCLASS_MSGQ                                    (28u)
#define HM_SECCLASS_SHM                                     (29u)
#define HM_SECCLASS_IPC                                     (30u)
#define HM_SECCLASS_NETLINK_ROUTE_SOCKET                    (31u)
#define HM_SECCLASS_NETLINK_TCPDIAG_SOCKET                  (32u)
#define HM_SECCLASS_NETLINK_NFLOG_SOCKET                    (33u)
#define HM_SECCLASS_NETLINK_XFRM_SOCKET                     (34u)
#define HM_SECCLASS_NETLINK_SELINUX_SOCKET                  (35u)
#define HM_SECCLASS_NETLINK_ISCSI_SOCKET                    (36u)
#define HM_SECCLASS_NETLINK_AUDIT_SOCKET                    (37u)
#define HM_SECCLASS_NETLINK_FIB_LOOKUP_SOCKET               (38u)
#define HM_SECCLASS_NETLINK_CONNECTOR_SOCKET                (39u)
#define HM_SECCLASS_NETLINK_NETFILTER_SOCKET                (40u)
#define HM_SECCLASS_NETLINK_DNRT_SOCKET                     (41u)
#define HM_SECCLASS_ASSOCIATION                             (42u)
#define HM_SECCLASS_NETLINK_KOBJECT_UEVENT_SOCKET           (43u)
#define HM_SECCLASS_NETLINK_GENERIC_SOCKET                  (44u)
#define HM_SECCLASS_NETLINK_SCSITRANSPORT_SOCKET            (45u)
#define HM_SECCLASS_NETLINK_RDMA_SOCKET                     (46u)
#define HM_SECCLASS_NETLINK_CRYPTO_SOCKET                   (47u)
#define HM_SECCLASS_APPLETALK_SOCKET                        (48u)
#define HM_SECCLASS_PACKET                                  (49u)
#define HM_SECCLASS_KEY                                     (50u)
#define HM_SECCLASS_DCCP_SOCKET                             (51u)
#define HM_SECCLASS_MEMPROTECT                              (52u)
#define HM_SECCLASS_PEER                                    (53u)
#define HM_SECCLASS_CAPABILITY2                             (54u)
#define HM_SECCLASS_KERNEL_SERVICE                          (55u)
#define HM_SECCLASS_TUN_SOCKET                              (56u)
#define HM_SECCLASS_BINDER                                  (57u)
#define HM_SECCLASS_CAP_USERNS                              (58u)
#define HM_SECCLASS_CAP2_USERNS                             (59u)
#define HM_SECCLASS_SCTP_SOCKET                             (60u)
#define HM_SECCLASS_ICMP_SOCKET                             (61u)
#define HM_SECCLASS_AX25_SOCKET                             (62u)
#define HM_SECCLASS_IPX_SOCKET                              (63u)
#define HM_SECCLASS_NETROM_SOCKET                           (64u)
#define HM_SECCLASS_ATMPVC_SOCKET                           (65u)
#define HM_SECCLASS_X25_SOCKET                              (66u)
#define HM_SECCLASS_ROSE_SOCKET                             (67u)
#define HM_SECCLASS_DECNET_SOCKET                           (68u)
#define HM_SECCLASS_ATMSVC_SOCKET                           (69u)
#define HM_SECCLASS_RDS_SOCKET                              (70u)
#define HM_SECCLASS_IRDA_SOCKET                             (71u)
#define HM_SECCLASS_PPPOX_SOCKET                            (72u)
#define HM_SECCLASS_LLC_SOCKET                              (73u)
#define HM_SECCLASS_CAN_SOCKET                              (74u)
#define HM_SECCLASS_TIPC_SOCKET                             (75u)
#define HM_SECCLASS_BLUETOOTH_SOCKET                        (76u)
#define HM_SECCLASS_IUCV_SOCKET                             (77u)
#define HM_SECCLASS_RXRPC_SOCKET                            (78u)
#define HM_SECCLASS_ISDN_SOCKET                             (79u)
#define HM_SECCLASS_PHONET_SOCKET                           (80u)
#define HM_SECCLASS_IEEE802154_SOCKET                       (81u)
#define HM_SECCLASS_CAIF_SOCKET                             (82u)
#define HM_SECCLASS_ALG_SOCKET                              (83u)
#define HM_SECCLASS_NFC_SOCKET                              (84u)
#define HM_SECCLASS_VSOCK_SOCKET                            (85u)
#define HM_SECCLASS_KCM_SOCKET                              (86u)
#define HM_SECCLASS_QIPCRTR_SOCKET                          (87u)
#define HM_SECCLASS_SMC_SOCKET                              (88u)
#define HM_SECCLASS_INFINIBAND_PKEY                         (89u)
#define HM_SECCLASS_INFINIBAND_ENDPORT                      (90u)
#define HM_SECCLASS_BPF                                     (91u)
#define HM_SECCLASS_XDP_SOCKET                              (92u)
#define HM_SECCLASS_PERF_EVENT                              (93u)
#define HM_SECCLASS_LOCKDOWN                                (94u)
#define HM_SECCLASS_XPM                                     (95u)
#define HM_SECCLASS_HMPSF                                   (96u)
#define HM_SECCLASS_CODE_SIGN                                (97u)
#define HM_SECCLASS_HMPROBE                                 (98u)
#define HM_SECCLASS_HMCAP                                   (99u)
#define HM_SECCLASS_VSOCK_HOST_SOCKET                       (100u)
#define HM_SECCLASS_NUM                                     (101u)

#define HM_SECINITSID_KERNEL                                   (1u)
#define HM_SECINITSID_SECURITY                                 (2u)
#define HM_SECINITSID_UNLABELED                                (3u)
#define HM_SECINITSID_FILE                                     (5u)
#define HM_SECINITSID_ANY_SOCKET                               (8u)
#define HM_SECINITSID_PORT                                     (9u)
#define HM_SECINITSID_NETIF                                   (10u)
#define HM_SECINITSID_NETMSG                                  (11u)
#define HM_SECINITSID_NODE                                    (12u)
#define HM_SECINITSID_DEVNULL                                 (27u)

#define HM_SECINITSID_NUM (27u)
#define HM_SECSID_NULL    (0u) /* unspecified SID */

typedef uint16_t hmsel_class_t;

static bool is_socket_class(hmsel_class_t kern_tclass)
{
	bool sock = false;

	switch (kern_tclass) {
	case HM_SECCLASS_SOCKET:
	case HM_SECCLASS_TCP_SOCKET:
	case HM_SECCLASS_UDP_SOCKET:
	case HM_SECCLASS_RAWIP_SOCKET:
	case HM_SECCLASS_NETLINK_SOCKET:
	case HM_SECCLASS_PACKET_SOCKET:
	case HM_SECCLASS_KEY_SOCKET:
	case HM_SECCLASS_UNIX_STREAM_SOCKET:
	case HM_SECCLASS_UNIX_DGRAM_SOCKET:
	case HM_SECCLASS_APPLETALK_SOCKET:
	case HM_SECCLASS_DCCP_SOCKET:
	case HM_SECCLASS_TUN_SOCKET:
	case HM_SECCLASS_SCTP_SOCKET:
	case HM_SECCLASS_ICMP_SOCKET:
	case HM_SECCLASS_AX25_SOCKET:
	case HM_SECCLASS_IPX_SOCKET:
	case HM_SECCLASS_NETROM_SOCKET:
	case HM_SECCLASS_ATMPVC_SOCKET:
	case HM_SECCLASS_X25_SOCKET:
	case HM_SECCLASS_ROSE_SOCKET:
	case HM_SECCLASS_DECNET_SOCKET:
	case HM_SECCLASS_ATMSVC_SOCKET:
	case HM_SECCLASS_RDS_SOCKET:
	case HM_SECCLASS_IRDA_SOCKET:
	case HM_SECCLASS_PPPOX_SOCKET:
	case HM_SECCLASS_LLC_SOCKET:
	case HM_SECCLASS_CAN_SOCKET:
	case HM_SECCLASS_TIPC_SOCKET:
	case HM_SECCLASS_BLUETOOTH_SOCKET:
	case HM_SECCLASS_IUCV_SOCKET:
	case HM_SECCLASS_RXRPC_SOCKET:
	case HM_SECCLASS_ISDN_SOCKET:
	case HM_SECCLASS_PHONET_SOCKET:
	case HM_SECCLASS_IEEE802154_SOCKET:
	case HM_SECCLASS_CAIF_SOCKET:
	case HM_SECCLASS_ALG_SOCKET:
	case HM_SECCLASS_NFC_SOCKET:
	case HM_SECCLASS_VSOCK_SOCKET:
	case HM_SECCLASS_VSOCK_HOST_SOCKET:
	case HM_SECCLASS_KCM_SOCKET:
	case HM_SECCLASS_QIPCRTR_SOCKET:
	case HM_SECCLASS_SMC_SOCKET:
	case HM_SECCLASS_XDP_SOCKET:
		sock = true;
		break;
	default:
		break;
	}

	return sock;
}

static bool is_netlink_socket_class(hmsel_class_t kern_tclass)
{
	bool sock = false;

	switch (kern_tclass) {
	case HM_SECCLASS_NETLINK_ROUTE_SOCKET:
	case HM_SECCLASS_NETLINK_TCPDIAG_SOCKET:
	case HM_SECCLASS_NETLINK_NFLOG_SOCKET:
	case HM_SECCLASS_NETLINK_XFRM_SOCKET:
	case HM_SECCLASS_NETLINK_SELINUX_SOCKET:
	case HM_SECCLASS_NETLINK_ISCSI_SOCKET:
	case HM_SECCLASS_NETLINK_AUDIT_SOCKET:
	case HM_SECCLASS_NETLINK_FIB_LOOKUP_SOCKET:
	case HM_SECCLASS_NETLINK_CONNECTOR_SOCKET:
	case HM_SECCLASS_NETLINK_NETFILTER_SOCKET:
	case HM_SECCLASS_NETLINK_DNRT_SOCKET:
	case HM_SECCLASS_NETLINK_KOBJECT_UEVENT_SOCKET:
	case HM_SECCLASS_NETLINK_GENERIC_SOCKET:
	case HM_SECCLASS_NETLINK_SCSITRANSPORT_SOCKET:
	case HM_SECCLASS_NETLINK_RDMA_SOCKET:
	case HM_SECCLASS_NETLINK_CRYPTO_SOCKET:
		sock = true;
		break;
	default:
		break;
	}

	return sock;
}

static inline bool hmsel_socket_class(hmsel_class_t kern_tclass)
{
	return is_socket_class(kern_tclass) || is_netlink_socket_class(kern_tclass);
}
#endif
