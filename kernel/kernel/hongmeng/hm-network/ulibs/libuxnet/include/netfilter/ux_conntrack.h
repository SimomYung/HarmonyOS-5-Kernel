/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: conntrac 5-tuple
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 28 17:29:30 2021
 */

#ifndef _UX_CONNTRACK_H
#define _UX_CONNTRACK_H

#include "ux_netdev.h"
#include "ux_pktbuf.h"
#include "ux_conntrack_timer.h"

/* Metadata for packet of connection. */
typedef struct ux_conntrack_tuple {
	/* parts of src */
	struct {
		uint32_t ip;
		uint16_t port;
	} src;

	/* parts of dst */
	struct {
		uint32_t ip;
		union {
			uint16_t all;
			struct {
				uint16_t port;
			} pt;
			struct {
				uint8_t type, code;
			} icmp;
		} ud;
		/* Protocol type. */
		uint8_t protonum;
		/* Dirction: original & reply */
		uint8_t dir;
	} dst;
} ux_conntrack_tuple_t;

enum UX_CT_DIR_TYPE {
	UX_CT_DIR_ORIGINAL,
	UX_CT_DIR_REPLY,
	UX_CT_DIR_MAX
};

/* Two entries: egress & ingress */
typedef struct ux_ct_tuple_hash {
	/* Point to correspond net connection */
	struct dlist_node list;
	ux_conntrack_tuple_t tuple;
} ux_ct_tuple_hash_t;

static inline int ux_ct_tuple_src_equal(const struct ux_conntrack_tuple *t1,
					const struct ux_conntrack_tuple *t2)
{
	if (t1->src.ip != t2->src.ip) {
		return 0;
	}
	if (t1->src.port != t2->src.port) {
		return 0;
	}
	return 1;
}

static inline int _ux_ct_tuple_dst_equal(const struct ux_conntrack_tuple *t1,
					 const struct ux_conntrack_tuple *t2)
{
	if (t1->dst.ip != t2->dst.ip) {
		return 0;
	}
	if (t1->dst.ud.all != t2->dst.ud.all) {
		return 0;
	}
	return 1;
}

static inline int ux_ct_tuple_dst_equal(const struct ux_conntrack_tuple *t1,
					const struct ux_conntrack_tuple *t2)
{
	int ret = _ux_ct_tuple_dst_equal(t1, t2);
	if (ret == 0) {
		return 0;
	}
	if (t1->dst.protonum != t2->dst.protonum) {
		return 0;
	}
	return 1;
}

static inline int ux_ct_tuple_equal(const struct ux_conntrack_tuple *t1,
				    const struct ux_conntrack_tuple *t2)
{
	int src, dst;

	src = ux_ct_tuple_src_equal(t1, t2);
	dst = ux_ct_tuple_dst_equal(t1, t2);
	if (src != 0 && dst != 0) {
		return 1;
	} else {
		return 0;
	}
}

#define UX_CT_HI_PRIO (-100)
#define UX_CT_LW_PRIO 100
#define IP_CT_DIR_MAX 2
/* Connection state, this is standard for netfilter */
typedef enum ux_conntrack_stage_states {
	UX_CT_ESTABLISHED,
	/* Related to an existing connection */
	UX_CT_RELATED,
	/* Started a new connection */
	UX_CT_NEW,
	/* A-->B above, >= this, is B--->A */
	UX_CT_IS_REPLY,
	/* Total UX_CT types(NEW only one). */
	UX_CT_NUMBER = UX_CT_IS_REPLY * IP_CT_DIR_MAX - 1
} ux_conntrack_stage_states_t;

/* Fine-grained status of connection. */
enum ux_conntrack_status {
	/* Related connection case */
	IPS_EXPECTED_BIT = 0,
	/* Set 1 when packets reply */
	IPS_SEEN_REPLY_BIT = 1,
	/* Always existed. */
	IPS_ASSURED_BIT = 2,
	/* Packets are not filtered out. */
	IPS_CONFIRMED_BIT = 3,
};

/* One flow in netfilter is called one connection. */
typedef struct ux_conntrack {
	void (*destroy)(struct ux_conntrack *);
	/* Fine-grained connection status, bit. */
	unsigned long status;
	/* Coarse-grained ct status */
	int ctflag;
	/* The conntrack tuples: original and reply */
	struct ux_ct_tuple_hash tuplehash[IP_CT_DIR_MAX];
	struct ux_ct_timer timer;
	raw_atomic_int_t refcnt;
} ux_conntrack_t;

typedef struct conntrack_buf {
	/* Coarse-grained ct status */
	int ctflag;
	ux_conntrack_t *ctrk;
	/* packet data */
	void *payload;
} conntrack_buf_t;

/* protocol ops */
typedef struct ux_conntrack_protocol_desc {
	uint8_t protocol;
	const char *name;
	/* Turn packet into tuple */
	int (*get_tuple_from_pkt)(const void *skb,
				  unsigned int dataoff,
				  struct ux_conntrack_tuple *tuple);

	int (*swap_tuple)(struct ux_conntrack_tuple *inverse,
			  const struct ux_conntrack_tuple *orig);

	/* Check packet valid, and ajust connection */
	int (*handle_packet)(struct ux_conntrack *conntrack,
			     const void *skb,
			     enum ux_conntrack_stage_states ctinfo);

	/* Create connection when the protocol found */
	int (*create)(struct ux_conntrack *conntrack, const void *skb);

	/* Destroy protocol with conntrack entry */
	void (*destroy)(struct ux_conntrack *conntrack);
	int (*error)(const void *skb, enum ux_conntrack_stage_states *ctinfo,
		     unsigned int hooknum);
} ux_ct_protocol_desc_t;

static inline struct ux_conntrack *
ct_of_tuplehash(const struct ux_ct_tuple_hash *hash)
{
	return container_of(hash, struct ux_conntrack,
			    tuplehash[hash->tuple.dst.dir]);
}

/* protocols */
ux_ct_protocol_desc_t *ux_get_tcp_proto(void);
ux_ct_protocol_desc_t *ux_get_udp_proto(void);
ux_ct_protocol_desc_t *ux_get_icmp_proto(void);
ux_ct_protocol_desc_t *ux_get_default_proto(void);

struct ux_netdev;
int ux_conntrack_in(unsigned int hooknum,
		    ux_pktbuf_t *pctrkb,
		    struct ux_netdev *in,
		    struct ux_netdev *out);
struct ux_conntrack *ux_conntrack_get(const ux_pktbuf_t *pctrkb,
				      enum ux_conntrack_stage_states *ctinfo);

int ux_conntrack_find_get(const ux_pktbuf_t *pkt);
void ux_ct_gc_handler(void);
int ux_ct_confirm(ux_pktbuf_t *pctb);
int ux_conntrack_init(void);
void ux_conntrack_exit(void);

int ux_nf_conntrack_init(void);
void ux_nf_conntrack_exit(void);

#endif /* _UX_CONNTRACK_H */
