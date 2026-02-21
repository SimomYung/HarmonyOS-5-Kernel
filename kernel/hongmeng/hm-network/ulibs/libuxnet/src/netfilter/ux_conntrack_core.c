/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: filters for various protocols
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 29 15:39:58 2021
 */

#include <netdb.h>

#include <lib/dlist.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsrv_sys/hm_timer.h>

#include "ux_conntrack_timer.h"
#include "ux_socket.h"
#include "ux_pbuf.h"
#include "ux_conntrack.h"
#include "ux_netfilter.h"

#define CT_HASH_TABLE_SIZE		2   /* |_0_|_1_| */
#define CT_NUM_MAX			4096
#define PACKET_IHDR_SIZE		4
#define CT_TIMEOUT			(1000 * 120)   /* unit: ms */
ux_ct_protocol_desc_t *g_ip_ct_protos[IPPROTO_MAX];
struct dlist_node *g_conntrack_hash;
static struct raw_rwlock ux_ct_lock = RAW_RDLOCK_INITIALIZER;

/* Needs this when new connection */
raw_atomic_int_t  ux_conntrack_count = RAW_ATOMIC_VAL_INIT(0);
/* Put confirm list only when packet accept */
struct dlist_node g_unconfirm_list;

static int get_conntrack_idx(const struct ux_conntrack_tuple *tuple)
{
	int index;

	index = tuple->src.ip % CT_HASH_TABLE_SIZE;
	return index;
}

static int ux_ct_get_tuple(const struct iphdr *iph,
			   const void *payload,
			   unsigned int dataoff,
			   struct ux_conntrack_tuple *tuple,
			   const ux_ct_protocol_desc_t *protocol)
{
	tuple->src.ip = iph->saddr;
	tuple->dst.ip = iph->daddr;
	tuple->dst.protonum = iph->protocol;
	tuple->dst.dir = UX_CT_DIR_ORIGINAL;

	/* Get correspond ports mainly */
	return protocol->get_tuple_from_pkt(payload, dataoff, tuple);
}

static int ux_ct_invert_tuple(struct ux_conntrack_tuple *inverse,
			      const struct ux_conntrack_tuple *orig,
			      const ux_ct_protocol_desc_t *protocol)
{
	inverse->src.ip = orig->dst.ip;
	inverse->dst.ip = orig->src.ip;
	inverse->dst.protonum = orig->dst.protonum;
	inverse->dst.dir = !orig->dst.dir;

	/* get invert ports mainly */
	return protocol->swap_tuple(inverse, orig);
}

static void destroy_conntrack(struct ux_conntrack *ct)
{
	uint8_t num;
	ux_ct_protocol_desc_t *proto;

	if (ct == NULL) {
		return;
	}

	num = ct->tuplehash[UX_CT_DIR_REPLY].tuple.dst.protonum;
	proto = g_ip_ct_protos[num];
	if (proto && proto->destroy) {
		proto->destroy(ct);
	}

	(void)raw_atomic_int_dec(&ux_conntrack_count);
	free(ct);
}

static inline int conntrack_tuple_cmp(const struct ux_ct_tuple_hash *h,
				      const struct ux_conntrack_tuple *tuple)
{
	if (ct_of_tuplehash(h) != NULL) {
		return ux_ct_tuple_equal(tuple, &h->tuple);
	}

	return 0;
}

static struct ux_ct_tuple_hash *
__ux_conntrack_find(const struct ux_conntrack_tuple *tuple)
{
	struct ux_ct_tuple_hash *h;
	unsigned int hash = get_conntrack_idx(tuple);
	struct dlist_node *head = NULL;

	head = &g_conntrack_hash[hash];
	dlist_for_each_entry(h, head, struct ux_ct_tuple_hash, list) {
		if (conntrack_tuple_cmp(h, tuple)) {
			return h;
		}
	}

	return NULL;
}

static struct ux_ct_tuple_hash *
ux_conntrack_find(const struct ux_conntrack_tuple *tuple)
{
	struct ux_ct_tuple_hash *h = NULL;

	(void)raw_rwlock_rdlock(&ux_ct_lock);
	h = __ux_conntrack_find(tuple);
	(void)raw_rwlock_unlock(&ux_ct_lock);

	return h;
}

int ux_conntrack_find_get(const ux_pktbuf_t *pkt)
{
	int ret = 0;
	unsigned char nr;
	struct ux_conntrack_tuple tuple;
	struct ux_ct_tuple_hash *h = NULL;
	struct ux_conntrack *ct = NULL;
	ux_pbuf_header_t *pbhdr = (ux_pbuf_header_t *)(pkt->payload);

	nr = pbhdr->iphdr.protocol;
	if (ux_ct_get_tuple(&pbhdr->iphdr, pbhdr, (pbhdr->iphdr.ihl) * PACKET_IHDR_SIZE,
				&tuple, g_ip_ct_protos[nr]) == 0) {
		return ret;
	}

	(void)raw_rwlock_rdlock(&ux_ct_lock);
	h = __ux_conntrack_find(&tuple);
	if (h != NULL) {
		ct = ct_of_tuplehash(h);
		(void)raw_atomic_int_inc(&ct->refcnt);
		ret = 1;
	}
	(void)raw_rwlock_unlock(&ux_ct_lock);

	return ret;
}

struct ux_conntrack *ux_conntrack_get(const ux_pktbuf_t *pkt,
				      enum ux_conntrack_stage_states *ctinfo)
{
	struct ux_conntrack *pct = NULL;

	pct = (struct ux_conntrack *)pkt->ctrk;

	if (pct != NULL) {
		*ctinfo = pct->ctflag;
	} else {
		net_warn(HMNET_NETFILTER, "conntrack buf is NULL\n");
	}
	return pct;
}

static void ux_conntrack_put(struct ux_conntrack *ctrack)
{
	struct ux_conntrack *ct = (struct ux_conntrack *)ctrack;

	if (ct == NULL) {
		return;
	}
	if (raw_atomic_int_dec(&ct->refcnt) == 0) {
		ct->destroy(ct);
	}
}

static bool ux_ct_is_confirm_list(const struct ux_ct_tuple_hash *h,
				  struct ux_conntrack *ct)
{
	bool ret = false;

	if (h == &ct->tuplehash[UX_CT_DIR_ORIGINAL] || \
	    h == &ct->tuplehash[UX_CT_DIR_REPLY]) {
		ret = true;
	}
	return ret;
}

static void ux_ct_gc_traverse_cnfmlist(int index, unsigned long cur_time)
{
	struct ux_conntrack *ct = NULL;
	struct ux_ct_tuple_hash *h = NULL;
	struct ux_ct_tuple_hash *n = NULL;

	dlist_for_each_entry_safe(h, n, &g_conntrack_hash[index],
			     struct ux_ct_tuple_hash, list) {
		ct = ct_of_tuplehash(h);
		if ((ct->timer.expire  < cur_time) && (ct->timer.expire > 0)) {
			if (ux_ct_is_confirm_list(n, ct) == true) {
				n = dlist_next_entry(n, struct ux_ct_tuple_hash, list);
			}
			dlist_delete(&ct->tuplehash[UX_CT_DIR_ORIGINAL].list);
			dlist_delete(&ct->tuplehash[UX_CT_DIR_REPLY].list);
			ux_conntrack_put(ct);
		}
	}
}

static void ux_ct_gc_confirm_list(unsigned long cur_time)
{
	int index;

	/* Traverse hash list , free each conntrack */
	for (index = 0; index < CT_HASH_TABLE_SIZE; index++) {
		ux_ct_gc_traverse_cnfmlist(index, cur_time);
	}
}

void ux_ct_gc_handler(void)
{
	unsigned long cur_time = 0;
	struct ux_ct_tuple_hash *h = NULL, *n = NULL;
	struct ux_conntrack *ct = NULL;
	struct timespec ts;

	(void)hm_clock_gettime(CLOCK_MONOTONIC, &ts);
	/* time unit, ms */
	cur_time = (unsigned long)(ts.tv_nsec / NSEC_PER_MSEC) +
		(unsigned long)(ts.tv_sec * MSEC_PER_SEC);
	(void)raw_rwlock_wrlock(&ux_ct_lock);
	/* 1. Recycle unconfrim resource */
	dlist_for_each_entry_safe(h, n, &g_unconfirm_list, struct ux_ct_tuple_hash, list) {
		ct = ct_of_tuplehash(h);
		if ((ct->timer.expire < cur_time) && (ct->timer.expire > 0)) {
			dlist_delete(&ct->tuplehash[UX_CT_DIR_ORIGINAL].list);
			ux_conntrack_put(ct);
		}
	}

	/* 2. Recycle confirm resource */
	ux_ct_gc_confirm_list(cur_time);
	(void)raw_rwlock_unlock(&ux_ct_lock);
}

/* Save ct only when packets accept */
int ux_ct_confirm(ux_pktbuf_t *pkt)
{
	enum UX_CT_DIR_TYPE ct_dir;
	unsigned int oidx;
	unsigned int ridx;
	struct ux_conntrack *ct;
	enum ux_conntrack_stage_states ctinfo;
	struct ux_ct_tuple_hash *original = NULL;
	struct ux_ct_tuple_hash *reply = NULL;

	/* Get the current conntrack with the packet */
	ct = ux_conntrack_get(pkt, &ctinfo);
	if (ct == NULL) {
		return UX_NF_ACCEPT;
	}

	ct_dir = (ctinfo) >= UX_CT_IS_REPLY ? \
		UX_CT_DIR_REPLY : UX_CT_DIR_ORIGINAL;
	/* New connection need to confirm, otherwise not */
	if (ct_dir != UX_CT_DIR_ORIGINAL) {
		return UX_NF_ACCEPT;
	}

	oidx = get_conntrack_idx(&ct->tuplehash[UX_CT_DIR_ORIGINAL].tuple);
	ridx = get_conntrack_idx(&ct->tuplehash[UX_CT_DIR_REPLY].tuple);

	(void)raw_rwlock_wrlock(&ux_ct_lock);
	original = __ux_conntrack_find(&ct->tuplehash[UX_CT_DIR_ORIGINAL].tuple);
	reply = __ux_conntrack_find(&ct->tuplehash[UX_CT_DIR_REPLY].tuple);
	if (original == NULL && reply == NULL) {
		dlist_delete(&ct->tuplehash[UX_CT_DIR_ORIGINAL].list);
		dlist_insert_head(&g_conntrack_hash[oidx], &ct->tuplehash[UX_CT_DIR_ORIGINAL].list);
		/* Consider to separate original and reply */
		dlist_insert_head(&g_conntrack_hash[ridx], &ct->tuplehash[UX_CT_DIR_REPLY].list);
		ct->status |= (1UL << IPS_CONFIRMED_BIT);
	}
	(void)raw_rwlock_unlock(&ux_ct_lock);
	return UX_NF_ACCEPT;
}

/* Allocate a new conntrack. */
static struct ux_ct_tuple_hash *
create_conntrack(const struct ux_conntrack_tuple *tuple,
		 const ux_ct_protocol_desc_t *protocol,
		 void *skb)
{
	int count;
	struct ux_conntrack *conntrack;
	struct ux_conntrack_tuple repl_tuple;
	struct timespec ts;

	count = raw_atomic_int_read(&ux_conntrack_count);
	if (count >= CT_NUM_MAX) {
		net_error(HMNET_NETFILTER, "No longer connect track, nums over limit\n");
		return NULL;
	}

	if (!ux_ct_invert_tuple(&repl_tuple, tuple, protocol)) {
		net_debug(HMNET_NETFILTER, "Invert tuple failed\n");
		return NULL;
	}

	/* use array/list temporarily */
	conntrack = calloc(1, sizeof(*conntrack));
	if (conntrack == NULL) {
		net_error(HMNET_NETFILTER, "malloc conntrack fail.\n");
		return NULL;
	}

	if (!protocol->create(conntrack, skb)) {
		free(conntrack);
		return NULL;
	}
	conntrack->destroy = destroy_conntrack;
	conntrack->tuplehash[UX_CT_DIR_ORIGINAL].tuple = *tuple;
	conntrack->tuplehash[UX_CT_DIR_REPLY].tuple = repl_tuple;
	(void)raw_atomic_int_set(&conntrack->refcnt, 1);
	(void)raw_rwlock_wrlock(&ux_ct_lock);
	/* wait for confirmation */
	dlist_insert_head(&g_unconfirm_list, &conntrack->tuplehash[UX_CT_DIR_ORIGINAL].list);
	(void)raw_rwlock_unlock(&ux_ct_lock);
	(void)raw_atomic_int_inc(&ux_conntrack_count);

	(void)hm_clock_gettime(CLOCK_MONOTONIC, &ts);
	unsigned long time = (unsigned long)(ts.tv_nsec / NSEC_PER_MSEC) +
		(unsigned long)(ts.tv_sec * MSEC_PER_SEC);
	ux_timer_init(&conntrack->timer,
		      time, CT_TIMEOUT);

	return &conntrack->tuplehash[UX_CT_DIR_ORIGINAL];
}

static void ux_ct_update_time(ux_ct_timer_t *timer, unsigned long interval)
{
	unsigned long cur_time;
	unsigned long new_timeout;
	struct timespec ts;

	(void)hm_clock_gettime(CLOCK_MONOTONIC, &ts);
	cur_time = (unsigned long)(ts.tv_nsec / NSEC_PER_MSEC) +
		(unsigned long)(ts.tv_sec * MSEC_PER_SEC);
	/* update timeout time */
	new_timeout = cur_time + interval;
	timer->expire = new_timeout;
}

static ux_conntrack_t *handle_normal_ct(struct ux_pbuf_header *pbufhdr,
					const ux_ct_protocol_desc_t *proto,
					ux_pktbuf_t *pkt,
					int *set_reply)
{
	enum ux_conntrack_stage_states ctinfo;
	struct ux_conntrack_tuple tuple = {0};
	struct ux_ct_tuple_hash *h;
	struct ux_conntrack *ct = NULL;

	/* sizeof(ip_header) equal dataoff */
	if (ux_ct_get_tuple(&pbufhdr->iphdr, pbufhdr,
			    (pbufhdr->iphdr.ihl) * PACKET_IHDR_SIZE,
			    &tuple, proto) == 0) {
		return NULL;
	}

	/* find conntrack related tuple */
	h = ux_conntrack_find(&tuple);
	if (h == NULL) {
		h = create_conntrack(&tuple, proto, pbufhdr);
		if (h == NULL) {
			return NULL;
		}
	} else {
		/* update ct timeout for confirm packet */
		ux_ct_update_time(&ct_of_tuplehash(h)->timer, CT_TIMEOUT);
	}

	ct = ct_of_tuplehash(h);
	if (ct == NULL) {
		return ct;
	}

	if (h->tuple.dst.dir == UX_CT_DIR_REPLY) {
		ctinfo = UX_CT_ESTABLISHED + UX_CT_IS_REPLY;
		/* Set reply direction flag of the packet */
		*set_reply = 1;
	} else {
		/* A-->B---A is existed, so think established */
		if (ct->status & (1UL << IPS_SEEN_REPLY_BIT)) {
			ctinfo = UX_CT_ESTABLISHED;
		} else {
			ctinfo = UX_CT_NEW;
		}
		*set_reply = 0;
	}
	/* Save ct and corese state */
	ct->ctflag = (int)ctinfo;
	pkt->ctrk = ct;

	return ct;
}

int ux_conntrack_in(unsigned int hooknum, struct ux_pktbuf *pkt,
		    struct ux_netdev *in,
		    struct ux_netdev *out)
{
	int is_reply = 0;
	int ret = 0;
	unsigned char pro = 0;
	ux_conntrack_t *ct = NULL;
	enum ux_conntrack_stage_states ctinfo = 0;
	ux_ct_protocol_desc_t *proto = NULL;
	ux_pbuf_header_t *pskb = (ux_pbuf_header_t *)(pkt->payload);

	if (pskb == NULL) {
		return UX_NF_ACCEPT;
	}

	/* Return accept if the connetion is existed already ? */
	/* specific protocol */
	pro = pskb->iphdr.protocol;
	proto = g_ip_ct_protos[pro];

	/* Find conntrack which matches with the packet */
	ct = handle_normal_ct(pskb, proto, pkt, &is_reply);
	if (ct == NULL) {
		/* Invalid connection */
		pkt->ctrk = NULL;
		return UX_NF_ACCEPT;
	}

	ctinfo = pkt->ctrk->ctflag;
	ret = proto->handle_packet(ct, pskb, ctinfo);
	if (ret < 0) {
		net_error(HMNET_NETFILTER, "proto handle packet fail, ret=%d\n", ret);
		return -1;
	}

	if (is_reply) {
		ct->status |= (1UL << IPS_SEEN_REPLY_BIT);
	}

	return ret;
}

static void ux_ct_protocol_init(void)
{
	int i;

	for (i = 0; i < IPPROTO_MAX; i++) {
		g_ip_ct_protos[i] = ux_get_default_proto();
	}
	/* Set specific protocols. */
	g_ip_ct_protos[IPPROTO_TCP] = ux_get_tcp_proto();
	g_ip_ct_protos[IPPROTO_UDP] = ux_get_udp_proto();
	g_ip_ct_protos[IPPROTO_ICMP] = ux_get_icmp_proto();
}

int ux_conntrack_init(void)
{
	int i;
	int ret;

	g_conntrack_hash = (struct dlist_node *)malloc(sizeof(struct dlist_node)
						   * CT_HASH_TABLE_SIZE);
	if (g_conntrack_hash == NULL) {
		net_error(HMNET_NETFILTER, "conntrack list malloc failed\n");
		return -1;
	}
	ux_ct_protocol_init();

	for (i = 0; i < CT_HASH_TABLE_SIZE; i++) {
		dlist_init(&g_conntrack_hash[i]);
	}
	dlist_init(&g_unconfirm_list);
	ux_set_nf_pre_ct_hook(ux_conntrack_find_get);
	ux_set_nf_post_ct_hook(ux_conntrack_put);
	ret = ux_ct_timer_create_init();

	return ret;
}

void ux_conntrack_exit(void)
{
	struct ux_ct_tuple_hash *h, *n;
	unsigned int hash;
	struct ux_conntrack *ct;

	(void)raw_rwlock_rdlock(&ux_ct_lock);
	/* Traverse hash list , free each conntrack */
	for (hash = 0; hash < CT_HASH_TABLE_SIZE; hash++) {
		dlist_for_each_entry_safe(h, n, &g_conntrack_hash[hash],
					  struct ux_ct_tuple_hash, list) {
			ct = ct_of_tuplehash(h);
			if (ux_ct_is_confirm_list(n, ct) == true) {
				n = dlist_next_entry(n, struct ux_ct_tuple_hash, list);
			}
			dlist_delete(&ct->tuplehash[UX_CT_DIR_ORIGINAL].list);
			dlist_delete(&ct->tuplehash[UX_CT_DIR_REPLY].list);
			free(ct);
			ct = NULL;
		}
	}
	(void)raw_rwlock_unlock(&ux_ct_lock);

	free(g_conntrack_hash);
	g_conntrack_hash = NULL;
}
