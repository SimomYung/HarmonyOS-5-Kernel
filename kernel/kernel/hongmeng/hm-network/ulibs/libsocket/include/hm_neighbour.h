/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Header file for neighbour
 * Author: Huawei OS Kernel Lab
 * Create: Sat Dec 28 09:50:26 2019
 */

/*
 * This header file defines structures, enums and macros for network
 * neighbour subsystem. In order to maintain compatibility with existing
 * user applications, we make these definitions keep consistent with
 * LINUX neighbour.h, which could be found at /usr/include/linux/ in
 * linux operating system.
 */
#ifndef _HM_NEIGHBOUR_H
#define _HM_NEIGHBOUR_H

#include <bits/alltypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ETH_ALEN
#define ETH_ALEN	6
#endif
#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

struct ndmsg {
	uint8_t		ndm_family;
	uint8_t		ndm_pad1;
	uint16_t		ndm_pad2;
	int		ndm_ifindex;
	uint16_t		ndm_state;
	uint8_t		ndm_flags;
	uint8_t		ndm_type;
};

enum {
	NDA_UNSPEC,
	NDA_DST,
	NDA_LLADDR,
	NDA_CACHEINFO,
	NDA_PROBES,
	NDA_VLAN,
	NDA_PORT,
	NDA_VNI,
	NDA_IFINDEX,
	NDA_MASTER,
	NDA_LINK_NETNSID,
	NDA_SRC_VNI,
	__NDA_MAX
};

#define NDA_MAX (__NDA_MAX - 1)

#define NTF_USE		0x01
#define NTF_SELF	0x02
#define NTF_MASTER	0x04
#define NTF_PROXY	0x08
#define NTF_EXT_LEARNED	0x10
#define NTF_OFFLOADED   0x20
#define NTF_ROUTER	0x80

#define NUD_INCOMPLETE	0x01
#define NUD_REACHABLE	0x02
#define NUD_STALE	0x04
#define NUD_DELAY	0x08
#define NUD_PROBE	0x10
#define NUD_FAILED	0x20

#define NUD_NOARP	0x40
#define NUD_PERMANENT	0x80
#define NUD_NONE	0x00

struct nda_cacheinfo {
	uint32_t		ndm_confirmed;
	uint32_t		ndm_used;
	uint32_t		ndm_updated;
	uint32_t		ndm_refcnt;
};

struct ndt_stats {
	uint64_t		ndts_allocs;
	uint64_t		ndts_destroys;
	uint64_t		ndts_hash_grows;
	uint64_t		ndts_res_failed;
	uint64_t		ndts_lookups;
	uint64_t		ndts_hits;
	uint64_t		ndts_rcv_probes_mcast;
	uint64_t		ndts_rcv_probes_ucast;
	uint64_t		ndts_periodic_gc_runs;
	uint64_t		ndts_forced_gc_runs;
	uint64_t		ndts_table_fulls;
};

enum {
	NDTPA_UNSPEC,
	NDTPA_IFINDEX,
	NDTPA_REFCNT,
	NDTPA_REACHABLE_TIME,
	NDTPA_BASE_REACHABLE_TIME,
	NDTPA_RETRANS_TIME,
	NDTPA_GC_STALETIME,
	NDTPA_DELAY_PROBE_TIME,
	NDTPA_QUEUE_LEN,
	NDTPA_APP_PROBES,
	NDTPA_UCAST_PROBES,
	NDTPA_MCAST_PROBES,
	NDTPA_ANYCAST_DELAY,
	NDTPA_PROXY_DELAY,
	NDTPA_PROXY_QLEN,
	NDTPA_LOCKTIME,
	NDTPA_QUEUE_LENBYTES,
	NDTPA_MCAST_REPROBES,
	NDTPA_PAD,
	__NDTPA_MAX
};
#define NDTPA_MAX (__NDTPA_MAX - 1)

struct ndtmsg {
	uint8_t		ndtm_family;
	uint8_t		ndtm_pad1;
	uint16_t		ndtm_pad2;
};

struct ndt_config {
	uint16_t		ndtc_key_len;
	uint16_t		ndtc_entry_size;
	uint32_t		ndtc_entries;
	uint32_t		ndtc_last_flush;
	uint32_t		ndtc_last_rand;
	uint32_t		ndtc_hash_rnd;
	uint32_t		ndtc_hash_mask;
	uint32_t		ndtc_hash_chain_gc;
	uint32_t		ndtc_proxy_qlen;
};

enum {
	NDTA_UNSPEC,
	NDTA_NAME,
	NDTA_THRESH1,
	NDTA_THRESH2,
	NDTA_THRESH3,
	NDTA_CONFIG,
	NDTA_PARMS,
	NDTA_STATS,
	NDTA_GC_INTERVAL,
	NDTA_PAD,
	__NDTA_MAX
};
#define NDTA_MAX (__NDTA_MAX - 1)

typedef struct nd_entry {
	uint32_t dst;
	char ifname[IFNAMSIZ]; // dev->name
	unsigned char lladdr[ETH_ALEN]; // link addr
	struct nda_cacheinfo ci;
	uint32_t probe;
	uint16_t state;
	uint8_t flags;
}nd_entry_t;

#ifdef __cplusplus
}
#endif

#endif
