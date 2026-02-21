/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: filters for various protocols
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 22 16:32:17 2021
 */

#include <lib/dlist.h>
#include <hmkernel/errno.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsync/raw_rwlock.h>

#include "ux_pktbuf.h"
#include "ux_netdev.h"
#include "ux_socket.h"
#include "ux_netfilter.h"

/* each hook list head */
struct dlist_node ux_nf_hooks[NPROTO][UX_NF_MAX_HOOKS];
/* socket list head */
static DLIST_HEAD(ux_nf_sockopts);

static int (*ux_nf_pre_ct_hook_ops)(const ux_pktbuf_t *pkt) = NULL;
static void (*ux_nf_post_ct_hook_ops)(struct ux_conntrack *res) = NULL;

static struct raw_rwlock ux_netfilter_lock = RAW_RDLOCK_INITIALIZER;

void ux_set_nf_pre_ct_hook(int (*hook)(const struct ux_pktbuf *pkt))
{
	ux_nf_pre_ct_hook_ops = hook;
}

void ux_set_nf_post_ct_hook(void (*hook)(struct ux_conntrack *res))
{
	ux_nf_post_ct_hook_ops = hook;
}

static int ux_nf_hook_traverse(int protocol, unsigned int hook, ux_pktbuf_t *pkt,
			       ux_netdev_t *indev, ux_netdev_t *outdev)
{
	int found = 0;
	int res = UX_NF_ACCEPT;
	struct dlist_node *pos;
	struct ux_nf_hook_ops *mem = NULL;

	(void)raw_rwlock_rdlock(&ux_netfilter_lock);
	if (ux_nf_pre_ct_hook_ops != NULL) {
		found = ux_nf_pre_ct_hook_ops(pkt);
	}
	/* The list should be first member of struct ux_nf_hook_ops */
	dlist_for_each(pos, &ux_nf_hooks[protocol][hook]) {
		mem = container_of(pos, struct ux_nf_hook_ops, list);
		res = mem->hook(hook, pkt, indev, outdev);
		if (res != UX_NF_ACCEPT) {
			if (res != UX_NF_REPEAT) {
				break;
			}
			/* Retry again */
			pos = pos->prev;
		}
	}
	if (found && ux_nf_post_ct_hook_ops != NULL) {
		ux_nf_post_ct_hook_ops(pkt->ctrk);
	}
	(void)raw_rwlock_unlock(&ux_netfilter_lock);

	return res;
}

/*
 * Packet header should be universal, so use void here for buf.
 * Return values: >=0: Hook has not eaten the packet, packet is processed as normal
 *                < 0: Hook has filter out the packet.
 */
int ux_nf_hook(int protocol, unsigned int hook,
	       ux_pktbuf_t *pktbuf,
	       ux_netdev_t *indev, ux_netdev_t *outdev)
{
	int ret = 0;

	BUG_ON(protocol < 0 || protocol >= NPROTO);
	BUG_ON(hook >= UX_NF_MAX_HOOKS);
	ret = ux_nf_hook_traverse(protocol, hook, pktbuf, indev, outdev);

	return ret;
}

int ux_nf_register_hook(struct ux_nf_hook_ops *hook_op)
{
	int pf;
	int hook;
	struct dlist_node *tmp = NULL;
	struct ux_nf_hook_ops *cur = NULL;

	BUG_ON(hook_op == NULL);
	pf = hook_op->pf;
	BUG_ON(pf < 0 || pf >= NPROTO);
	hook = hook_op->hooknum;
	BUG_ON(hook < 0 || hook >= UX_NF_MAX_HOOKS);

	(void)raw_rwlock_wrlock(&ux_netfilter_lock);
	dlist_for_each(tmp, &ux_nf_hooks[pf][hook]) {
		cur = container_of(tmp, struct ux_nf_hook_ops, list);
		if (hook_op->priority < cur->priority) {
			break;
		}
	}
	dlist_insert(tmp->prev, &hook_op->list);
	(void)raw_rwlock_unlock(&ux_netfilter_lock);

	return 0;
}

void ux_nf_unregister_hook(struct ux_nf_hook_ops *hook_op)
{
	(void)raw_rwlock_wrlock(&ux_netfilter_lock);
	dlist_delete(&hook_op->list);
	(void)raw_rwlock_unlock(&ux_netfilter_lock);
}

/* Init all hook lists */
int ux_netfilter_init(void)
{
	int proto;
	int hook;

	for (proto = 0; proto < NPROTO; proto++) {
		for (hook = 0; hook < UX_NF_NUMHOOKS; hook++) {
			dlist_init(&ux_nf_hooks[proto][hook]);
		}
	}

	return 0;
}
